#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
extern "C"{
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
}
#include <cerrno>
#include <cstdio>
#include <QTime>
#include "threadhelper.h"

ThreadHelper threadHelper;

const char *html_template="<html>"
                            "<head/>"
                            "<body>"
                                "(%1) <span style=\"color:blue;font-style:italic\">%2</span>:"
                            "</body>"
                          "</html>"
                          "%3";

const char *key_name="/home/";
const size_t default_size=0x1000;
enum {
    shmem_key_id=1,
    shmem_lock_key_id,
    sem_notified_key_id,
};


int shmem,shmem_lock,sem_notified; char *shmdata=NULL;int *clients_count;

void SemLock(int semid){
    sembuf ops;
    ops.sem_num=0;
    ops.sem_flg=0;
    ops.sem_op=-1;
    semop(semid,&ops,1);
}

void SemUnlock(int semid){
    sembuf ops;
    ops.sem_num=0;
    ops.sem_flg=0;
    ops.sem_op=1;
    semop(semid,&ops,1);
}

void *updater(void *args){
    sembuf ops;

    ops.sem_flg=0;
    ops.sem_num=0;
    while(true){
        ops.sem_op=-1;
        if(semop(sem_notified,&ops,1)<0)
            pthread_exit(NULL);

        threadHelper.emitAddMessage(shmdata);

        ops.sem_op=0;
        if(semop(sem_notified,&ops,1)<0)
            pthread_exit(NULL);

    }
    pthread_exit(NULL);
}

bool CreateShMemLock(int semid,const int key_id){
    key_t key;
    if((key=ftok(key_name,key_id))<0){
        perror(NULL);
        qDebug()<<"Key error!";
        return false;
    }
    if((semid=semget(key,1,IPC_CREAT|0600))<0){
        if(errno==EEXIST){
            if((semid=semget(key,1,0600))>=0)
                return true;
        }
        perror(NULL);
        qDebug()<<"init sem error!";
        return false;
    } else {
        sembuf ops;
        ops.sem_op=1;
        ops.sem_num=0;
        ops.sem_flg=0;
        semop(semid,&ops,1);
    };
    return true;
}

bool CreateSem(int &semid,const int key_id){
    key_t key;
    if((key=ftok(key_name,key_id))<0){
        perror(NULL);
        qDebug()<<"Key error!";
        return false;
    }
    if((semid=semget(key,1,IPC_CREAT|0666))<0){
        perror(NULL);
        qDebug()<<"init sem error!";
        return false;
    };
    return true;
}

bool CreateShMem(int &shmid,void *&data){
    key_t key;
    if((key=ftok(key_name,shmem_key_id))<0){
        perror(NULL);
        qDebug()<<"Key error!";
        return false;
    }
    if((shmid=shmget(key,default_size,IPC_CREAT|0666))<0){
        perror(NULL);
        qDebug()<<"shmget error!";
        return false;
    }
    if((data=shmat(shmid,NULL,0))==(void*)-1){
        perror(NULL);
        qDebug()<<"shmat error!";
        return false;
    }
    return true;
}
void SendMessage(const char *msg){
	
    sembuf ops;
    ops.sem_op=0;
    ops.sem_flg=0;
    ops.sem_num=0;

    semop(sem_notified,&ops,1);

    SemLock(shmem_lock);
    strcpy(shmdata,msg);
    SemUnlock(shmem_lock);

    ops.sem_op=*clients_count;

    semop(sem_notified,&ops,1);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&threadHelper,SIGNAL(addMessage(char*)),this,SLOT(addMessage(char*)));

    if(!CreateShMemLock(shmem_lock,shmem_lock_key_id))
        exit(-1);
    if(!CreateSem(sem_notified,sem_notified_key_id))
        exit(-1);

    void *temp;
    if(!CreateShMem(shmem,temp))
        exit(-1);

    clients_count=(int*)temp;

    SemLock(shmem_lock);
    (*clients_count)++;
    SemUnlock(shmem_lock);

    shmdata=(char*)(clients_count+1);

    pthread_t new_thread;
    pthread_create(&new_thread,NULL,&updater,NULL);
}

MainWindow::~MainWindow()
{
    delete ui;

    SemLock(shmem_lock);
    (*clients_count)--;
    bool last=*clients_count==0;
    SemUnlock(shmem_lock);

    if(last){
        shmdt(shmdata);
        shmctl(shmem,IPC_RMID,NULL);
        semctl(sem_notified,0,IPC_RMID);
        semctl(shmem_lock,0,IPC_RMID);
    } else {
        shmdt(shmdata);
    }
}


void MainWindow::addMessage(char *msg){
    ui->textEdit_2->insertHtml(QString::fromUtf8(msg));
    ui->textEdit_2->append(QString(""));
}


void MainWindow::on_pushButton_clicked()
{
    QString msg=QString(html_template).arg(QTime::currentTime().toString("HH:mm:ss"),ui->lineEdit->text(),ui->textEdit->toHtml());
    SendMessage(msg.toUtf8().constData());
}
