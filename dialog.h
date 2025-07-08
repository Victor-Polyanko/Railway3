#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>

#include "display.h"
#include "Train.h"

const int cNotSet = -1;

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(Display *mDisplay, QWidget *aParent = nullptr);
    ~Dialog();

signals:
    void ready();

protected:
    bool fillNeighbours(int aStationId = cNotSet);
    void showTrains();
    void showTimes();

protected:
    Ui::Dialog *ui;
    Display *mDisplay;
    Way mWayResult;
};



class AddTrainDialog : public Dialog
{
    Q_OBJECT
public:
    explicit AddTrainDialog(Display *mDisplay, QWidget *aParent = nullptr);

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
    explicit DelTrainDialog(Display *mDisplay, QWidget *aParent = nullptr);

private:
    void accept();
};



class ChangeTimeDialog : public Dialog
{
    Q_OBJECT
public:
    explicit ChangeTimeDialog(Display *mDisplay, QWidget *aParent = nullptr);

private:
    void accept();

private:
    int mTrainId;
};



class AddWayDialog : public Dialog
{
    Q_OBJECT
public:
    explicit AddWayDialog(Display *mDisplay, QWidget *aParent = nullptr);

private:
    void accept();
};



class DelWayDialog : public Dialog
{
    Q_OBJECT
public:
    explicit DelWayDialog(Display *mDisplay, QWidget *aParent = nullptr);

private:
    void accept();
    bool deleteWayWithTrains();
};

#endif // DIALOG_H
