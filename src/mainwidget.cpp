#include "mainwidget.h"

#include <QObjectList>
#include <QList>

#include "vbutton.h"


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{

}

void MainWidget::setBaseSize(int w, int h)
{
    bsize.setWidth(w);
    bsize.setHeight(h);

}
void MainWidget::updateGroupState(const ModifierGroupStateMap& stateMap)
{
    QObjectList buttons = this->children();
    ModifierGroupStateMapIterator itr(stateMap);
    QList<QChar> text;

    while (itr.hasNext()) {
        itr.next();
        QString group_name = itr.key();
        bool state = itr.value();

        for (int a=0; a<buttons.count(); a++) {

            VButton *btn = (VButton*)buttons.at(a);

            QString group_toggle = btn->property("group_toggle").toString();
            QString btn_group_name = btn->property("group_name").toString();
            QString group_label = btn->property("group_label").toString();
            QString label = btn->property("label").toString();

            if (QString::compare(group_toggle,group_name)==0) {

                if (group_label.length()>0 && label.length()>0) {
                    if (state) {
                        btn->setText(group_label);
                    }
                    else {
                        btn->setText(label);
                    }
                }

            }
            else if (QString::compare(group_name,"capslock")==0) {

                btn->setCaps(state);
                btn->updateText();
            }

            if (QString::compare(btn_group_name, group_name)==0) {
                btn->setChecked(state);
            }

        }

    }

}

void MainWidget::textSwitch(bool setShift)
{
    QObjectList buttons = this->children();

    for (int a=0; a<buttons.count(); a++) {

        VButton *btn = (VButton*)buttons.at(a);

        btn->setShift(setShift);
        btn->updateText();
    }

}
void MainWidget::updateLayout(int index, QString layout_name)
{
    QObjectList buttons = this->children();

    VKeyboard *vkbd = (VKeyboard*)QObject::sender();

    for (int a=0; a<buttons.count(); a++) {

        VButton *btn = (VButton*)buttons.at(a);

        if (btn->property("label").toString().length()<1) {
            ButtonText text;

            vkbd->textForKeyCode(btn->getKeyCode(), text);
            btn->setButtonText(text);
            btn->updateText();

        }

        if (btn->objectName()=="currentLayout") {
            btn->setText(layout_name);
        }
    }

}


void MainWidget::resizeEvent(QResizeEvent *ev)
{

    const QSize& size = ev->size();

    double dw = (double)size.width() / (double)bsize.width();
    double dh = (double)size.height() / (double)bsize.height();

    QObjectList buttons = this->children();
    for (int a=0; a<buttons.count(); a++) {

        VButton *btn = (VButton*)buttons.at(a);
        const QRect& geom = btn->VRect();

        btn->setGeometry((geom.x() * dw), (geom.y() * dh), (geom.width() * dw), (geom.height() * dh));

    }

    updateFont(this->parentWidget()->font());

}

void MainWidget::updateFont(const QFont& widgetFont)
{

    int fontSize = widgetFont.pointSize();
    if ( parentWidget()->property("autoresfont").toBool() ) {
        fontSize = (8.0 / 500.0) * this->parentWidget()->size().width();
    }
    QString buttonStyle = QString("VButton { font-family:'%1'; font-size: %2px; font-weight:%3; font-style: %4; }").arg(widgetFont.family()).arg(fontSize).arg(widgetFont.bold() ? "bold" : "normal").arg(widgetFont.italic() ? "italic" : "normal");
    this->setStyleSheet(buttonStyle);

}
#include "mainwidget.moc"
