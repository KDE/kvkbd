#include "vbutton.h"
#include <QPainter>

#define TIMER_INTERVAL_SHORT 40
#define TIMER_INTERVAL_LONG  200

int VButton::RepeatShortDelay = TIMER_INTERVAL_SHORT;
int VButton::RepeatLongDelay = TIMER_INTERVAL_LONG;


VButton::VButton(QWidget *parent) :
    QPushButton(parent)
{

    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_AlwaysShowToolTips);

    keyCode = 0;

    rightClicked = false;
    mTextIndex = 0;
    isCaps = false;

    keyTimer = new QTimer(this);

    connect(keyTimer, SIGNAL(timeout()), this, SLOT(repeatKey()));

}

void VButton::storeSize()
{
    vpos = geometry();

}

QRect VButton::VRect()
{
    return vpos;

}

void VButton::setKeyCode(unsigned int keyCode)
{
    this->keyCode = keyCode;
}

unsigned int VButton::getKeyCode()
{
    return this->keyCode;

}

void VButton::setButtonText(const ButtonText& text)
{
    this->mButtonText = text;
}

ButtonText VButton::buttonText() const
{
    return this->mButtonText;
}

void VButton::setTextIndex(int index)
{
    this->mTextIndex = index;
}

int VButton::textIndex()
{
    return this->mTextIndex;

}

void VButton::nextText()
{
    if (mButtonText.count()<1)return;

    mTextIndex++;
    int textCount = mButtonText.count()-1;
    if (mTextIndex>textCount) mTextIndex=0;


    updateText();


}

void VButton::setCaps(bool mode)
{
    if (mButtonText.count()<1)return;

    isCaps = mode;



}
void VButton::setShift(bool mode)
{
    if (mButtonText.count()<1)return;
    if (mode) {
        this->mTextIndex = 1;
    }
    else {
        this->mTextIndex = 0;

    }
    isShift = mode;
}
void VButton::updateText()
{
    if (mButtonText.count()<1)return;

    QString text = mButtonText.at(this->mTextIndex);
    if (text == "&") {
        text+="&";
    }

    bool doCaps = isCaps ;
    if (isShift) doCaps = !doCaps;

    if (doCaps) {
        text = text.toUpper();
    }
    else {
        text = text.toLower();
    }

    this->setText(text);

}

void VButton::sendKey()
{
    emit keyClick(this->keyCode);
}

void VButton::mousePressEvent(QMouseEvent *e)
{

    rightClicked = false;
    if (e->button() == Qt::RightButton) {

        rightClicked = true;

    }
    QPushButton::mousePressEvent(e);

    if (this->keyCode>0) {
	sendKey();

	if (!isCheckable()) {
	    if (!keyTimer->isActive()) {
		//200 ms is a bit more that the time needed for a single click
		keyTimer->start(VButton::RepeatLongDelay);

	    }

	}
    }
}

void VButton::mouseReleaseEvent(QMouseEvent *e)
{

    if (keyTimer->isActive())keyTimer->stop();
    QPushButton::mouseReleaseEvent(e);
}

void VButton::repeatKey()
{
    //if the user is still pressing the button after 200 ms, we assume
    //he wants the key to be quickly repeated and we decrease the interval
    if (keyTimer->interval() == VButton::RepeatLongDelay) {
        //TODO: make this configurable?
        keyTimer->setInterval(VButton::RepeatShortDelay);
    }

    sendKey();
}
