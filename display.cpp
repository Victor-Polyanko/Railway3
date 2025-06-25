#include "display.h"

#include <QFile>
#include <QPen>

const int cStep = 3;
const int cStep2 = cStep * cStep;
const int cPartWidth = 20;
const int cPartHeight = 20;
const int cHalfWidth = cPartWidth / 2;
const int cHalfHeight = cPartHeight / 2;
const int cLineWidth = 5;

const QColor cLavenderBlush(255, 240, 245);
const QColor cLavender(230, 230, 250);
const QColor cHoneydew(240,255,240);
const QColor cMistyRose(255, 228, 225);
const QColor cNavy(0, 0, 128);
const QColor cWhite(255, 255, 255);
const QColor cRed(0, 0, 128);
const QColor cBlue(0, 0, 128);
const QColor cOrange(0, 0, 128);

Display::Display() {}

void Display::generate()
{
    mMap.generate();
    convertColors();
}

void Display::convertColors()
{
    mColors.reserve(mMap.getColors().size());
    for (auto &color : mMap.getColors())
        switch (color)
        {
        case Map::Color::LavenderBlush:
            mColors.emplace_back(QColor(cLavenderBlush));
            break;
        case Map::Color::Lavender:
            mColors.emplace_back(QColor(cLavender));
            break;
        case Map::Color::Honeydew:
            mColors.emplace_back(QColor(cHoneydew));
            break;
        case Map::Color::MistyRose:
            mColors.emplace_back(QColor(cMistyRose));
            break;
        default: break;
        }
}

QString Display::load(const QString &aFileName)
{
    QFile file(aFileName);
    if (!file.open(QIODevice::ReadOnly))
        return " Файл не знайдено.\n Спробуйте вiдкрити iнший файл або почнiть нову гру.";
    try
    {   QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);
        mMap.load(stream);
        convertColors();
        file.close();
    }
    catch (...)
    {
        return " Сталася помилка зчитування даних. \n Причина: Невiдповiднiсть форматiв даних.\n Оберіть iнший файл або почнiть нову гру.";
    }
    return "";
}

QString Display::save(const QString &aFileName)
{
    QFile file(aFileName);
    if (!file.open(QIODevice::WriteOnly))
        return "Сталася помилка пiдготовки до запису.\n Спробуйте iншу назву файл або змініть розташування.";
    try
    {   QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);
        mMap.save(stream);
        file.close();
    }
    catch (...)
    {
        return "Сталася помилка запису даних.";
    }
    return "";
}

void Display::showTrains(QPainter &aPainter, const TimePoint &aTime) const
{
    if (!aTime.isSet())
        return;
    float width = static_cast<double>(aPainter.window().width());
    float height = static_cast<double>(aPainter.window().height());
    float kx = width / mMap.getDimention().getX();
    float ky = height / mMap.getDimention().getY();

    QPen textPen;
    textPen.setColor(cWhite);
    QFont font = QFont("Arial", 8);
    aPainter.setFont(font);
    int radius = static_cast<int>(kx * mMap.getDimention().getX() / mMap.getDistrictsQuantity().getX() / 120);
    int diameter = 2 * radius;
    for (auto &train : mMap.getTrains())
    {
        Point trainXY = mMap.findTrainPosition(train, aTime);
        if(trainXY.getX() != -1)
        {
            QPen pen;
            if (mLastTime.isSet())
            switch(train.getType())
            {
                case Train::Fast: pen.setColor(cRed);
                    break;
                case Train::Passenger: pen.setColor(cBlue);
                    break;
                default: pen.setColor(cOrange);
                    break;
            }
            else
                pen.setColor(cWhite);
            float x = kx * trainXY.getX();
            float y = ky * trainXY.getY();
            pen.setWidth(8);
            aPainter.setPen(pen);
            aPainter.drawEllipse(static_cast<int>(x - radius), static_cast<int>(y - radius), diameter, diameter);
            QPointF point(static_cast<float>(x - ((train.getNumber() < 10) ? 3 : 5) * radius / 2),
                          static_cast<float>(y - 5 * radius / 2));
            aPainter.setPen(textPen);
            aPainter.drawText(point, QString::number(train.getNumber()));
        }
    }
}

void Display::showDistricts(QPainter &aPainter) const
{
    if (mMap.getDimention().getX() <= 0 ||
        mMap.getDimention().getY() <= 0 ||
        mColors.empty())
        return;
    double width = static_cast<double>(aPainter.window().width());
    double height = static_cast<double>(aPainter.window().height());
    double kx = width / mMap.getDimention().getX();
    double ky = height * cStep / mMap.getDimention().getY();
    for(int i = 0; i < mMap.getDimention().getX() - cStep; i += cStep)
    {
        int kxi = static_cast<int>(kx * i) - cHalfWidth;
        int n = i * mMap.getDimention().getY() / cStep2;
        for(int j = 0; j < mMap.getDimention().getY() / cStep; ++j)
        {
            aPainter.setBrush(mColors[n + j]);
            aPainter.setPen(mColors[n + j]);
            aPainter.drawRect(kxi, static_cast<int>(ky * j) - cHalfHeight, cPartWidth, cPartHeight);
        }
    }
}

void Display::showStationsAndWays(QPainter &aPainter) const
{
    double width = static_cast<double>(aPainter.window().width());
    double height = static_cast<double>(aPainter.window().height());
    double kx = width / mMap.getDimention().getX();
    double ky = height / mMap.getDimention().getY();
    QPen pen;
    pen.setColor(cNavy);
    pen.setWidth(cLineWidth);
    aPainter.setPen(pen);
    aPainter.setBrush(cWhite);
    for (auto &station : mMap.getStations())
    {
        int radius = static_cast<int>(kx * mMap.getStationRadius(station.getStatus()));
        int diameter = radius * 2;
        aPainter.drawEllipse(static_cast<int>(kx * station.getX() - radius), static_cast<int>(ky * station.getY() - radius), diameter, diameter);
        QPointF point(static_cast<float>(kx * station.getX() + radius), static_cast<float>(ky * station.getY()));
        QFont font;
        font.setUnderline(true);
        switch(station.getStatus())
        {
            case Station::Status::Capital: font = QFont("Arial", 15);
                break;
            case Station::Status::City: font = QFont("Arial", 10);
                break;
            default: font = QFont("Arial", 10);
                font.setUnderline(false);
                break;
        }
        aPainter.setFont(font);
        aPainter.drawText(point, station.getName());
    }
    for (auto &way : mMap.getWays())
        aPainter.drawLine(static_cast<int>(kx * mMap.getStations()[way.first].getX()),
                          static_cast<int>(ky * mMap.getStations()[way.first].getY()),
                          static_cast<int>(kx * mMap.getStations()[way.second].getX()),
                          static_cast<int>(ky * mMap.getStations()[way.second].getY()));
}

void Display::updateLastTime(const TimePoint &aTime)
{
    mLastTime = aTime;
}
