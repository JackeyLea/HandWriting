#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"程序启动成功";
    ui->buttonSVMTest->setEnabled(false);
    ui->buttonKNNTest->setEnabled(false);

    svmtt = new SVMTT();
    knntt = new KNNTT();
}

void MainWindow::on_buttonSVMTrain_clicked()
{
    ui->buttonSVMTrain->setEnabled(false);
    svmtt->svmTrain();
    qDebug()<<"SVM训练完成";
    ui->buttonSVMTest->setEnabled(true);
}

void MainWindow::on_buttonSVMTest_clicked()
{
    ui->doubleSpinBox->setValue(svmtt->testSVM());
    qDebug()<<"SVM测试完成";
    ui->buttonSVMTest->setEnabled(false);
    ui->buttonSVMTrain->setEnabled(true);
}

void MainWindow::on_buttonKNNTrain_clicked()
{
    ui->buttonKNNTrain->setEnabled(false);
    knntt->KNNTrain();
    qDebug()<<"KNN训练完成";
    ui->buttonKNNTest->setEnabled(true);
}

void MainWindow::on_buttonKNNTest_clicked()
{
    ui->doubleSpinBox->setValue(knntt->testKNN());
    qDebug()<<"KNN测试完成";
    ui->buttonKNNTest->setEnabled(false);
    ui->buttonKNNTrain->setEnabled(true);
}
