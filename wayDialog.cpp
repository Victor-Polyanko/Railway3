#include "wayDialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

const QString cLastStation = "Введіть кінцеву станцію";

WayDialog::WayDialog(const QString &aType, Display *aDisplay, QWidget *aParent) :
    Dialog(aType, aDisplay, aParent)
    , mWayResult(std::pair<int, int>(cNotSet, cNotSet))
{
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
            if (!deleteWayWithTrains())
                return;
            mDisplay->delWay(mWayResult);
        }
        emit ready();
        close();
    }
}

bool WayDialog::deleteWayWithTrains()
{
    QVector<int> deletedTrains;
    for (int trainId = 0; trainId < mDisplay->getTrains().size(); ++trainId)
    {
        int first = cNotSet;
        int second = cNotSet;
        for (int stationId = 0; stationId < mDisplay->getTrains()[trainId].getStations().size(); ++stationId)
        {
            const auto station = mDisplay->getTrains()[trainId].getStations()[stationId];
            if (station.stationId == mWayResult.first)
                first = stationId;
            else if (station.stationId == mWayResult.second)
                second = stationId;
        }
        if (first != cNotSet && second != cNotSet && abs(first - second) == 1)
            deletedTrains.emplace_back(trainId);
    }
    if (!deletedTrains.empty() && QMessageBox::Cancel ==
            QMessageBox::information(this, cWarning,
            "Розбір цієї ділянки призведе до скасування " + QString::number(deletedTrains.size())+ " потяга(ів).",
            QMessageBox::Ok | QMessageBox::Cancel))
        return false;
    for (auto train = deletedTrains.rbegin(); train != deletedTrains.rend(); ++ train)
        mDisplay->delTrain(*train);
    return true;
}
