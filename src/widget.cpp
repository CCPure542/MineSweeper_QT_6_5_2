#include "widget.h"
#include "ui_widget.h"
#include "rulecusdialog.h"
#include<ctime>
#include<QTime>
#include<QEventLoop>
#include<QCoreApplication>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // ui setting
    ui->clockLabel->setStyleSheet("color:#ff0000;");
    ui->flagNumLabel->setStyleSheet("color:#ff0000;");
    this->setStyleSheet("background-color:rgb(225,225,225);");
    // timer - used to run clock
    timer = new QTimer(this);
    // default difficulty
    setFieldSizeAndMineNum(APPRENTICE);
    storeConnect();
    restartGame();  
}

bool Widget::setFieldSizeAndMineNum(RuleGame rule) {
    switch(rule) {
        case(APPRENTICE):
            fieldWidth = 9;
            fieldHeight = 9;
            numMine = 10;
            return true;
        case(ADEPT):
            fieldWidth = 16;
            fieldHeight = 16;
            numMine = 40;
            return true;
        case(EXPERT):
            fieldWidth = 30;
            fieldHeight = 16;
            numMine = 99;
            return true;
        case(CUSTOMIZATION): {
            RuleCusDialog d;
            int diaCode = d.exec();
            if(diaCode == QDialog::DialogCode::Accepted) {
                fieldWidth = d.inputWidth;
                fieldHeight = d.inputHeight;
                numMine = d.inputMineNum;
                return true;
            }
            else
                return false;
        }
        default:
            return false;
    }
}

void Widget::restartGame() {
    // reset
    ui->btn_Restart->setIcon(QIcon(":/res/star.png"));
    numDetected = 0;
    gameOver = false;
    gameStart(false);
    ui->clockLabel->setText(QString::number(0));
    ui->flagNumLabel->setText(QString::number(numMine));
    // Setting the field size
    ui->fieldWidget->setFixedSize(fieldWidth*25,fieldHeight*25);
    // reset the box array
    qDeleteAll(ui->fieldWidget->findChildren<QObject *>());
    boxArray.resize(0);
    QVector<QVector<BoxMsLabel *>>(boxArray).swap(boxArray);
    MineArray.resize(0);
    QVector<BoxMsLabel *>(MineArray).swap(MineArray);
    // reserve space for box array
    MineArray.reserve(numMine);
    boxArray.reserve(fieldHeight);
    for(int j=0;j<fieldHeight;j++) {
        QVector<BoxMsLabel *> array;
        array.reserve(fieldWidth);
        boxArray.push_back(array);
    }
    // deploy box but not deploy mines first
    for(int h=0;h<fieldHeight;h++) {
        for(int w=0;w<fieldWidth;w++) {
            BoxMsLabel * box = new BoxMsLabel(h,w);
            box->setParent(ui->fieldWidget);
            box->setGeometry(w*25,h*25,25,25);
            box->show();
            boxArray[h].push_back(box);
        }
    }
}

bool Widget::setFlagFromChild(bool flagUsed) {
    int num = ui->flagNumLabel->text().toInt();
    // Try set the flag when flagNum > 0
    if(flagUsed && num!=0) {
        ui->flagNumLabel->setText(QString::number(--num));
        return(true);
    }
    // Try set the flag when flagNum == 0
    else if(flagUsed && num==0){
        return(false);
    }
    // Try remove flag
    else if(!flagUsed){
        ui->flagNumLabel->setText(QString::number(++num));
        return(true);
    }
    else
        return(false);
}

void Widget::gameStart(bool isStartLocal) {
    isStart = isStartLocal;
    emit signalGameStart(isStartLocal);
}

int Widget::accessSurroundBox(BoxMsLabel * box_local) {
    int h_box_local = box_local->getPosH();
    int w_box_local = box_local->getPosW();
    QVector<BoxMsLabel *> targetArray;
    int numReturn = 0;
    // Randomly deploy mines only after the first click and assign the attribute 'isStart'
    if (!isStart) {
        deployMine(h_box_local,w_box_local);
        gameStart(true);
    }
    // access surronding boxes
    for(int h_target = h_box_local-1; h_target <= h_box_local+1; h_target++) {
        for(int w_target = w_box_local-1; w_target <= w_box_local+1; w_target++) {
            if(h_target<0 || h_target >= fieldHeight) continue;
            else if (w_target<0 || w_target >= fieldWidth) continue;
            else {
                BoxMsLabel * box_target = boxArray[h_target][w_target];
                targetArray.push_back(box_target);
                if(box_target->isMine())
                    numReturn++;
                else
                    continue;
            }
        }
    }
    // if the current box show 0 then automatically detect other surronding boxes
    if(numReturn==0) {
        for(int i=0;i<targetArray.size();i++) {
            targetArray[i]->detectAuto();
        }
    }
    return(numReturn);
}

