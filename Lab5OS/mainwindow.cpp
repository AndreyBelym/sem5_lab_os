#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mailslots.h"
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    num=0;
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    uuid=QUuid::createUuid();
    MakeFile(hNotify,PutServerNotify);
    SendNotify(hNotify,uuid);
    //ui->graphicsView_2->setAttribute(Qt::WA_OpaquePaintEvent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_send_triggered()
{
    QBuffer buffer;
    int w=scene.width()+1,h=scene.height()+1;
    QRectF src=scene.sceneRect(),dst;
    dst.setBottomLeft(QPoint(0,0));
    src.setWidth(w); src.setHeight(h);
    dst.setWidth(w); dst.setHeight(h);
    QPixmap picture(w,h);
    picture.fill(QColor("white"));
    QPainter painter(&picture);
    scene.render(&painter,dst,src);
    painter.end();
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out<<picture;
    buffer.close();
    qDebug()<<"Sending:"<<buffer.size();
    GenUUIDSlot(hSlot,uuid);
    TransmitData(hSlot,buffer.data().constData(),buffer.data().size(),num);
    CloseHandle(hSlot);
}


void MainWindow::on_action_load_triggered()
{
    QString name=QFileDialog::getOpenFileName();
    if(name!=""){
        QPixmap pixmap;
        pixmap.load(name);
        scene.clear();
        scene.bg=scene.addPixmap(pixmap);
    }
}
