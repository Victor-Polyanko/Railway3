#include "map.h"

Map::Map()
{
    init(300, 300, 3, 3, 5);
}

Map::Map(int aXd, int aYd, int aXq, int aYq, int aQuantity)
{
    init( aXd, aYd, aXq, aYq, aQuantity);
}

void Map::init(int aXd, int aYd, int aXq, int aYq, int aDistrictStationsQuantity)
{
    mDimention = Point(aXd, aYd);
    mDistrictQuantity = Point(aXq, aYq);
    mDistrictStationsQuantity = aDistrictStationsQuantity;
    auto stationsQuantity = mDistrictQuantity.size() * mDistrictStationsQuantity;
    mStations.resize(stationsQuantity);
    mWays.resize(stationsQuantity * stationsQuantity);
    mTrains.resize(stationsQuantity * stationsQuantity);
    mTime = Point( -1, -1);
}

void Map::newGame()
{
    Point districtSize = mDimention / mDistrictQuantity;
    Random r = new Random();
    for(int i = 0; i < mDistrictQuantity.getX(); i++)
        for(int j = 0; j < mDistrictQuantity.getY(); j++)
        {
            for(int k = 0; k < mDistrictStationsQuantity; k++)
            {
                auto index = (i * mDistrictQuantity.getY() + j) * mDistrictStationsQuantity + k;
                mStations[index] = Station(i * districtSize.getX() + r.Next(districtSize.getX() - 10) + 5, j * districtSize.getY() + r.Next(districtSize.getY() - 10) + 5, StationStatus.Town, townQuantity * townQuantity);
            }
        }
    //Створюємо глобальну мережу сполучень
    globalConnection(0, mStations.size() - 1);

    for(int i = 0; i < mDistrictQuantity.getX(); i++)
        for(int j = 0; j < mDistrictQuantity.getY(); j++)
        {
            auto p = Point(2 * i + 1, 2 * j + 1) * mDimention / mDistrictQuantity / Point(2, 2);
            int n0 = (i * mDistrictQuantity.getY() + j) * mDistrictStationsQuantity;
            int n = n0;
            for(int k = 1; k < mDistrictStationsQuantity; k++)
                if((mStations[n0 + k].getConnectionsSize() > mStations[n].getConnectionsSize()) ||
                    ((mStations[n0 + k].getConnectionsSize() == mStations[n].getConnectionsSize())
                        && (distance(mStations[n0 + k], p) < distance(mStations[n], p))))
                {
                    n = n0 + k;
                }
            mStations[n].setStatus((mDistrictQuantity.getX() / 2 == i && mDistrictQuantity.getY() / 2 == j) ? 3 : 2);
        }
    buildParts();
}

void Map::globalConnection(int i0, int i1)
{
    int i2;
    //Йдемо по всім містам
    for(int i = i0; i < i1; i++)
    {
        i2 = i;
        //Розширюємо радіус пошуку: від 1 і до ...
        for(int d = 1; d < mDimention.getX(); d ++ )
        {
            int j = 0;
            //Для кожного радіусу знаходимо міста, які потрапляють у його коло
            for(j = 0; j < mStations.size(); j++)
            {
                if(i2 != j && distance(i2, j) < d && distance(i2, j) >= d - 2)
                {
                    if(madeConnection(i2, j, d))
                        break;
                }
            }
            //Якщо побудували дорогу, то переходимо до наступного міста
            if(j < mStations.size())
            {
                break;
            }
        }
    }
}

bool Map::foundConnection(int i, int j, int k)
{
    if (i == j)
        return true;
    for (auto &way : mWays)
        if(way.getX() == i && way.getY() != k)
        {
            if(foundConnection(way.getY(), j, i))
                return true;
        }
        else if(way.getY() == i && way.getX() != k)
        {
            if(foundConnection(way.getX(), j, i))
                return true;
        }
    return false;
}

