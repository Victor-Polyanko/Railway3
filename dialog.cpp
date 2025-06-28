#include "dialog.h"
#include "ui_dialog.h"

const int cNotSet = -1;

Dialog::Dialog(const QString &aTitle, const QString &aFirstText, const QString &aSecondText, const QStringList &aNames, QWidget *aParent) :
    QDialog(aParent)
    , ui(new Ui::Dialog)
    , mSecondText(aSecondText)
    , mResult(std::pair<int, int>(cNotSet, cNotSet))
{
    ui->setupUi(this);
    this->setWindowTitle(aTitle);
    ui->label->setText(aFirstText);
    ui->comboBox->addItems(aNames);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::accept()
{
    if (ui->label->text() != mSecondText)
    {
        mResult.first = ui->comboBox->currentIndex();
        ui->comboBox->removeItem(mResult.first);
        ui->label->setText(mSecondText);
        mResult.second = cNotSet;
    }
    else
    {
        mResult.second = ui->comboBox->currentIndex();
        if (mResult.second >= mResult.first)
            mResult.second++;
        emit getResult(mResult);
        close();
    }
}
