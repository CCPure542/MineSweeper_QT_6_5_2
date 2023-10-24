#include "boxmslabel.h"
#include "widget.h"
#include<QMouseEvent>
BoxMsLabel::BoxMsLabel(int h, int w, QWidget *parent)
    : QLabel{parent},m_h(h),m_w(w)
{
    boxMine = false;
    boxDetected = false;
    boxFlag = false;
    setPixmap(QPixmap(":/res/box_normal.png"));
}

QObject * BoxMsLabel::getMainWidget() {
    return(parent()->parent()->parent()->parent()->parent());
}

void BoxMsLabel::mousePressEvent(QMouseEvent * ev) {
    setIcon(ev);
}

void BoxMsLabel::setIcon(QMouseEvent * ev) {
    Widget * w = (Widget *)getMainWidget();
    if((ev->buttons() & Qt::RightButton) && !w->isOver()) {
        // Undetected box - right click - set a flag
        if((!boxDetected) && (!boxFlag) && w->setFlagFromChild(true)) {
            setPixmap(QPixmap(":/res/flag.png"));
            boxFlag = true;
        }
        // flag box - right click - remove flag
        else if((!boxDetected) && (boxFlag) && w->setFlagFromChild(false)) {
            setPixmap(QPixmap(":/res/box_normal.png"));
            boxFlag = false;
        }
    }
    if((ev->buttons() & Qt::LeftButton) && !w->isOver()) {
        // Undetected box - Left click - touch non-mine box
        if((!boxDetected) && (!boxFlag) && (!boxMine)) {
            boxDetected = true;
            int numReturn = w->accessSurroundBox(this);
            showNumber(numReturn);
        }
        // Undetected box - Left click - touch mine box
        else if((!boxDetected) && (!boxFlag) && (boxMine)) {
            w->setOver();
            w->setCentreIcon(false);
            showMine(true);
        }
        // flag box - left click - remove flag
        else if((!boxDetected) && (boxFlag) && w->setFlagFromChild(false)) {
            setPixmap(QPixmap(":/res/box_normal.png"));
            boxFlag = false;
        }
        // Detected box - left click - double blink surronding boxes
        else if(boxDetected) {
            w->flashBox(this);
        }
    }
}

void BoxMsLabel::detectAuto() {
    Widget * w = (Widget *)getMainWidget();
    if(boxMine==true) return;
    if(boxDetected==true) return;
    if(boxFlag==true) return;
    boxDetected = true;
    int numReturn = w->accessSurroundBox(this);
    showNumber(numReturn);
}

void BoxMsLabel::showNumber(int numShow) {
    switch(numShow) {
    case 0:
        setPixmap(QPixmap(":/res/box_0.png"));
        break;
    case 1:
        setPixmap(QPixmap(":/res/box_1.png"));
        break;
    case 2:
        setPixmap(QPixmap(":/res/box_2.png"));
        break;
    case 3:
        setPixmap(QPixmap(":/res/box_3.png"));
        break;
    case 4:
        setPixmap(QPixmap(":/res/box_4.png"));
        break;
    case 5:
        setPixmap(QPixmap(":/res/box_5.png"));
        break;
    case 6:
        setPixmap(QPixmap(":/res/box_6.png"));
        break;
    case 7:
        setPixmap(QPixmap(":/res/box_7.png"));
        break;
    case 8:
        setPixmap(QPixmap(":/res/box_8.png"));
        break;
    }
    // check if all non-mine boxes is clear
    Widget * w = (Widget *)getMainWidget();
    w->numDetected++;
    if(w->checkIfVictory()) {
        w->setOver();
        w->setCentreIcon(true);
        showMine(false);
    }
}

void BoxMsLabel::showMine(bool stat) {
    Widget * w = (Widget *)getMainWidget();
    for(int i=0;i<w->MineArray.size();i++) {
        w->MineArray[i]->setPixmap(QPixmap(":/res/mine.png"));
    }
    // if you touch a mine then this box should be mine with red background
    if(stat==true) {
        setPixmap(QPixmap(":/res/explode.png"));
    }
}

int BoxMsLabel::getPosH() {
    return(m_h);
}

int BoxMsLabel::getPosW() {
    return(m_w);
}

bool BoxMsLabel::isMine() {
    return(boxMine);
}

bool BoxMsLabel::isDetected() {
    return(boxDetected);
}

void BoxMsLabel::setMine() {
    boxMine = true;
}

BoxMsLabel::~BoxMsLabel() {
//    qDebug() << "destructor_BoxMsLabel";
}
