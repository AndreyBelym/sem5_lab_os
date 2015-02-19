#ifndef LAB1_THREADS_H
#define LAB1_THREADS_H
#include <afxtempl.h>

#define WM_UPDATELOG (WM_APP+1)

typedef CList<HANDLE> THREADLIST;

extern HWND win;
extern DWORD sleep_t;

DWORD WINAPI ThreadFunc(VOID* PARAMS);
bool SpawnThread(THREADLIST& threads);
bool KillThread(THREADLIST& threads,INT_PTR num);

#endif //LAB1_THREADS_H