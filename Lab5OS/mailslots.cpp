#include "mailslots.h"
#include <QDebug>
#include <QByteArray>
BOOL WriteSlot(HANDLE hSlot, void* lpszMessage,int size)
{
   BOOL fResult;
   DWORD cbWritten;

   fResult = WriteFile(hSlot,
     lpszMessage,
     size,
     &cbWritten,
     NULL);

   if (!fResult)
   {
      int e=GetLastError();
      qDebug()<<QString("WriteFile failed with %1.").arg(QString::number(e));
      return FALSE;
   }

   return TRUE;
}

bool TransmitData(HANDLE hFile, const char *source, int size,int &num){
   int psource=0,pdata=0;
   Package package;

   package.type=Package_Head;
   package.num=num;num++;
   package.total_size=size;

   WriteSlot(hFile,&package,MAXLEN);
  // WriteSlot(hFile,&package,MAXLEN);
   //WriteSlot(hFile,&package,MAXLEN+1);

   while(psource+MAXDATA<=size){
       package.type=Package_Body;
       package.num=num;num++;
       for (pdata=0;pdata<MAXDATA&&psource<size;++pdata,++psource){
           package.data[pdata]=source[psource];
       }

       WriteSlot(hFile,&package,MAXLEN);
       //Sleep(10);
       //WriteSlot(hFile,&package,MAXLEN);
       //WriteSlot(hFile,&package,MAXLEN+1);
   }

   package.type=Package_Tail;
   package.num=num;num++;
   for (pdata=0;pdata<MAXDATA&&psource<size;++pdata,++psource){
       package.data[pdata]=source[psource];
   }

   WriteSlot(hFile,&package,MAXLEN);
   //WriteSlot(hFile,&package,MAXLEN);
   //WriteSlot(hFile,&package,MAXLEN);

   return true;
}

void GenUUIDSlot(HANDLE &hSlot,QUuid &uuid){
    QString slotname=QString("\\\\*\\mailslot\\%1").arg(uuid.toString());
    wchar_t *lpSlotName=new wchar_t[slotname.length()];
    slotname.toWCharArray(lpSlotName);
    MakeFile(hSlot,lpSlotName);
    delete lpSlotName;
}

void OpenUUIDSlot(HANDLE &hSlot,QUuid &uuid){
    QString slotname=QString("\\\\.\\mailslot\\%1").arg(uuid.toString());
    wchar_t *lpSlotName=new wchar_t[slotname.length()];
    slotname.toWCharArray(lpSlotName);
    MakeSlot(hSlot,lpSlotName);
    delete lpSlotName;
}

void SendNotify(HANDLE &hSlot,QUuid &uuid){
    char *data=uuid.toByteArray().data();
    int size=uuid.toByteArray().size();
    qDebug()<<uuid.toString();
    qDebug()<<size;
    WriteSlot(hSlot,data,size);
}

bool ReceiveNotify(HANDLE &hSlot,QUuid &uuid){
    DWORD size;
    DWORD fResult=GetMailslotInfo( hSlot, // mailslot handle
            (LPDWORD) NULL,               // no maximum message size
            &size,                   // size of next message
            NULL,                    // number of messages
            (LPDWORD) NULL);
    if (!fResult)
    {
        qDebug()<<"GetMailslotInfo failed with "<<GetLastError();
        return false;
    }
    if(size!=MAILSLOT_NO_MESSAGE){
        char data[size];
        qDebug()<<size;
        fResult=ReadFile(hSlot,
                    data,
                    size,
                    NULL,
                    NULL);
        if (!fResult)
        {
            qDebug()<<"ReadFile failed with "<<GetLastError();
            return false;
        }
        uuid=QUuid(QByteArray(data,size));
     } else
        return false;
    return true;
}

bool MakeFile(HANDLE &hFile,LPCTSTR SlotName){

    hFile = CreateFile(SlotName,
      GENERIC_WRITE,
      FILE_SHARE_READ,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
       qDebug()<<QString("CreateFile failed with %1.").arg(QString::number(GetLastError()));
       return false;
    }
    return true;
}

bool MakeSlot(HANDLE &hSlot,LPCTSTR lpszSlotName)
{
    hSlot = CreateMailslot(lpszSlotName,
        0,                             // no maximum message size
        MAILSLOT_WAIT_FOREVER,         // no time-out for operations
        NULL); // default security

    if (hSlot == INVALID_HANDLE_VALUE)
    {
        qDebug()<<"CreateMailslot failed with "<<GetLastError();
        return false;
    }
    return true;
}

