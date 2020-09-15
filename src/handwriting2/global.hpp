/*
 * 2020-09-15
 * gloabl function for all
 */

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QWidget>
#include <QThread>
#include <QFrame>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

using namespace cv;
using namespace cv::ml;
using namespace std;

enum RUNMODE{
    TRAIN,
    TEST,
    STOP
};

//big-endian to little-endian
inline int reverseDigit(int num){
    unsigned char c1,c2,c3,c4;
    c1=num&255;
    c2=(num>>8)&255;
    c3=(num>>16)&255;
    c4=(num>>24)&255;

    return ((int)c1<<24)+((int)c2<<16)+((int)c3<<8)+c4;
}

#endif // GLOBAL_HPP
