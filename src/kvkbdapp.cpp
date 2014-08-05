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

#include "kvkbdapp.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QDomDocument>
#include <QFile>
#include <QGridLayout>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QDir>

#include <KConfig>
#include <KConfigGroup>
#include <KAction>
#include <KToggleAction>
#include <KHelpMenu>
#include <KFontDialog>

#include <KCmdLineArgs>


#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <fixx11h.h>



QList<VButton*> modKeys;

#include <iostream>
using namespace std;

#define DEFAULT_WIDTH 	640
#define DEFAULT_HEIGHT 	210

#include "x11keyboard.h"


KvkbdApp::KvkbdApp(bool loginhelper) : KUniqueApplication(), is_login(loginhelper)
// : KApplication()
{

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(buttonAction(const QString &)));

    widget = new ResizableDragWidget(0);
    widget->setContentsMargins(10,10,10,10);
    widget->setProperty("name", "main");

    KConfigGroup cfg = KGlobal::config()->group("");

    
    
    if (!is_login) {
      widget->setAttribute(Qt::WA_ShowWithoutActivating);
      widget->setAttribute(Qt::WA_DeleteOnClose, false);
    }

    widget->setWindowFlags( Qt::ToolTip | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint );

    dock = new KbdDock(widget->winId());
    connect(dock, SIGNAL(requestVisibility()), widget, SLOT(toggleVisibility()));

    tray = new KbdTray(widget);
    connect(tray, SIGNAL(requestVisibility()), widget, SLOT(toggleVisibility()));
   
    
    layout = new QGridLayout(widget);
    layout->setContentsMargins(0,0,0,0);
    widget->setLayout(layout);


    xkbd = new X11Keyboard(this);


    themeLoader = new ThemeLoader(widget);


    connect(themeLoader, SIGNAL(partLoaded(MainWidget*, int, int)), this, SLOT(partLoaded(MainWidget*, int, int)));
    connect(themeLoader, SIGNAL(buttonLoaded(VButton*)), this, SLOT(buttonLoaded(VButton*)));



    
    QMenu *cmenu = tray->contextMenu();

    KAction *chooseFontAction = new KAction(KIcon("preferences-desktop-font"), i18nc("@action:inmenu", "Choose Font..."), this);
    connect(chooseFontAction, SIGNAL(triggered(bool)), this, SLOT(chooseFont()) );
    cmenu->addAction(chooseFontAction);

    KToggleAction *autoResizeAction = new KToggleAction(i18nc("@action:inmenu", "Auto Resize Font"), this);
    bool autoResizeEnabled = cfg.readEntry("autoresfont",true);
    autoResizeAction->setChecked(autoResizeEnabled);
    widget->setProperty("autoresfont", autoResizeEnabled);
    cmenu->addAction(autoResizeAction);
    connect(autoResizeAction,SIGNAL(triggered(bool)), this, SLOT(autoResizeFont(bool)));


    bool blur = cfg.readEntry("blurBackground", QVariant(true)).toBool();

    KToggleAction *blurBackgroundAction = new KToggleAction(i18nc("@action:inmenu", "Blur Background"), this);
    blurBackgroundAction->setChecked(blur);
    cmenu->addAction(blurBackgroundAction);
    widget->blurBackground(blur);
    connect(blurBackgroundAction,SIGNAL(triggered(bool)), widget, SLOT(blurBackground(bool)));
    dock->blurBackground(blur);
    connect(blurBackgroundAction,SIGNAL(triggered(bool)), dock, SLOT(blurBackground(bool)));
    widget->blurBackground(blur);
    dock->blurBackground(blur);

    bool dockVisible = cfg.readEntry("showdock", QVariant(false)).toBool();
    KToggleAction *showDockAction = new KToggleAction(i18nc("@action:inmenu", "Show Dock"), this);
    showDockAction->setChecked(dockVisible);
    cmenu->addAction(showDockAction);
    connect(showDockAction,SIGNAL(triggered(bool)), dock, SLOT(setVisible(bool)));

    bool isLocked = cfg.readEntry("locked", QVariant(false)).toBool();
    KToggleAction *lockOnScreenAction = new KToggleAction(i18nc("@action:inmenu", "Lock on Screen"), this);
    lockOnScreenAction->setChecked(isLocked);
    cmenu->addAction(lockOnScreenAction);
    connect(lockOnScreenAction,SIGNAL(triggered(bool)), widget, SLOT(setLocked(bool)));

    bool stickyModKeys = cfg.readEntry("stickyModKeys", QVariant(false)).toBool();
    KToggleAction *stickyModKeysAction = new KToggleAction(i18nc("@action:inmenu", "Sticky Modifier Keys"), this);
    stickyModKeysAction->setChecked(stickyModKeys);
    cmenu->addAction(stickyModKeysAction);
    connect(stickyModKeysAction,SIGNAL(triggered(bool)), this, SLOT(setStickyModKeys(bool)));
    widget->setProperty("stickyModKeys", stickyModKeys);
    
    
    QFont font = cfg.readEntry("font", widget->font());
    widget->setFont(font);

    QString colorsFilename = cfg.readEntry("colors");
    QMenu *colors = new QMenu(widget);
    themeLoader->findColorStyles(colors, colorsFilename);
    cmenu->addMenu(colors);
    connect(themeLoader, SIGNAL(colorStyleChanged()), widget, SLOT(repaint()));
    connect(themeLoader, SIGNAL(colorStyleChanged()), dock, SLOT(repaint()));

    KHelpMenu *helpMenu = new KHelpMenu(widget, KCmdLineArgs::aboutData());
    cmenu->addMenu((QMenu*)helpMenu->menu());

    QString themeName = cfg.readEntry("layout", "standart");
    themeLoader->loadTheme(themeName);
    widget->setProperty("layout", themeName);


    QSize defaultSize(DEFAULT_WIDTH,DEFAULT_HEIGHT);
    QDesktopWidget *pDesktop = QApplication::desktop();

    QRect screenGeometry = pDesktop->availableGeometry(pDesktop->underMouse());
    qDebug() << "ScreenGeometry: " << screenGeometry;

    QPoint bottomRight = screenGeometry.bottomRight()-QPoint(defaultSize.width(), defaultSize.height());

    QRect widgetGeometry(bottomRight, defaultSize);
    qDebug() << "widgetGeometry: " << widgetGeometry;

    QRect c_geometry = cfg.readEntry("geometry", widgetGeometry);
    if (!screenGeometry.contains(c_geometry, true)) {
        c_geometry = widgetGeometry;
    }
    widget->setGeometry(c_geometry);

    QPoint pos = c_geometry.topLeft();
    pos.setY(pos.y()-dock->height());

    QRect dockGeometry(pos, dock->size());
    
    QRect c_dock_geometry = cfg.readEntry("dockGeometry", dockGeometry);
    if (!screenGeometry.contains(c_dock_geometry, true)) {
	c_dock_geometry = dockGeometry;
    }
    dock->setGeometry(c_dock_geometry);
    
    widget->show();

    
    bool extensionVisible = cfg.readEntry("extentVisible", QVariant(true)).toBool();
    if (!extensionVisible) {
        toggleExtension();
    }

    
    
    setQuitOnLastWindowClosed (is_login);
    
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(storeConfig()));
    emit fontUpdated(widget->font());

    if (dockVisible && !is_login) {
      dock->show();
    }
    
    xkbd->start();

    if (!is_login) {
      bool vis = cfg.readEntry("visible", QVariant(true)).toBool();
      if (!vis ) {
	
	widget->showMinimized();
	
      }
      widget->setWindowTitle("kvkbd");
      tray->show();
    }
    else {
	QTimer *timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), widget, SLOT(raise()));
	timer->start();
	widget->setWindowTitle("kvkbd.login");
    }

}

