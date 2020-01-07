#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    qDebug()<<"程序启动成功";

    train = new Training();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_buttonTrainKNN_clicked()
{
    train->knnTrain();
    qDebug()<<"KNN训练完成";
}

void MainWindow::on_buttonTestKNN_clicked()
{
    ui.doubleSpinBox->setValue(train->testMnistKNN());
    qDebug()<<"KNN测试完成";
}

void MainWindow::on_buttonSVMTrain_clicked()
{
    train->svmTrain();
    qDebug()<<"SVM训练完成";
}

void MainWindow::on_buttonSVMTest_clicked()
{
    ui.doubleSpinBox->setValue(train->testMnistSVM());
    qDebug()<<"SVM测试完成";
}
