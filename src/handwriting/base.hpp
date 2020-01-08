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

#endif // BASE_HPP
