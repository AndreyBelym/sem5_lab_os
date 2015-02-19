#ifndef MAILSLOTS_H
#define MAILSLOTS_H
extern "C"{
#include "windows.h"
}
#include <QUuid>
#include <QList>

const int MAXLEN=400;

const LPCTSTR GetServerNotify=TEXT("\\\\.\\mailslot\\{ed19aa6c-8132-4a3c-82d7-519145548b39}"),
              PutServerNotify=TEXT("\\\\*\\mailslot\\{ed19aa6c-8132-4a3c-82d7-519145548b39}");
enum PackageType{
    Package_Head,
    Package_Body,
    Package_Tail
};

const int MAXDATA=MAXLEN-sizeof(PackageType)-sizeof(int),
          MAXINFO=MAXLEN-MAXDATA;

typedef struct {
    PackageType type;
    int num;
    union{
        int total_size;
        char data[MAXDATA];
    };
} Package;


typedef struct {
    QList<Package*> transmissions,heads;
    QList<int> lost,tails;
    bool first_run;
    int current_num;
} ServerState;

bool MakeFile(HANDLE &hFile,LPCTSTR SlotName);
bool MakeSlot(HANDLE &hSlot,LPCTSTR lpszSlotName);

bool ReceiveNotify(HANDLE &hSlot,QUuid &uuid);
void SendNotify(HANDLE &hSlot,QUuid &uuid);

void OpenUUIDSlot(HANDLE &hSlot,QUuid &uuid);
void GenUUIDSlot(HANDLE &hSlot,QUuid &uuid);

bool TransmitData(HANDLE hFile, const char * source, int size, int &num);
bool ReceiveData(HANDLE hSlot, char * &data, int &size, ServerState &state);
#endif // MAILSLOTS_H