bool Map::madeConnection(int &i, int j, int &d)
{
    auto way = mWays.begin();
    //перевірка на те, чи шляху ще не було
    //якщо шлях був, то відкидаємо це місто і шукаємо далі
    if(foundConnection(i, j, i))
        return false;
    //перевірка на те, чи немає звязаного міста, ближчого до знайденого
    int l = 0;
    for (; way != mWays.end(); ++way)
        if(way->getX() == i || way->getY() == i)
        {
            l = (way->getX() == i) ? way->getY() : way->getX();
            //Якщо є ближче місто
            if(distance(l, j) < distance(i, j))
            {
                break;
            }
        }
    //якщо є, то переходимо до розгляду цього міста
    if (way != mWays.end())
    {
        i = l;
        d = distance(i, j);
        return madeConnection(i, j, d);
    }
    //інакше будуємо дорогу
    else
    {
        mWays.emplace_back(Point(i, j));
        mStations[i].incConnectionsSize();
        mStations[j].incConnectionsSize();
        return true;
    }
}

int Map::distance(int i, int j) const
{
    return mStations[i].distance(mStations[j]);
}

void Map::buildParts()
{
    mPart = new SolidBrush[mDimention.getX() * mDimention.getY() / 3];
    for(int i = 0; i < mDimention.getX(); i += 3)
    {
        int n = i * mDimention.getY() / 9;
        for(int j = 0; j < mDimention.getY(); j += 3)
        {
            int r = 2;
            int k;
            do
            {
                for(k = 0; k < mStations.size(); k++)
                    if(distance(i, j, mStations[k].getX(), mStations[k].getY()) < r)
                        break;
                if(k == mStations.size())
                    r += 2;
                else
                    break;
            } while(true);
            r = k / mDistrictStationsQuantity;
            if((r / mDistrictQuantity.getY()) % 2 == 0)
            {
                if(r % 2 == 0)
                    mPart[n + j / 3] = new SolidBrush(Color.LavenderBlush);
                else
                    mPart[n + j / 3] = new SolidBrush(Color.Lavender);
            }
            else
            {
                if(r % 2 == 0)
                    mPart[n + j / 3] = new SolidBrush(Color.Honeydew);
                else
                    mPart[n + j / 3] = new SolidBrush(Color.MistyRose);
            }
        }
    }
}

bool Map::isEarlier(Point first, Point last){
    if(first.getX() == last.getX()){
        if(first.getY() <= last.getY())
            return true;
        else
            return false;
    }
    else
        if(first.getX() < last.getX())
            return true;
        else
            return false;
}

int Map::getMinutes(Point first, Point last){
    int minutes = (last.getX() - first.getX()) * 60 + last.getY() - first.getY();
    if(minutes < 0)
        minutes += 24 * 60;
    return minutes;
}

