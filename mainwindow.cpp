#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addFileMenu();
    addTrainMenu();
    addWaysMenu();
    addLaunchMenu();
    addAboutMenu(parent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFileMenu()
{
    QMenu *fileMenu = ui->menubar->addMenu("Головне меню");
    QAction *generateAction = new QAction("Згенерувати Україну");
    fileMenu->addAction(generateAction);
    QAction *loadAction = new QAction("Завантажити мапу");
    fileMenu->addAction(loadAction);
    QAction *saveAction = new QAction("Зберегти мапу");
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    QAction *exitAction = new QAction("Вийти");
    fileMenu->addAction(exitAction);

    QObject::connect(generateAction, &QAction::triggered, [&]() {
        mDisplay.generate();
        //QMessageBox msgBox;
        //QString text = "ways = " + QString::number(mDisplay.mMap.getWays().size())\
        //    + "\n = " + QString::number(mDisplay.mMap.getColors().size());
        //msgBox.setText(text);
        //msgBox.setWindowTitle("Message Box Title");
        //msgBox.exec();
        //mDisplay.show(mPainter);
        this->show();
    });
    QObject::connect(exitAction, &QAction::triggered, [&]() { QApplication::quit(); });
}

void MainWindow::addTrainMenu()
{
    QMenu *trainMenu = ui->menubar->addMenu("Потяги");
    QAction *createTrainAction = new QAction("Створити потяг");
    trainMenu->addAction(createTrainAction);
    QAction *deleteTrainAction = new QAction("Знищити потяг");
    trainMenu->addAction(deleteTrainAction);
    QAction *watchTrainAction = new QAction("Проглянути потяг");
    trainMenu->addAction(watchTrainAction);
    QAction *changeTimeAction = new QAction("Змінити час відправлення");
    trainMenu->addAction(changeTimeAction);
}

void MainWindow::addWaysMenu()
{
    QMenu *waysMenu = ui->menubar->addMenu("Залізниця");
    QAction *createWayAction = new QAction("Прокласти гілку");
    waysMenu->addAction(createWayAction);
    QAction *deleteWayAction = new QAction("Знищити гілку");
    waysMenu->addAction(deleteWayAction);
    QAction *watchStationAction = new QAction("Проглянути станцію");
    waysMenu->addAction(watchStationAction);
    QAction *watchRouteAction = new QAction("Як проїхати між...");
    waysMenu->addAction(watchRouteAction);
}

void MainWindow::addLaunchMenu()
{
    QMenu *launchMenu = ui->menubar->addMenu("Запуск");
}

void MainWindow::addAboutMenu(QWidget *parent)
{
    QMenu *aboutMenu = ui->menubar->addMenu("Інформація");
    QAction *aboutSoftAction = new QAction("Про програму");
    aboutMenu->addAction(aboutSoftAction);
    QAction *aboutGameAction = new QAction("Про гру");
    aboutMenu->addAction(aboutGameAction);

    QObject::connect(aboutSoftAction, &QAction::triggered, [&]() {
        QMessageBox::information(parent, "Про програму",
            "Програму 'Залізниця-3' стоврив\nВіктор Полянко\n" \
            "11-15 червня 2025 року.\nPolyanko_Victor@ukr.net");
    });
    QObject::connect(aboutGameAction, &QAction::triggered, [&]() {
        QMessageBox::information(parent, "Про гру",
            "Це - гра-спостереження.\nНадається можливість змоделювати карту,\n" \
            "прокласти по ній маршрути потягів і спостерігати як вони рухаються.\n" \
            "Також є можливість прокладати нові гілки або знищувати існуючі.\n" \
            "Типи поїздів впливають на їхні швидкості та часи зупинки на станціях.\n" \
            "На часи стоянок поїздів впливають також типи самих станцій.\n" \
            "Крім візуального спостереження можна проглядати розклади руху\n" \
            "самих потягів, окремих станцій або отримувати інформацію про те,\n" \
            "як дістатися з однієї станції до іншої.\n\n" \
            "Приємного часопроведення! :-)");
    });
}

void MainWindow::paintEvent(QPaintEvent *event)  {
    QPainter painter(this);
    if (!mDisplay.mMap.getWays().empty())
        mDisplay.show(painter);
}
