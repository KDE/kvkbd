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

#ifndef KBDDOCK_H
#define KBDDOCK_H

#include "dragwidget.h"

#include <QPixmap>
#include <QMouseEvent>


class KbdDock : public DragWidget
{
    Q_OBJECT


public:
    KbdDock(const WId& window);
    ~KbdDock();

    void paintEvent(QPaintEvent *);
    void setPixmap(const QPixmap& pm);

signals:
    void requestVisibility();


protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    WId wID;
    
    QPixmap pm;

};

#endif // KBDDOCK_H
