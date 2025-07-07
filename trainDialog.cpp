#include "trainDialog.h"
#include "timePoint.h"
#include "ui_dialog.h"

#include <QPushButton>

#include <QMessageBox>

const QString cTrainType = "Введіть тип потяга";
const QString cDepartureTime = "Введіть час відправлення";
const QString cNextStation = "Введіть наступну станцію";

TrainDialog::TrainDialog(const QString &aType, Display *aDisplay, QWidget *aParent) :
    Dialog(aType, aDisplay, aParent)
{
    this->setWindowTitle(aType + QString::number(newTrainNumber()));
    ui->label->setText(cTrainType);
    ui->comboBox->addItem("Швидкий");
    ui->comboBox->addItem("Пасажирський");
    ui->comboBox->addItem("Приміський");
}

int TrainDialog::newTrainNumber() const
{
    return mDisplay->getTrains().size() + 1;
}

void TrainDialog::accept()
{
    if (ui->label->text() == cTrainType)
    {
        mTrainType = static_cast<Train::Type>(ui->comboBox->currentIndex());
        ui->label->setText(cDepartureTime);
        ui->comboBox->clear();
        for (int i = 0; i < 24; ++i)
            ui->comboBox->addItem(QString::number(i) + " : 00");
        ui->comboBox->setCurrentIndex(12);
    }
    else if (ui->label->text() == cDepartureTime)
    {
        mTrainResult = Train(newTrainNumber(), mTrainType, ui->comboBox->currentIndex());
        ui->label->setText(cFirstStation);
        ui->comboBox->clear();
        ui->comboBox->addItems(mDisplay->getAllNames());
    }
    else if (ui->label->text() == cFirstStation)
    {
        Schedule firstStation;
        firstStation.stationId = ui->comboBox->currentIndex();
        if (!fillNeighbours())
            return;
        firstStation.depart = TimePoint(mTrainResult.getStartTime());
        mTrainResult.addStation(firstStation);
        ui->label->setText(cNextStation);
        ui->buttonBox->addButton(QDialogButtonBox::Apply);
        connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &TrainDialog::apply);
    }
    else if (ui->label->text() == cNextStation)
    {
        auto trainAddition = static_cast<int>(mTrainResult.getType());
        auto stationId = mDisplay->getStationIdForConnection(mTrainResult.getStations().back().stationId, ui->comboBox->currentIndex());
        auto prevStation = mTrainResult.getStations().back();
        auto minutes = prevStation.depart.getY() + (2 + trainAddition) * mDisplay->getStationPosition(stationId).distance(
                                                        mDisplay->getStationPosition(mTrainResult.getStations().back().stationId)) / 2;
        TimePoint arrive = prevStation.depart + minutes;
        auto wait = 2 * (mDisplay->getStationStatus(stationId) - 1 + trainAddition);
        TimePoint depart = arrive + wait;
        mTrainResult.addStation(stationId, arrive, wait, depart);
        fillNeighbours(stationId);
    }
}

void TrainDialog::apply()
{
    accept();
    mDisplay->addTrain(mTrainResult);
    close();
}
