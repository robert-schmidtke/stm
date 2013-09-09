#ifndef PGTM_H
#define PGTM_H 1

#include <stdint.h>

typedef struct _Thread Thread;

#ifdef __cplusplus
extern "C" {
#endif

void		PgtmBegin		(Thread*, int*); // read only

Thread*		PgtmNewThread	();

void		PgtmFreeThread	(Thread*);
void		PgtmInitThread	(Thread*, long id);
int			PgtmEnd			(Thread*);
void		PgtmAbort		(Thread*);
intptr_t	PgtmRead		(Thread*, volatile intptr_t*);
void		PgtmWrite		(Thread*, volatile intptr_t*, intptr_t);
void		PgtmStartup		();
void		PgtmShutdown	();

void*		PgtmAlloc		(Thread*, size_t);
void		PgtmFree		(Thread*, void*);

#ifdef __cplusplus
}
#endif

#endif /* PGTM_H */