KvkbdApp::~KvkbdApp()
{

}

void KvkbdApp::storeConfig()
{
    KConfigGroup cfg = KGlobal::config()->group("");
    
    cfg.writeEntry("visible", widget->isVisible());
    cfg.writeEntry("geometry", widget->geometry());
    cfg.writeEntry("locked", widget->isLocked());
    cfg.writeEntry("stickyModKeys", widget->property("stickyModKeys"));
    
    cfg.writeEntry("showdock", dock->isVisible());
    cfg.writeEntry("dockGeometry", dock->geometry());
    
    cfg.writeEntry("layout", widget->property("layout"));
    cfg.writeEntry("colors", widget->property("colors"));
    cfg.writeEntry("font", widget->font());
    cfg.writeEntry("autoresfont", widget->property("autoresfont").toBool());
    cfg.writeEntry("blurBackground", widget->property("blurBackground").toBool());

    MainWidget *prt = parts.value("extension");
    if (prt) {
        cfg.writeEntry("extentVisible", prt->isVisible());
    }

    cfg.sync();

}


void KvkbdApp::autoResizeFont(bool mode)
{
    widget->setProperty("autoresfont", QVariant(mode));
    emit fontUpdated(widget->font());

}
void KvkbdApp::setStickyModKeys(bool mode)
{
    widget->setProperty("stickyModKeys", QVariant(mode));
}

