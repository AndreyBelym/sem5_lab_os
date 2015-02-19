#ifndef LAB2THREADS_H
#define LAB2THREADS_H
extern "C"{
    #include <windows.h>
}
#include <QObject>
#include <QList>
#include <QTableWidget>
#include <QPlainTextEdit>
#define _EnterCriticalSection(CS) if(use_sync)EnterCriticalSection(CS);
#define _LeaveCriticalSection(CS) if(use_sync)LeaveCriticalSection(CS);
#define _WaitForSingleObject(OBJ,TIME) if(use_sync)WaitForSingleObject(OBJ,TIME);
#define _ReleaseSemaphore(SEM,CNT,WTF) if(use_sync)ReleaseSemaphore(SEM,CNT,WTF);
extern int K;
extern bool use_sync;
extern HANDLE FreeSem,UsedSem;


typedef struct CONSDESC{
    HANDLE thread;
    struct PRODDESC* partner;
    double hairs;
    CRITICAL_SECTION guard;
    HANDLE enter,exit;
} CONSDESC;

typedef struct PRODDESC{
    HANDLE thread;
    struct CONSDESC* partner;
    double cutting_rate;
    bool status;
    CRITICAL_SECTION guard;
    HANDLE enter,exit;
} PRODDESC;

typedef QList<CONSDESC*> CONSLIST;
typedef QList<PRODDESC*> PRODLIST;

class ThreadManager: public QObject
{
    Q_OBJECT
    public:
    explicit ThreadManager():QObject(){}
        ~ThreadManager(){}
    //virtual ~ThreadManager();
        void addToLog(QString msg){
            emit appendLog(msg);}
        void updateStats(PRODDESC* p,CONSDESC* c){
            emit changeStats(p,c);
        }
        void emitAddProducer(PRODDESC* p){
            emit addProducer(p);
        }
        void emitDelProducer(PRODDESC* p){
            emit delProducer(p);
        }

        void emitAddConsumer(CONSDESC* p){
            emit addConsumer(p);
        }
        void emitDelConsumer(CONSDESC* p){
            emit delConsumer(p);
        }

    signals:
        void appendLog(QString msg);
        void changeStats(PRODDESC*,CONSDESC*);
        void addProducer(PRODDESC*);
        void delProducer(PRODDESC*);
        void addConsumer(CONSDESC*);
        void delConsumer(CONSDESC*);
};

extern QPlainTextEdit* textlog;
extern QTableWidget* stats;

extern CONSLIST consumers;
extern PRODLIST producers;

extern CRITICAL_SECTION consumers_modifying,producers_modifying;
extern CRITICAL_SECTION log_editing,stats_editing;

extern ThreadManager manager;

void InitSections();

DWORD WINAPI ProducerFunc(LPVOID PARAM);
DWORD WINAPI ConsumerFunc(LPVOID PARAM);

void SpawnConsumer();
void SpawnProducer();

void SafeLogEdit(QString msg);



#endif // LAB2THREADS_H
