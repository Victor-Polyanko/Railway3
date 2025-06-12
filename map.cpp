#include "map.h"
#include <cstdlib>
#include <ctime>

const int cDefaultXDimention = 300;
const int cDefaultYDimention = 300;
const int cDefaultXQuantity = 3;
const int cDefaultYQuantity = 3;
const int cDefaultDistrictStationsQuantity = 5;
const int cMinutesInHour = 60;
const int cHoursInDay = 24;

Map::Map()
{
    init(cDefaultXDimention, cDefaultYDimention,
         cDefaultXQuantity, cDefaultYQuantity,
         cDefaultDistrictStationsQuantity);
}

Map::Map(int aXd, int aYd, int aXq, int aYq, int aQuantity)
{
    init(aXd, aYd, aXq, aYq, aQuantity);
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

QVector<Color> Map::getColors() const
{
    return mColors;
}

QVector<Train> Map::getTrains() const
{
    return mTrains;
}

QVector<Point> Map::getWays() const
{
    return mWays;
}

void Map::newGame()
{
    Point districtSize = mDimention / mDistrictQuantity;
    srand(time(0));
    int stationsQuantity2 = mStations.size() * mStations.size();
    for(int i = 0; i < mDistrictQuantity.getX(); i++)
        for(int j = 0; j < mDistrictQuantity.getY(); j++)
        {
            for(int k = 0; k < mDistrictStationsQuantity; k++)
            {
                auto index = (i * mDistrictQuantity.getY() + j) * mDistrictStationsQuantity + k;
                Point randomPoint(rand() % (districtSize.getX() - 10), rand() % (districtSize.getY() - 10));
                mStations[index] = Station(i * districtSize.getX() + randomPoint.getX() + 5,
                                           j * districtSize.getY() + randomPoint.getY() + 5,
                                           Station::Status::Town, stationsQuantity2);
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
    //перевірка на те, чи немає зв'язаного міста, ближчого до знайденого
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

void Map::fillColors()
{
    const int step = 3;
    const int step2 = step * step;
    mColors.resize(mDimention.getX() * mDimention.getY() / step);
    for (int i = 0; i < mDimention.getX(); i += step)
    {
        int n = i * mDimention.getY() / step2;
        for (int j = 0; j < mDimention.getY(); j += step)
        {
            int r = 2;
            int k = 0;
            do
            {
                for (k = 0; k < mStations.size(); k++)
                    if (mStations[k].distance(Point(i, j)) < r)
                        break;
                if (k == mStations.size())
                    r += 2;
            } while (k < mStations.size());
            r = k / mDistrictStationsQuantity;
            if((r / mDistrictQuantity.getY()) % 2 == 0)
            {
                if(r % 2 == 0)
                    mColors[n + j / step] = Color::LavenderBlush;
                else
                    mColors[n + j / step] = Color::Lavender;
            }
            else
            {
                if(r % 2 == 0)
                    mColors[n + j / step] = Color::Honeydew;
                else
                    mColors[n + j / step] = Color::MistyRose;
            }
        }
    }
}

bool Display::isEarlier(Point &aFirst, Point &aLast) const
{
    if (aFirst.getX() == aLast.getX())
    {
        if (aFirst.getY() <= aLast.getY())
            return true;
        else
            return false;
    }
    else
    {
        if (aFirst.getX() < aLast.getX())
            return true;
        else
            return false;
    }
}

int Map::getMinutes(Point aFirst, Point aLast)
{
    int minutes = (aLast.getX() - aFirst.getX()) * cMinutesInHour + aLast.getY() - aFirst.getY();
    if (minutes < 0)
        minutes += cMinutesInHour * cHoursInDay;
    return minutes;
}

Point Map::findTrainPosition(Train &aTrain) const
{
    trainXY = Point(-1,-1);
    //пошук на проміжних станціях
    for(int j = 1; j < train.stationQuantity - 1; j++)
        if(isEarlier(train.station[j].arrive,time) && IsEarlier(time, train.station[j].depart))
        {
            trainXY = Point(mStations[train.station[j].number]);
            break;
        }
    //пошук між станціями
    if(trainXY.getX() == -1 )
        for(int j = 1; j < train.stationQuantity; j++)
            if(IsEarlier(time,train.station[j].arrive) && IsEarlier(train.station[j - 1].depart, time)){
                int trainMinutes = GetMinutes(train.station[j - 1].depart,time);
                int stationMinutes = GetMinutes(train.station[j - 1].depart,train.station[j].arrive);
                float relation = (float)trainMinutes / stationMinutes;
                trainXY = Point(mStations[train.station[j - 1].number].getX() + (int)((mStations[train.station[j].number].getX() - mStations[train.station[j - 1].number].getX()) * relation),
                                mStations[train.station[j - 1].number].getY() + (int)((mStations[train.station[j].number].getY() - mStations[train.station[j - 1].number].getY()) * relation));
            }
    //пошук на кінцевих станціях
    if(trainXY.getX() == -1 )
    {
        int minutes = GetMinutes(train.station[train.stationQuantity - 1].arrive,train.station[0].depart);
        if (minutes < 120)
            minutes /= 2;
        else
            minutes = cMinutesInHour;
        cPoint firstPoint = train.station[0].depart;
        firstPoint.getY() -= minutes % cMinutesInHour;
        if (firstPoint.getY() < 0)
        {
            firstPoint.getY() += cMinutesInHour;
            firstPoint.getX()--;
        }
        if (firstPoint.getX() < 0)
            firstPoint.getX() += cHoursInDay;
        cPoint secondPoint = train.station[train.stationQuantity - 1].arrive;
        secondPoint.getY() += minutes % cMinutesInHour;
        if (secondPoint.getY() >= cMinutesInHour)
        {
            secondPoint.getY() -= cMinutesInHour;
            secondPoint.getX()++;
        }
        if (secondPoint.getX() >= cHoursInDay)
            secondPoint.getX() -= cHoursInDay;
        if (isEarlier(time,firstPoint) && GetMinutes(time,firstPoint) <= cMinutesInHour)
            trainXY = Point(mStations[train.station[0].number]);
        else
            if (isEarlier(secondPoint,time) && GetMinutes(secondPoint,time) <= cMinutesInHour)
                trainXY = Point(mStations[train.station[train.stationQuantity - 1].number]);

    }
}
