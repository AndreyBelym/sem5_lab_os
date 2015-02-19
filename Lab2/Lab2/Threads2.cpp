#include "stdafx.h"
#include "Threads2.h"
#include <iostream>
THREADLIST productors,consumers;
DWORD WINAPI ProductorFunc(VOID* PARAMS){
	THREADDESC& params=*(THREADDESC*)PARAMS;
	time_t start=time(NULL);
	CString msg;
	while(true){
		while(!params.partner);
		while(time(NULL)-start<=params.time){
			msg.Format(_T("%ld"),time(NULL));
			AfxMessageBox(msg);
			Sleep(100);
		}
	
	return 0;
}

DWORD WINAPI ConsumerFunc(VOID* PARAMS){
	THREADDESC params=*(THREADDESC*)PARAMS;
	if(consumers.GetCount()<=K){
		consumers.AddTail(params);

	}
}