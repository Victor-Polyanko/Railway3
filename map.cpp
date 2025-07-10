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

const QVector<QVector<QVector<QString>>> cNames = {
    {{"Луцьк", "Ковель", "Володимир", "Камінь-Каширський", "Ківерці"},
     {"Рівне", "Вараш", "Дубно", "Сарни", "Костопіль"},
     {"Житомир", "Коростень", "Новоград-Волинський", "Малин", "Бердичів"}},
    {{"Київ", "Буча", "Біла Церква", "Вишгород", "Боярка"},
     {"Київ", "Вишневе", "Фастів", "Бориспіль", "Обухів"},
     {"Київ", "Ірпінь", "Васильків", "Бровари", "Переяслав"}},
    {{"Чернігів", "Корюківка", "Ніжин", "Новгород-Сіверський", "Прилуки"},
     {"Суми", "Конотоп", "Ромни", "Шостка", "Охтирка"},
     {"Полтава", "Лубни", "Кременчук", "Миргород", "Горішні Плавні"}},
    {{"Львів", "Самбір", "Дрогобич","Шептицький","Стрий"},
     {"Тернопіль", "Бережани", "Чортків", "Кременець", "Збараж"},
     {"Хмельницький", "Нетішин", "Славута", "Шепетівка", "Кам'янець-Подільський"}},
    {{"Вінниця", "Жмеринка", "Могилів-Подільський", "Хмільник", "Гайсин"},
     {"Черкаси", "Звенигородка", "Умань", "Канів", "Золотоноша"},
     {"Кропивницький", "Знам'янка", "Долинська", "Світловодськ", "Олександрія"}},
    {{"Дніпро", "Кам'янське", "Кривий Ріг", "Павлоград", "Нікополь"},
     {"Харків", "Первомайський", "Лозова", "Чугуїв", "Ізюм"},
     {"Луганськ", "Лисичанськ", "Алчевськ", "Сіверськодонецьк", "Хрустальний"}},
    {{"Ужгород", "Мукачеве", "Берегове", "Хуст", "Виноградів"},
     {"Івано-Франківськ", "Калуш", "Надвірна", "Коломия", "Косів"},
     {"Чернівці", "Сторожинець", "Красноїльськ", "Новодністровськ", "Хотин"}},
    {{"Одеса", "Подільськ", "Ізмаїл", "Чорноморськ", "Білгород-Дністровський"},
     {"Миколаїв", "Первомайськ", "Південноукраїнськ", "Новий Буг", "Вознесенськ"},
     {"Сімферополь", "Євпаторія", "Севастополь", "Керч", "Ялта"}},
    {{"Херсон", "Берислав", "Скадовськ", "Нова Каховка", "Генічеськ"},
     {"Запоріжжя", "Василівка", "Мелітополь", "Пологи", "Бердянськ"},
     {"Донецьк", "Краматорськ", "Маріуполь", "Горлівка", "Макіївка"}}};

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
    mTime = Point();
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

int Map::getStationIdForConnection(int aStationId, int aConnectionId) const
{
    return mStations[aStationId].getConnections()[aConnectionId];
}

QString Map::getStationName(int aStationId) const
{
    return mStations[aStationId].getName();
}

Point Map::getStationPosition(int aStationId) const
{
    return mStations[aStationId];
}

int Map::getStationStatus(int aStationId) const
{
    return mStations[aStationId].getStatus();
}

QList<int> Map::getStationTrainsNumbers(int aStationId) const
{
    return mStations[aStationId].getTrainsNumbers();
}

QStringList Map::getAllNames() const
{
    return mAllNames;
}

QStringList Map::getNamesForStation(int aStationId) const
{
    QStringList names;
    for (auto const &id : mStations[aStationId].getConnections())
        names.emplace_back(mAllNames[id]);
    return names;
}

QString Map::getTrainName(const Train &aTrain) const
{
    auto stations = aTrain.getStations();
    if (stations.size() < 2)
        return "Не призначений";
    auto firstStation = getStationName(stations.front().stationId);
    auto lastStation = getStationName(stations.back().stationId);
    return firstStation + " - " + lastStation;
}

void Map::load(QDataStream &aStream)
{
    int xDimention, yDimention, xQuantity, yQuantity, waysQuantity, trainsQuantity;
    aStream >> xDimention >> yDimention >> xQuantity >> yQuantity >> mDistrictStationsQuantity >> waysQuantity >> trainsQuantity;
    init(cDefaultXDimention, cDefaultYDimention, yQuantity, yQuantity, mDistrictStationsQuantity, waysQuantity, trainsQuantity);
    loadStations(aStream);
    loadWays(aStream);
    loadTrains(aStream);
    collectAllNames();
    fillDistricts();
}

void Map::loadStations(QDataStream &aStream)
{
    for (auto &station : mStations)
    {
        int x, y, s, connections; // connections is obsolete
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
        station = Station(x, y, status, name);
    }
}

