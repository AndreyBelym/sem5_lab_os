#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <wchar.h>
#include "threadmanager.h"
#define ADD_TO_LOG(msg) manager.emitAddToLog(QString()<<msg);

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

DWORD WINAPI ServiceThread(LPVOID);
VOID GenerateReply(LPTSTR, LPTSTR, LPDWORD);

ThreadManager manager;

DWORD WINAPI MainServerThread(LPVOID PARAMS){
    BOOL   fConnected = FALSE;
    DWORD  dwThreadId = 0;
    HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
    LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\MythematicaPipe");
    for(;;){
        SECURITY_ATTRIBUTES m_pSecAttrib;
        SECURITY_DESCRIPTOR m_pSecDesc;

        InitializeSecurityDescriptor(&m_pSecDesc,
        SECURITY_DESCRIPTOR_REVISION);


        SetSecurityDescriptorDacl(&m_pSecDesc,TRUE,(PACL)NULL,FALSE);

        m_pSecAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
        m_pSecAttrib.bInheritHandle = TRUE;
        m_pSecAttrib.lpSecurityDescriptor = &m_pSecDesc;
        hPipe=CreateNamedPipe(lpszPipename,
                                PIPE_ACCESS_DUPLEX,
                                PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
                                PIPE_UNLIMITED_INSTANCES,
                                BUFSIZE,                  // output buffer size
                                BUFSIZE,                  // input buffer size
                                0,                        // client time-out
                                &m_pSecAttrib);
        if (hPipe == INVALID_HANDLE_VALUE)
         {
             ADD_TO_LOG("CreateNamedPipe failed, GLE="<<GetLastError()<<".\n");
             return -1;
         }

         // Wait for the client to connect; if it succeeds,
         // the function returns a nonzero value. If the function
         // returns zero, GetLastError returns ERROR_PIPE_CONNECTED.

         fConnected = ConnectNamedPipe(hPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

         if (fConnected)
         {
            ADD_TO_LOG("Клиент подключен, запускается обслуживающий поток.\n");

            // Create a thread for this client.
            hThread = CreateThread(
               NULL,              // no security attribute
               0,                 // default stack size
               ServiceThread,    // thread proc
               (LPVOID) hPipe,    // thread parameter
               0,                 // not suspended
               &dwThreadId);      // returns thread ID

            if (hThread == NULL)
            {
               ADD_TO_LOG("CreateThread failed, GLE="<<GetLastError()<<".\n");
               return -1;
            }
            else CloseHandle(hThread);
          }
         else
           // The client could not connect, so close the pipe.
            CloseHandle(hPipe);
    }

    return 0;
}

DWORD WINAPI ServiceThread(LPVOID lpvParam)
// This routine is a thread processing function to read from and reply to a client
// via the open pipe connection passed from the main loop. Note this allows
// the main loop to continue executing, potentially creating more threads of
// of this procedure to run concurrently, depending on the number of incoming
// client connections.
{
  HANDLE hHeap      = GetProcessHeap();
  TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
  TCHAR* pchReply   = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));

  DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
  BOOL fSuccess = FALSE;
  HANDLE hPipe  = NULL;

  // Do some extra error checking since the app will keep running even if this
  // thread fails.

  if (lpvParam == NULL)
  {
      ADD_TO_LOG( "\nERROR - Pipe Server Failure:\n");
      ADD_TO_LOG( "   InstanceThread got an unexpected NULL value in lpvParam.\n");
      ADD_TO_LOG( "   InstanceThread exitting.\n");
      if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
      if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
      return (DWORD)-1;
  }

  if (pchRequest == NULL)
  {
      ADD_TO_LOG( "\nERROR - Pipe Server Failure:\n");
      ADD_TO_LOG( "   InstanceThread got an unexpected NULL heap allocation.\n");
      ADD_TO_LOG( "   InstanceThread exitting.\n");
      if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
      return (DWORD)-1;
  }

  if (pchReply == NULL)
  {
      ADD_TO_LOG( "\nERROR - Pipe Server Failure:\n");
      ADD_TO_LOG( "   InstanceThread got an unexpected NULL heap allocation.\n");
      ADD_TO_LOG( "   InstanceThread exitting.\n");
      if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
      return (DWORD)-1;
  }

  // Print verbose messages. In production code, this should be for debugging only.
  ADD_TO_LOG("Обслуживающий поток создан, ожидает сообщение.\n");

