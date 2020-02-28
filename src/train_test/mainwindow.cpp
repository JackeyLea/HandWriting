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

MainWindow::~MainWindow()
{
    delete svmtt;
    delete knntt;
    delete ui;
}

void MainWindow::on_buttonSVMTrain_clicked()
{
    time_start = (double)clock();
    ui->buttonSVMTrain->setEnabled(false);
    svmtt->svmTrain();
    qDebug()<<"SVM训练完成";
    ui->buttonSVMTest->setEnabled(true);
    time_end = (double)clock();
    qDebug()<<"SVM training spend "<<(time_end-time_start)/1000.0<<"ms";
}

void MainWindow::on_buttonSVMTest_clicked()
{
    time_start = (double)clock();
    ui->doubleSpinBox->setValue(svmtt->testSVM());
    qDebug()<<"SVM测试完成";
    ui->buttonSVMTest->setEnabled(false);
    ui->buttonSVMTrain->setEnabled(true);
    time_end = (double)clock();
    qDebug()<<"SVM test spend "<<(time_end-time_start)/1000.0<<"ms";
}

void MainWindow::on_buttonKNNTrain_clicked()
{
    time_start = (double)clock();
    ui->buttonKNNTrain->setEnabled(false);
    knntt->KNNTrain();
    qDebug()<<"KNN训练完成";
    ui->buttonKNNTest->setEnabled(true);
    time_end = (double)clock();
    qDebug()<<"KNN train spend"<<(time_end-time_start)/1000.0<<"ms";
}

void MainWindow::on_buttonKNNTest_clicked()
{
    time_start = (double)clock();
    ui->doubleSpinBox->setValue(knntt->testKNN());
    qDebug()<<"KNN测试完成";
    ui->buttonKNNTest->setEnabled(false);
    ui->buttonKNNTrain->setEnabled(true);
    time_end = (double)clock();
    qDebug()<<"KNN test spend"<<(time_end-time_start)/1000.0<<"ms";
}
