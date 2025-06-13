#include "display.h"

const int cStep = 3;
const int cStep2 = cStep * cStep;
const int cPartWidth = 20;
const int cPartHeight = 20;
const int cHalfWidth = cPartWidth / 2;
const int cHalfHeight = cPartHeight / 2;

Display::Display() {}

void Display::generate()
{
    mMap.generate();
    mParts.reserve(mMap.getColors().size());
    for (auto &color : mMap.getColors())
        switch (color)
        {
        case Map::Color::LavenderBlush:
            mParts.emplace_back(QBrush(QColor(255, 240, 245)));
            break;
        case Map::Color::Lavender:
            mParts.emplace_back(QBrush(QColor(230, 230, 250)));
            break;
        case Map::Color::Honeydew:
            mParts.emplace_back(QBrush(QColor(240,255,240)));
            break;
        case Map::Color::MistyRose:
            mParts.emplace_back(QBrush(QColor(255, 228, 225)));
            break;
        default: break;
        }
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

void Display::show(QPainter &aPainter) const
{
    double width = static_cast<double>(aPainter.window().width());
    double height = static_cast<double>(aPainter.window().height());
    double kx = width / mMap.getDimention().getX();
    double ky = height * 3 / mMap.getDimention().getY();
    for(int i = 0; i < mMap.getDimention().getX(); i += cStep)
    {
        int kxi = (int)(kx * i) - cHalfWidth;
        int n = i * mMap.getDimention().getY() / cStep2;
        for(int j = 0; j < mMap.getDimention().getY() / cStep; j++)
        {
            aPainter.setBrush(mParts[n + j]);
            aPainter.setPen(mParts[n + j].color());
            aPainter.drawRect(kxi, (int)(ky * j) - cHalfHeight, cPartWidth, cPartHeight);
        }
    }
    //QuickShow(g, w, h);
}

//void Display::quickShow(Graphics g, int w, int h)
//{
//    int diametr = mDimention.getX() / mDistrictQuantity.getX() / 20;
//    int radius;
//    double kx = (double)(w) / mDimention.getX();
//    double ky = (double)(h) / mDimention.getY();
//    SolidBrush brush = new SolidBrush(Color.Navy);
//    Pen pen = new Pen(brush, 5);
//    Font f;
//    PointF p;
//    int diakx =(int)(kx * diametr / 3);
//    for (auto &station : mStations)
//    {
//        radius = diakx * station.getStatus();
//        g.DrawEllipse(pen, (int)(kx * station.getX() - radius), (int)(ky * station.getY() - radius), 2 * radius, 2 * radius);
//        p = new PointF((float)(kx * station.getX() + radius), (float)(ky * station.getY()));
//        switch(station.getStatus())
//        {
//        case Station::Status::Town: f = new Font("Arial", 10, FontStyle.Regular);break;
//        case Station::Status::City: f = new Font("Arial", 10, FontStyle.Underline);break;
//        default: f = new Font("Arial", 15, FontStyle.Underline);break;
//        }
//        g.DrawString(station.getName(), f, brush, p);
//    }
//    for (auto &way : mMap.getWays())
//    {
//        g.DrawLine(pen, (int)(kx * mStations[way.getX()].getX()), (int)(ky * mStations[way.getX()].getY()),
//                   (int)(kx * mStations[way.getY()].getX()), (int)(ky * mStations[way.getY()].getY()));
//    }
//}

