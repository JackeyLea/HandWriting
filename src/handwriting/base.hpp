#ifndef BASE_HPP
#define BASE_HPP

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QString>
#include <QUrl>
#include <QWidget>

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

inline QString loadTheme(QString themeName){
    QString themeCtx;
    QFile file(QString(":/resources/themes/%1.qss").arg(themeName));
    qDebug()<<"theme file path is:"<<file.fileName();
    if(file.open(QIODevice::Text|QIODevice::ReadOnly)){
        QTextStream readIn(&file);
        themeCtx = readIn.readAll();
    }
    else{
        qDebug()<<"Cannot open file: "<<file.errorString();
    }
    file.close();

    return themeCtx;
}

inline QImage toQImage(Mat img){
    QImage qimg=QImage(static_cast<unsigned char*>(img.data),
                       img.cols,img.rows,img.cols*img.channels(),QImage::Format_RGB888);
    return qimg;
}

inline Mat toMat(QImage image){
    Mat mat;
    //qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}


inline cv::Mat getGrayImg(cv::Mat img) {
    cv::Mat grayImg;
    if(img.channels() ==4){
        //cv::cvtColor(img,grayImg,cv);
    }
    if (img.channels() == 3) {
        cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
    } else {
        grayImg = img;
    }
    qDebug()<<"Result channel is: "<<grayImg.channels();

    return grayImg;
}

inline cv::Mat getBinImg(cv::Mat grayImg, int threholdValue=-1) {
    cv::Mat result;
    int     middle = 0;

    if (threholdValue == -1) {
        uchar *       p;
        int           nrows = grayImg.rows;
        int           ncols = grayImg.cols;
        long long int count = 0;
        int           mid   = 0;
        //获取并计算灰度值的平均值
        for (int i = 0; i < nrows; i++) {
            p = grayImg.ptr<uchar>(i);
            for (int j = 0; j < ncols; j++) {
                mid += p[ j ];
                count++;
            }
        }
        middle = int((mid / count) * 2 / 3); //
    } else {
        middle = threholdValue;
    }
    qDebug() << "Average pixel is: " << middle;
    //以平均值二值化图片
    threshold(grayImg, result, middle, 255, cv::THRESH_BINARY);

    //如果黑色像素点较多则翻转图片像素
    int white = 0;
    int black = 0;
    for (int i = 0; i < result.rows; i++) {
        uchar *ptr = result.ptr(i);
        for (int j = 0; j < result.cols; j++) {
            if (ptr[ j ] == 0)
                black++;
            else
                white++;
        }
    }

    return result;
}

inline cv::Mat getShadowXResult(cv::Mat bin) {
    assert(!bin.empty());
    //是否为白色或者黑色根据二值图像的处理得来
    cv::Mat painty(bin.size(), CV_8UC1, cv::Scalar(255)); //初始化为全白

    //水平投影
    int *pointcount = new int[ bin.rows ]; //在二值图片中记录行中特征点的个数
    memset(pointcount, 0, bin.rows * 4);   //注意这里需要进行初始化

    for (int i = 0; i < bin.rows; i++) {
        for (int j = 0; j < bin.cols; j++) {
            if (bin.at<uchar>(i, j) == 0) {
                pointcount[ i ]++; //记录每行中黑色点的个数 //水平投影按行在y轴上的投影
            }
        }
    }

    for (int i = 0; i < bin.rows; i++) {
        for (int j = 0; j < pointcount[ i ]; j++) //根据每行中黑色点的个数，进行循环
        {
            painty.at<uchar>(i, j) = 0;
        }
    }

    cv::imshow("y",painty);

    cv::Mat            result;
    int                     startindex = 0;
    int                     endindex   = 0;
    bool                  inblock    = false; //是否遍历到字符位置

    for (int i = 0; i < painty.rows; i++) {
        if (!inblock && pointcount[ i ] != 0) //进入有字符区域
        {
            inblock    = true;
            startindex = i;
            qDebug() << "startindex:" << startindex;
        }
        if (inblock && pointcount[ i ] == 0) //进入空白区
        {
            endindex    = i;
            inblock     = false;
            cv::Mat roi = bin.rowRange(
                startindex, endindex + 1); //从而记录从开始到结束行的位置，即可进行行切分
            result=roi;
            break;
        }
    }
    delete[] pointcount;

    return result;
}

inline cv::Mat getShadowYResult(cv::Mat bin) {
    assert(!bin.empty());

    cv::Mat paintx(bin.size(), CV_8UC1, cv::Scalar(255)); //创建一个全白图片，用作显示

    int *pointcount = new int[ bin.cols ];
    memset(pointcount, 0, bin.cols * 4);

    for (int i = 0; i < bin.rows; i++) {
        for (int j = 0; j < bin.cols; j++) {
            if (bin.at<uchar>(i, j) == 0) {
                pointcount[ j ]++; //垂直投影按列在x轴进行投影
            }
        }
    }
    for (int i = 0; i < bin.cols; i++) {
        for (int j = 0; j < pointcount[ i ]; j++) {
            paintx.at<uchar>(bin.rows - 1 - j, i) = 0; //翻转到下面，便于观看
        }
    }

    cv::Mat              result;
    int                  startindex = 0;
    int                  endindex   = 0;
    bool                 inblock    = false; //是否遍历到字符位置

    for (int i = 0; i < paintx.cols; i++) {
        if (!inblock && pointcount[ i ] != 0) //进入有字符区域
        {
            inblock    = true;
            startindex = i;
            qDebug() << "startindex:" << startindex;
        }
        if (inblock && pointcount[ i ] == 0) //进入空白区
        {
            endindex    = i;
            inblock     = false;
            cv::Mat roi = bin.colRange(
                startindex, endindex + 1); //从而记录从开始到结束行的位置，即可进行行切分
            result=roi;
        }
    }

    delete[] pointcount;

    return result;
}

#endif // BASE_HPP
