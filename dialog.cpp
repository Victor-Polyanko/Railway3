#include "dialog.h"
#include "ui_dialog.h"

#include <qmessagebox.h>
#include <QPushButton>

const QString cWarning = "Халепонька";
const QString cTrainType = "Введіть тип потяга";
const QVector<QString> cTypeNames = {"Швидкий ", "Пасажирський ", "Приміський "};
const QString cDepartureTime = "Введіть час відправлення";
const QString cFirstStation = "Введіть початкову станцію";
const QString cNextStation = "Введіть наступну станцію";
const QString cLastStation = "Введіть кінцеву станцію";
const QString cSelectTrain = "Оберіть потяг";

Dialog::Dialog(Display *aDisplay, QWidget *aParent) :
    QDialog(aParent)
    , ui(new Ui::Dialog)
    , mDisplay(aDisplay)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::fillNeighbours(int aStationId)
{
    if (aStationId == cNotSet)
        aStationId = ui->comboBox->currentIndex();
    auto newList = mDisplay->getNamesForStation(aStationId);
    if (newList.empty())
    {
        QMessageBox::information(this, cWarning, "Ця станція не має сусідів - оберіть іншу.");
        return false;
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(newList);
    return true;
}

void Dialog::showTrains()
{
    ui->label->setText(cSelectTrain);
    for (const auto &train : mDisplay->getTrains())
    {
        auto firstStation = mDisplay->getStationName(train.getStations().front().stationId);
        auto lastStation = mDisplay->getStationName(train.getStations().back().stationId);
        ui->comboBox->addItem(cTypeNames[train.getType()] + "№" + QString::number(train.getNumber())
                              + ". " + firstStation + " - " + lastStation);
    }
}

void Dialog::showTimes()
{
    ui->label->setText(cDepartureTime);
    ui->comboBox->clear();
    for (int i = 0; i < 24; ++i)
        ui->comboBox->addItem(QString::number(i) + " : 00");
}


AddTrainDialog::AddTrainDialog(Display *aDisplay, QWidget *aParent) :
    Dialog(aDisplay, aParent)
{
    this->setWindowTitle("Створення потяга №" + QString::number(newTrainNumber()));
    ui->label->setText(cTrainType);
    for (const auto &type : cTypeNames)
        ui->comboBox->addItem(type);
}

int AddTrainDialog::newTrainNumber() const
{
    return mDisplay->getTrains().size() + 1;
}

void AddTrainDialog::accept()
{
    if (ui->label->text() == cTrainType)
    {
        mTrainType = static_cast<Train::Type>(ui->comboBox->currentIndex());
        showTimes();
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
        connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &AddTrainDialog::apply);
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

void AddTrainDialog::apply()
{
    accept();
    mDisplay->addTrain(mTrainResult);
    emit ready();
    close();
}



DelTrainDialog::DelTrainDialog(Display *aDisplay, QWidget *aParent) :
    Dialog(aDisplay, aParent)
{
    this->setWindowTitle("Скасування потяга");
    showTrains();
}

void DelTrainDialog::accept()
{
    mDisplay->delTrain(ui->comboBox->currentIndex());
    emit ready();
    close();
}



ChangeTimeDialog::ChangeTimeDialog(Display *aDisplay, QWidget *aParent) :
    Dialog(aDisplay, aParent)
{
    this->setWindowTitle("Зміна часу відправлення");
    showTrains();
}

void ChangeTimeDialog::accept()
{
    if (ui->label->text() == cSelectTrain)
    {
        mTrainId = ui->comboBox->currentIndex();
        showTimes();
        ui->comboBox->setCurrentIndex(mDisplay->getTrains()[mTrainId].getStartTime().getX());
    }
    else if (ui->label->text() == cDepartureTime)
    {
        mDisplay->setTrainTime(mTrainId, TimePoint(ui->comboBox->currentIndex(), 0));
        emit ready();
        close();
    }
}



AddWayDialog::AddWayDialog(Display *aDisplay, QWidget *aParent) :
    Dialog(aDisplay, aParent)
{
    this->setWindowTitle("Створення колії");
    ui->label->setText(cFirstStation);
    ui->comboBox->addItems(mDisplay->getAllNames());
    mWayResult = Way(cNotSet, cNotSet);
}

void AddWayDialog::accept()
{
    if (ui->label->text() == cFirstStation)
    {
        mWayResult.first = ui->comboBox->currentIndex();
        ui->comboBox->removeItem(mWayResult.first);
        ui->label->setText(cLastStation);
    }
    else
    {
        mWayResult.second = ui->comboBox->currentIndex();
        if (mWayResult.second >= mWayResult.first)
            mWayResult.second++;
        mDisplay->addWay(mWayResult);
        emit ready();
        close();
    }
}



DelWayDialog::DelWayDialog(Display *aDisplay, QWidget *aParent) :
    Dialog(aDisplay, aParent)
{
    this->setWindowTitle("Розбирання колії");
    ui->label->setText(cFirstStation);
    ui->comboBox->addItems(mDisplay->getAllNames());
    mWayResult = Way(cNotSet, cNotSet);
}

void DelWayDialog::accept()
{
    if (ui->label->text() == cFirstStation)
    {
        mWayResult.first = ui->comboBox->currentIndex();
        if (!fillNeighbours())
            return;
        ui->label->setText(cLastStation);
    }
    else
    {
        mWayResult.second = ui->comboBox->currentIndex();
        mWayResult.second = mDisplay->getStationIdForConnection(mWayResult.first, mWayResult.second);
        if (!deleteWayWithTrains())
            return;
        mDisplay->delWay(mWayResult);
        emit ready();
        close();
    }
}

bool DelWayDialog::deleteWayWithTrains()
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
