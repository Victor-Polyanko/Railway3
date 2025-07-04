#ifndef TRAIN_DIALOG_H
#define TRAIN_DIALOG_H

#include "dialog.h"
#include "Train.h"


class TrainDialog : public Dialog
{
    Q_OBJECT
public:
    explicit TrainDialog(const QString &aType, Display *mDisplay, QWidget *aParent = nullptr);

private:
    int newTrainNumber() const;
    void accept();

private:
    int mTrainType;
    Train mTrainResult;
};

#endif // TRAIN_DIALOG_H
