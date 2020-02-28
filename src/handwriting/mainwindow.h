#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

//#include <torch/script.h>

#include "base.hpp"

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QDesktopServices>
#include <QFileDialog>

using namespace cv;
using namespace cv::ml;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    int knnPredicted(Mat img);

    int svmPredicted(cv::Mat img);

    int torchPredicted(cv::Mat img);

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionGithub_triggered();

    void on_actionClear_triggered();

    void on_actionGo_triggered();

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;

    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;

    cv::Mat img;
};

#endif // MAINWINDOW_H
