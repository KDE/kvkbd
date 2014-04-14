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

#include "kbdtray.h"

#include <KAction>
#include <KToggleAction>
#include <KToggleAction>
#include <KLocalizedString>
#include <KIcon>
#include <KIconLoader>
#include <KCmdLineArgs>
#include <KHelpMenu>
#include <KMenu>
#include <KFontDialog>
#include <KUniqueApplication>

// TODO: find a way to hide the widget before asking for quit
KbdTray::KbdTray(QWidget* parent) : KSystemTrayIcon(parent)
{
    setObjectName("KvkbdTray");
    setIcon(UserIcon("tray"));

    QAction *titleAction = contextMenuTitle();
    titleAction->setText("Kvkbd");

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activationHandler(QSystemTrayIcon::ActivationReason)));

}

KbdTray::~KbdTray()
{

}




QMenu* KbdTray::getContextMenu()
{
    return this->contextMenu();

}
void KbdTray::activationHandler(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        emit requestVisibility();
    }
}