void KvkbdApp::chooseFont()
{
    bool restore = false;

    if (widget->isVisible()) {
        widget->hide();
        restore = true;
    }

    QFont widgetFont = widget->font();

    int result = KFontDialog::getFont( widgetFont, KFontChooser::NoDisplayFlags, widget );
    if ( result == KFontDialog::Accepted ) {

        widget->setFont(widgetFont);
        emit fontUpdated(widgetFont);

    }

    if (restore) {
        widget->show();
    }


}




void KvkbdApp::buttonLoaded(VButton *btn)
{
    if (btn->property("modifier").toBool() == true) {
        modKeys.append(btn);
    }
    else {
        QObject::connect(btn, SIGNAL(keyClick(unsigned int)), xkbd, SLOT(processKeyPress(unsigned int)) );
    }
    QString bAction = btn->property("action").toString();

    if (bAction.length()>0) {
        connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(btn, bAction);
        actionButtons.insertMulti(bAction, btn);
    }

    QString tooltip = btn->property("tooltip").toString();
    if (tooltip.length()>0) {
        btn->setToolTip(tooltip);

    }

}
void KvkbdApp::partLoaded(MainWidget *vPart, int total_rows, int total_cols)
{
    //cout << "Col Strech: " << total_cols << endl;
    // cout << "Row Strech: " << total_rows << endl;

    QString partName = vPart->property("part").toString();

    int row_pos = 0;
    int col_pos = 0;

    if (layoutPosition.count()>0) {
        QString partName = layoutPosition.keys().at(0);
        QRect lp = layoutPosition.value(partName);
        col_pos = lp.width();
    }

    layout->addWidget(vPart,row_pos,col_pos,total_rows,total_cols);
    //cout << "Insert to layout: " << qPrintable(partName) << " RowStart: " << row_pos << " ColStart: " << col_pos << " RowSpan: " << total_rows << " ColSpan: " << total_cols << endl;
    parts.insert(partName, vPart);
    layoutPosition.insert(partName, QRect(col_pos,row_pos,total_cols,total_rows));

    QObject::connect(xkbd, SIGNAL(layoutUpdated(int,QString)), vPart, SLOT(updateLayout(int,QString)));
    QObject::connect(xkbd, SIGNAL(groupStateChanged(const ModifierGroupStateMap&)), vPart, SLOT(updateGroupState(const ModifierGroupStateMap&)));
    QObject::connect(xkbd, SIGNAL(keyProcessComplete(unsigned int)), this, SLOT(keyProcessComplete(unsigned int)));
    
    QObject::connect(this, SIGNAL(textSwitch(bool)), vPart, SLOT(textSwitch(bool)));
    QObject::connect(this, SIGNAL(fontUpdated(const QFont&)), vPart, SLOT(updateFont(const QFont&)));



}
void KvkbdApp::keyProcessComplete(unsigned int)
{
    if (widget->property("stickyModKeys").toBool()) return;
    
    QListIterator<VButton *> itr(modKeys);
    while (itr.hasNext()) {
        VButton *mod = itr.next();
        if (mod->isChecked()) {
            mod->click();
        }
    }
}

void KvkbdApp::buttonAction(const QString &action)
{

    if (QString::compare(action , "toggleVisibility")==0) {
        if (!is_login) {
	  widget->toggleVisibility();
	}
    }
    else if (QString::compare(action , "toggleExtension")==0) {

        toggleExtension();
    }
    else if (QString::compare(action, "shiftText")==0) {
        if (actionButtons.contains(action)) {
            QList<VButton*> buttons = actionButtons.values(action);
            QListIterator<VButton *> itr(buttons);
            bool setShift = false;
            while (itr.hasNext()) {
                VButton *btn = itr.next();
                if (btn->isCheckable() && btn->isChecked()) setShift=true;
            }
            emit textSwitch(setShift);
        }
    }
}
void KvkbdApp::toggleExtension()
{
    MainWidget *prt = parts.value("extension");
    if (prt->isVisible()) {
        prt->hide();
        layout->removeWidget(prt);
    }
    else {
        QString partName = prt->property("part").toString();
        QRect span = layoutPosition.value(partName);
        layout->addWidget(prt,span.y(),span.x(), span.height(), span.width());
        prt->show();
    }
}




#include "kvkbdapp.moc"