void Map::loadWays(QDataStream &aStream)
{
    for (auto &way : mWays)
    {
        int x, y;
        aStream >> x >> y;
        way = Way(x, y);
        mStations[x].addConnection(y);
        mStations[y].addConnection(x);
    }
}

void Map::loadTrains(QDataStream &aStream)
{
    for (auto &train : mTrains)
    {
        int number, hours, minutes, stations, t;
        aStream >> number >> hours >> minutes >> stations >> t;
        Train::Type type = static_cast<Train::Type>(t);
        train = Train(number, type, hours, minutes, stations);
        for (int i = 0; i < stations; ++i)
        {
            int arriveHours, arriveMinutes, wait, departHours, departMinutes, stationId;
            aStream >> arriveHours >> arriveMinutes >> wait >> departHours >> departMinutes >> stationId;
            train.addStation(stationId, arriveHours, arriveMinutes, wait, departHours, departMinutes);
            mStations[stationId].addTrainNumber(number);
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
        aStream << static_cast<int>(station.getConnections().size());
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
            aStream << station.stationId;
        }
    }
}

void Map::generate()
{
    generateStations();
    buildWays();
    collectAllNames();
    fillDistricts();
}

void Map::generateStations()
{
    Point districtSize = mDimention / mDistrictQuantity;
    srand(time(0));
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
    auto names = cNames.begin();
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
            auto name = (*names++)[rand() % 3].begin();
            if (center < limit)
                *station++ = Station(left + center + centerBorder, yMiddle, centerStatus, *name++);
            else
                *station++ = Station(xMiddle, top + center - limit + centerBorder, centerStatus, *name++);
            auto findXY = [&](int aLeft, int aTop) {
                return Point(aLeft + rand() % width, aTop + rand() % height);
            };
            auto position = findXY(leftBorder, topBorder);
            *station++ = Station(position, defaultStatus, *name++);
            position = findXY(leftBorder, yMiddleBorder);
            *station++ = Station(position, defaultStatus, *name++);
            position = findXY(xMiddleBorder, topBorder);
            *station++ = Station(position, defaultStatus, *name++);
            position = findXY(xMiddleBorder, yMiddleBorder);
            *station++ = Station(position, defaultStatus, *name++);
        }
    }
    mStations[mDistrictStationsQuantity].setStatus(static_cast<int>(Station::Status::Capital));
}

void Map::buildWays()
{
    auto distances = findAllDistances();
    auto waysSize = mStations.size() - 1;
    QVector<int> nearestStationId(waysSize, 0);
    QVector<int> groups(mStations.size(), 0);
    int newGroup = 1;
    mWays.clear();
    mWays.reserve(waysSize);
    while (mWays.size() < waysSize)
    {
        addWay(findMinWay(distances, groups, nearestStationId));
        int firstId = mWays.back().first;
        int secondId = mWays.back().second;
        nearestStationId[firstId]++;
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
            auto oldValue = groups[firstId];
            auto newValue = groups[secondId];
            for (auto &g : groups)
                if (g == oldValue)
                    g = newValue;
        }
    }
    ConnectAlonesInDistricts();
}

QVector<QVector<QPair<int, int>>> Map::findAllDistances() const
{
    auto waysSize = mStations.size() - 1;
    QVector<QVector<QPair<int, int>>> distances;
    distances.reserve(waysSize);
    for (auto station = mStations.begin(); station != mStations.end() - 1; ++station)
    {
        int i = distances.size() + 1;
        QVector<QPair<int, int>> currentDistances;
        currentDistances.reserve(waysSize);
        for (auto anotherStation = station + 1; anotherStation != mStations.end(); ++anotherStation)
            currentDistances.push_back(QPair<int, int>(station->distance(*anotherStation), i++));
        std::sort(currentDistances.begin(), currentDistances.end(),
                  [](const QPair<int, int>& a, const QPair<int, int>& b) {
                      return a.first < b.first;
                  });
        distances.push_back(currentDistances);
    }
    return distances;
}

Way Map::findMinWay(QVector<QVector<QPair<int, int>>> &aDistances, QVector<int> &aGroups, QVector<int> &aNearestStationId) const
{
    auto minId = 0;
    auto minDistance = std::numeric_limits<int>::max();
    auto nearestDistanceFor = [&](int id) { return aDistances[id][aNearestStationId[id]].first; };
    auto nearestIdFor = [&](int id) { return aDistances[id][aNearestStationId[id]].second; };
    for (auto id = minId; id < aNearestStationId.size(); ++id)
    {
        auto stillHaveWays = [&]() {return aNearestStationId[id] < aNearestStationId.size() - id;};
        auto stationsInTheSameGroup = [&]() {return aGroups[id] && aGroups[id] == aGroups[nearestIdFor(id)];};
        while (stillHaveWays() && stationsInTheSameGroup())
            ++aNearestStationId[id];
        if (stillHaveWays())
        {
            auto currDistance = nearestDistanceFor(id);
            if (minDistance > currDistance)
            {
                minDistance = currDistance;
                minId = id;
            }
        }
    }
    return Way(minId, nearestIdFor(minId));
}

