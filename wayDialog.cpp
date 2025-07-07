#include "wayDialog.h"
#include "ui_dialog.h"

const QString cLastStation = "Введіть кінцеву станцію";

WayDialog::WayDialog(const QString &aType, Display *aDisplay, QWidget *aParent) :
    Dialog(aType, aDisplay, aParent)
    , mWayResult(std::pair<int, int>(cNotSet, cNotSet))
{
    this->setWindowTitle(aType);
    ui->label->setText(cFirstStation);
    ui->comboBox->addItems(mDisplay->getAllNames());
}

void WayDialog::accept()
{    
    if (ui->label->text() == cFirstStation)
    {
        mWayResult.first = ui->comboBox->currentIndex();
        if (mType == cAddWay)
            ui->comboBox->removeItem(mWayResult.first);
        else if (mType == cDelWay && !fillNeighbours())
            return;
        ui->label->setText(cLastStation);
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
