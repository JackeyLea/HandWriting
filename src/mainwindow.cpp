#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QImage drawImg = ui->wgtDrawing->getImage();
    cv::Mat img = toMat(drawImg);
    //cv::imshow("drawImg",img);//correct
    QString msg =QString("draw img ->type: %1,cols: %2,rows: %3,dims: %4,channels: %5")
              .arg(img.type())
              .arg(img.cols).arg(img.rows).arg(img.dims)
              .arg(img.channels());
    //QMessageBox::information(this,tr("message"),msg,QMessageBox::Ok);

    cv::Mat gray;
    switch(img.channels()){
    case 3:
        cv::cvtColor(img,gray,COLOR_RGB2GRAY);
        break;
    case 4:
        cv::cvtColor(img,gray,COLOR_RGBA2GRAY);
        break;
    }
    //cv::imshow("gray",gray);//correct
    cv::Mat bin=getBinImg(gray);
    //cv::imshow("bin",bin);//correct
    cv::Mat binResult;
    cv::resize(bin,binResult,cv::Size(28,28));
    //cv::imshow("bin result",binResult);

    cv::Mat temp(28,28,CV_8UC1);
    for(int i=0;i<binResult.cols;i++){
        for(int j=0;j<binResult.rows;j++){
            uchar a = binResult.at<uchar>(i,j);
            temp.at<uchar>(0,i*28+j)=a;
        }
    }
    cv::Mat temp2(1,28*28,CV_32F);
    temp.convertTo(temp2,CV_32F);

    int result=-1;
    switch (ui->boxType->currentIndex()) {
    case 0:
//        result = knn->predict(temp2);
//        if(result>=0){
//            result = knn->getPredictResult();
//        }
        break;
    case 1:
//        result = svm->predict(temp2);
//        if(result>=0){
//            result = knn->getPredictResult();
//        }
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
