#include "rulecusdialog.h"
#include "ui_rulecusdialog.h"
#include<QMessageBox>
RuleCusDialog::RuleCusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleCusDialog)
{
    ui->setupUi(this);
    this->setFixedSize(140,150);
    inputWidth = 9;
    inputHeight = 9;
    inputMineNum = 10;
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,[=](){
        validInput = 0;
        //check Width
        inputWidth = checkInput(ui->lineEditWidth,true);
        if(inputWidth!=-1) validInput++;
        //check Height
        inputHeight = checkInput(ui->lineEditHeight,true);
        if(inputHeight!=-1) validInput++;
        //check numMine
        inputMineNum = checkInput(ui->lineEditMineNum,false);
        if(inputMineNum!=-1) validInput++;
        //Verify if all correct
        if(validInput == 3) {
            this->accept();
        } else {
            QMessageBox mbox;
            mbox.critical(this,"Invalid Input",
                "Width and Height must be greater than 2 and less than 30\n"
                    "The number of mines should be greater than 0 and less than multiply Width*Height");
        }
    });
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,[=](){
        this->reject();
    });
}

int RuleCusDialog::checkInput(QLineEdit * input, bool isSize) {
    bool ok = true;
    int retVal = input->text().toInt(&ok);
    if(isSize) {
        if(ok==false || retVal < 2 || retVal>30)
            return(-1);
        else
            return(retVal);
    }
    else {
        if(ok==false || retVal < 0
            || retVal > (inputHeight*inputWidth))
                return(-1);
        else
            return(retVal);
    }
}

RuleCusDialog::~RuleCusDialog()
{
    delete ui;
}
