#ifndef SCHEDULEVIEWER_H
#define SCHEDULEVIEWER_H

#include "map.h"

#include <QMainWindow>

namespace Ui {
class ScheduleViewer;
}

class ScheduleViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScheduleViewer(const Map &aMap, QWidget *aParent = nullptr);
    ~ScheduleViewer();

    void showTrainSchedule(int aTrainId);
    void showStationSchedule(int aStationId);
    bool showRouteSchedule(const Way &aWay);

private:
    Ui::ScheduleViewer *ui;

public:
    Map mMap;
};

#endif // SCHEDULEVIEWER_H
