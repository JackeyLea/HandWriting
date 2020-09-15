#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

//#include <torch/script.h>

#include "base.hpp"



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

    int svmPredicted(Mat &img);

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

private:
    Ui::MainWindow *ui;

    cv::Mat img;
};

#endif // MAINWINDOW_H
