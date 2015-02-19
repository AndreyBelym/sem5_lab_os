#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QLabel>
#define WINVER 0x0620

extern "C"{
#include <windows.h>
}
const int BUFSIZE=512;
const char pipe_name[]="\\\\%s\\pipe\\MythematicaPipe";
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

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_action_eval_triggered();

    void on_pushButton_clicked();

private:
    HANDLE hPipe;
    QCompleter complete;

    Ui::MainWindow *ui;
    int OpenPipe(LPTSTR lpszPipename);
    int Communicate(LPTSTR lpvMessage);
};

#endif // MAINWINDOW_H
