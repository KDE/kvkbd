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

#include "themeloader.h"

#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QFileInfo>
#include <QDir>
#include <QMenu>

#include <KUniqueApplication>
#include <KStandardDirs>
#include <KAction>
#include <KToggleAction>


#include <iostream>
using namespace std;


int defaultWidth = 25;
int defaultHeight = 25;

#define DEFAULT_CSS ":/theme/standart.css"

ThemeLoader::ThemeLoader(QWidget *parent) : QObject(parent)
{

}

ThemeLoader::~ThemeLoader()
{

}
void ThemeLoader::loadTheme(QString& themeName)
{
    bool loading = true;
    while (loading) {
        int ret = this->loadLayout(themeName, ":/theme/");

        if (ret == 0) {
            break;
        }
        //bail out
        else {
            if (QString::compare(themeName, "standart")==0) {
                loading = false;
                kapp->quit();
            }
            else {
                themeName="standart";
            }
        }
    }
}
void ThemeLoader::loadColorFile(const QString& fileName)
{
    QFile themeFile;

    themeFile.setFileName(fileName);

    if (!themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QMessageBox::information(0, "Error", QString("Unable to open css file: %1").arg(themeFile.fileName()));
        return;
    }

    ((QWidget*)parent())->setStyleSheet(themeFile.readAll());
    ((QWidget*)parent())->setProperty("colors", fileName);
    themeFile.close();
    
    ((QWidget*)parent())->repaint();
    
    emit colorStyleChanged();
    
    
}
void ThemeLoader::loadColorStyle()
{

    QAction *action = (QAction*)QObject::sender();
   
    QFileInfo info(action->data().toString());
    
    this->loadColorFile(info.absoluteFilePath());

}
void ThemeLoader::findColorStyles(QMenu *colors, const QString& configSelectedStyle)
{
    KStandardDirs kdirs;
    QStringList dirs = kdirs.findDirs("data", "kvkbd");

    QActionGroup *color_group = new QActionGroup(colors);
    color_group->setExclusive(true);


    QAction *item = new QAction(colors);
    item->setCheckable(true);
    item->setText("standart");
    item->setData(":/theme/standart.css");
    colors->addAction(item);
    color_group->addAction(item);
    
    



    colors->setTitle("Color Style");
    colors->setIcon(KIcon("preferences-desktop-color"));
    QListIterator<QString> itr(dirs);
    while (itr.hasNext()) {
        QString data_path = itr.next() + "colors";

        QFileInfo info(data_path);
        if (info.isDir() && info.isReadable()) {
            QDir colors_dir(info.absoluteFilePath(), "*.css");
            QFileInfoList list = colors_dir.entryInfoList();

            QListIterator<QFileInfo> itr(list);
            while (itr.hasNext()) {
                QFileInfo fileInfo = itr.next();
                //std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                //				    .arg(fileInfo.fileName()));
                //std::cout << std::endl;

                QAction *item = new QAction(this);
                item->setCheckable(true);

                item->setText(fileInfo.baseName());
                item->setData(fileInfo.absoluteFilePath());
                colors->addAction(item);
                color_group->addAction(item);


            }

        }

    }

    QString selectedStyle = configSelectedStyle;
    if (selectedStyle.length()<1) {
	selectedStyle = DEFAULT_CSS;
    }
    QAction *selectedAction = 0;
    
    QListIterator<QAction*> itrActions(color_group->actions());
    while (itrActions.hasNext()) {
        QAction *item = itrActions.next();
	
	if (item->data().toString() == selectedStyle) {
	    item->setChecked(true);
	    selectedAction = item;
	}
      
	
        connect(item, SIGNAL(triggered(bool)), this, SLOT(loadColorStyle()));
    }
    
    if (selectedAction) {
      selectedAction->trigger();
    }

}


