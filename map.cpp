#include "map.h"
#include <cstdlib>
#include <ctime>

const int cDefaultXDimention = 300;
const int cDefaultYDimention = 300;
const int cDefaultXQuantity = 3;
const int cDefaultYQuantity = 3;
const int cDefaultDistrictStationsQuantity = 5;

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
    mWays.reserve(stationsQuantity * stationsQuantity);
    mTrains.reserve(stationsQuantity * stationsQuantity);
    mTime = Point( -1, -1);
}

QVector<Map::Color> Map::getColors() const
{
    return mColors;
}

QVector<Station> Map::getStations() const
{
    return mStations;
}

QVector<Train> Map::getTrains() const
{
    return mTrains;
}

QVector<Point> Map::getWays() const
{
    return mWays;
}

Point Map::getDimention() const
{
    return mDimention;
}

int Map::getDistrictsQuantity() const
{
    return mDistrictStationsQuantity;
}

void Map::generate()
{
    generateStations();
    //Створюємо глобальну мережу сполучень
    globalConnection();
    updateStationsStatuses();
    fillColors();
}

void Map::generateStations()
{
    Point districtSize = mDimention / mDistrictQuantity;
    //srand(time(0));
    int stationsQuantity2 = mStations.size() * mStations.size();
    for (int i = 0; i < mDistrictQuantity.getX(); i++)
        for (int j = 0; j < mDistrictQuantity.getY(); j++)
        {
            for (int k = 0; k < mDistrictStationsQuantity; k++)
            {
                auto index = (i * mDistrictQuantity.getY() + j) * mDistrictStationsQuantity + k;
                Point randomPoint(rand() % (districtSize.getX() - 10), rand() % (districtSize.getY() - 10));
                int x = i * districtSize.getX() + randomPoint.getX() + 5;
                int y = j * districtSize.getY() + randomPoint.getY() + 5;
                Station::Status s = Station::Status::Town;
                mStations[index] = Station(x, y, s, stationsQuantity2);
            }
        }
}

void Map::globalConnection()
{
    QVector<QVector<QPair<int, int>>> distances;
    distances.reserve(mStations.size() - 1);
    QVector<QVector<QPair<int, int>>::Iterator> minDistances;
    minDistances.reserve(mStations.size() - 1);
    for (auto station = mStations.begin(); station != mStations.end() - 1; ++station)
    {
        int i = distances.size() + 1;
        QVector<QPair<int, int>> currentDistances;
        currentDistances.reserve(mStations.size() - i);
        for (auto anotherStation = station + 1; anotherStation != mStations.end(); ++anotherStation)
            currentDistances.push_back(QPair<int, int>(station->distance(*anotherStation), i++));
        std::sort(currentDistances.begin(), currentDistances.end(),
                  [](const QPair<int, int>& a, const QPair<int, int>& b) {
                      return a.first < b.first;
                  });
        minDistances.push_back(currentDistances.begin());
        distances.push_back(currentDistances);
    }

    QVector<int> groups(mStations.size(), 0);
    int newGroup = 1;
    mWays.clear();
    mWays.reserve(distances.size());
    while (mWays.size() < distances.size())
    {
        auto currDistance = minDistances.begin();
        for (auto dist = currDistance + 1; dist != minDistances.end(); ++dist)
            if ((*currDistance)->first > (*dist)->first)
            {
                currDistance = dist;
            }
        auto firstId = currDistance - minDistances.begin();
        auto secondId = (*currDistance)->second;
        mWays.push_back(Point(firstId, secondId));
        if (groups[firstId] == 0)
            ++minDistances[firstId];
        if (groups[secondId] == 0)
            ++minDistances[secondId];
        if (groups[firstId] == 0 && groups[secondId] == 0)
        {
            groups[firstId] = newGroup;
            groups[secondId] = newGroup;
            ++newGroup;
        }
        else if (groups[firstId] == 0)
            groups[firstId] = groups[secondId];
        else if (groups[secondId] == 0)
            groups[secondId] = groups[firstId];
        else
        {
            for (auto &g : groups)
                if (g == groups[firstId])
                    g = groups[secondId];
        }
    }
}

