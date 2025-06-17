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

Display::Display() {}

void Display::generate()
{
    mMap.generate();
    mParts.reserve(mMap.getColors().size());
    for (auto &color : mMap.getColors())
        switch (color)
        {
        case Map::Color::LavenderBlush:
            mParts.emplace_back(QBrush(cLavenderBlush));
            break;
        case Map::Color::Lavender:
            mParts.emplace_back(QBrush(cLavender));
            break;
        case Map::Color::Honeydew:
            mParts.emplace_back(QBrush(cHoneydew));
            break;
        case Map::Color::MistyRose:
            mParts.emplace_back(QBrush(cMistyRose));
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
        file.close();
    }
    catch (...)
    {
        return " Сталася помилка зчитування даних. \n Причина: Невiдповiднiсть форматiв даних.\n Оберіть iнший файл або почнiть нову гру.";
    }
    return "";
}

//void Display::showTrains(Graphics g, int w, int h, Point &aTime){
//    g.FillRectangle(new SolidBrush(Color.White),0,0,w,h);
//    Show(g, w, h);
//    double kx = (double)(w) / mDimention.getX();
//    double ky = (double)(h) / mDimention.getY();
//    SolidBrush brush;
//    Pen pen;
//    Font f = new Font("Arial", 8, FontStyle.Regular);
//    PointF p;
//    cPoint trainXY;
//    int radius =(int)(kx * mDimention.getX() / mDistrictQuantity.getX() / 120);
//    for (auto &train : mMaps.getTrains())
//    {
//        trainXY = mMap.findTrainPosition(train);
//        if(trainXY.getX() != -1)
//        {
//            switch(train.status)
//            {
//                case TrainStatus.Fast: brush = new SolidBrush(Color.Red);
//                    break;
//                case TrainStatus.Passenger: brush = new SolidBrush(Color.Blue);
//                    break;
//                default: brush = new SolidBrush(Color.Green);
//                    break;
//            }
//            pen = new Pen(brush, 8);
//            g.DrawEllipse(pen, (int)(kx * trainXY.getX() - radius), (int)(ky * trainXY.getY() - radius), 2 * radius, 2 * radius);
//            p = new PointF((float)(kx * trainXY.getX() - ((train.number < 10) ? 3 : 5) * radius / 2), (float)(ky * trainXY.getY() - 5 * radius / 2));
//            brush = new SolidBrush(Color.White);
//            g.DrawString(train.number.ToString(), f, brush, p);
//        }
//    }
//}

void Display::showDistricts(QPainter &aPainter) const
{
    if (mMap.getDimention().getX() <= 0 ||
        mMap.getDimention().getY() <= 0 ||
        mParts.empty())
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
            aPainter.setBrush(mParts[n + j]);
            aPainter.setPen(mParts[n + j].color());
            aPainter.drawRect(kxi, static_cast<int>(ky * j) - cHalfHeight, cPartWidth, cPartHeight);
        }
    }
}

void Display::showStationsAndWays(QPainter &aPainter) const
{
    int diametr = mMap.getDimention().getX() / mMap.getDistrictsStationsQuantity() / 20;
    double width = static_cast<double>(aPainter.window().width());
    double height = static_cast<double>(aPainter.window().height());
    double kx = width / mMap.getDimention().getX();
    double ky = height / mMap.getDimention().getY();
    QPen pen;
    pen.setColor(cNavy);
    pen.setWidth(cLineWidth);
    aPainter.setPen(pen);
    int diakx = static_cast<int>(kx * diametr);
    for (auto &station : mMap.getStations())
    {
        int radius = diakx * station.getStatus();
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