int ThemeLoader::loadLayout(const QString& themeName, const QString& path)
{

//     const KArchiveDirectory * KArchive::directory	(		)	 const


    QFile themeFile;



    QDomDocument doc;

    themeFile.setFileName(QString(path + "%1.xml").arg(themeName));

    if (!themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "Error", QString("Unable to open theme xml file: %1").arg(themeFile.fileName()));
        return -1;
    }
    if (!doc.setContent(&themeFile)) {
        QMessageBox::information(0, "Error", QString("Unable to parse theme xml file: %1").arg(themeFile.fileName()));
        return -2;
    }
    themeFile.close();


    QDomElement docElem = doc.documentElement();

    QDomNodeList wList = docElem.elementsByTagName("buttonWidth");
    QDomNode wNode = wList.at(0);

    //read default button width
    defaultWidth = wNode.attributes().namedItem("width").toAttr().value().toInt();

    QDomNodeList nList = (wNode.toElement()).elementsByTagName("item");
    for (int a=0; a<nList.count(); a++) {
        QDomNode node = nList.at(a);
        int width = node.attributes().namedItem("width").toAttr().value().toInt();
        QString hintName = node.attributes().namedItem("name").toAttr().value();
        widthMap.insert(hintName, width);
        //cout  << "widths[" << qPrintable(hintName) << "]=>"<< width << endl;
    }

    wList = docElem.elementsByTagName("buttonHeight");
    wNode = wList.at(0);
    nList = (wNode.toElement()).elementsByTagName("item");
    for (int a=0; a<nList.count(); a++) {
        QDomNode node = nList.at(a);
        int height = node.attributes().namedItem("height").toAttr().value().toInt();
        QString hintName = node.attributes().namedItem("name").toAttr().value();
        heightMap.insert(hintName, height);
        //cout  << "heights[" << qPrintable(hintName) << "]=>"<< height << endl;
    }


    wList = docElem.elementsByTagName("spacingHints");
    wNode = wList.at(0);
    nList = (wNode.toElement()).elementsByTagName("item");
    for (int a=0; a<nList.count(); a++) {
        QDomNode node = nList.at(a);
        int width = node.attributes().namedItem("width").toAttr().value().toInt();
        QString hintName = node.attributes().namedItem("name").toAttr().value();
        spacingMap.insert(hintName, width);
        //cout  << "spacing[" << qPrintable(hintName) << "]=>"<< width << endl;
    }


    wList = docElem.elementsByTagName("part");
    wNode = wList.at(0);

    //insert main part to widget
    QString partName = wNode.attributes().namedItem("name").toAttr().value();

    MainWidget *part = new MainWidget((QWidget*)parent());
    part->setProperty("part", "main");


    loadKeys(part, wNode);

    wList = wNode.childNodes();

    for (int a=0; a<wList.size(); a++) {

        QDomNode wNode = wList.at(a);
        if (wNode.toElement().tagName() == "extension") {
            MainWidget *widget1 = new MainWidget((QWidget*)parent());
            widget1->setProperty("part", "extension");
            loadKeys(widget1, wNode);
            break;
        }
    }



    return 0;
}
bool ThemeLoader::applyProperty(VButton *btn, const QString& attributeName, QDomNamedNodeMap *attributes, QVariant defaultValue)
{
    bool ret = false;

    QString attributeValue = attributes->namedItem(attributeName).toAttr().value();
    if (attributeValue.length()>0) {
        btn->setProperty(qPrintable(attributeName), attributeValue);
        ret = true;
    }
    else {
        if (defaultValue.toString().length()>0) {
            btn->setProperty(qPrintable(attributeName), defaultValue);
            ret = true;
        }
    }
    return ret;
}
void ThemeLoader::loadKeys(MainWidget *vPart, const QDomNode& wNode)
{
    int max_sx = 0;
    int max_sy = 0;

    int sx = 0;
    int sy = 0;
    int rowMarginLeft = 0;
    int rowSpacingY = 0;
    int rowSpacingX = 0;

    int total_cols = 0;
    int total_rows = 0;

    QDomNodeList nList = wNode.childNodes();

    //(wNode.toElement()).elementsByTagName("row");

    for (int a=0; a<nList.size(); a++) {

        QDomNode wNode = nList.at(a);
        if (wNode.toElement().tagName() != "row") continue;

        total_rows++;

        int rowHeight = defaultHeight;

        int row_buttons = 0;


        QDomNodeList key_list = wNode.childNodes();

        QString rowHeightHint = wNode.attributes().namedItem("height").toAttr().value();
        if (heightMap.contains(rowHeightHint)) {
            rowHeight = heightMap.value(rowHeightHint);
        }


        for (int b=0; b<key_list.count(); b++) {
            QDomNode node = key_list.at(b);
            QDomNamedNodeMap attributes = node.attributes();

            if (node.toElement().tagName()== "key") {

                VButton *btn = new VButton(vPart);
                row_buttons++;
                //width

                int buttonWidth = defaultWidth;
                int buttonHeight = defaultHeight;

                QString widthHint = attributes.namedItem("width").toAttr().value();
                if (widthMap.contains(widthHint)) {
                    buttonWidth = widthMap.value(widthHint);
                }

                QString heightHint = attributes.namedItem("height").toAttr().value();
                if (heightMap.contains(heightHint)) {
                    buttonHeight = heightMap.value(heightHint);
                }

                //name
                QString button_name = attributes.namedItem("name").toAttr().value();
                if (button_name.length()>0) {
                    btn->setObjectName(button_name);
                }


                if (applyProperty(btn, "label", &attributes)) {
                    btn->setText(btn->property("label").toString());
                }


                applyProperty(btn, "group_label", &attributes);

                applyProperty(btn, "group_toggle", &attributes);

                applyProperty(btn, "group_name", &attributes);

                applyProperty(btn, "colorGroup", &attributes, "normal");

		applyProperty(btn, "tooltip", &attributes);

                QString modifier = attributes.namedItem("modifier").toAttr().value();
                if (modifier.toInt()>0) {
                    btn->setProperty("modifier", true);
                    btn->setCheckable(true);
                }

                unsigned int key_code = attributes.namedItem("code").toAttr().value().toInt();
                if (key_code>0) {
                    btn->setKeyCode(key_code);
                }

                if (applyProperty(btn, "action", &attributes)) {
                    btn->setProperty("action", btn->property("action").toString());

                }

                
                int is_checkable = attributes.namedItem("checkable").toAttr().value().toInt();
                if (is_checkable>0) {
                    btn->setCheckable(true);
                    btn->setChecked(false);
                }

                btn->move(sx,sy);
                btn->resize(buttonWidth, buttonHeight);
                btn->storeSize();

// 		btn->setNode(node);
                // cout  << "ColorGroup: " << qPrintable(colorGroup) << endl;

                sx+=buttonWidth+rowSpacingX;

                emit buttonLoaded(btn);
            }
            else if (node.toElement().tagName()=="spacing") {

                QString widthHint = attributes.namedItem("width").toAttr().value();
                QString heightHint = attributes.namedItem("height").toAttr().value();


                if (spacingMap.contains(widthHint)) {
                    int spacingWidth = spacingMap.value(widthHint);

                    sx+=spacingWidth;


                }
                if (heightMap.contains(heightHint)) {
                    int spacingHeight = heightMap.value(heightHint);

                    if (spacingHeight>rowHeight) rowHeight = spacingHeight;

                }

            }

            //cout << "X=>"<<sx<<" | Y=>"<<sy<<endl;
            //
        }//row

        if (sx>max_sx)max_sx = sx;

        sy+=(rowHeight+rowSpacingY);
        sx=0+rowMarginLeft;

        if (row_buttons>total_cols)total_cols=row_buttons;

    }
    if (sy>max_sy) max_sy = sy;

    vPart->setBaseSize(max_sx, max_sy);

    emit partLoaded(vPart, total_rows, total_cols);
    


}

#include "themeloader.moc"
