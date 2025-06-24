#include "map.h"
#include "Station.h"
#include <cstdlib>
#include <ctime>
#include <qiodevice>

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

void Map::init(int aXd, int aYd, int aXq, int aYq, int aDistrictStationsQuantity, int aWaysQuantity, int aTrainsQuantity)
{
    mDimention = Point(aXd, aYd);
    mDistrictQuantity = Point(aXq, aYq);
    mDistrictStationsQuantity = cDefaultDistrictStationsQuantity; // default is set intentionally
    auto stationsQuantity = aXq * aYq * mDistrictStationsQuantity;
    mStations.clear();
    mStations.resize(stationsQuantity);
    mWays.clear();
    mWays.resize(aWaysQuantity);
    mTrains.clear();
    mTrains.resize(aTrainsQuantity);
    mTime = Point( -1, -1);
    auto multiplier = mDimention.getX() / mDistrictStationsQuantity / 20;
    for (auto i = 0; i < 4; ++i)
        mStationRadius.emplace_back(i * multiplier);
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

QVector<Way> Map::getWays() const
{
    return mWays;
}

Point Map::getDimention() const
{
    return mDimention;
}

Point Map::getDistrictsQuantity() const
{
    return mDistrictQuantity;
}

int Map::getDistrictsStationsQuantity() const
{
    return mDistrictStationsQuantity;
}

int Map::getStationRadius(int aStatus) const
{
    return mStationRadius[aStatus];
}

void Map::load(QDataStream &aStream)
{
    int xDimention, yDimention, xQuantity, yQuantity, waysQuantity, trainsQuantity;
    aStream >> xDimention >> yDimention >> xQuantity >> yQuantity >> mDistrictStationsQuantity >> waysQuantity >> trainsQuantity;
    init(cDefaultXDimention, cDefaultYDimention, yQuantity, yQuantity, mDistrictStationsQuantity, waysQuantity, trainsQuantity);
    loadStations(aStream);
    loadWays(aStream);
    loadTrains(aStream);
    fillDistricts();
}

void Map::loadStations(QDataStream &aStream)
{
    for (auto &station : mStations)
    {
        int x, y, s, connections;
        aStream >> x >> y >> connections >> s;
        Station::Status status = static_cast<Station::Status>(s);
        quint8 length;
        aStream >> length;
        QString name;
        if (length)
        {
            QByteArray stringData(length, Qt::Uninitialized);
            aStream.readRawData(stringData.data(), length);
            name = QString::fromUtf8(stringData);
        }
        else
            aStream >> name;
        station = Station(x, y, connections, status, name);
    }
}

void Map::loadWays(QDataStream &aStream)
{
    for (auto &way : mWays)
    {
        int x, y;
        aStream >> x >> y;
        way = Way(x, y);
    }
}

void Map::loadTrains(QDataStream &aStream)
{
    for (auto &train : mTrains)
    {
        int number, hours, minutes, stations, t;
        aStream >> number >> hours >> minutes >> stations >> t;
        Train::Type type = static_cast<Train::Type>(t);
        train = Train(number, hours, minutes, stations, type);
        for (int i = 0; i < train.getStationsQuantity(); ++i)
        {
            int arriveHours, arriveMinutes, wait, departHours, departMinutes, number;
            aStream >> arriveHours >> arriveMinutes >> wait >> departHours >> departMinutes >> number;
            train.setStation(i, arriveHours, arriveMinutes, wait, departHours, departMinutes, number);
        }
    }
}

void Map::save(QDataStream &aStream) const
{
    saveHeader(aStream);
    saveStations(aStream);
    saveWays(aStream);
    saveTrains(aStream);
}

void Map::saveHeader(QDataStream &aStream) const
{
    aStream << mDimention.getX();
    aStream << mDimention.getY();
    aStream << mDistrictQuantity.getX();
    aStream << mDistrictQuantity.getY();
    aStream << mDistrictStationsQuantity;
    aStream << static_cast<int>(mWays.size());
    aStream << static_cast<int>(mTrains.size());
}

void Map::saveStations(QDataStream &aStream) const
{
    for (const auto &station : mStations)
    {
        aStream << station.getX();
        aStream << station.getY();
        aStream << station.getConnectionsSize();
        aStream << station.getStatus();
        quint8 compatibilityByte = 0;
        aStream << compatibilityByte;
        aStream << station.getName();
    }
}

void Map::saveWays(QDataStream &aStream) const
{
    for (const auto &way : mWays)
    {
        aStream << way.first;
        aStream << way.second;
    }
}

void Map::saveTrains(QDataStream &aStream) const
{
    for (const auto &train : mTrains)
    {
        aStream << train.getNumber();
        aStream << train.getStartTime().getX();
        aStream << train.getStartTime().getY();
        aStream << train.getStationsQuantity();
        aStream << train.getType();
        for (const auto &station : train.getStations())
        {
            aStream << station.arrive.getX();
            aStream << station.arrive.getY();
            aStream << station.wait;
            aStream << station.depart.getX();
            aStream << station.depart.getY();
            aStream << station.number;
        }
    }
}

void Map::generate()
{
    generateStations();
    buildWays();
    fillDistricts();
}

void Map::generateStations()
{
    Point districtSize = mDimention / mDistrictQuantity;
    srand(time(0));
    int stationsQuantity2 = mStations.size() * mStations.size();
    auto station = mStations.begin();
    auto defaultStatus = Station::Status::Town;
    auto centerStatus = Station::Status::City;
    auto border = mStationRadius[defaultStatus];
    auto centerBorder = mStationRadius[Station::Status::Capital];
    auto half = Point(districtSize.getX() / 2, districtSize.getY() / 2);
    auto shift = 2 * border + centerBorder;
    auto width = half.getX() - shift;
    auto height = half.getY() - shift;
    auto centerCalculation = districtSize.getX() + districtSize.getY() - 4 * centerBorder;
    for (int i = 0; i < mDistrictQuantity.getY(); i++)
    {
        auto top = i * districtSize.getY();
        auto topBorder = top + border;
        auto yMiddle = top + half.getY();
        auto yMiddleBorder = yMiddle + centerBorder;
        for (int j = 0; j < mDistrictQuantity.getX(); j++)
        {
            auto left = j * districtSize.getX();
            auto leftBorder = left + border;
            auto xMiddle = left + half.getX();
            auto xMiddleBorder = xMiddle + centerBorder;
            auto limit = districtSize.getX() - 2 * centerBorder;
            auto center = rand() % centerCalculation;
            if (center < limit)
                *station++ = Station(left + center + centerBorder, yMiddle, stationsQuantity2, centerStatus);
            else
                *station++ = Station(xMiddle, top + center - limit + centerBorder, stationsQuantity2, centerStatus);
            auto findXY = [&](int aLeft, int aTop) {
                return Point (aLeft + rand() % width, aTop + rand() % height);
            };
            auto position = findXY(leftBorder, topBorder);
            *station++ = Station(position, stationsQuantity2, defaultStatus);
            position = findXY(leftBorder, yMiddleBorder);
            *station++ = Station(position, stationsQuantity2, defaultStatus);
            position = findXY(xMiddleBorder, topBorder);
            *station++ = Station(position, stationsQuantity2, defaultStatus);
            position = findXY(xMiddleBorder, yMiddleBorder);
            *station++ = Station(position, stationsQuantity2, defaultStatus);
        }
    }
    mStations[mDistrictStationsQuantity].setStatus(static_cast<int>(Station::Status::Capital));
}

void Map::buildWays()
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
        mWays.push_back(Way(firstId, secondId));
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

void Map::fillDistricts()
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
    if (aTrain.getStations().size() > 2)
    for (auto station = aTrain.getStations().begin() + 1; station != aTrain.getStations().end() - 1; ++station)
        if (station->arrive.isEarlierThan(aTime) && aTime.isEarlierThan(station->depart))
        {
            trainXY = Point(mStations[station->number]);
            break;
        }
    //пошук між станціями
    if (trainXY.getX() == -1)
    {
        auto prevStation = aTrain.getStations().begin();
        for (auto station = prevStation + 1; station != aTrain.getStations().end(); ++station, prevStation = station)
            if (aTime.isEarlierThan(station->arrive) && prevStation->depart.isEarlierThan(aTime))
            {
                int trainMinutes = prevStation->depart.getMinutesTo(aTime);
                int stationMinutes = prevStation->depart.getMinutesTo(station->arrive);
                float relation = (float)trainMinutes / stationMinutes;
                trainXY = Point(mStations[prevStation->number].getX() + (int)((mStations[station->number].getX() - mStations[prevStation->number].getX()) * relation),
                                mStations[prevStation->number].getY() + (int)((mStations[station->number].getY() - mStations[prevStation->number].getY()) * relation));
            }
    }
    //пошук на кінцевих станціях
    if(trainXY.getX() == -1 )
    {
        auto firstStation = aTrain.getStations().front();
        auto secondStation = aTrain.getStations().back();
        int minutes = secondStation.arrive.getMinutesTo(firstStation.depart);
        TimePoint::correctMinutes(minutes);
        TimePoint firstTime = firstStation.depart;
        firstTime.substractMinutes(minutes);
        TimePoint secondTime = secondStation.arrive;
        secondTime.addMinutes(minutes);
        if (aTime.isEarlierThan(firstTime) && TimePoint::isLessThanHour(aTime.getMinutesTo(firstTime)))
            trainXY = Point(mStations[firstStation.number]);
        else if (secondTime.isEarlierThan(aTime) && TimePoint::isLessThanHour(secondTime.getMinutesTo(aTime)))
            trainXY = Point(mStations[secondStation.number]);
    }
    return trainXY;
}
