#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "boxmslabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    /* Public Attributes */
    // Number of detected boxes used to check if victory
    int numDetected;
    // Arrays - Points to boxes with mines
    QVector<BoxMsLabel *> MineArray;
    /* Public Functions */
    // Set Flag icons
    bool setFlagFromChild(bool);
    // Assign the Attribute 'isStart' and emit the signal
    void gameStart(bool);
    // Assign the attribute 'gameOver' to true
    void setOver();
    // Return attribute 'gameOver' bool value
    bool isOver();
    // check if current game had deployed mines (and clock)
    bool getIsStart();
    // Accessing surrounding boxes from one box
    int accessSurroundBox(BoxMsLabel *);
    // Click on the detected box to blink the surrounding box
    void flashBox(BoxMsLabel *);
    // For each successful detection, check if all non-mine boxes have been detected
    bool checkIfVictory();
    // Set the top center star icon to a fork or check icon
    void setCentreIcon(bool);
private:
    Ui::Widget *ui;
    /* Private Attributes */
    // Enum for difficulty
    enum RuleGame {
        APPRENTICE,
        ADEPT,
        EXPERT,
        CUSTOMIZATION
    };
    // Width and Height of mined area
    int fieldWidth;
    int fieldHeight;
    // Number of Mines
    int numMine;
    // Bool - Mine deployed or not (and clock)
    bool isStart;
    // Bool - Win or fail
    bool gameOver;
    // For clock
    QTimer * timer;
    // Array 2d - point to all boxes
    QVector<QVector<BoxMsLabel *>> boxArray;
    /* Private Function */
    //Stores all the 'connect(?)'
    void storeConnect();
    // Setting mined area size and number of mines
    bool setFieldSizeAndMineNum(RuleGame);
    // Reset when choose a difficulty or restart game
    void restartGame();
    // Deploy mines when first click
    void deployMine(int,int);
signals:
    // Signal to active the timer
    void signalGameStart(bool);
};
#endif // WIDGET_H
