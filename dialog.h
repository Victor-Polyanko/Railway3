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

const QString cAddWay = "Створення колії";
const QString cDelWay = "Розбирання колії";

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(const QString &aType, const QString &aFirstText, const QString &aSecondText, Display *mDisplay, QWidget *aParent = nullptr);
    ~Dialog();

signals:
    void getResult(QPair<int, int> &aResult);

private:
    void accept();

private:
    Ui::Dialog *ui;
    QString mType;
    QString mSecondText;
    Display *mDisplay;
    QPair<int, int> mResult;
};

#endif // DIALOG_H
