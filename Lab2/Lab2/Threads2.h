#ifndef LAB2_THREADS_H
#define LAB2_THREADS_H
#include "Threads1.h"

extern THREADLIST productors,consumers;
DWORD WINAPI ProductorFunc(VOID* PARAMS);
DWORD WINAPI ConsumerFunc(VOID* PARAMS);

#endif