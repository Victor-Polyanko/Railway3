#include "scheduleviewer.h"
#include "ui_scheduleviewer.h"

#include <QStandardItemModel>

namespace
{
const QString cArrival = "Прибуття";
const QString cWait = "Стоянка";
const QString cDeparture = "Відправлення";
const QString cNumber = "Номер";
const QString cType = "Тип";
const QString cTime = "Час у дорозі";
}

ScheduleViewer::ScheduleViewer(const Map &aMap, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScheduleViewer)
    , mMap(aMap)
{
    ui->setupUi(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

ScheduleViewer::~ScheduleViewer()
{
    delete ui;
}

void ScheduleViewer::showTrainSchedule(int aTrainId)
{
    auto train = mMap.getTrains()[aTrainId];
    auto number = QString::number(train.getNumber());
    auto first = mMap.getStationName(train.getStations().front().stationId);
    auto last = mMap.getStationName(train.getStations().back().stationId);
    this->setWindowTitle("Графік руху потяга №" + number + " " + first + " - " + last);
    QStandardItemModel *model = new QStandardItemModel(train.getStations().size(), 3);
    model->setHeaderData(0, Qt::Horizontal, cArrival);
    model->setHeaderData(1, Qt::Horizontal, cWait);
    model->setHeaderData(2, Qt::Horizontal, cDeparture);
    for (int i = 0; i < train.getStations().size(); ++i)
    {
        auto t = train.getStations()[i];
        model->setHeaderData(i, Qt::Vertical, mMap.getStationName(t.stationId));
        if (t.arrive.isSet())
            model->setItem(i, 0, new QStandardItem(t.arrive.showAsString()));
        if (t.wait != cNotSet)
            model->setItem(i, 1, new QStandardItem(QString::number(t.wait)));
        if (t.depart.isSet())
            model->setItem(i, 2, new QStandardItem(t.depart.showAsString()));
    }
    ui->tableView->setModel(model);
    show();
}

void ScheduleViewer::showStationSchedule(int aStationId)
{
    this->setWindowTitle("Розклад руху потягів по станції " + mMap.getStationName(aStationId));
    auto trains = mMap.getStationTrainsNumbers(aStationId);
    QStandardItemModel *model = new QStandardItemModel(trains.size(), 5);
    model->setHeaderData(0, Qt::Horizontal, cNumber);
    model->setHeaderData(1, Qt::Horizontal, cType);
    model->setHeaderData(2, Qt::Horizontal, cArrival);
    model->setHeaderData(3, Qt::Horizontal, cWait);
    model->setHeaderData(4, Qt::Horizontal, cDeparture);
    for (int i = 0; i < trains.size(); ++i)
    {
        int j = 0;
        while (j < mMap.getTrains().size() && mMap.getTrains()[j].getNumber() != trains[i])
            ++j;
        auto train = mMap.getTrains()[j];
        model->setHeaderData(i, Qt::Vertical, mMap.getTrainName(train));
        model->setItem(i, 0, new QStandardItem(QString::number(train.getNumber())));
        model->setItem(i, 1, new QStandardItem(train.getTypeAsString()));
        for (const auto &station : train.getStations())
        {
            if (station.stationId != aStationId)
                continue;
            if (station.arrive.getX() != cNotSet)
                model->setItem(i, 2, new QStandardItem(station.arrive.showAsString()));
            if (station.wait != cNotSet)
                model->setItem(i, 3, new QStandardItem(QString::number(station.wait)));
            if (station.depart.getX() != cNotSet)
                model->setItem(i, 4, new QStandardItem(station.depart.showAsString()));
            break;
        }
    }
    ui->tableView->setModel(model);
    ui->tableView->sortByColumn(2, Qt::SortOrder::AscendingOrder);
    show();
}

bool ScheduleViewer::showRouteSchedule(const Way &aWay)
{
    auto firstName = mMap.getStationName(aWay.first);
    auto lastName = mMap.getStationName(aWay.second);
    this->setWindowTitle("Прямі потяги від станції " + firstName + " до станції " + lastName);
    QStandardItemModel *model = new QStandardItemModel(0, 5);
    model->setHeaderData(0, Qt::Horizontal, cNumber);
    model->setHeaderData(1, Qt::Horizontal, cType);
    model->setHeaderData(2, Qt::Horizontal, firstName);
    model->setHeaderData(3, Qt::Horizontal, lastName);
    model->setHeaderData(4, Qt::Horizontal, cTime);
    for (const auto &train : mMap.getTrains())
    {
        TimePoint departure, arrival;
        for (const auto &station : train.getStations())
        {
            if (!departure.isSet() &&
                aWay.first == station.stationId &&
                station.arrive.isSet())
            {
                departure = station.depart;
            }
            else if (!arrival.isSet() &&
                       aWay.second == station.stationId &&
                       station.depart.isSet())
            {
                arrival = station.arrive;
                break;
            }
        }
        if (!departure.isSet() || !arrival.isSet())
            continue;
        auto time = arrival - departure;
        QList<QStandardItem*> rowItems;
        rowItems.append(new QStandardItem(QString::number(train.getNumber())));
        rowItems.append(new QStandardItem(train.getTypeAsString()));
        rowItems.append(new QStandardItem(departure.showAsString()));
        rowItems.append(new QStandardItem(arrival.showAsString()));
        rowItems.append(new QStandardItem(time.showAsString()));
        model->appendRow(rowItems);
        model->setHeaderData(model->rowCount() - 1, Qt::Vertical, mMap.getTrainName(train));
    }
    if (model->rowCount() == 0)
        return false;
    ui->tableView->setModel(model);
    ui->tableView->sortByColumn(2, Qt::SortOrder::AscendingOrder);
    show();
    return true;
}
