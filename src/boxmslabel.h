#ifndef BOXMSLABEL_H
#define BOXMSLABEL_H

#include <QLabel>

class BoxMsLabel : public QLabel
{
    Q_OBJECT
public:
    explicit BoxMsLabel(int, int, QWidget *parent = nullptr);
    ~BoxMsLabel();
    /* Public Function */
    // Return the specific private attributes
    int getPosH();
    int getPosW();
    bool isMine();
    bool isDetected();
    // Change the private attribute 'boxMine'
    void setMine();
    // if the current box show 0 then automatically detect other surronding boxes
    void detectAuto();
private:
    /* Private Function */
    // Get the pointer of my main widget object
    QObject * getMainWidget();
    // Rewrite - Mouse Events
    void mousePressEvent(QMouseEvent *);
    // Trigger for clicking boxes
    void setIcon(QMouseEvent *);
    // Display a number or blank(0)
    void showNumber(int);
    // Show mines or the touching mine with red background
    void showMine(bool);
    /* Private attributes*/
    // Determine if box is mine
    bool boxMine;
    // Determine if box has been detected
    bool boxDetected;
    // Determine if box should show flag
    bool boxFlag;
    // Position of box
    int m_h;
    int m_w;
signals:
    void useFlag(bool);
};

#endif // BOXMSLABEL_H
