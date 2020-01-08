#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "base.hpp"

using namespace cv;
using namespace cv::ml;
using namespace Silkworm;

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

private:
    Ui::MainWindow *ui;

    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;
};

#endif // MAINWINDOW_H
