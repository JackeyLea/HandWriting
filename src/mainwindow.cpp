#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    knn = new KNNTT;
    svm = new SVMTT;
}

MainWindow::~MainWindow()
{
    delete knn;
    delete svm;
    delete ui;
}

void MainWindow::on_btnTrain_clicked()
{
    // type
    qDebug()<<"当前训练的神经网络类型为："<<ui->boxType->currentText();
    switch(ui->boxType->currentIndex()){
    case 0:
        knn->setRunMode(TRAIN);
        knn->start();
        break;
    case 1:
        svm->setRunMode(TRAIN);
        svm->start();
        break;
    }
}

void MainWindow::on_btnTest_clicked()
{
    qDebug()<<"当前测试的神经网络类型为："<<ui->boxType->currentText();
    switch (ui->boxType->currentIndex()) {
    case 0:
        knn->setRunMode(TEST);
        knn->start();
        break;
    case 1:
        svm->setRunMode(TEST);
        svm->start();
        break;
    default:
        break;
    }
}

void MainWindow::on_btnSave_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
    if(filePath.isEmpty()){
        QMessageBox::information(this,tr("Tips"),tr("Nothing"),QMessageBox::Ok);
        return;
    }
    bool isOk = ui->wgtDrawing->saveImage(filePath);
    if(isOk){
        QMessageBox::information(this,tr("Tips"),tr("Picture has been saved"),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,tr("Tips"),tr("Something was wrong"),QMessageBox::Ok);
    }
}
//开始识别图片
void MainWindow::on_btnGo_clicked()
{
    int result = -1;
    Mat temp=Mat(1,28*28,CV_8UC1);
    //QImage drawImg = ui->wgtDrawing->getImage();
    //Mat matImg= toMat(drawImg);
    Mat matImg = imread("digit.png");
    Mat mat3c = Mat(matImg.cols,matImg.rows,CV_8UC1);
    cvtColor(matImg,mat3c,COLOR_BGRA2GRAY);

    cv::resize(mat3c,mat3c,Size(28,28));
    for(int i=0;i<mat3c.cols;i++){
        for(int j=0;j<mat3c.rows;j++){
            uchar a = mat3c.at<uchar>(i,j);
            temp.at<uchar>(0,i*28+j)=a;
        }
    }

    temp.convertTo(temp,CV_32F);
    switch (ui->boxType->currentIndex()) {
    case 0:
        result = knn->predict(temp);
        if(result>=0){
            result = knn->getPredictResult();
        }
        break;
    case 1:
        result = svm->predict(temp);
        if(result>=0){
            result = knn->getPredictResult();
        }
        break;
    default:
        break;
    }
    QMessageBox::information(this,tr("Result"),QString::number(result),QMessageBox::Ok);
}

void MainWindow::on_btnAbout_clicked()
{
    QString msg=QString("<h1>HandWriting</h1>"
                        "<h2>A tool to recoginize handwriting character</h2>"
                        "<h3>JackeyLea@Github</h3>"
                        "<h3>Mail:1768478912@qq.com</h3>"
                        "<h4>If you got any problem please contact me</h4>");
    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_btnAboutQt_clicked()
{
    qApp->aboutQt();
}

void MainWindow::on_btnIndex_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/HandWriting"));
}

void MainWindow::on_btnClear_clicked()
{
    ui->wgtDrawing->clearImage();
}
