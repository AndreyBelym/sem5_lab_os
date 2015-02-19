#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBuffer>
#include <QPicture>
#include <QUuid>
#include "graphicsscene.h"
extern "C"{
#include "windows.h"
}

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
    void on_action_send_triggered();

    void on_action_load_triggered();

private:
    Ui::MainWindow *ui;
    GraphicsScene scene;
    HANDLE hSlot,hNotify;
    QUuid uuid;
    int num;
};

#endif // MAINWINDOW_H
