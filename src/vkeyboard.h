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

#ifndef VKEYBOARD_H
#define VKEYBOARD_H

#include <QObject>
#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QChar>

//caps state, numlock state
typedef QMap<QString, bool> ModifierGroupStateMap;
typedef QMapIterator<QString, bool> ModifierGroupStateMapIterator;
//normal text, shift text
typedef QList<QChar> ButtonText;

class VKeyboard : public QObject
{
    Q_OBJECT

public:
    VKeyboard(QObject *parent=0);
    virtual ~VKeyboard();
    
    virtual void textForKeyCode(unsigned int keyCode, ButtonText& text)=0;
    
public slots:
    virtual void processKeyPress(unsigned int)=0;
    virtual void queryModState()=0;
    virtual void constructLayouts()=0;
    virtual void layoutChanged()=0;
    virtual void start()=0;
    
signals:
    //key sent successfully
    void keyProcessComplete(unsigned int);

    void groupStateChanged(const ModifierGroupStateMap& modifier_state);

    //layout index in list, layout caption
    void layoutUpdated(int, QString);
};

#endif // VKEYBOARD_H
