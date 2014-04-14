/*
 * This file is part of the Kvkbd project.
 * Copyright (C) 2007-2014 Todor Gyumyushev <yodor1@gmail.com>
 * Copyright (C) 2008 Guillaume Martres <smarter@ubuntu.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "resizabledragwidget.h"

#include <QPoint>
#include <QPainter>

#include <QMouseEvent>


#include <iostream>
using namespace std;

ResizableDragWidget::ResizableDragWidget(QWidget *parent) :
    DragWidget(parent), doResize(false)
{


}

ResizableDragWidget::~ResizableDragWidget()
{
}

void ResizableDragWidget::mousePressEvent(QMouseEvent * ev)
{
//     cout << "ResizableDragWidget::mousePressEvent" << endl;
    
    DragWidget::mousePressEvent(ev);

    if (dragPoint.x() > width() - 20 && dragPoint.x() < width() && dragPoint.y() > height() - 20 && dragPoint.y() < height()) {
        
        dragPoint = QPoint(width() - ev->pos().x(), height() - ev->pos().y());
	
	dragged = false;
	doResize = true;
    }

}

void ResizableDragWidget::mouseMoveEvent(QMouseEvent * ev)
{
//     cout << "ResizableDragWidget::mouseMoveEvent | Resize: " << doResize << " | Dragged: " << dragged << endl;
    
    
    DragWidget::mouseMoveEvent(ev);
    
    if (!doResize) return;
    
    QPoint curr(ev->globalPos().x(), ev->globalPos().y());
    QPoint pos = QWidget::pos();
    int nw = curr.x() - pos.x() + dragPoint.x();
    int nh = curr.y() - pos.y() + dragPoint.y();

    resize(nw, nh);

}

void ResizableDragWidget::mouseReleaseEvent(QMouseEvent * e)
{
    DragWidget::mouseReleaseEvent(e);

    doResize = false;

}

void ResizableDragWidget::paintEvent(QPaintEvent *ev)
{

    DragWidget::paintEvent(ev);

    QPainter p(this);

    for (int a = 0; a < 20; a += 4) {

        p.setPen(QColor(170, 170, 170));
        p.drawLine(width() - 20 + a, height() - 2, width() - 2, height() - 20 + a);
        p.setPen(QColor(200, 200, 200));
        p.drawLine(width() - 19 + a, height() - 2, width() - 2, height() - 19 + a);
    }

}



#include "resizabledragwidget.moc"
