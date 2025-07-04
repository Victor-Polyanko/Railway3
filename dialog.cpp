#include "dialog.h"
#include "ui_dialog.h"

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
