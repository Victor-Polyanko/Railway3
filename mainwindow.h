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
    void paintEvent(QPaintEvent *event) override;

private:
    void addFileMenu();
    void addTrainMenu();
    void addWaysMenu();
    void addLaunchMenu();
    void addAboutMenu(QWidget *parent);

private:
    Ui::MainWindow *ui;
    Display mDisplay;
};
#endif // MAINWINDOW_H
