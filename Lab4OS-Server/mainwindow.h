#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#define WINVER 0x0620

extern "C"{
#include <windows.h>
}
#include <muParser.h>
const int BUFSIZE=512;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
private slots:
    void addToLog(QString msg);
};

#endif // MAINWINDOW_H
