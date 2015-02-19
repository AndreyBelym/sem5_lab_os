#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cstdlib>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time_min=ui->doubleSpinBox_2->value();
    time_max=ui->doubleSpinBox->value();
    //ui->listWidget->;
    textlog=ui->plainTextEdit;
    QObject::connect(&manager,SIGNAL(appendLog(QString)),this,SLOT(addToLog(QString)));
    QObject::connect(&manager,SIGNAL(changeStats(PRODDESC*,CONSDESC*)),this,SLOT(updateStats(PRODDESC*,CONSDESC*)),Qt::BlockingQueuedConnection);
    QObject::connect(&manager,SIGNAL(addProducer(PRODDESC*)),this,SLOT(addProducer(PRODDESC*)),Qt::BlockingQueuedConnection);
    QObject::connect(&manager,SIGNAL(delProducer(PRODDESC*)),this,SLOT(delProducer(PRODDESC*)),Qt::BlockingQueuedConnection);
    QObject::connect(&manager,SIGNAL(addConsumer(CONSDESC*)),this,SLOT(addConsumer(CONSDESC*)),Qt::BlockingQueuedConnection);
    QObject::connect(&manager,SIGNAL(delConsumer(CONSDESC*)),this,SLOT(delConsumer(CONSDESC*)),Qt::BlockingQueuedConnection);
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
    InitSections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timer_timeout(){
    SpawnConsumer();
    double t=(1000*(time_max-time_min)*rand())/RAND_MAX+1000*time_min;
    qDebug()<<t;
    timer.setInterval(t);
}

void MainWindow::addProducer(PRODDESC *p){
    QString(temp);
    _EnterCriticalSection(&producers_modifying);
    int i=producers.size();
    producers<<p;
    _LeaveCriticalSection(&producers_modifying);
    if(i!=-1){
       // _EnterCriticalSection(&p->guard);
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *cell=new QTableWidgetItem(0);
        cell->setText(temp.sprintf("%p",p->thread));
        ui->tableWidget->setItem(i,0,cell);
        cell=new QTableWidgetItem(0);
        cell->setText(temp.sprintf("%f",p->cutting_rate));
        ui->tableWidget->setItem(i,1,cell);
        cell=new QTableWidgetItem(0);
        cell->setText("Свободен");
        ui->tableWidget->setItem(i,2,cell);
        cell=new QTableWidgetItem(0);
        ui->tableWidget->setItem(i,3,cell);
        cell=new QTableWidgetItem(0);
        ui->tableWidget->setItem(i,4,cell);
       // _LeaveCriticalSection(&p->guard);
   }
}

void MainWindow::addToLog(QString msg){
    ui->plainTextEdit->appendPlainText(msg);
}
 void MainWindow::updateStats(PRODDESC *p, CONSDESC *c){
     _EnterCriticalSection(&producers_modifying);

     int i=producers.indexOf(p);
     if (i!=-1){
         QString temp;
         QTableWidgetItem* cell=ui->tableWidget->item(i,0);
         cell->setText(temp.sprintf("%p",p->thread));
         cell=ui->tableWidget->item(i,2);
         if(c!=NULL)
             cell->setText(temp.sprintf("%p",c->thread));
         else
             cell->setText("Свободен");
         cell=ui->tableWidget->item(i,3);
         if(c!=NULL)
            cell->setText(temp.sprintf("%d",int(c->hairs)));
         else
             cell->setText("");
         cell=ui->tableWidget->item(i,4);
         if(c!=NULL){
            int v=int(p->cutting_rate*c->hairs);
            cell->setText(temp.sprintf("%d",v));
         }else
             cell->setText("");

     }
     _LeaveCriticalSection(&producers_modifying);
 }

 void MainWindow::delProducer(PRODDESC *p){
     _EnterCriticalSection(&producers_modifying);
    // _EnterCriticalSection(&p->guard);
     int i=producers.indexOf(p);
     if (i!=-1){
         ui->tableWidget->removeRow(i);
     }
     if (i!=-1)
         producers.removeAt(i);
    // _LeaveCriticalSection(&p->guard);
     _LeaveCriticalSection(&producers_modifying);
 };

 void MainWindow::addConsumer(CONSDESC *p){
     _EnterCriticalSection(&consumers_modifying);
     //_EnterCriticalSection(&p->guard);
     int i=consumers.size();
     consumers<<p;
     if (i!=-1){
         QString name;
         ui->tableWidget_2->insertRow(i);
         QTableWidgetItem *cell=new QTableWidgetItem(0);
         name.sprintf("%p",p->thread);
         cell->setText(name);
         ui->tableWidget_2->setItem(i,0,cell);
         cell=new QTableWidgetItem(0);
         name.sprintf("%d",int(p->hairs));
         //cell->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
         cell->setText(name);
         ui->tableWidget_2->setItem(i,1,cell);
     }
     //_LeaveCriticalSection(&p->guard);
     _LeaveCriticalSection(&consumers_modifying);
 };
 void MainWindow::delConsumer(CONSDESC *p){
     _EnterCriticalSection(&consumers_modifying);
     //_EnterCriticalSection(&p->guard);
     int i=consumers.indexOf(p);
     if (i!=-1){
         ui->tableWidget_2->removeRow(i);
     }
     if (i!=-1)
         consumers.removeAt(i);
     //_LeaveCriticalSection(&p->guard);
     _LeaveCriticalSection(&consumers_modifying);
 };

