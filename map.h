#ifndef MAP_H
#define MAP_H

//#define DEBUG_MODE //comment for release

#include "TimePoint.h"
#include "Station.h"
#include "Train.h"

#include <QDataStream>
#include <qvector.h>
#include <tuple>

typedef QPair<int, int> Way;
typedef QPair<float, float> Position;

class Map
{
public:
    enum Color {LavenderBlush, Lavender, Honeydew, MistyRose};
public:
    Map();
    void generate(bool areShortestWays, bool areAdditionalWays);
    void load(QDataStream &aStream);
    void save(QDataStream &aStream) const;

    QVector<Color> getColors() const;
    QVector<Station> getStations() const;
    QList<Train> getTrains() const;
    QList<Way> getWays() const;
    Point getDimention() const;
    Point getDistrictsQuantity() const;
    int getDistrictsStationsQuantity() const;
    int getStationRadius(int aStatus) const;
    int getStationIdForConnection(int aStationId, int aConnectionId) const;
    QString getStationName(int aStationId) const;
    Point getStationPosition(int aStationId) const;
    int getStationStatus(int aStationId) const;
    QList<int> getStationTrainsNumbers(int aStationId) const;
    QStringList getAllNames() const;
    QStringList getNamesForStation(int aStationId) const;
    QString getTrainName(const Train &aTrain) const;
    Position findTrainPosition(const Train &aTrain, const TimePoint &aTime) const;
    void addTrain(const Train &aTrain);
    void delTrain(const Train &aTrain);
    void setTrainTime(int aTrainId, TimePoint aTime);
    void addWay(Way aWay);
    void delWay(Way aWay);
#ifdef DEBUG_MODE
    QVector<QVector<size_t>> getAllDistances2();
#endif

private:
    void init(int aXd, int aYd, int aXq, int aYq, int aQ, int aWaysQuantity = 0, int aTrainsQuantity = 0);
    void loadStations(QDataStream &aStream);
    void loadWays(QDataStream &aStream);
    void loadTrains(QDataStream &aStream);
    void saveHeader(QDataStream &aStream) const;
    void saveStations(QDataStream &aStream) const;
    void saveWays(QDataStream &aStream) const;
    void saveTrains(QDataStream &aStream) const;
    void generateStations();
    void buildShortestWays(bool aAreAdditionalWays);
    void buildCentralizedWays(bool aAreAdditionalWays);
    QVector<QVector<QPair<int, int>>> findAllDistances() const;
    Way findMinWay(QVector<QVector<QPair<int, int>>> &aDistances, QVector<int> &aGroups, QVector<int> &aNearestStationId) const;
    void ConnectAlonesInDistricts();
    void connectInsideDistrincts();
    void connectOutsideDistricts(bool aAreAllConnected);
    void findAllDistances2();
    Way findMinWay(QVector<size_t> firstGroup, QVector<size_t> secondGroup);
    Way findMinWay(const QVector<Way> &aWays);
    void collectAllNames();
    void fillDistricts();
    Position findTrainAtMiddleStations(const Train &aTrain, const TimePoint &aTime) const;
    Position findTrainBetweenStations(const Train &aTrain, const TimePoint &aTime) const;
    Position findTrainAtEdgeStations(const Train &aTrain, const TimePoint &aTime) const;
    void fillTimeTable();
    void shiftAllTrains();
    void shiftTrains(Train::Type aType);

private:
    Point mDimention;
    Point mDistrictQuantity;
    int mDistrictStationsQuantity;
    QVector<Station> mStations;
    QList<Way> mWays;
    QList<Train> mTrains;
    QVector<Color> mColors;
    Point mTime;
    QVector<int> mStationRadius;
    QStringList mAllNames;
    QVector<QVector<QList<std::tuple<int, TimePoint, TimePoint>>>> mTimeTable;
    QVector<QVector<size_t>> mDistances2;
};

#endif // MAP_H
