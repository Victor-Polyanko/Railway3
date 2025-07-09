#include "scheduleviewer.h"
#include "ui_scheduleviewer.h"

#include <QStandardItemModel>

ScheduleViewer::ScheduleViewer(const Map &aMap, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScheduleViewer)
    , mMap(aMap)
{
    ui->setupUi(this);
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
    model->setHeaderData(0, Qt::Horizontal, "Прибуття");
    model->setHeaderData(1, Qt::Horizontal, "Стоянка");
    model->setHeaderData(2, Qt::Horizontal, "Відправлення");
    for (int i = 0; i < train.getStations().size(); ++i)
    {
        auto t = train.getStations()[i];
        model->setHeaderData(i, Qt::Vertical, mMap.getStationName(t.stationId));
        if (t.arrive.getX() != cNotSet)
            model->setItem(i, 0, new QStandardItem(t.arrive.showAsString()));
        if (t.wait != cNotSet)
            model->setItem(i, 1, new QStandardItem(QString::number(t.wait)));
        if (t.depart.getX() != cNotSet)
            model->setItem(i, 2, new QStandardItem(t.depart.showAsString()));
    }
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    show();
}
