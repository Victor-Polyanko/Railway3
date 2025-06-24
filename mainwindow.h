#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

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
    virtual void paintEvent(QPaintEvent *) override;
    virtual void closeEvent(QCloseEvent *) override;

private:
    void addFileMenu();
    void addTrainMenu();
    void addWaysMenu();
    void addLaunchMenu();
    void addAboutMenu(QWidget *parent);
    void generateMap();
    void loadMap();
    void saveMap(const QString &aFileName = "");
    void showInfo(const QString &aText) const;
    void showMessage(const QString &aTitle, const QString &aText, const QString &aDetails = "") const;
    bool keepGoing();
    void updateFileName(const QString &aFileName);
    bool areNotSavedChanges() const;
    void markTitleWithChanges();

private:
    Ui::MainWindow *ui;
    QString mFileName;
    Display mDisplay;
};
#endif // MAINWINDOW_H
