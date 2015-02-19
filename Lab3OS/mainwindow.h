#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#define WINVER 0x0620
extern "C"{
    #include <windows.h>
}

typedef struct{
    HANDLE file,mapping;
    char* view;
} MAPPING;

namespace Ui {
class MainWindow;
}

typedef int (*casefunc)(const int);

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MAPPING m;
private slots:
    void ChangeCase(casefunc f);

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
