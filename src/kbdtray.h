/*
 * Copyright (C) 2007-2014 Todor Gyumyushev <yodor1@gmail.com>
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

#ifndef KBDTRAY_H
#define KBDTRAY_H

#include <KSystemTrayIcon>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QFont>

class KbdTray : public KSystemTrayIcon
{
    Q_OBJECT
    
public:
    explicit KbdTray(QWidget* parent = 0);
    ~KbdTray();
    QMenu* getContextMenu();

public slots:
    void activationHandler(QSystemTrayIcon::ActivationReason reason);
    
    
signals:
    void requestVisibility();
    
};


#endif // KBDTRAY_H
