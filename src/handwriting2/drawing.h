#ifndef DRAWING_H
#define DRAWING_H

#include "global.hpp"

namespace Ui {
class Drawing;
}

class Drawing : public QWidget
{
    Q_OBJECT

public:
    explicit Drawing(QWidget *parent = 0);
    ~Drawing();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::Drawing *ui;

    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;
};

#endif // DRAWING_H
