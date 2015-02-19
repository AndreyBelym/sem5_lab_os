#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <cctype>



MAPPING OpenFileMap(QString name);
void CloseFileMap(MAPPING m);
void SaveFileMap(QString name,MAPPING m);

int swapcase(int c){
    if (isupper(c))
        return tolower(c);
    else
        return toupper(c);
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

void MainWindow::on_action_triggered()
{
    QString name=QFileDialog::getOpenFileName(this,"Открытие файла","","Текстовые файлы (*.txt)");
    if(name!=""){
        m=OpenFileMap(name);
        ui->plainTextEdit->setPlainText(QString(m.view));
    };
}

void MainWindow::on_action_2_triggered()
{
    QString name=QFileDialog::getSaveFileName(this,"Сохранение файла","","Текстовые файлы (*.txt)");
    if(name!="")
        SaveFileMap(name,m);
}

void CloseFileMap(MAPPING m){
    FlushViewOfFile(m.view,0);

    UnmapViewOfFile(m.view);
    CloseHandle(m.mapping);
    CloseHandle(m.file);
}

MAPPING OpenFileMap(QString name){
    MAPPING res;
    LARGE_INTEGER size;
    int namesize=name.length();
    wchar_t *buf=new wchar_t[namesize+1];
    buf[namesize]='\0';
    name.toWCharArray(buf);
    if((res.file=CreateFile(buf,GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL))!=INVALID_HANDLE_VALUE){
        GetFileSizeEx(res.file,&size);
        qDebug()<<size.HighPart;
        qDebug()<<size.LowPart;
        //size.QuadPart++;
        if((res.mapping=CreateFileMapping(res.file,NULL,PAGE_WRITECOPY,0,0,NULL))!=NULL){
            res.view=(char*)MapViewOfFile(res.mapping,FILE_MAP_COPY,0,0,0);
            qDebug()<<int(res.view[size.LowPart]);
            //res.view[0]='\0';
        } else {
            qDebug()<<GetLastError();
        }
    }
    return res;
}

void SaveFileMap(QString name,MAPPING m){
    MAPPING res;
    LARGE_INTEGER size;
    int namesize=name.length();
    wchar_t *buf=new wchar_t[namesize+1];
    buf[namesize]='\0';
    name.toWCharArray(buf);
    if((res.file=CreateFile(buf,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL))!=INVALID_HANDLE_VALUE){
        GetFileSizeEx(m.file,&size);
        qDebug()<<size.QuadPart;
        SetFilePointerEx(res.file,size,NULL,FILE_BEGIN);
        SetEndOfFile(res.file);
        if((res.mapping=CreateFileMapping(res.file,NULL,PAGE_READWRITE,0,0,NULL))!=NULL){
            res.view=(char*)MapViewOfFile(res.mapping,FILE_MAP_ALL_ACCESS,0,0,0);

            strcpy(res.view,m.view);

            CloseFileMap(res);
        } else {
            qDebug()<<GetLastError();
        }
    }
}

void MainWindow::ChangeCase(casefunc f){
    QTextCursor curs=ui->plainTextEdit->textCursor();
    if (curs.hasSelection()){
        int start=curs.anchor()>curs.position()?curs.position():curs.anchor();
        int stop=curs.anchor()<curs.position()?curs.position():curs.anchor();
        for (int i=start;i<stop;++i){
            m.view[i]=f(m.view[i]);
        }
        ui->plainTextEdit->setPlainText(QString(m.view));

    }
}

void MainWindow::on_action_3_triggered()
{
    ChangeCase(&toupper);
}

void MainWindow::on_action_4_triggered()
{
    ChangeCase(&tolower);
}

void MainWindow::on_action_5_triggered()
{
    ChangeCase(&swapcase);
}

void MainWindow::on_action_7_triggered()
{
    CloseFileMap(m);
    ui->plainTextEdit->clear();
}

void MainWindow::on_action_8_triggered()
{
    this->close();
}
