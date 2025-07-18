#include "dialog.h"
#include "ui_dialog.h"
#include "scheduleViewer.h"

#include <qmessagebox.h>
#include <QPushButton>

const QString cTrainType = "Оберіть тип потяга";
const QString cDepartureTime = "Оберіть час відправлення";
const QString cFirstStation = "Оберіть початкову станцію";
const QString cNextStation = "Оберіть наступну станцію";
const QString cLastStation = "Оберіть кінцеву станцію";
const QString cSecondTrain = "Якщо бажаєте додати зворотній рейс, оберіть його час відправлення";
const QString cSelectTrain = "Оберіть потяг";
const QString cSelectStation = "Оберіть станцію";
const QString cSelectSpeed = "Оберіть швидкість анімації";

Dialog::Dialog(Map *aMap, QWidget *aParent) :
    QDialog(aParent)
    , ui(new Ui::Dialog)
    , mMap(aMap)
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
    auto newList = mMap->getNamesForStation(aStationId);
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
    for (const auto &train : mMap->getTrains())
        ui->comboBox->addItem(train.getTypeAsString() + "№" + QString::number(train.getNumber())
                              + ". " + mMap->getTrainName(train));
}

void Dialog::showTimes()
{
    ui->label->setText(cDepartureTime);
    ui->comboBox->clear();
    for (int i = 0; i < 24; ++i)
        ui->comboBox->addItem(QString::number(i) + " : 00");
    ui->comboBox->setCurrentIndex(12);
}

void Dialog::showStations()
{
    ui->label->setText(cFirstStation);
    ui->comboBox->addItems(mMap->getAllNames());
    mWayResult = Way();
}

bool Dialog::completeWay()
{
    if (ui->label->text() == cFirstStation)
    {
        mWayResult.first = ui->comboBox->currentIndex();
        ui->comboBox->removeItem(mWayResult.first);
        ui->label->setText(cLastStation);
        return false;
    }
    mWayResult.second = ui->comboBox->currentIndex();
    if (mWayResult.second >= mWayResult.first)
        mWayResult.second++;
    return true;
}



AddTrainDialog::AddTrainDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
{
    this->setWindowTitle("Створення потяга №" + QString::number(mMap->getTrains().size() + 1));
    ui->label->setText(cTrainType);
    for (const auto &type : Train::Types)
        ui->comboBox->addItem(type);
}

void AddTrainDialog::accept()
{
    if (ui->label->text() == cTrainType)
    {
        mTrainType = static_cast<Train::Type>(ui->comboBox->currentIndex());
        showTimes();
    }
    else if (ui->label->text() == cDepartureTime)
    {
        mTrainResult = createNewTrain();
        ui->label->setText(cFirstStation);
        ui->comboBox->clear();
        ui->comboBox->addItems(mMap->getAllNames());
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
        mApplyButton = ui->buttonBox->addButton(QDialogButtonBox::Apply);
        connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &AddTrainDialog::apply);
    }
    else if (ui->label->text() == cNextStation)
    {
        auto stationId = mMap->getStationIdForConnection(mTrainResult.getStations().back().stationId, ui->comboBox->currentIndex());
        auto arrive = calculateArrivalToStation(mTrainResult, stationId);
        auto wait = 2 * (mMap->getStationStatus(stationId) - 1 + static_cast<int>(mTrainResult.getType()));
        auto depart = arrive + TimePoint(0, wait);
        mTrainResult.addStation(stationId, arrive, wait, depart);
        fillNeighbours(stationId);
    } else if (ui->label->text() == cSecondTrain)
    {
        auto secondTrain = createNewTrain();
        Schedule firstStation;
        auto stations = mTrainResult.getStations();
        firstStation.stationId = stations.back().stationId;
        firstStation.depart = TimePoint(secondTrain.getStartTime());
        secondTrain.addStation(firstStation);
        for (auto station = stations.rbegin() + 1; station < stations.rend() - 1; ++station)
        {
            auto arrive = calculateArrivalToStation(secondTrain, station->stationId);
            auto wait = station->wait;
            auto depart = arrive + TimePoint(0, wait);
            secondTrain.addStation(station->stationId, arrive, wait, depart);
        }
        Schedule lastStation;
        lastStation.stationId = stations.front().stationId;
        lastStation.arrive = calculateArrivalToStation(secondTrain, lastStation.stationId);
        secondTrain.addStation(lastStation);
        mMap->addTrain(secondTrain);
        close();
    }
}

void AddTrainDialog::apply()
{
    Schedule lastStation;
    lastStation.stationId = mMap->getStationIdForConnection(mTrainResult.getStations().back().stationId, ui->comboBox->currentIndex());
    lastStation.arrive = calculateArrivalToStation(mTrainResult, lastStation.stationId);
    mTrainResult.addStation(lastStation);
    mMap->addTrain(mTrainResult);
    emit ready();
    showTimes();
    ui->label->setText(cSecondTrain);
    ui->buttonBox->removeButton(mApplyButton);
}