void Map::updateStationsStatuses()
{
    auto halfPoint = Point(2, 2);
    for (int i = 0; i < mDistrictQuantity.getX(); i++)
        for (int j = 0; j < mDistrictQuantity.getY(); j++)
        {
            Point p = Point(2 * i + 1, 2 * j + 1) * mDimention / mDistrictQuantity / halfPoint;
            int n0 = (i * mDistrictQuantity.getY() + j) * mDistrictStationsQuantity;
            int n = n0;
            for (int k = 1; k < mDistrictStationsQuantity; k++)
                if ((mStations[n0 + k].getConnectionsSize() > mStations[n].getConnectionsSize()) ||
                    ((mStations[n0 + k].getConnectionsSize() == mStations[n].getConnectionsSize())
                        && (p.distance(mStations[n0 + k]) < p.distance(mStations[n]))))
                {
                    n = n0 + k;
                }
            mStations[n].setStatus((mDistrictQuantity.getX() / 2 == i && mDistrictQuantity.getY() / 2 == j) ? 3 : 2);
        }
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
            } while (k == mStations.size());
            r = k / mDistrictStationsQuantity;
            if ((r / mDistrictQuantity.getY()) % 2 == 0)
            {
                if (r % 2 == 0)
                    mColors[n + j / step] = Color::LavenderBlush;
                else
                    mColors[n + j / step] = Color::Lavender;
            }
            else
            {
                if (r % 2 == 0)
                    mColors[n + j / step] = Color::Honeydew;
                else
                    mColors[n + j / step] = Color::MistyRose;
            }
        }
    }
}

Point Map::findTrainPosition(const Train &aTrain, const TimePoint &aTime) const
{
    auto trainXY = Point(-1,-1);
    //пошук на проміжних станціях
    for (int j = 1; j < aTrain.getStationsQuantity() - 1; j++)
        if (aTrain.getStation(j).getArrive().isEarlierThan(aTime) && aTime.isEarlierThan(aTrain.getStation(j).getDepart()))
        {
            trainXY = Point(mStations[aTrain.getStation(j).getNumber()]);
            break;
        }
    //пошук між станціями
    if (trainXY.getX() == -1)
        for (int j = 1; j < aTrain.getStationsQuantity(); j++)
            if (aTime.isEarlierThan(aTrain.getStation(j).getArrive()) && aTrain.getStation(j - 1).getDepart().isEarlierThan(aTime))
            {
                int trainMinutes = aTrain.getStation(j - 1).getDepart().getMinutesTo(aTime);
                int stationMinutes = aTrain.getStation(j - 1).getDepart().getMinutesTo(aTrain.getStation(j).getArrive());
                float relation = (float)trainMinutes / stationMinutes;
                trainXY = Point(mStations[aTrain.getStation(j - 1).getNumber()].getX() + (int)((mStations[aTrain.getStation(j).getNumber()].getX() - mStations[aTrain.getStation(j - 1).getNumber()].getX()) * relation),
                                mStations[aTrain.getStation(j - 1).getNumber()].getY() + (int)((mStations[aTrain.getStation(j).getNumber()].getY() - mStations[aTrain.getStation(j - 1).getNumber()].getY()) * relation));
            }
    //пошук на кінцевих станціях
    if(trainXY.getX() == -1 )
    {
        int minutes = aTrain.getStation(aTrain.getStationsQuantity() - 1).getArrive().getMinutesTo(aTrain.getStation(0).getDepart());
        TimePoint::correctMinutes(minutes);
        TimePoint firstTime = aTrain.getStation(0).getDepart();
        firstTime.substractMinutes(minutes);
        TimePoint secondTime = aTrain.getStation(aTrain.getStationsQuantity() - 1).getArrive();
        secondTime.addMinutes(minutes);
        if (aTime.isEarlierThan(firstTime) && TimePoint::isLessThanHour(aTime.getMinutesTo(firstTime)))
            trainXY = Point(mStations[aTrain.getStation(0).getNumber()]);
        else if (secondTime.isEarlierThan(aTime) && TimePoint::isLessThanHour(secondTime.getMinutesTo(aTime)))
            trainXY = Point(mStations[aTrain.getStation(aTrain.getStationsQuantity() - 1).getNumber()]);
    }
    return trainXY;
}
