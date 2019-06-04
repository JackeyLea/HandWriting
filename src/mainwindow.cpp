#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

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
    qDebug()<<"KNN train done";
}

void MainWindow::on_buttonTestKNN_clicked()
{
    train->testMnistKNN();
    qDebug()<<"All done";
}

void MainWindow::on_buttonSVMTrain_clicked()
{
    train->svmTrain();
    qDebug()<<"SVM train done";
}

void MainWindow::on_buttonSVMTest_clicked()
{
    train->testMnistSVM();
    qDebug()<<"All done";
}
