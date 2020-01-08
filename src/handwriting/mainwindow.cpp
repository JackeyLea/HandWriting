#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pix = QPixmap(this->width(),this->height());
    pix.fill(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::knnPredicted(cv::Mat img)
{
    Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("knn.yml");

    float predicted = knn->predict(img);

    return static_cast<int>(predicted);
}

int MainWindow::svmPredicted(Mat img)
{
    Ptr<SVM> svm1 = StatModel::load<SVM>("svm.xml");

    float predicted = svm1->predict(img);

    return static_cast<int>(predicted);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open"),QString(),QString("*.png *.jpg *.bmp\n*"));

    if(filePath.isEmpty()){
        QMessageBox::information(this,tr("Warning"),tr("Select nothing"),QMessageBox::Ok);
        return;
    }

    pix.load(filePath);
}

void MainWindow::on_actionSave_triggered()
{
     QString filePath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
     if(filePath.isEmpty()){
         QMessageBox::information(this,tr("Tips"),tr("Nothing"),QMessageBox::Ok);
         return;
     }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg=QString("<h1>HandWriting</h1>"
                        "<h2>A tool to recoginize handwriting character</h2>"
                        "<h3>JackeyLea@Github</h3>"
                        "<h3>Mail:1768478912@qq.com</h3>"
                        "<h3>Phone:13812991101</h3>"
                        "<h4>If you got any problem please contact me</h4>");
    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/HandWriting"));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pp(&pix);
    pp.drawLine(lastPoint,endPoint);
    pp.setRenderHint(QPainter::HighQualityAntialiasing,true);

    lastPoint = endPoint;
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
    }
    endPoint = lastPoint;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}

void MainWindow::on_actionClear_triggered()
{
    pix.fill(Qt::white);
    update();
}

void MainWindow::on_actionGo_triggered()
{
    QPixmap writen = this->grab(this->centralWidget()->geometry());

    cv::Mat img = Silkworm::QPixmapToCvMat(writen,true);
}
