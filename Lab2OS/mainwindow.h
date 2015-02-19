#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Lab2Threads.h"

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
    QTimer timer;
    double time_min,time_max;

private slots:
    void timer_timeout();
    void addToLog(QString msg);
    void updateStats(PRODDESC* p,CONSDESC* c);
    void addProducer(PRODDESC* p);
    void delProducer(PRODDESC* p);
    void addConsumer(CONSDESC*);
    void delConsumer(CONSDESC*);
    void on_tableWidget_2_itemChanged(QTableWidgetItem *item);
    void on_tableWidget_cellChanged(int row, int column);
    void on_spinBox_editingFinished();
    void on_checkBox_stateChanged(int arg1);
    void on_spinBox_2_editingFinished();
    void on_checkBox_2_stateChanged(int arg1);
    void on_doubleSpinBox_2_editingFinished();
    void on_doubleSpinBox_editingFinished();

};

#endif // MAINWINDOW_H