void Map::showTrains(Graphics g, int w, int h, Point time){
    g.FillRectangle(new SolidBrush(Color.White),0,0,w,h);
    Show(g, w, h);
    int diametr = mDimention.getX() / mDistrictQuantity.getX() / 20;
    double kx = (double)(w) / mDimention.getX();
    double ky = (double)(h) / mDimention.getY();
    SolidBrush brush;
    Pen pen;
    Font f = new Font("Arial", 8, FontStyle.Regular);
    PointF p;
    cPoint trainXY;
    int radius =(int)(kx * mDimention.getX() / mDistrictQuantity.getX() / 120);
    for(int i = 0; i < trainQuantity; i++)
    {
        trainXY = Point(-1,-1);
        //пошук на проміжних станціях
        for(int j = 1; j < mTrains[i].stationQuantity - 1; j++)
            if(IsEarlier(mTrains[i].station[j].arrive,time) && IsEarlier(time, mTrains[i].station[j].depart)){
                trainXY = Point(mStations[mTrains[i].station[j].number]);
                break;
            }
        //пошук між станціями
        if(trainXY.getX() == -1 )
            for(int j = 1; j < mTrains[i].stationQuantity; j++)
                if(IsEarlier(time,mTrains[i].station[j].arrive) && IsEarlier(mTrains[i].station[j - 1].depart, time)){
                    int trainMinutes = GetMinutes(mTrains[i].station[j - 1].depart,time);
                    int stationMinutes = GetMinutes(mTrains[i].station[j - 1].depart,mTrains[i].station[j].arrive);
                    float relation = (float)trainMinutes / stationMinutes;
                    trainXY = Point(mStations[mTrains[i].station[j - 1].number].getX() + (int)((mStations[mTrains[i].station[j].number].getX() - mStations[mTrains[i].station[j - 1].number].getX()) * relation),
                                         mStations[mTrains[i].station[j - 1].number].getY() + (int)((mStations[mTrains[i].station[j].number].getY() - mStations[mTrains[i].station[j - 1].number].getY()) * relation));
                }
        //пошук на кінцевих станціях
        if(trainXY.getX() == -1 )
        {
            int minutes = GetMinutes(mTrains[i].station[mTrains[i].stationQuantity - 1].arrive,mTrains[i].station[0].depart);
            if(minutes < 120)
                minutes /= 2;
            else
                minutes = 60;
            cPoint firstPoint = mTrains[i].station[0].depart;
            firstPoint.getY() -= minutes % 60;
            if(firstPoint.getY() < 0)
            {
                firstPoint.getY() += 60;
                firstPoint.getX()--;
            }
            if(firstPoint.getX() < 0)
                firstPoint.getX() += 24;
            cPoint secondPoint = mTrains[i].station[mTrains[i].stationQuantity - 1].arrive;
            secondPoint.getY() += minutes % 60;
            if(secondPoint.getY() >= 60)
            {
                secondPoint.getY() -= 60;
                secondPoint.getX()++;
            }
            if(secondPoint.getX() >=24)
                secondPoint.getX() -= 24;
            if(IsEarlier(time,firstPoint) && GetMinutes(time,firstPoint) <= 60)
                trainXY = Point(mStations[mTrains[i].station[0].number]);
            else
                if(IsEarlier(secondPoint,time) && GetMinutes(secondPoint,time) <= 60)
                    trainXY = Point(mStations[mTrains[i].station[mTrains[i].stationQuantity - 1].number]);

        }
        if(trainXY.getX() != -1)
        {
            switch(mTrains[i].status)
            {
            case TrainStatus.Fast: brush = new SolidBrush(Color.Red);break;
            case TrainStatus.Passenger: brush = new SolidBrush(Color.Blue);break;
            default: brush = new SolidBrush(Color.Green);break;
            }
            pen = new Pen(brush, 8);
            g.DrawEllipse(pen, (int)(kx * trainXY.getX() - radius), (int)(ky * trainXY.getY() - radius), 2 * radius, 2 * radius);
            p = new PointF((float)(kx * trainXY.getX() - ((mTrains[i].number < 10) ? 3 : 5) * radius / 2), (float)(ky * trainXY.getY() - 5 * radius / 2));
            brush = new SolidBrush(Color.White);
            g.DrawString(mTrains[i].number.ToString(), f, brush, p);
        }
    }
}

void Map::show(Graphics g, int w, int h)
{
    double kx = (double)(w) / mDimention.getX();
    double ky = (double)(h * 3) / mDimention.getY();
    SolidBrush brush = new SolidBrush(Color.White);
    for(int i = 0; i < mDimention.getX(); i += 3)
    {
        int kxi = (int)(kx * i) - 10;
        int n = i * mDimention.getY() / 9;
        for(int j = 0; j < mDimention.getY() / 3; j++)
            g.FillRectangle(mPart[n + j], kxi, (int)(ky * j) - 10, 20, 20);
    }
    QuickShow(g, w, h);
}

void Map::quickShow(Graphics g, int w, int h)
{
    int diametr = mDimention.getX() / mDistrictQuantity.getX() / 20;
    int radius;
    double kx = (double)(w) / mDimention.getX();
    double ky = (double)(h) / mDimention.getY();
    SolidBrush brush = new SolidBrush(Color.Navy);
    Pen pen = new Pen(brush, 5);
    Font f;
    PointF p;
    int diakx =(int)(kx * diametr / 3);
    for (auto &station : mStations)
    {
        radius = diakx * station.getStatus();
        g.DrawEllipse(pen, (int)(kx * station.getX() - radius), (int)(ky * station.getY() - radius), 2 * radius, 2 * radius);
        p = new PointF((float)(kx * station.getX() + radius), (float)(ky * station.getY()));
        switch(station.getStatus())
        {
        case Station::Status::Town: f = new Font("Arial", 10, FontStyle.Regular);break;
        case Station::Status::City: f = new Font("Arial", 10, FontStyle.Underline);break;
        default: f = new Font("Arial", 15, FontStyle.Underline);break;
        }
        g.DrawString(station.getName(), f, brush, p);
    }
    for (auto &way : mWays)
    {
        g.DrawLine(pen, (int)(kx * mStations[way.getX()].getX()), (int)(ky * mStations[way.getX()].getY()),
                   (int)(kx * mStations[way.getY()].getX()), (int)(ky * mStations[way.getY()].getY()));
    }
}
