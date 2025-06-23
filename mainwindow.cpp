#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

const QString cDefaultTitle = "Залізниці 3.0";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(cDefaultTitle);
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
    QAction *saveAsAction = new QAction("Зберегти мапу як...");
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    QAction *exitAction = new QAction("Вийти");
    fileMenu->addAction(exitAction);

    QObject::connect(generateAction, &QAction::triggered, [&]() {
        mDisplay.generate();
        updateFileName("");
    });
    QObject::connect(loadAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Завантажити файл", "", "Railway Files (*.rw1)");
        if (!fileName.isEmpty())
        {
            QString result = mDisplay.load(fileName);
            updateFileName(fileName);
            showInfo(result);
        }
    });
    QObject::connect(saveAction, &QAction::triggered, [&]() {
        if (mFileName.isEmpty())
            mFileName = QFileDialog::getSaveFileName(this, "Зберегти файл", "", "Railway Files (*.rw1)");
        if (!mFileName.isEmpty())
        {
            QString result = mDisplay.save(mFileName);
            showInfo(result);
        }
    });
    QObject::connect(saveAsAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Зберегти файл як...", "", "Railway Files (*.rw1)");
        if (!fileName.isEmpty())
        {
            QString result = mDisplay.save(fileName);
            updateFileName(fileName);
            showInfo(result);
        }
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

void MainWindow::showInfo(const QString &aText) const
{
#ifdef DEBUG_MODE
    QString result = aText;
    result += "\nРозміри мапи: x = " + QString::number(mDisplay.mMap.getDimention().getX());
    result += " y = " + QString::number(mDisplay.mMap.getDimention().getY());
    result += "\nКількість регіонів: x = " + QString::number(mDisplay.mMap.getDistrictsQuantity().getX());
    result += " y = " + QString::number(mDisplay.mMap.getDistrictsQuantity().getX());
    result += "\nКількість міст в регіоні: " + QString::number(mDisplay.mMap.getDistrictsStationsQuantity());
    result += "\nКількість станцій: " + QString::number(mDisplay.mMap.getStations().size());
    result += "\nКількість шляхів: " + QString::number(mDisplay.mMap.getWays().size());
    result += "\nКількість потягів: " + QString::number(mDisplay.mMap.getTrains().size());
    QString details = "Станції:";
    for (const auto &station : mDisplay.mMap.getStations())
    {
        switch(station.getStatus())
        {
        case 3: details += "\nСтолиця "; break;
        case 2: details += "\nОблцентр "; break;
        case 1: details += "\nМісто "; break;
        default: details += "\nКазна що "; break;
        }
        details += station.getName() + " x = " + QString::number(station.getX()) + " y = " + QString::number(station.getY());
        details += " має " + QString::number(station.getConnectionsSize()) + " сполучень\t";
    }
    details += "\n\nШляхи:\n";
    for (const auto &way : mDisplay.mMap.getWays())
        details += mDisplay.mMap.getStations()[way.first].getName() + " - " +
                   mDisplay.mMap.getStations()[way.second].getName() + "\t";

    details += "\n\nПотяги:\n";
    for (const auto &train : mDisplay.mMap.getTrains())
    {
        switch(train.getType())
        {
        case Train::Fast: details += "\nШвидкий"; break;
        case Train::Passenger: details += "\nПасажирський"; break;
        case Train::Local: details += "\nПриміський"; break;
        default: details += "\nКазна який"; break;
        }
        details += " потяг №" + QString::number(train.getNumber()) + " ";
        details += mDisplay.mMap.getStations()[train.getStations().front().number].getName() + " - " +
                   mDisplay.mMap.getStations()[train.getStations().back().number].getName();
        details += " вирушає о " + QString::number(train.getStartTime().getX()) +
                   ":" + QString::number(train.getStartTime().getY());
    }
    showMessage("Деталі при відкритті файлу", result, details);
#else
    if (!aText.isEmpty())
        showMessage("Помилка", aText);
#endif
}

void MainWindow::showMessage(const QString &aTitle, const QString &aText, const QString &aDetails) const
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(aTitle);
    msgBox.setText(aText);
    if (!aDetails.isEmpty())
    {
        msgBox.setDetailedText(aDetails);
        msgBox.setStyleSheet("QLabel{min-width: 400px;}");
    }
    msgBox.exec();
}

void MainWindow::updateFileName(const QString &aFileName)
{
    mFileName = aFileName;
    this->setWindowTitle(cDefaultTitle + " " + mFileName);
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    mDisplay.showDistricts(painter);
    mDisplay.showStationsAndWays(painter);
}