void Widget::deployMine(int h_avoid,int w_avoid) {
    // Randomly deploy mines except for the first click position
    int numHaveDeployed = 0;
    int h_rand;
    int w_rand;
    srand((unsigned int)time(NULL));
    qDebug() << MineArray.size();
    while(numHaveDeployed<numMine) {
        h_rand = rand()%fieldHeight;
        w_rand = rand()%fieldWidth;
        BoxMsLabel * box_rand = boxArray[h_rand][w_rand];
        if(h_rand==h_avoid && w_rand==w_avoid)
            continue;
        else if(MineArray.count(box_rand) > 0) {
            continue;
        }
        else {
            MineArray.push_back(box_rand);
            box_rand->setMine();
            numHaveDeployed++;
        }
    }
    qDebug() << MineArray.size();
}

void Widget::flashBox(BoxMsLabel * box_local) {
    int h_box_local = box_local->getPosH();
    int w_box_local = box_local->getPosW();
    QVector<BoxMsLabel *> targetArray;
    for(int h_target = h_box_local-1; h_target <= h_box_local+1; h_target++) {
        for(int w_target = w_box_local-1; w_target <= w_box_local+1; w_target++) {
            if(h_target<0 || h_target >= fieldHeight) continue;
            else if (w_target<0 || w_target >= fieldWidth) continue;
            else {
                BoxMsLabel * box_target = boxArray[h_target][w_target];
                if(!box_target->isDetected())
                    targetArray.push_back(box_target);
                else
                    continue;
            }
        }
    }
    QTime _time = QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < _time) {
        for(int i=0;i<targetArray.size();i++) {
            targetArray[i]->hide();
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        for(int i=0;i<targetArray.size();i++) {
            targetArray[i]->show();
        }
    }
}

void Widget::setOver() {
    gameOver = true;
    emit signalGameStart(false);
}

bool Widget::isOver() {
    return(gameOver);
}

bool Widget::checkIfVictory() {
    if(numDetected==(fieldHeight*fieldWidth) - numMine) {
        return true;
    }
    else
        return false;
}

void Widget::setCentreIcon(bool stat) {
    if(stat)
        ui->btn_Restart->setIcon(QIcon(":/res/victory.png"));
    else
        ui->btn_Restart->setIcon(QIcon(":/res/fail.png"));
}

void Widget::storeConnect() {
    // The signal/slot when click the center star button
    connect(ui->btn_Restart,&QPushButton::clicked,this,[=](){
        restartGame();
    });
    // The signal/slot when click the top 4 buttons that set difficulty
    connect(ui->btn_apprentice,&QPushButton::clicked,this,[=](){
        bool result = setFieldSizeAndMineNum(APPRENTICE);
        if(result==true) restartGame();
    });
    connect(ui->btn_adept,&QPushButton::clicked,this,[=](){
        bool result = setFieldSizeAndMineNum(ADEPT);
        if(result==true) restartGame();
    });
    connect(ui->btn_expert,&QPushButton::clicked,this,[=](){
        bool result = setFieldSizeAndMineNum(EXPERT);
        if(result==true) restartGame();
    });
    connect(ui->btn_custom,&QPushButton::clicked,this,[=](){
        bool result = setFieldSizeAndMineNum(CUSTOMIZATION);
        if(result==true) restartGame();
    });
    // The first part signal/slot for timer
    connect(this,&Widget::signalGameStart,this,[=](bool targetStat){
        if(targetStat) {
            timer->start(1000);
        }
        if(!targetStat) {
            timer->stop();
        }
    });
    // The second part signal/slot for timer
    connect(timer,&QTimer::timeout,ui->clockLabel,[=](){
        int num = ui->clockLabel->text().toInt()+1;
        ui->clockLabel->setText(QString::number(num));
    });
}

Widget::~Widget()
{
    delete ui;
    if(timer!=NULL) {
        delete timer;
    }
}