bool ReceiveData(HANDLE hSlot,char * &data,int &size,ServerState &state)
{

    DWORD cbMessage, cMessage, cbRead;
    BOOL fResult;
    Package* buffer=NULL;
    QList<Package*> &transmissions=state.transmissions,&heads=state.heads;
    QList<int> &lost=state.lost,&tails=state.tails;
    bool &first_run=state.first_run;
    int &current_num=state.current_num;
    int pbuffer,pdata=0;

    cbMessage = cMessage = cbRead = 0;

    fResult = GetMailslotInfo( hSlot, // mailslot handle
        (LPDWORD) NULL,               // no maximum message size
        &cbMessage,                   // size of next message
        &cMessage,                    // number of messages
        (LPDWORD) NULL);              // no read time-out

    if (!fResult)
    {
        qDebug()<<"GetMailslotInfo failed with "<<GetLastError();
        return false;
    }

    if (cbMessage == MAILSLOT_NO_MESSAGE)
    {
        qDebug()<<("No messages...");
        return false;
    }

    while (cMessage != 0)  // retrieve all messages
    {
        buffer=new Package;

        fResult = ReadFile(hSlot,
            buffer,
            cbMessage,
            &cbRead,
            NULL);

        if(cbRead!=MAXLEN){
            qDebug("OH SHI~!");
            return false;
        }
        if (!fResult)
        {
            qDebug()<<"ReadFile failed with "<<GetLastError();
            return false;
        }
        qDebug()<<buffer->num;
        if(first_run){
            if (buffer->type==Package_Head){
                current_num=buffer->num;
                first_run=false;
                transmissions<<buffer;
            } else {
                qDebug()<<"Waiting for head packet!";
                delete buffer;
                buffer=NULL;
            }
        } else {
            if (buffer->num>current_num){
                for(int i=current_num+1;i<buffer->num;++i){
                    qDebug()<<"Package "<<i<<" lost!";
                    lost<<i; transmissions<<NULL;
                }
                transmissions<<buffer;
                current_num=buffer->num;   
            } else {
                int i=lost.indexOf(buffer->num);
                if(i!=-1){
                    qDebug()<<"Lost packet arrived!";
                    lost.removeAt(i);
                    transmissions[i]=buffer;
                } else {
                    qDebug()<<"Duplicate arrived!";
                    delete buffer;
                    buffer=NULL;
                }
            }
        }
        if(buffer!=NULL){
            switch(buffer->type){
                case Package_Head:{
                    heads<<buffer;
                    int temp=buffer->num+buffer->total_size/MAXDATA+(buffer->total_size%MAXDATA?1:0);
                    qDebug()<<"Expecting tail at "<<temp;
                    tails<<temp;
                    }
                    break;
                case Package_Tail:{
                    qDebug()<<"Tail received!";
                    int index=tails.indexOf(buffer->num);
                    if(index!=-1){
                        Package *head=heads.at(index);
                        bool ok=true;
                        size=head->total_size;
                        data=new char[size];
                        heads.removeAt(index);tails.removeAt(index);
                        int start=transmissions.indexOf(head);
                        for(int i=start+1;i<=start+buffer->num-head->num;++i){
                            qDebug()<<i;
                            Package* temp=transmissions.at(i);
                            if(temp==NULL){
                                qDebug()<<"There are lost packages!";
                                ok=false;
                            }
                            if(ok){
                                for (pbuffer=0;pbuffer<MAXDATA&&pdata<size;++pdata,++pbuffer){
                                    //qDebug()<<pdata;
                                    data[pdata]=temp->data[pbuffer];
                                }
                                delete temp;
                            } else {
                                lost.removeOne(head->num+i-start);
                            }
                        }
                        for(int i=start;i<=start+buffer->num-head->num;++i){
                            transmissions.removeAt(start);
                        }
                        if(ok){
                            qDebug()<<"Message received!";
                            return true;
                        } else {
                            delete data;
                            size=0;
                            return false;
                        }

                    }
                    }
                    break;
                case Package_Body:
                    break;
                default:
                    qDebug()<<"Error! Unknown packet type!";
            }
        }
        fResult = GetMailslotInfo( hSlot, // mailslot handle
            (LPDWORD) NULL,               // no maximum message size
            &cbMessage,                   // size of next message
            &cMessage,                    // number of messages
            (LPDWORD) NULL);              // no read time-out

        if (!fResult)
        {
            qDebug()<<"GetMailslotInfo failed with "<<GetLastError();
            return false;
        }
    }

    return false;
}

