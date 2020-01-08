#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QTime>

#include "svm.hpp"
#include "knn.hpp"

#include "sys/time.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonSVMTrain_clicked();

    void on_buttonSVMTest_clicked();

    void on_buttonKNNTrain_clicked();

    void on_buttonKNNTest_clicked();

private:
    Ui::MainWindow *ui;

    double time_start;
    double time_end;
    SVMTT *svmtt;
    KNNTT *knntt;
};

#endif // MAINWINDOW_H
