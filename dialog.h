#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>

#include "map.h"

const int cNotSet = -1;
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

protected:
    bool fillNeighbours(int aStationId = cNotSet);
    void showTrains();
    void showTimes();

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
    int newTrainNumber() const;
    void accept();
    void apply();

private:
    Train::Type mTrainType;
    Train mTrainResult;
};



class DelTrainDialog : public Dialog
{
    Q_OBJECT
public:
    explicit DelTrainDialog(Map *aMap, QWidget *aParent = nullptr);

private:
    void accept();
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

#endif // DIALOG_H
