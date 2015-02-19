#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <wchar.h>

#define ADD_TO_LOG(msg) ui->plainTextEdit->appendPlainText(QString()<<msg);

QString &operator<<(QString s,const QString &other){
    return s+=other;
}

QString &operator<<(QString s,const char* other){
    return s+=QString(other);
}

QString &operator<<(QString s,const int other){
    QString tmp;
    return s+=tmp.sprintf("%d",other);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    hPipe=INVALID_HANDLE_VALUE;
    ui->setupUi(this);
    ui->dockWidget->setFloating(true);
    ui->dockWidget->close();
    statusBar()->showMessage("Не подключено.");
    complete.setCaseSensitivity(Qt::CaseSensitive);
    ui->comboBox->setCompleter(&complete);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::OpenPipe(LPTSTR lpszPipename){
    // Try to open a named pipe; wait for it, if necessary.
    if(hPipe!=INVALID_HANDLE_VALUE)
        CloseHandle(hPipe);
    DWORD dwMode;
    BOOL fSuccess = FALSE;
    int e;
       while (1)
       {

          hPipe = CreateFile(
             lpszPipename,   // pipe name
             GENERIC_READ |  // read and write access
             GENERIC_WRITE,
             0,              // no sharing
             NULL,           // default security attributes
             OPEN_EXISTING,  // opens existing pipe
             0,              // default attributes
             NULL);          // no template file

       // Break if the pipe handle is valid.

          if (hPipe != INVALID_HANDLE_VALUE)
             break;

          // Exit if an error other than ERROR_PIPE_BUSY occurs.

          if ((e=GetLastError()) != ERROR_PIPE_BUSY)
          {
             ADD_TO_LOG("Could not open pipe. GLE="<<e<<".\n");
             return e;
          }

          // All pipe instances are busy, so wait for 20 seconds.

          if ( ! WaitNamedPipe(lpszPipename, 20000))
          {
             ADD_TO_LOG("Could not open pipe: 20 second wait timed out.");
             return WAIT_TIMEOUT;
          }
       }

    // The pipe connected; change to message-read mode.

       dwMode = PIPE_READMODE_MESSAGE;
       fSuccess = SetNamedPipeHandleState(
          hPipe,    // pipe handle
          &dwMode,  // new pipe mode
          NULL,     // don't set maximum bytes
          NULL);    // don't set maximum time
       if ( ! fSuccess)
       {
          e=GetLastError();
          ADD_TO_LOG("SetNamedPipeHandleState failed. GLE="<<e<<".\n");
          return e;
       }
       return 0;

}

int MainWindow::Communicate(LPTSTR lpvMessage){
       int e;
       BOOL fSuccess = FALSE;
       TCHAR  chBuf[BUFSIZE];
       DWORD  cbRead, cbToWrite, cbWritten;
       cbToWrite = (lstrlen(lpvMessage)+1)*sizeof(TCHAR);
       ADD_TO_LOG("Sending "<<cbToWrite<<" byte message: \""<<QString::fromWCharArray(lpvMessage)<<"\"\n");
       fSuccess = WriteFile(
          hPipe,                  // pipe handle
          lpvMessage,             // message
          cbToWrite,              // message length
          &cbWritten,             // bytes written
          NULL);                  // not overlapped

       if ( ! fSuccess)
       {
          e=GetLastError();
          ADD_TO_LOG("WriteFile to pipe failed. GLE="<<e<<".\n");
          return e;
       }

       ADD_TO_LOG("\nMessage sent to server, receiving reply as follows:\n");

       do
       {
       // Read from the pipe.

          fSuccess = ReadFile(
             hPipe,    // pipe handle
             chBuf,    // buffer to receive reply
             BUFSIZE*sizeof(TCHAR),  // size of buffer
             &cbRead,  // number of bytes read
             NULL);    // not overlapped

          if ( ! fSuccess && GetLastError() != ERROR_MORE_DATA )
             break;
          QString res=QString::fromWCharArray(chBuf);
          ADD_TO_LOG("Answer received from server:\""<<res<<"\".\n");
          ui->textBrowser->append(res);
       } while ( ! fSuccess);  // repeat loop if ERROR_MORE_DATA

       if ( ! fSuccess)
       {
          e=GetLastError();
          ADD_TO_LOG("ReadFile from pipe failed. GLE="<<e<<".\n");
          return e;
       }
       return 0;

}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    ui->action_eval->trigger();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString server;
    if(ui->radioButton->isChecked()){
        const char *serv_name=ui->lineEdit->text().toAscii().constData();
        server.sprintf(pipe_name,serv_name);
        QString tmp;
        statusBar()->showMessage(tmp.sprintf("Подключение к %s...",serv_name));
    } else {
        server.sprintf(pipe_name,".");
        statusBar()->showMessage("Подключение к локальному серверу...");
    }
    qDebug()<<server;
    LPTSTR buf=new wchar_t[server.length()+1];
    server.toWCharArray(buf);
    buf[server.length()]='\0';
    int e;
    if(!(e=OpenPipe(buf))){
        if(ui->radioButton->isChecked()){
            const char *serv_name=ui->lineEdit->text().toAscii().constData();
            QString tmp;
            statusBar()->showMessage(tmp.sprintf("Подключено к %s.",serv_name));
        } else {
            statusBar()->showMessage("Подключено к локальному серверу.");
        }
    } else {
        QString tmp;
        statusBar()->showMessage(tmp.sprintf("Ошибка подключения #%d.",e));
    }
    delete buf;
}

void MainWindow::on_action_eval_triggered()
{
    QString expr=ui->comboBox->currentText();
    LPTSTR buf=new wchar_t[expr.length()+1];
    expr.toWCharArray(buf);
    buf[expr.length()]='\0';
    Communicate(buf);
    delete buf;
}

void MainWindow::on_pushButton_clicked()
{
    ui->comboBox->addItem(ui->comboBox->currentText());
    ui->action_eval->trigger();
}
