#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    SVMTT *svm;
};

#endif // MAINWINDOW_H
