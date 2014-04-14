/*
 * This file is part of the Kvkbd project.
 * Copyright (C) 2007-2014 Todor Gyumyushev <yodor1@gmail.com>
 * Copyright (C) 2008 Guillaume Martres <smarter@ubuntu.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "kbddock.h"

#include <QDBusConnection>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>

#include <KIconLoader>
#include <KLocale>

#include "dockadaptor.h"

#include <iostream>
using namespace std;

#define DEFAULT_WIDTH 	105
#define DEFAULT_HEIGHT 	35

KbdDock::KbdDock(const WId& window) : DragWidget(0), wID(window)
{
    
    setAttribute(Qt::WA_AlwaysShowToolTips);
    setAttribute(Qt::WA_DeleteOnClose, false);
    
    setWindowFlags( Qt::ToolTip | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint );

    setFocusPolicy(Qt::NoFocus);
    
    setToolTip(i18nc("@info:tooltip Displayed on the movable dock", "Toggle keyboard visibility"));
    
    
    
    resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    raise();
}

KbdDock::~KbdDock()
{

  
}

void KbdDock::paintEvent(QPaintEvent *)
{
  
    QPixmap pix = QPixmap::grabWindow(wID);
    
    QPainter p(this);

    p.drawPixmap(0, 0, pix.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

}



void KbdDock::mouseReleaseEvent(QMouseEvent *ev)
{
  
     if (dragged && !moved) {
      emit requestVisibility();
     }

     DragWidget::mouseReleaseEvent(ev);
     raise();
}
