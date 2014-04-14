#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

class DragWidget : public QWidget
{
    Q_OBJECT

public:
    DragWidget(QWidget *parent = 0);
    ~DragWidget();

public slots:
    void toggleVisibility();
    void blurBackground(bool blurEnabled);
    void setLocked(bool mode);
    bool isLocked();
    
protected:
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e);

    virtual void paintEvent(QPaintEvent *e);


    QPoint dragPoint;
    QPoint gpress;
    
    bool dragged;
    bool moved;
    bool locked;
    
signals:
    void widgetShown();
};


#endif // DRAGWIDGET_H
