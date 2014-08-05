/*
 * This file is part of the Kvkbd project.
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

#ifndef KVKBDAPP_H
#define KVKBDAPP_H

#include <KUniqueApplication>
#include <QAction>
#include <QSignalMapper>
#include <QGridLayout>
#include <QDomNode>
#include <QDomNamedNodeMap>

#include "resizabledragwidget.h"
#include "mainwidget.h"
#include "kbdtray.h"
#include "vbutton.h"
#include "themeloader.h"
#include "kbddock.h"
#include "vkeyboard.h"

class KvkbdApp : public KUniqueApplication
{
    Q_OBJECT

public:
    KvkbdApp(bool loginhelper=false);
    ~KvkbdApp();

    

public slots:
    void keyProcessComplete(unsigned int);
    
    void buttonAction(const QString& action);
    void storeConfig();
    void toggleExtension();

    void chooseFont();
    void autoResizeFont(bool mode);
    void setStickyModKeys(bool mode);
   
    
    void partLoaded(MainWidget *vPart, int total_rows, int total_cols);
    void buttonLoaded(VButton *btn);

    
    
protected:

    ResizableDragWidget *widget;

    KbdTray *tray;


    QMap<QString, QString> colorMap;
    QMap<QString, MainWidget*> parts;
    QMap<QString, QRect> layoutPosition;

    QSignalMapper *signalMapper;

    
    QGridLayout *layout;

    
    QMap<QString, VButton*> actionButtons;
    
    ThemeLoader *themeLoader;
    
    KbdDock *dock;
    
    VKeyboard *xkbd;
    
    bool is_login;
    
signals:
    void textSwitch(bool);
    void fontUpdated(const QFont& font);
    void startupCompleted();
};

#endif // KVKBDAPP_H
