#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "knntt.h"
#include "svmtt.h"

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
    void on_btnTrain_clicked();

    void on_btnTest_clicked();

    void on_btnSave_clicked();

    void on_btnGo_clicked();

    void on_btnAbout_clicked();

    void on_btnAboutQt_clicked();

    void on_btnIndex_clicked();

    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;

    Mat _matImg;

    KNNTT *knn;
    SVMTT *svm;
};

#endif // MAINWINDOW_H
