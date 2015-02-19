#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = 0);
    void emitAddToLog(QString msg){emit addToLog(msg);}
signals:
    void addToLog(QString msg);
    
};

#endif // THREADMANAGER_H
