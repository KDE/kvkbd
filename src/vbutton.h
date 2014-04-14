#ifndef VBUTTON_H
#define VBUTTON_H

#include <QPushButton>
#include <QRect>
#include <QSize>
#include <QString>
#include <QMouseEvent>
#include <QTimer>
#include <QDomNode>
#include "x11keyboard.h"

class VButton : public QPushButton
{
    Q_OBJECT

public:
    explicit VButton(QWidget *parent = 0);

    void storeSize();

    void reposition(const QSize &baseSize, const QSize &size);
    QRect VRect();

    unsigned int getKeyCode();
    void setKeyCode(unsigned int keyCode);

    void setButtonText(const ButtonText& text);
    ButtonText buttonText() const;

    void setTextIndex(int index);
    int textIndex();
    void updateText();
    void nextText();
    void setCaps(bool mode);
    void setShift(bool mode);

signals:
    void keyClick(unsigned int);
    void buttonAction(const QString& action);

public slots:
    void sendKey();

protected:
    unsigned int keyCode;
    QRect vpos;

    bool rightClicked;
    QTimer *keyTimer;


    ButtonText mButtonText;
    int mTextIndex;

    bool isCaps;
    bool isShift;

    static int RepeatShortDelay;
    static int RepeatLongDelay;

protected slots:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void repeatKey();


};

#endif // VBUTTON_H
