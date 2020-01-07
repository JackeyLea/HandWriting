#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "training.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_buttonTrainKNN_clicked();
    void on_buttonTestKNN_clicked();
    void on_buttonSVMTrain_clicked();
    void on_buttonSVMTest_clicked();
private:
    Ui::MainWindow ui;
    Training *train;
};

#endif // MAINWINDOW_H
