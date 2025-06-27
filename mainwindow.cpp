#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

const QString cNewPrefix = "* ";
const QString cDefaultTitle = "Залізниця 3.0";
const QString cAboutSoft =
    "Програму '" + cDefaultTitle + "' стоврив\nВіктор Полянко\n" \
    "11-30 червня 2025 року.\nPolyanko_Victor@ukr.net";
const QString cAboutGame =
    "Це - гра-спостереження.\nНадається можливість змоделювати карту,\n" \
    "прокласти по ній маршрути потягів і спостерігати як вони рухаються.\n" \
    "Також є можливість прокладати нові гілки або знищувати існуючі.\n" \
    "Типи поїздів впливають на їхні швидкості та часи зупинки на станціях.\n" \
    "На часи стоянок поїздів впливають також типи самих станцій.\n" \
    "Крім візуального спостереження можна проглядати розклади руху\n" \
    "самих потягів, окремих станцій або отримувати інформацію про те,\n" \
    "як дістатися з однієї станції до іншої.\n\n" \
    "Приємного часопроведення! :-)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(cDefaultTitle);
    QObject::connect(ui->generateAction, &QAction::triggered, [&]() { generateMap(); });
    QObject::connect(ui->loadAction, &QAction::triggered, [&]() { loadMap(); });
    QObject::connect(ui->saveAction, &QAction::triggered, [&]() { saveMap(mFileName); });
    QObject::connect(ui->saveAsAction, &QAction::triggered, [&]() { saveMap(); });
    QObject::connect(ui->exitAction, &QAction::triggered, [&]() { QApplication::quit(); });
    QObject::connect(ui->aboutSoftAction, &QAction::triggered, [&]() {
        QMessageBox::information(parent, "Про програму", cAboutSoft); });
    QObject::connect(ui->aboutGameAction, &QAction::triggered, [&]() {
        QMessageBox::information(parent, "Про гру", cAboutGame); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateMap()
{
    if (!keepGoing())
        return;
    mDisplay.generate();
    updateFileName("");
}

void MainWindow::loadMap()
{
    if (!keepGoing())
        return;
    QString fileName = QFileDialog::getOpenFileName(this, "Завантажити файл", "", "Railway Files (*.rw1)");
    if (!fileName.isEmpty())
    {
        QString result = mDisplay.load(fileName);
        updateFileName(fileName);
        showInfo(result);
    }
}

void MainWindow::saveMap(const QString &aFileName)
{
    if (!areNotSavedChanges() && aFileName.isEmpty())
        return;
    auto name = aFileName.isEmpty()
        ? QFileDialog::getSaveFileName(this, "Зберегти файл як...", "", "Railway Files (*.rw1)")
        : aFileName;
    if (!name.isEmpty())
    {
        QString result = mDisplay.save(name);
        updateFileName(name);
        showInfo(result);
    }
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
    showMessage("Деталі файлу", result, details);
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

bool MainWindow::keepGoing()
{
    bool doNext = true;
    if (areNotSavedChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("Бажаєте спершу зберегти зміни?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        switch (msgBox.exec())
        {
        case QMessageBox::Save:
            saveMap(mFileName);
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            doNext = false;
            break;
        default:
            break;
        }
    }
    return doNext;
}

void MainWindow::updateFileName(const QString &aFileName)
{
    mFileName = aFileName;
    auto title = aFileName.isEmpty()
                     ? (cNewPrefix + cDefaultTitle)
                     : (cDefaultTitle + " - " + mFileName);
    this->setWindowTitle(title);
}

bool MainWindow::areNotSavedChanges() const
{
    return this->windowTitle().startsWith(cNewPrefix);
}

void MainWindow::markTitleWithChanges()
{
    this->setWindowTitle(cNewPrefix + this->windowTitle());
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    mDisplay.showDistricts(painter);
    mDisplay.showStationsAndWays(painter);
}

void MainWindow::closeEvent(QCloseEvent *aEvent)
{
    if (keepGoing())
        aEvent->accept();
    else
        aEvent->ignore();
}
