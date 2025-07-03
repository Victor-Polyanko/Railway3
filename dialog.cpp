#include "dialog.h"
#include "ui_dialog.h"

#include <qmessagebox.h>

const int cNotSet = -1;
const QString cWarning = "Халепонька";

Dialog::Dialog(const QString &aType, const QString &aFirstText, const QString &aSecondText, Display *aDisplay, QWidget *aParent) :
    QDialog(aParent)
    , ui(new Ui::Dialog)
    , mType(aType)
    , mSecondText(aSecondText)
    , mDisplay(aDisplay)
    , mResult(std::pair<int, int>(cNotSet, cNotSet))
{
    ui->setupUi(this);
    this->setWindowTitle(aType);
    ui->label->setText(aFirstText);
    ui->comboBox->addItems(mDisplay->getAllNames());
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
        if (mType == cAddWay)
            ui->comboBox->removeItem(mResult.first);
        else if (mType == cDelWay)
        {
            auto newList = mDisplay->getNamesForStation(mResult.first);
            if (newList.empty())
            {
                mResult.first = cNotSet;
                QMessageBox::information(this, cWarning, "Ця станція не має сусідів - оберіть іншу.");
                return;
            }
            else
            {
                ui->comboBox->clear();
                ui->comboBox->addItems(newList);
            }
        }
        ui->label->setText(mSecondText);
        mResult.second = cNotSet;
    }
    else
    {
        mResult.second = ui->comboBox->currentIndex();
        if (mType == cAddWay)
        {
            if (mResult.second >= mResult.first)
                mResult.second++;
        }
        else
            mResult.second = mDisplay->getStationIdForConnection(mResult.first, mResult.second);
        emit getResult(mResult);
        close();
    }
}
