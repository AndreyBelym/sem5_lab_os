#include "stdafx.h"
#include "Threads.h"
HWND win=NULL;
DWORD sleep_t=0;
DWORD WINAPI ThreadFunc(VOID* PARAMS){
	HANDLE my=*(HANDLE*)PARAMS;
	
	while (true){
		//AfxMessageBox(msg);
		SendMessage(win,WM_UPDATELOG,WPARAM(my),0);
		//edit->SetWindowTextW(CString("asd"));
		Sleep(sleep_t);
	}
	return 0;
}

bool SpawnThread(THREADLIST& threads){
	threads.AddTail(HANDLE());
	HANDLE& new_handle=threads.GetTail();
	if((new_handle=CreateThread(NULL,0,&ThreadFunc,(void*)&new_handle,0,NULL))==NULL){
		CString msg("Ошибка запуска потока!");
		AfxMessageBox(msg);
		threads.RemoveTail();
		return false;
	} ;
	return true;
}

bool KillThread(THREADLIST& threads,INT_PTR num){
	POSITION pos=threads.FindIndex(num);
	HANDLE del_handle=threads.GetAt(pos);
	if (TerminateThread(del_handle,1)){
		threads.RemoveAt(pos);
		return true;
	} else return false;
}