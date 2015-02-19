#ifndef THREADHELPER_H
#define THREADHELPER_H

#include <QObject>

class ThreadHelper : public QObject
{
    Q_OBJECT
public:
    explicit ThreadHelper(QObject *parent = 0);
    void emitAddMessage(char*msg){emit addMessage(msg);}
 signals:
    void addMessage(char *);
};

#endif // THREADHELPER_H
