#include <pqxx/pqxx>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "pgtm.h"
#include "util.h"

struct _Thread {
    long UniqID;
    int* ROFlag;
};

void PgtmStartup ()
{
	try
	{
		pqxx::connection c("dbname=robert user=robert port=5433");
		pqxx::work w(c);
		pqxx::result r = w.exec("SELECT 1");
		w.commit();
		printf("Result: %d", r[0][0].as<int>());
	}
	catch (const std::exception &e)
	{
		printf("Error: %s", e.what());
	}
}

void PgtmShutdown ()
{}

Thread* PgtmNewThread ()
{
	return 0;
}

void PgtmFreeThread (Thread* t)
{}

void PgtmInitThread (Thread* t, long id)
{}

void PgtmAbort (Thread* Self)
{}

void PgtmWrite (Thread* Self, volatile intptr_t* addr, intptr_t valu)
{}

intptr_t PgtmRead (Thread* Self, volatile intptr_t* Addr)
{
	return 0;
}

void PgtmBegin (Thread* Self, int* ROFlag)
{}

int PgtmEnd (Thread* Self)
{
	return 1;
}

void* PgtmAlloc (Thread* Self, size_t size)
{
	return 0;
}

void PgtmFree (Thread* Self, void* ptr)
{}
