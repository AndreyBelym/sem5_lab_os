#include <Lab2Threads.h>
#include <cstdio>
#include <QDebug>
#include <cmath>
#include <QMessageBox>

int K=20;
bool use_sync=true;

QPlainTextEdit* textlog;
QTableWidget* stats;

CONSLIST consumers;
PRODLIST producers;
HANDLE FreeSem,UsedSem;
int counter;

CRITICAL_SECTION consumers_modifying,producers_modifying;
CRITICAL_SECTION counter_edit;

ThreadManager manager;

DWORD WINAPI ProducerFunc(LPVOID PARAM){
    PRODDESC *pparam=(PRODDESC*)PARAM,&param=*pparam;
    QString msg;
    msg.sprintf("Парикмахер %p начинает работу.",param.thread);
    SafeLogEdit(msg);

    manager.emitAddProducer(pparam);

    bool wait=false;
    while(true){
        if(!param.status)
            break;
        msg.sprintf("Парикмахер %p ожидает посетителей...",param.thread);
        SafeLogEdit(msg);

        _ReleaseSemaphore(FreeSem,1,NULL);
        _WaitForSingleObject(UsedSem,INFINITE);

        msg.sprintf("Парикмахер %p ищет клиента...",param.thread);
        SafeLogEdit(msg);

        _EnterCriticalSection(&consumers_modifying);

        for (int i = 0; i < consumers.size(); ++i){

            if(consumers.at(i)->partner==NULL){
                consumers[i]->partner=pparam;
                param.partner=consumers[i];

                break;
            }

        }
        _LeaveCriticalSection(&consumers_modifying);

        msg.sprintf("Парикмахер %p вызывает клиента %p.",param.thread,param.partner->thread);
        SafeLogEdit(msg);

        SetEvent(param.partner->enter);
        _WaitForSingleObject(param.enter,INFINITE);
        ResetEvent(param.enter);

        msg.sprintf("Парикмахер %p начинает стричь клиента %p.",param.thread,param.partner->thread);
        SafeLogEdit(msg);

        wait=true;
        while(wait){

            wait=((param.partner->hairs)>1);

            param.partner->hairs-=1;

            double t=param.cutting_rate*param.partner->hairs;

            if(t-int(t)<0.1){

                manager.updateStats(pparam,param.partner);

            }

            if(wait)
                Sleep(1000*param.cutting_rate);
        }
        msg.sprintf("Парикмахер %p закончил стрижку %p.",param.thread,param.partner->thread);
        SafeLogEdit(msg);

        SetEvent(param.partner->exit);

        _WaitForSingleObject(param.exit,INFINITE);
        ResetEvent(param.exit);

        manager.updateStats(pparam,NULL);

    }

    msg.sprintf("Парикмахер %p завершает работу.",param.thread);
    SafeLogEdit(msg);
    manager.emitDelProducer(pparam);

    delete pparam;
    return 0;
}

DWORD WINAPI ConsumerFunc(LPVOID PARAM){

    CONSDESC *pparam=(CONSDESC*)PARAM,&param=*pparam;
    QString msg;

    msg.sprintf("Клиет %p приходит в парикмахерскую!",param.thread);
    SafeLogEdit(msg);
    int i;bool wait=true;

    if(FreeSem==NULL||UsedSem==NULL){
        msg.sprintf("Парикмахерская пуста, клиент %p уходит. Добавьте парикмахеров!!!",param.thread);
        SafeLogEdit(msg);
        delete pparam;
        return 0;
    }

    _EnterCriticalSection(&consumers_modifying);
    if(consumers.size()>=K){
        msg.sprintf("Очередь %d, клиент %p уходит!!!",consumers.size(),param.thread);
        SafeLogEdit(msg);
        _LeaveCriticalSection(&consumers_modifying);
        delete pparam;
        return 1;

    };

    _LeaveCriticalSection(&consumers_modifying);

    manager.emitAddConsumer(pparam);


    _WaitForSingleObject(FreeSem,INFINITE);
    _ReleaseSemaphore(UsedSem,1,NULL);

    msg.sprintf("Клиент %p заходит в парикмахерскую.",param.thread);
    SafeLogEdit(msg);

    _WaitForSingleObject(param.enter,INFINITE);

    msg.sprintf("Клиент %p заходит к парикмахеру %p.",param.thread,param.partner->thread);
    SafeLogEdit(msg);

    manager.emitDelConsumer(pparam);

    SetEvent(param.partner->enter);

    _WaitForSingleObject(param.exit,INFINITE);

    msg.sprintf("Клиент %p пострижен и уходит.",param.thread);
    SafeLogEdit(msg);
    CloseHandle(param.enter);
    CloseHandle(param.exit);

    SetEvent(param.partner->exit);

    delete pparam;
    return 0;
}

void InitSections(){
    FreeSem=NULL;
    UsedSem=NULL;
    InitializeCriticalSection(&producers_modifying);
    InitializeCriticalSection(&consumers_modifying);
    InitializeCriticalSection(&counter_edit);
}

void SpawnConsumer(){
    CONSDESC *new_consumer=new CONSDESC;
    new_consumer->hairs=10;
    new_consumer->partner=NULL;
    new_consumer->thread=NULL;
    new_consumer->enter=CreateEvent(NULL,FALSE,FALSE,NULL);
    new_consumer->exit=CreateEvent(NULL,FALSE,FALSE,NULL);
    InitializeCriticalSection(&new_consumer->guard);
    if((new_consumer->thread=CreateThread(NULL,0,&ConsumerFunc,(PVOID*)new_consumer,0,NULL))==NULL)
        printf("Error\n");
}

void SpawnProducer(){
    PRODDESC *new_producer=new PRODDESC;
    new_producer->cutting_rate=1;
    new_producer->partner=NULL;
    new_producer->status=true;
    new_producer->thread=NULL;
    new_producer->enter=CreateEvent(NULL,FALSE,FALSE,NULL);
    new_producer->exit=CreateEvent(NULL,FALSE,FALSE,NULL);
    InitializeCriticalSection(&new_producer->guard);
    if((new_producer->thread=CreateThread(NULL,0,&ProducerFunc,(PVOID*)new_producer,0,NULL))==NULL)
        printf("Error\n");
}

void SafeLogEdit(QString msg){

    manager.addToLog(msg);
    qDebug()<<msg;

}
