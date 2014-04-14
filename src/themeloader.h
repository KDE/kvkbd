/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Todor <email>
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

#ifndef THEMELOADER_H
#define THEMELOADER_H

#include <QObject>
#include <QDomNode>
#include <QDomNamedNodeMap>
#include <QVariant>
#include <QMap>
#include <QMenu>

#include "mainwidget.h"
#include "vbutton.h"


class ThemeLoader : public QObject
{
    Q_OBJECT

public:
    ThemeLoader(QWidget *parent);
    ~ThemeLoader();

    void loadTheme(QString& themeName);
    
    
    void loadColorFile(const QString& fileName);
    
    int loadLayout(const QString& themeName, const QString& path);

    void findColorStyles(QMenu *parent, const QString& selectedStyle);
    
protected:
    void loadKeys(MainWidget *vPart, const QDomNode& wNode);
    bool applyProperty(VButton *btn, const QString& attributeName, QDomNamedNodeMap *attributes, QVariant defaultValue="");

    QMap<QString, int> widthMap;
    QMap<QString, int> heightMap;
    QMap<QString, int> spacingMap;
    
public slots:
    void loadColorStyle();
    
signals:

    void partLoaded(MainWidget *vPart, int total_rows, int total_cols);
    void buttonLoaded(VButton *btn);
    void colorStyleChanged();
};

#endif // THEMELOADER_H
