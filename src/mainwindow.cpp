#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"程序启动成功";
    //ui->buttonSVMTest->setEnabled(false);
    ui->buttonKNNTest->setEnabled(false);

    svmtt = new SVMTT();
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
    //train->knnTrain();
    qDebug()<<"KNN训练完成";
}

void MainWindow::on_buttonKNNTest_clicked()
{
    //ui->doubleSpinBox->setValue(train->testMnistKNN());
    qDebug()<<"KNN测试完成";
}
