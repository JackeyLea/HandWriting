#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QFileDialog>

#include "svm.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_buttonSVMTrain_clicked();

    void on_buttonSVMTest_clicked();

    void on_buttonKNNTrain_clicked();

    void on_buttonKNNTest_clicked();

private:
    Ui::MainWindow *ui;

    SVMTT *svmtt;
};

#endif // MAINWINDOW_H
