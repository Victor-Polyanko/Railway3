#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>

#include "display.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

const QString cAddTrain = "Створення потяга №";
const QString cAddWay = "Створення колії";
const QString cDelWay = "Розбирання колії";

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(const QString &aType, Display *mDisplay, QWidget *aParent = nullptr);
    ~Dialog();

protected:
    Ui::Dialog *ui;
    QString mType;
    Display *mDisplay;
};

#endif // DIALOG_H
