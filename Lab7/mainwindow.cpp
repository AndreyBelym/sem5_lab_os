#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
extern "C"{
#include <unistd.h>
}

pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
double **A;

void *my_thread(void *arg){
    thread_arg* p=(thread_arg*)arg;
    int num=p->num,
        n=p->n,
        m=p->m,
        count=p->count;
    for(int i=num;i<n;i+=count){
        for(int j=0;j<m;++j){
            pthread_mutex_lock(&mutexA);
            A[i][j]=i+j+2;
            pthread_mutex_unlock(&mutexA);
        }
    }

    pthread_exit(NULL);
}

void calcMatrix(int n,int m,int count){
    QList<pthread_t*> mythreads;
    QList<thread_arg*> myargs;
    for(int i=0;i<count;++i){

        thread_arg* arg=new thread_arg;
        arg->count=count;
        arg->n=n;
        arg->m=m;
        arg->num=i;

        pthread_t* new_thread = new pthread_t;

        if(pthread_create(new_thread,NULL,&my_thread,arg)){
                qDebug()<<"Ошибка создания потока!";
                delete arg;
                delete new_thread;
        }
        myargs<<arg;
        mythreads<<new_thread;
    }

    while(mythreads.size()){

        if(pthread_join(*mythreads.first(),NULL))
            qDebug()<<"Ошибка ожидания потока!";

        delete mythreads.first();
        delete myargs.first();
        myargs.removeFirst();
        mythreads.removeFirst();
    }

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    int d=ui->tableWidget->rowCount()-arg1;
    if(d<0){
        for(;d<0;d++){
            int i=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(i);
            for(int j=0;j<ui->tableWidget->columnCount();++j){
                QTableWidgetItem* item=new QTableWidgetItem;
                ui->tableWidget->setItem(i,j,item);

            }
        }
    } else {
        for(;d>0;d--){
            ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
        }
    }

}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    int d=ui->tableWidget->columnCount()-arg1;
    if(d<0){
        for(;d<0;d++){
            int j=ui->tableWidget->columnCount();
            ui->tableWidget->insertColumn(j);
            for(int i=0;i<ui->tableWidget->rowCount();++i){
                QTableWidgetItem* item=new QTableWidgetItem;
                ui->tableWidget->setItem(i,j,item);

            }
        }
    } else {
        for(;d>0;d--){
            ui->tableWidget->removeColumn(ui->tableWidget->columnCount()-1);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    int n=ui->tableWidget->rowCount(),
        m=ui->tableWidget->columnCount(),
        count=ui->spinBox_3->value();


    A=new double*[n];
    for(int i=0;i<n;++i){
        A[i]=new double[m];
    }


    calcMatrix(n,m,count);

    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
            QTableWidgetItem *item=ui->tableWidget->item(i,j);
            if(item!=NULL)
                item->setText(QString::number(A[i][j]));
        }
    }

    for(int i=0;i<n;++i)
        delete A[i];
    delete A;
}

void MainWindow::on_pushButton_2_clicked()
{
    int n=ui->tableWidget->rowCount(),
        m=ui->tableWidget->columnCount();
    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
            QTableWidgetItem *item=ui->tableWidget->item(i,j);
            if(item!=NULL)
                item->setText(QString());
        }
    }
}