// The thread's parameter is a handle to a pipe object instance.

  hPipe = (HANDLE) lpvParam;

// Loop until done reading
  while (1)
  {
  // Read client requests from the pipe. This simplistic code only allows messages
  // up to BUFSIZE characters in length.
     fSuccess = ReadFile(
        hPipe,        // handle to pipe
        pchRequest,    // buffer to receive data
        BUFSIZE*sizeof(TCHAR), // size of buffer
        &cbBytesRead, // number of bytes read
        NULL);        // not overlapped I/O

     if (!fSuccess || cbBytesRead == 0)
     {
         if (GetLastError() == ERROR_BROKEN_PIPE)
         {
             ADD_TO_LOG("Обслуживающий поток: клиент отключен, ошибка #"<<GetLastError()<<".\n");
         }
         else
         {
             ADD_TO_LOG("Обслуживающий поток: ошибка ReadFile #"<<GetLastError()<<".\n");
         }
         break;
     }

  // Process the incoming message.
     GenerateReply(pchRequest, pchReply, &cbReplyBytes);

  // Write the reply to the pipe.
     fSuccess = WriteFile(
        hPipe,        // handle to pipe
        pchReply,     // buffer to write from
        cbReplyBytes, // number of bytes to write
        &cbWritten,   // number of bytes written
        NULL);        // not overlapped I/O

     if (!fSuccess || cbReplyBytes != cbWritten)
     {
         ADD_TO_LOG("Обслуживающий поток: ошибка WriteFile #"<<GetLastError()<<".\n");
         break;
     }
 }

// Flush the pipe to allow the client to read the pipe's contents
// before disconnecting. Then disconnect the pipe, and close the
// handle to this pipe instance.

  FlushFileBuffers(hPipe);
  DisconnectNamedPipe(hPipe);
  CloseHandle(hPipe);

  HeapFree(hHeap, 0, pchRequest);
  HeapFree(hHeap, 0, pchReply);

  ADD_TO_LOG("InstanceThread exitting.\n");
  return 1;
}

VOID GenerateReply(LPTSTR pchRequest,
                        LPTSTR pchReply,
                        LPDWORD pchBytes )
// This routine is a simple function to print the client request to the console
// and populate the reply buffer with a default data string. This is where you
// would put the actual client request processing code that runs in the context
// of an instance thread. Keep in mind the main thread will continue to wait for
// and receive other client connections while the instance thread is working.
{
    mu::Parser parser;
    QString tmp= QString::fromWCharArray(pchRequest);
   ADD_TO_LOG("Получен запрос от клиента:\""<<tmp<<"\"\n");
   parser.SetExpr(tmp.toAscii().constData());
   QString res;
   try{
       res.sprintf("%f",parser.Eval());
   } catch (mu::Parser::exception_type &e){
       res=QString(e.GetMsg().c_str());
       ADD_TO_LOG("Ошибка парсера:"<<res);
   }
   LPTSTR buf=new wchar_t[res.length()+1];
   res.toWCharArray(buf);
   buf[res.length()]='\0';
   // Check the outgoing message to make sure it's not too long for the buffer.
   if (!wcscpy( pchReply, buf))
   {
       *pchBytes = 0;
       pchReply[0] = 0;
       ADD_TO_LOG("Ошибка при копировании ответного сообщения.\n");
       return;
   }
   *pchBytes = (lstrlen(pchReply)+1)*sizeof(TCHAR);
   delete buf;
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&manager,SIGNAL(addToLog(QString)),this,SLOT(addToLog(QString)),Qt::BlockingQueuedConnection);
    CreateThread(NULL,
                 0,
                 &MainServerThread,
                 (LPVOID)NULL,
                 0,
                 (PDWORD)NULL);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToLog(QString msg){
    ui->plainTextEdit->appendPlainText(msg);
}

