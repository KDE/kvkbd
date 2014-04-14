#include "dragwidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QStylePainter>
#include <QX11Info>

#include <iostream>
using namespace std;


#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <fixx11h.h>


DragWidget::DragWidget(QWidget *parent) :
    QWidget(parent),  dragged(false), moved(false), locked(false)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

DragWidget::~DragWidget()
{

}

void DragWidget::blurBackground(bool blurEnabled)
{
  this->setProperty("blurBackground", QVariant(blurEnabled));
  
  Display *dpy = QX11Info::display();
  Atom net_wm_blur_region = XInternAtom(dpy, "_KDE_NET_WM_BLUR_BEHIND_REGION", False);
  
  if (blurEnabled) {

      XChangeProperty(dpy, this->winId(), net_wm_blur_region, XA_CARDINAL, 32, PropModeReplace, 0, 0);

  }
  else {
      XDeleteProperty(dpy, this->winId(), net_wm_blur_region);

  }
  repaint();
}
void DragWidget::setLocked(bool locked)
{
  this->locked = locked;
}
bool DragWidget::isLocked()
{
  return locked;
}

void DragWidget::mousePressEvent(QMouseEvent *e)
{

    if (locked) {
        return;
    }
    
    gpress = e->globalPos();
    dragged = true;
    dragPoint = e->pos();



}

void DragWidget::mouseMoveEvent(QMouseEvent *ev)
{
    

    if (!dragged) return;
    
    moved=true;
    
    QPoint curr(ev->globalPos().x() - dragPoint.x(), ev->globalPos().y() - dragPoint.y());
    move(curr);

}

void DragWidget::mouseReleaseEvent(QMouseEvent *)
{
   
    dragged = false;
    moved = false;
}

void DragWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QStylePainter p(this);
    p.drawPrimitive(QStyle::PE_Widget, opt);
}

void DragWidget::toggleVisibility()
{
    if (isMinimized()) {
        showNormal();
        show();
	raise();
    }
    else {
        showMinimized();
        hide();
	lower();
    }

}
