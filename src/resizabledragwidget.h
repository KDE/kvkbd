#ifndef RESIZABLEDRAGWIDGET_H
#define RESIZABLEDRAGWIDGET_H

#include "dragwidget.h"

class ResizableDragWidget : public DragWidget
{
    Q_OBJECT
    
public:
    explicit ResizableDragWidget(QWidget *parent = 0);
    ~ResizableDragWidget();




protected:
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e);
    virtual void paintEvent(QPaintEvent *e);

    bool doResize;
};



#endif // RESIZABLEDRAGWIDGET_H
