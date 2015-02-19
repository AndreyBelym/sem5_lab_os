#ifndef LAB1_THREADS_H
#define LAB1_THREADS_H
#include <afxtempl.h>

#define WM_UPDATELOG (WM_APP+1)

typedef struct _THREADDESC{
	HANDLE tid;
	_THREADDESC& partner;
	time_t time;
	long money;
	int status;
} THREADDESC;
typedef CList<THREADDESC> THREADLIST;

extern HWND win;
extern DWORD sleep_t;

bool SpawnThread(THREADLIST& threads,LPTHREAD_START_ROUTINE ThreadFunc, THREADDESC params);
bool KillThread(THREADLIST& threads,INT_PTR num);

#endif //LAB1_THREADS_H