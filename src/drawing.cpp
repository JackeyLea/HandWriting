#include "drawing.h"
#include "ui_drawing.h"

Drawing::Drawing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Drawing)
{
    ui->setupUi(this);
    //如果初始值为正值，会在界面绘制一个白色的点
    lastPoint=endPoint=QPoint(-10,-10);

    pix = QPixmap(this->width(),this->height());
    qDebug()<<pix.size();
    pix.fill(Qt::black);//填充图片的黑色背景
}

Drawing::~Drawing()
{
    delete ui;
}

QImage Drawing::getImage()
{
    return pix.toImage();
}

bool Drawing::saveImage(QString filePath)
{
    return pix.save(filePath);
}

void Drawing::clearImage()
{
    lastPoint=endPoint=QPoint(-10,-10);
    pix.fill(Qt::black);
    update();
}
//绘图
void Drawing::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //qDebug()<<this->size();

    QPainter pp(&pix);
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(10);
    pp.setPen(pen);
    pp.drawLine(lastPoint,endPoint);
    pp.setRenderHint(QPainter::HighQualityAntialiasing,true);

    lastPoint = endPoint;
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
}
//鼠标按下
void Drawing::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
    }
    endPoint = lastPoint;
}
//鼠标松开
void Drawing::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}
//鼠标移动
void Drawing::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}
//如果界面的尺寸发生了调整
void Drawing::resizeEvent(QResizeEvent *event)
{
    pix = QPixmap(event->size());
    //pix.scaled(event->size());
    pix.fill(Qt::black);
    update();
}
