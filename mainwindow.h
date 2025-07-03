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
    void generateMap();
    void loadMap();
    void saveMap(const QString &aFileName = "");
    void showInfo(const QString &aText) const;
    void showMessage(const QString &aTitle, const QString &aText, const QString &aDetails = "") const;
    bool keepGoing();
    void updateFileName(const QString &aFileName);
    bool areNotSavedChanges() const;
    void markTitleWithChanges();
    void launchDialog(const QString &aTitle);

private:
    Ui::MainWindow *ui;
    QString mFileName;
    Display mDisplay;
};
#endif // MAINWINDOW_H