void Map::ConnectAlonesInDistricts()
{
    auto allDistricts = mDistrictQuantity.getX() * mDistrictQuantity.getY();
    for (int districtId = 0; districtId < allDistricts; ++districtId)
    {
        int firstId = districtId * mDistrictStationsQuantity;
        int lastId = firstId + mDistrictStationsQuantity;
        for (int currentId = firstId; currentId < lastId; ++currentId)
        {
            bool isConnected = false;
            for (int i = firstId; i < currentId; ++i)
                if (mWays.contains(Way(i, currentId)))
                {
                    isConnected = true;
                    break;
                }
            if (isConnected)
                continue;
            for (int i = currentId + 1; i < lastId; ++i)
                if (mWays.contains(Way(currentId, i)))
                {
                    isConnected = true;
                    break;
                }
            if (isConnected)
                continue;
            int minId = 0;
            int minDistance = std::numeric_limits<int>::max();
            for (int id = firstId; id < lastId; ++id)
            {
                if (id == currentId)
                    continue;
                auto distance = mStations[id].distance(mStations[currentId]);
                if (minDistance > distance)
                {
                    minDistance = distance;
                    minId = id;
                }
            }
            addWay(Way(currentId, minId));
        }
    }
}

void Map::collectAllNames()
{
    mAllNames.clear();
    mAllNames.reserve(mStations.size());
    for (auto &station : mStations)
        mAllNames.emplace_back(station.getName());
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
    Point trainXY;
    //пошук на проміжних станціях
    if (aTrain.getStations().size() > 2)
    for (auto station = aTrain.getStations().begin() + 1; station != aTrain.getStations().end() - 1; ++station)
        if (station->arrive.isEarlierThan(aTime) && aTime.isEarlierThan(station->depart))
        {
            trainXY = Point(mStations[station->stationId]);
            break;
        }
    //пошук між станціями
    if (!trainXY.isSet())
    {
        auto prevStation = aTrain.getStations().begin();
        for (auto station = prevStation + 1; station != aTrain.getStations().end(); ++station, prevStation = station)
            if (aTime.isEarlierThan(station->arrive) && prevStation->depart.isEarlierThan(aTime))
            {
                int trainMinutes = prevStation->depart.getMinutesTo(aTime);
                int stationMinutes = prevStation->depart.getMinutesTo(station->arrive);
                float relation = (float)trainMinutes / stationMinutes;
                trainXY = Point(mStations[prevStation->stationId].getX() + (int)((mStations[station->stationId].getX() - mStations[prevStation->stationId].getX()) * relation),
                                mStations[prevStation->stationId].getY() + (int)((mStations[station->stationId].getY() - mStations[prevStation->stationId].getY()) * relation));
            }
    }
    //пошук на кінцевих станціях
    if (!trainXY.isSet())
    {
        auto firstStation = aTrain.getStations().front();
        auto secondStation = aTrain.getStations().back();
        int minutes = secondStation.arrive.getMinutesTo(firstStation.depart);
        TimePoint::correctMinutes(minutes);
        TimePoint firstTime = firstStation.depart;
        firstTime.substractTime(TimePoint(0, minutes));
        TimePoint secondTime = secondStation.arrive;
        secondTime.addTime(TimePoint(0, minutes));
        if (aTime.isEarlierThan(firstTime) && TimePoint::isLessThanHour(aTime.getMinutesTo(firstTime)))
            trainXY = Point(mStations[firstStation.stationId]);
        else if (secondTime.isEarlierThan(aTime) && TimePoint::isLessThanHour(secondTime.getMinutesTo(aTime)))
            trainXY = Point(mStations[secondStation.stationId]);
    }
    return trainXY;
}

void Map::addTrain(const Train &aTrain)
{
    mTrains.emplace_back(aTrain);
    for (const auto &station : aTrain.getStations())
        mStations[station.stationId].addTrainNumber(aTrain.getNumber());
}

void Map::delTrain(const Train &aTrain)
{
    mTrains.removeOne(aTrain);
    for (const auto &station : aTrain.getStations())
        mStations[station.stationId].delTrainNumber(aTrain.getNumber());
}

void Map::setTrainTime(int aTrainId, TimePoint aTime)
{
    mTrains[aTrainId].setStartTime(aTime);
}

void Map::addWay(Way aWay)
{
    if (aWay.first > aWay.second)
        qSwap(aWay.first, aWay.second);
    mWays.emplace_back(aWay);
    mStations[aWay.first].addConnection(aWay.second);
    mStations[aWay.second].addConnection(aWay.first);
}

void Map::delWay(Way aWay)
{
    if (aWay.first > aWay.second)
        qSwap(aWay.first, aWay.second);
    mWays.removeOne(aWay);
    mStations[aWay.first].delConnection(aWay.second);
    mStations[aWay.second].delConnection(aWay.first);
}
