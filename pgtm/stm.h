// The STAMP lib/tm.h expects the following STM_ macros to be defined for
// Software Transactional Memory.

// #define TM_ARG						STM_SELF,
// #define TM_ARG_ALONE					STM_SELF
// #define TM_ARGDECL					STM_THREAD_T* TM_ARG
// #define TM_ARGDECL_ALONE				STM_THREAD_T* TM_ARG_ALONE
// #define TM_STARTUP(numThread)		STM_STARTUP(); \
// 										STM_NEW_THREADS(numThread)
// #define TM_SHUTDOWN()				STM_SHUTDOWN()
// #define TM_STARTUP(numThread)		STM_STARTUP(); \
// 										STM_NEW_THREADS(numThread)
// #define TM_SHUTDOWN()				STM_SHUTDOWN()
// 										thread_getId()); \
// 										STM_SET_SELF(TM_ARG_ALONE)
// #define TM_THREAD_EXIT()				STM_FREE_THREAD(TM_ARG_ALONE)
// #define TM_STARTUP(numThread)		STM_STARTUP()
// #define TM_SHUTDOWN()				STM_SHUTDOWN()
// #define TM_STARTUP(numThread)		STM_STARTUP()
// #define TM_SHUTDOWN()				STM_SHUTDOWN()
// #define TM_THREAD_ENTER()			TM_ARGDECL_ALONE = STM_NEW_THREAD(); \
// 										STM_INIT_THREAD(TM_ARG_ALONE, thread_getId())
// #define TM_THREAD_EXIT()				STM_FREE_THREAD(TM_ARG_ALONE)
// #define TM_MALLOC(size)				STM_MALLOC(size)
// #define TM_FREE(ptr)					STM_FREE(ptr)
// #define TM_BEGIN()					STM_BEGIN_WR()
// #define TM_BEGIN_RO()				STM_BEGIN_RD()
// #define TM_END()						STM_END()
// #define TM_RESTART()					STM_RESTART()
// #define TM_SHARED_READ(var)			STM_READ(var)
// #define TM_SHARED_READ_P(var)		STM_READ_P(var)
// #define TM_SHARED_READ_F(var)		STM_READ_F(var)
// #define TM_SHARED_WRITE(var, val)	STM_WRITE((var), val)
// #define TM_SHARED_WRITE_P(var, val)	STM_WRITE_P((var), val)
// #define TM_SHARED_WRITE_F(var, val)	STM_WRITE_F((var), val)
// #define TM_LOCAL_WRITE(var, val)		STM_LOCAL_WRITE(var, val)
// #define TM_LOCAL_WRITE_P(var, val)	STM_LOCAL_WRITE_P(var, val)
// #define TM_LOCAL_WRITE_F(var, val)	STM_LOCAL_WRITE_F(var, val)

#ifndef STM_H
#define STM_H 1

#include <stdint.h>
#include "pgtm.h"
#include "util.h"

#define STM_THREAD_T					Thread // Thread structure
#define STM_SELF						Self // Name of Thread argument
#define STM_RO_FLAG						ROFlag // Name of Read-Only flag in the Thread structure

#define STM_MALLOC(size)				PgtmAlloc(STM_SELF, size) // allocate memory, return pointer
#define STM_FREE(ptr)					PgtmFree(STM_SELF, ptr) // free allocated memory from pointer

#define STM_VALID()						(1)
#define STM_RESTART()					PgtmAbort(STM_SELF)

#define STM_STARTUP()					PgtmStartup()
#define STM_SHUTDOWN()					PgtmShutdown()

#define STM_NEW_THREAD()				PgtmNewThread()
#define STM_INIT_THREAD(t, id)			PgtmInitThread(t, id)
#define STM_FREE_THREAD(t)				PgtmFreeThread(t)

#define STM_BEGIN(isReadOnly)			do { \
											int STM_RO_FLAG = isReadOnly; \
											PgtmBegin(STM_SELF, &STM_RO_FLAG); \
										} while (0) /* enforce comma */

#define STM_BEGIN_RD()					STM_BEGIN(1)
#define STM_BEGIN_WR()					STM_BEGIN(0)
#define STM_END()						PgtmEnd(STM_SELF)

typedef volatile intptr_t vintp;

#define STM_READ(var)					PgtmRead(STM_SELF, (vintp*)(void*)&(var))
#define STM_READ_F(var)					IP2F(PgtmRead(STM_SELF, \
											(vintp*)FP2IPP(&(var))))
#define STM_READ_P(var)					IP2VP(PgtmRead(STM_SELF, \
											(vintp*)(void*)&(var)))

#define STM_WRITE(var, val)				PgtmWrite(STM_SELF, \
											(vintp*)(void*)&(var), \
											(intptr_t)(val))
#define STM_WRITE_F(var, val)			PgtmWrite(STM_SELF, \
											(vintp*)FP2IPP(&(var)), \
											F2IP(val))
#define STM_WRITE_P(var, val)			PgtmWrite(STM_SELF, \
											(vintp*)(void*)&(var), \
											VP2IP(val))

#define STM_LOCAL_WRITE(var, val)		({var = val; var;})
#define STM_LOCAL_WRITE_F(var, val)		({var = val; var;})
#define STM_LOCAL_WRITE_P(var, val)		({var = val; var;})


#endif /* STM_H */
