#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QDebug>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MakeSlot(hNotify,GetServerNotify);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_receive_triggered()
{
    char *data; int size;
    ui->action_clients->trigger();
    for(int i=0;i<hSlots.size();++i){
        if(ReceiveData(hSlots[i],data,size,*states[i])){

            QBuffer buffer;
            QPixmap picture;

            buffer.setData(QByteArray::fromRawData(data,size));

            buffer.open(QIODevice::ReadOnly);
            QDataStream in(&buffer);
            in>>picture;
            buffer.close();
            scenes[i]->clear();
            scenes[i]->addPixmap(picture);
            delete data;
        }
    }
}

void MainWindow::on_action_clients_triggered()
{
    QUuid uuid; HANDLE hSlot;
    if(ReceiveNotify(hNotify,uuid)){
        qDebug()<<uuid.toString();
        if(uuids.indexOf(uuid)==-1){
            ServerState *state=new ServerState;
            state->current_num=0;state->first_run=true;
            states<<state;
            OpenUUIDSlot(hSlot,uuid);
            uuids<<uuid;
            hSlots<<hSlot;
            QGraphicsView *gv=new QGraphicsView;
            QGraphicsScene *gs=new QGraphicsScene;
            scenes<<gs;
            gv->setScene(gs);
            ui->tabWidget->addTab(gv,uuid.toString());
        }
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    CloseHandle(hSlots[index]); hSlots.removeAt(index);
    uuids.removeAt(index);
    delete scenes[index];scenes.removeAt(index);
    ui->tabWidget->removeTab(index);
}
