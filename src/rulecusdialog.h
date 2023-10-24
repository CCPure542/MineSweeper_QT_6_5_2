#ifndef RULECUSDIALOG_H
#define RULECUSDIALOG_H

#include <QDialog>
#include<QLineEdit>

namespace Ui {
class RuleCusDialog;
}

class RuleCusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RuleCusDialog(QWidget *parent = nullptr);
    ~RuleCusDialog();
    // dialog box for inputting custom data
    int inputWidth;
    int inputHeight;
    int inputMineNum;
    int validInput;
    // Check if data is valid
    int checkInput(QLineEdit *,bool);

private:
    Ui::RuleCusDialog *ui;
};

#endif // RULECUSDIALOG_H
