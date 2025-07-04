#include "trainDialog.h"
#include "ui_dialog.h"

const QString cTrainType = "Введіть тип потяга";
const QString cDepartureTime = "Введіть час відправлення";
const QString cFirstStation = "Введіть початкову станцію";
const QString cNextStation = "Введіть наступну станцію";

TrainDialog::TrainDialog(const QString &aType, Display *aDisplay, QWidget *aParent) :
    Dialog(aType, aDisplay, aParent)
{
    this->setWindowTitle(aType + QString::number(newTrainNumber()));
    ui->label->setText(cTrainType);
    //ui->comboBox->addItems(12 00);
}

int TrainDialog::newTrainNumber() const
{
    return mDisplay->getTrains().size() + 1;
}

void TrainDialog::accept()
{
    if (ui->label->text() == cTrainType)
    {
        mTrainType = ui->comboBox->currentIndex();
        ui->label->setText(cDepartureTime);
        ui->comboBox->clear();
    }
    else if (ui->label->text() == cDepartureTime)
    {
        //mTrainResult = Train(newTrainNumber(), mTrainType, ui->comboBox->currentIndex());
        ui->label->setText(cFirstStation);
        ui->comboBox->clear();
        ui->comboBox->addItems(mDisplay->getAllNames());
    }
    else if (ui->label->text() == cFirstStation)
    {
        //mTrainResult.addStation(ui->comboBox->currentIndex());
        ui->label->setText(cNextStation);
        ui->comboBox->clear();
    }
    else if (ui->label->text() == cNextStation)
    {
        //mTrainResult.addStation(ui->comboBox->currentIndex());
        ui->comboBox->clear();
    }
}