Train AddTrainDialog::createNewTrain() const
{
    return Train(mMap->getTrains().size() + 1, mTrainType, ui->comboBox->currentIndex());
}

TimePoint AddTrainDialog::calculateArrivalToStation(const Train &aTrain, int aStationId) const
{
    auto prevStation = aTrain.getStations().back();
    auto minutes = (2 + static_cast<int>(aTrain.getType())) * mMap->getStationPosition(aStationId).distance(
                       mMap->getStationPosition(aTrain.getStations().back().stationId)) / 2;
    return prevStation.depart + minutes;
}



DelTrainDialog::DelTrainDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
{
    this->setWindowTitle("Скасування потяга");
    showTrains();
}

void DelTrainDialog::accept()
{
    mMap->delTrain(mMap->getTrains()[ui->comboBox->currentIndex()]);
    emit ready();
    close();
}



ViewTrainDialog::ViewTrainDialog(Map *aMap, QWidget *aParent)
    : Dialog(aMap, aParent)
    , mViewer(new ScheduleViewer(*aMap, aParent))
{
    this->setWindowTitle("Перегляд розкладу потяга");
    showTrains();
}

void ViewTrainDialog::accept()
{
    mViewer->showTrainSchedule(ui->comboBox->currentIndex());
    close();
}



ChangeTimeDialog::ChangeTimeDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
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
        ui->comboBox->setCurrentIndex(mMap->getTrains()[mTrainId].getStartTime().getX());
    }
    else if (ui->label->text() == cDepartureTime)
    {
        mMap->setTrainTime(mTrainId, TimePoint(ui->comboBox->currentIndex(), 0));
        close();
    }
}



AddWayDialog::AddWayDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
{
    this->setWindowTitle("Створення колії");
    showStations();
}

void AddWayDialog::accept()
{
    if (completeWay())
    {
        mMap->addWay(mWayResult);
        emit ready();
        close();
    }
}



DelWayDialog::DelWayDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
{
    this->setWindowTitle("Розбирання колії");
    showStations();
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
        mWayResult.second = mMap->getStationIdForConnection(mWayResult.first, mWayResult.second);
        if (!deleteWayWithTrains())
            return;
        mMap->delWay(mWayResult);
        emit ready();
        close();
    }
}

bool DelWayDialog::deleteWayWithTrains()
{
    QVector<int> deletedTrains;
    for (int trainId = 0; trainId < mMap->getTrains().size(); ++trainId)
    {
        int first = cNotSet;
        int second = cNotSet;
        for (int stationId = 0; stationId < mMap->getTrains()[trainId].getStations().size(); ++stationId)
        {
            const auto station = mMap->getTrains()[trainId].getStations()[stationId];
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
        mMap->delTrain(*train);
    return true;
}



ViewStationDialog::ViewStationDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
    , mViewer(new ScheduleViewer(*aMap, aParent))
{
    this->setWindowTitle("Перегляд станції");
    ui->label->setText(cSelectStation);
    ui->comboBox->addItems(mMap->getAllNames());
}

void ViewStationDialog::accept()
{
    if (mMap->getStationTrainsNumbers(ui->comboBox->currentIndex()).empty())
        QMessageBox::information(this, cWarning, "Через цю станцію не проходить жоден потяг - оберіть іншу.");
    else
    {
        mViewer->showStationSchedule(ui->comboBox->currentIndex());
        close();
    }
}



ViewRouteDialog::ViewRouteDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
    , mViewer(new ScheduleViewer(*aMap, aParent))
{
    this->setWindowTitle("Знаходження прямого сполучення між станціями");
    showStations();
}

void ViewRouteDialog::accept()
{
    if (completeWay())
    {
        if (mViewer->showRouteSchedule(mWayResult))
            close();
        else
            QMessageBox::information(this, cWarning, "З цією станцією немає прямого сполучення - оберіть іншу.");
    }
}



SpeedDialog::SpeedDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
{
    this->setWindowTitle("Швидкість анімації");
    ui->label->setText(cSelectSpeed);
    ui->comboBox->clear();
    for (int i = 0; i < 10; ++i)
        ui->comboBox->addItem(QString::number(i + 1));
    ui->comboBox->setCurrentIndex(0);
}

void SpeedDialog::accept()
{
    auto speed = ui->comboBox->currentText().toInt();
    emit setSpeed(speed);
    close();
}



LaunchDialog::LaunchDialog(Map *aMap, QWidget *aParent) :
    Dialog(aMap, aParent)
{
    this->setWindowTitle("Запуск анімації");
    showTimes();
}

void LaunchDialog::accept()
{
    auto time = TimePoint(ui->comboBox->currentIndex(), 0);
    emit setTime(time);
    close();
}
