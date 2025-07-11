#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>

#include "map.h"
#include "scheduleviewer.h"

const QString cWarning = "Халепонька";

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(Map *aMap, QWidget *aParent = nullptr);
    ~Dialog();

signals:
    void ready();
    void setSpeed(int &aSpeed);
    void setTime(TimePoint &aTime);

protected:
    bool fillNeighbours(int aStationId = cNotSet);
    void showTrains();
    void showTimes();
    void showStations();
    bool completeWay();

protected:
    Ui::Dialog *ui;
    Map *mMap;
    Way mWayResult;
};



class AddTrainDialog : public Dialog
{
    Q_OBJECT
public:
    explicit AddTrainDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
    void apply();
    Train createNewTrain() const;
    TimePoint calculateArrivalToStation(const Train &aTrain, int aStationId) const;

private:
    Train::Type mTrainType;
    Train mTrainResult;
    QPushButton *mApplyButton;
};



class DelTrainDialog : public Dialog
{
    Q_OBJECT
public:
    explicit DelTrainDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
};



class ViewTrainDialog : public Dialog
{
    Q_OBJECT
public:
    explicit ViewTrainDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();

private:
    ScheduleViewer *mViewer;
};



class ChangeTimeDialog : public Dialog
{
    Q_OBJECT
public:
    explicit ChangeTimeDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();

private:
    int mTrainId;
};



class AddWayDialog : public Dialog
{
    Q_OBJECT
public:
    explicit AddWayDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
};



class DelWayDialog : public Dialog
{
    Q_OBJECT
public:
    explicit DelWayDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
    bool deleteWayWithTrains();
};



class ViewStationDialog : public Dialog
{
    Q_OBJECT
public:
    explicit ViewStationDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();

private:
    ScheduleViewer *mViewer;
};



class ViewRouteDialog : public Dialog
{
    Q_OBJECT
public:
    explicit ViewRouteDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();

private:
    ScheduleViewer *mViewer;
};



class SpeedDialog : public Dialog
{
    Q_OBJECT
public:
    explicit SpeedDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
};



class LaunchDialog : public Dialog
{
    Q_OBJECT
public:
    explicit LaunchDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
};

#endif // DIALOG_H
