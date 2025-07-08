#ifndef MAP_H
#define MAP_H

#include "TimePoint.h"
#include "Station.h"
#include "Train.h"

#include <QDataStream>
#include <qvector.h>

typedef QPair<int, int> Way;

class Map
{
public:
    enum Color {LavenderBlush, Lavender, Honeydew, MistyRose};
public:
    Map();
    void generate();
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
    QStringList getAllNames() const;
    QStringList getNamesForStation(int aStationId) const;
    Point findTrainPosition(const Train &aTrain, const TimePoint &aTime) const;
    void addTrain(const Train &aTrain);
    void delTrain(int aTrainId);
    void setTrainTime(int aTrainId, TimePoint aTime);
    void addWay(Way aWay);
    void delWay(Way aWay);

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
    void buildWays();
    QVector<QVector<QPair<int, int>>> findAllDistances() const;
    Way findMinWay(QVector<QVector<QPair<int, int>>> &aDistances, QVector<int> &aGroups, QVector<int> &aNearestStationId) const;
    void ConnectAlonesInDistricts();
    void collectAllNames();
    void fillDistricts();

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
};

#endif // MAP_H
