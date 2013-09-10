#include <assert.h>
#include <pqxx/pqxx>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "pgtm.h"
#include "tmalloc.h"
#include "util.h"

struct _Thread {
    long Uid;
    int* ROFlag;
    int IsRO;
    pqxx::connection* pConn;
    pqxx::basic_transaction* pTrans;
	tmalloc_t* allocPtr;
	tmalloc_t* freePtr;
};

void PgtmStartup ()
{
	try
	{
		// libpq must be thread-safe!
		pqxx::thread_safety_model tsm = pqxx::describe_thread_safety();
		printf("libpq thread safety\n");
		printf("  - description: %s", tsm.description.c_str());
		printf("  - have_strerror_r: %d\n", tsm.have_strerror_r);
		printf("  - safe_kerberos: %d\n", tsm.safe_kerberos);
		printf("  - safe_libpq: %d\n", tsm.safe_libpq);
		printf("  - safe_query_cancel: %d\n", tsm.safe_query_cancel);
		printf("  - safe_result_copy: %d\n", tsm.safe_result_copy);

		// expect peer authentication with the running database
		// the username and database name used have to match the name of the user running this program
		pqxx::connection c("dbname=robert user=robert port=5433");
		pqxx::work w(c);
		pqxx::result r = w.exec("SELECT 1");
		w.commit();
		printf("Starting Pgtm succeeded: %d\n\n", r[0][0].as<int>());
	}
	catch (const std::exception &e)
	{
		printf("Starting Pgtm failed: %s\n", e.what());
	}
}

void PgtmShutdown ()
{
	printf("Shutting down Pgtm succeeded\n");
}

Thread* PgtmNewThread ()
{
#ifdef PROFILE
	printf("New thread ... \n");
#endif
	Thread* t = (Thread*) malloc(sizeof(Thread));
	assert(t);
	return t;
}

void PgtmFreeThread (Thread* t)
{
#ifdef PROFILE
	printf("Free thread ... \n");
#endif
	tmalloc_free(t->allocPtr);
	tmalloc_free(t->freePtr);
	delete t->pConn;
	free(t);
}

void PgtmInitThread (Thread* t, long id)
{
#ifdef PROFILE
	printf("Init thread ... \n");
#endif
	t->Uid = id;
	t->allocPtr = tmalloc_alloc(1);
	assert(t->allocPtr);
	t->freePtr = tmalloc_alloc(1);
	assert(t->freePtr);
	t->pConn = new pqxx::connection("dbname=robert user=robert port=5433");
}

void PgtmAbort (Thread* Self)
{
#ifdef PROFILE
	printf("Abort ... \n");
#endif
	Self->pTrans->abort();
}

void PgtmWrite (Thread* Self, volatile intptr_t* addr, intptr_t valu)
{
#ifdef PROFILE
	printf("Write ... \n");
#endif
	char* query = new char[128];
	try {
		if(Self->IsRO) {
			PgtmAbort(Self);
			return;
		} else {
			char* query = new char[128];
			sprintf(query, "INSERT INTO pgtm (addr, valu) VALUES ('%p', %d)", addr, valu);
			Self->pTrans->exec(query);
		}
	} catch(...) {
#ifdef PROFILE
		printf("Write failed ...\n");
#endif
	}
	delete query;
}

intptr_t PgtmRead (Thread* Self, volatile intptr_t* Addr)
{
#ifdef PROFILE
	printf("Read ... \n");
#endif
	char* query = new char[128];
	try {
		sprintf(query, "SELECT valu FROM pgtm WHERE addr='%p'", Addr);
		pqxx::result r = Self->pTrans->exec(query);
		Self->pTrans->commit();
		delete query;
		return r.empty() ? 0 : r[0][0].as<intptr_t>();
	} catch(...) {
#ifdef PROFILE
		printf("Read failed ...\n");
#endif
		delete query;
		return 0;
	}
}

void PgtmBegin (Thread* Self, int* ROFlag)
{
#ifdef PROFILE
	printf("Begin ... \n");
#endif
	Self->ROFlag = ROFlag;
	Self->IsRO = ROFlag ? *ROFlag : 0;
	if(*ROFlag) {
		Self->pTrans = new pqxx::read_transaction(*(Self->pConn));
	} else {
		Self->pTrans = new pqxx::work(*(Self->pConn));
	}
}

int PgtmEnd (Thread* Self)
{
#ifdef PROFILE
	printf("End ... \n");
#endif
	try {
		Self->pTrans->commit();
		delete Self->pTrans;
		return 1;
	}
	catch (const pqxx::in_doubt_error &)
	{
		// connection lost to database, this is somewhat terrible
		printf("Connection lost to DB ... \n");
		throw;
	}
	catch (const std::exception &)
	{
		// transaction aborted
#ifdef PROFILE
		printf("Transaction aborted ... \n");
#endif
		return 0;
	}
}

void* PgtmAlloc (Thread* Self, size_t size)
{
#ifdef PROFILE
	printf("Alloc ... \n");
#endif
	void* ptr = tmalloc_reserve(size);
	if (ptr) {
		tmalloc_append(Self->allocPtr, ptr);
	}

	return ptr;
}

void PgtmFree (Thread* Self, void* ptr)
{
#ifdef PROFILE
	printf("Free ... \n");
#endif
	tmalloc_append(Self->freePtr, ptr);
}
