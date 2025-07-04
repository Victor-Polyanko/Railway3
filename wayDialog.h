#ifndef WAY_DIALOG_H
#define WAY_DIALOG_H

#include "dialog.h"

class WayDialog : public Dialog
{
    Q_OBJECT
public:
    explicit WayDialog(const QString &aType, Display *mDisplay, QWidget *aParent = nullptr);

private:
    void accept();

private:
    QPair<int, int> mWayResult;
};

#endif // WAY_DIALOG_H
