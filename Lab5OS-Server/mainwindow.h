#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QUuid>
#include <QList>
#include "../Lab5OS/mailslots.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_action_receive_triggered();

    void on_action_clients_triggered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QList<QGraphicsScene*> scenes;
    HANDLE hNotify;
    QList<HANDLE> hSlots;
    QList<QUuid> uuids;
    QList<ServerState*> states;
};

#endif // MAINWINDOW_H
