#include "drawing.h"
#include "ui_drawing.h"

Drawing::Drawing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Drawing)
{
    ui->setupUi(this);

    pix = QPixmap(this->width(),this->height());
    pix.fill(Qt::black);
}

Drawing::~Drawing()
{
    delete ui;
}

void Drawing::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

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

void Drawing::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
    }
    endPoint = lastPoint;
}

void Drawing::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}

void Drawing::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}

void Drawing::resizeEvent(QResizeEvent *event)
{
    pix = QPixmap(event->size());
    pix.fill(Qt::black);
    update();
}
