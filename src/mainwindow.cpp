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
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.bmp *.jpg)"));
    if (filePath.isEmpty())
    {
        QMessageBox::information(this, tr("Tips"), tr("Nothing"), QMessageBox::Ok);
        return;
    }
    bool isOk = ui->wgtDrawing->saveImage(filePath);
    if (isOk)
    {
        QMessageBox::information(this, tr("Tips"), tr("Picture has been saved"), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this, tr("Tips"), tr("Something was wrong"), QMessageBox::Ok);
    }
}
//开始识别图片
void MainWindow::on_btnGo_clicked()
{
    QImage drawImg = ui->wgtDrawing->getImage();
    QImage scaleImg = drawImg.scaled(28,28);
    cv::Mat img = toMat(scaleImg);
    //cv::imshow("drawImg",img);//correct
    QString msg = QString("draw img ->type: %1,cols: %2,rows: %3,dims: %4,channels: %5")
                      .arg(img.type())
                      .arg(img.cols)
                      .arg(img.rows)
                      .arg(img.dims)
                      .arg(img.channels());
    //QMessageBox::information(this,tr("message"),msg,QMessageBox::Ok);

    cv::Mat gray;
    switch (img.channels())
    {
    case 3:
        cv::cvtColor(img, gray, COLOR_RGB2GRAY);
        break;
    case 4:
        cv::cvtColor(img, gray, COLOR_RGBA2GRAY);
        break;
    }
    //cv::imshow("gray",gray);//correct
    cv::Mat bin = getBinImg(gray);
    //cv::imshow("bin",bin);//correct
    cv::Mat binResult;
    cv::resize(bin, binResult, cv::Size(28, 28));
    //cv::imshow("bin result",binResult);

    cv::Mat temp(1,28* 28, CV_8UC1);
    for(int i=0;i<binResult.rows;i++){
        for(int j=0;j<binResult.cols;j++){
            uchar a=binResult.at<uchar>(i,j);
            temp.at<uchar>(0,i*28+j)=a;
        }
    }
    temp.convertTo(temp,CV_32F);

    int result = -1;
    float predicted;
    std::ifstream file;
    switch (ui->boxType->currentIndex())
    {
    case 0:
    {
        std::cout << "knn预测开始" << std::endl;
        file.open("knn.xml");
        if (!file.is_open())
        {
            std::cout << "->KNN训练结果文件knn.xml不存在" << std::endl;
        }

        std::cout << "->开始加载knn模型" << std::endl;
        cv::Ptr<cv::ml::KNearest> knn = cv::Algorithm::load<cv::ml::KNearest>("knn.xml");
        std::cout << "->knn模型加载完毕" << std::endl;

        cv::Mat tmp3; //没有用，用于占位置
        predicted = knn->predict(temp, tmp3);
        result = static_cast<int>(predicted);
        std::cout << "knn预测结束" << std::endl;
        break;
    }
    case 1:
    {
        std::cout << "svm预测开始" << std::endl;
        file.open("svm.xml");
        if (!file.is_open())
        {
            std::cout << "->SVM训练结果文件svm.xml不存在" << std::endl;
        }

        std::cout << "->开始加载svm模型" << std::endl;
        cv::Ptr<cv::ml::SVM> svm = cv::Algorithm::load<cv::ml::SVM>("svm.xml");
        std::cout << "->svm模型加载完毕" << std::endl;

        predicted = svm->predict(temp);
        result = static_cast<int>(predicted);
        std::cout << "svm预测结束" << std::endl;
        break;
    }
    default:
        break;
    }
    QMessageBox::information(this, tr("Result"), QString::number(result), QMessageBox::Ok);
}

void MainWindow::on_btnAbout_clicked()
{
    QString msg = QString("<h1>HandWriting</h1>"
                          "<h2>A tool to recoginize handwriting character</h2>"
                          "<h3>JackeyLea@Github</h3>"
                          "<h3>Mail:1768478912@qq.com</h3>"
                          "<h4>If you got any problem please contact me</h4>");
    QMessageBox::information(this, tr("About"), msg, QMessageBox::Ok);
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
