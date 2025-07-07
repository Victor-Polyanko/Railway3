#include "dialog.h"
#include "ui_dialog.h"

#include <qmessagebox.h>

Dialog::Dialog(const QString &aType, Display *aDisplay, QWidget *aParent) :
    QDialog(aParent)
    , ui(new Ui::Dialog)
    , mType(aType)
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
