#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(const QString &aTitle, const QString &aFirstText, const QString &aSecondText, const QStringList &aNames, QWidget *aParent = nullptr);
    ~Dialog();

signals:
    void getResult(QPair<int, int> &aResult);

private:
    void accept();

private:
    Ui::Dialog *ui;
    QString mSecondText;
    QPair<int, int> mResult;
};

#endif // DIALOG_H
