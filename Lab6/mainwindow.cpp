#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
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

double f(double x){
    return exp(x)-pow(x,M_E);
}

void MainWindow::on_pushButton_clicked()
{
    bool ok;
    double x=ui->lineEdit->text().toDouble(&ok);
    if (ok){
        QString txt;
        ui->label_3->setText(txt.sprintf("f(%f)=%f",x,f(x)));
    }
}
