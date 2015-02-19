#include "stdafx.h"
#include "Threads1.h"
HWND win=NULL;
DWORD sleep_t=0;

bool SpawnThread(THREADLIST& threads,LPTHREAD_START_ROUTINE ThreadFunc,THREADDESC params){
	threads.AddTail(params);
	THREADDESC& new_thread=threads.GetTail();
	if((new_thread.tid=CreateThread(NULL,0,ThreadFunc,(void*)&new_thread,0,NULL))==NULL){
		CString msg("Ошибка запуска потока!");
		AfxMessageBox(msg);
		threads.RemoveTail();
		return false;
	} ;
	return true;
}

bool KillThread(THREADLIST& threads,INT_PTR num){
	POSITION pos=threads.FindIndex(num);
	THREADDESC del_thread=threads.GetAt(pos);
	if (TerminateThread(del_thread.tid,1)){
		threads.RemoveAt(pos);
		return true;
	} else return false;
}