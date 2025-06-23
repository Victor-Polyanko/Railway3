#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "display.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void paintEvent(QPaintEvent *) override;

private:
    void addFileMenu();
    void addTrainMenu();
    void addWaysMenu();
    void addLaunchMenu();
    void addAboutMenu(QWidget *parent);
    void showInfo(const QString &aText) const;
    void showMessage(const QString &aTitle, const QString &aText, const QString &aDetails = "") const;
    void updateFileName(const QString &aFileName);

private:
    Ui::MainWindow *ui;
    QString mFileName;
    Display mDisplay;
};
#endif // MAINWINDOW_H
