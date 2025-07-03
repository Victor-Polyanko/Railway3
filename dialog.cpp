#include "dialog.h"
#include "ui_dialog.h"

#include <qmessagebox.h>

const int cNotSet = -1;
const QString cWarning = "Халепонька";
const QString cTrainType = "Введіть тип потяга";
const QString cDepartureTime = "Введіть час відправлення";
const QString cFirstStation = "Введіть початкову станцію";
const QString cNextStation = "Введіть наступну станцію";
const QString cLastStation = "Введіть кінцеву станцію";

Dialog::Dialog(const QString &aType, Display *aDisplay, QWidget *aParent) :
    QDialog(aParent)
    , ui(new Ui::Dialog)
    , mType(aType)
    , mSecondText(cLastStation)
    , mDisplay(aDisplay)
    , mWayResult(std::pair<int, int>(cNotSet, cNotSet))
{
    ui->setupUi(this);
    if (aType == cAddTrain)
    {
        this->setWindowTitle(aType + QString::number(mDisplay->getTrains().size() + 1));
        ui->label->setText(cTrainType);
    }
    else
    {
        this->setWindowTitle(aType);
        ui->label->setText(cFirstStation);
    }
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
        mWayResult.first = ui->comboBox->currentIndex();
        if (mType == cAddWay)
            ui->comboBox->removeItem(mWayResult.first);
        else if (mType == cDelWay)
        {
            auto newList = mDisplay->getNamesForStation(mWayResult.first);
            if (newList.empty())
            {
                mWayResult.first = cNotSet;
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
        mWayResult.second = cNotSet;
    }
    else
    {
        mWayResult.second = ui->comboBox->currentIndex();
        if (mType == cAddWay)
        {
            if (mWayResult.second >= mWayResult.first)
                mWayResult.second++;
            mDisplay->addWay(mWayResult);
        }
        else
        {
            mWayResult.second = mDisplay->getStationIdForConnection(mWayResult.first, mWayResult.second);
            mDisplay->delWay(mWayResult);
        }
        close();
    }
}
