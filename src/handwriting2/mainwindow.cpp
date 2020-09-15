#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    svm = new SVMTT;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnTrain_clicked()
{
    // type
    switch(ui->boxType->currentIndex()){
    case 0:
        qDebug()<<ui->boxType->currentText();
        break;
    case 1:
        qDebug()<<ui->boxType->currentText();
        svm->setRunMode(TRAIN);
        svm->start();
        break;
    }
}

void MainWindow::on_btnTest_clicked()
{
    switch (ui->boxType->currentIndex()) {
    case 0:

        break;
    case 1:
        svm->setRunMode(TEST);
        svm->start();
        break;
    default:
        break;
    }
}