void MainWindow::on_tableWidget_2_itemChanged(QTableWidgetItem *item)
{
    _EnterCriticalSection(&consumers_modifying);
    QString tmp;
    if(item->column()==1&&item->row()<consumers.size()){
       // _EnterCriticalSection(&consumers.at(item->row())->guard);
        bool ok;
        int t=item->text().toInt(&ok);
        if (ok)
            consumers.at(item->row())->hairs=t;
        else
            item->setText(tmp.sprintf("%d",int(consumers.at(item->row())->hairs)));
        qDebug()<<"hairs:"<<consumers.at(item->row())->hairs;
       // _LeaveCriticalSection(&consumers.at(item->row())->guard);
    }
    _LeaveCriticalSection(&consumers_modifying);
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    QTableWidgetItem *item= ui->tableWidget->item(row,column);
    _EnterCriticalSection(&producers_modifying);
    QString tmp;
    if(item->column()==1&&item->row()<producers.size()){
       // _EnterCriticalSection(&producers.at(item->row())->guard);
        bool ok;
        double t=item->text().toDouble(&ok);
        if(ok)
            producers.at(item->row())->cutting_rate=t;
        else
            item->setText(tmp.sprintf("%f",producers.at(item->row())->cutting_rate));
       // _LeaveCriticalSection(&producers.at(item->row())->guard);
    }
    _LeaveCriticalSection(&producers_modifying);
}

void MainWindow::on_spinBox_editingFinished()
{
    _EnterCriticalSection(&producers_modifying);
    if(ui->spinBox->value()>producers.size()){
        if (FreeSem!=NULL)
            CloseHandle(FreeSem);
        if(UsedSem!=NULL)
            CloseHandle(UsedSem);
        FreeSem=CreateSemaphore(NULL,0,ui->spinBox->value(),NULL);
        UsedSem=CreateSemaphore(NULL,0,ui->spinBox->value(),NULL);
        for(int i=producers.size();ui->spinBox->value()>i;++i){

            SpawnProducer();
        }
    }

    for(int i=producers.size()-1;ui->spinBox->value()<i+1;--i){
       // _EnterCriticalSection(&producers.at(i)->guard);
        producers.at(i)->status=false;
       // _LeaveCriticalSection(&producers.at(i)->guard);
     }
    _LeaveCriticalSection(&producers_modifying);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
        timer.start();
    else if(arg1==Qt::Unchecked)
        timer.stop();
}

void MainWindow::on_spinBox_2_editingFinished()
{
    K=ui->spinBox_2->value();
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    use_sync=arg1==Qt::Checked;
}

void MainWindow::on_doubleSpinBox_2_editingFinished()
{
    if(ui->doubleSpinBox_2->value()<=time_max)
        time_min=ui->doubleSpinBox_2->value();
    else
        ui->doubleSpinBox_2->setValue(time_min);
}

void MainWindow::on_doubleSpinBox_editingFinished()
{
    if(ui->doubleSpinBox->value()>=time_min)
        time_max=ui->doubleSpinBox->value();
    else
        ui->doubleSpinBox->setValue(time_max);
}
