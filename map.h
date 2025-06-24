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
    QVector<Train> getTrains() const;
    QVector<Way> getWays() const;
    Point getDimention() const;
    Point getDistrictsQuantity() const;
    int getDistrictsStationsQuantity() const;
    int getStationRadius(int aStatus) const;
    Point findTrainPosition(const Train &aTrain, const TimePoint &aTime) const;

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
    void fillDistricts();

private:
    Point mDimention;
    Point mDistrictQuantity;
    int mDistrictStationsQuantity;
    QVector<Station> mStations;
    QVector<Way> mWays;
    QVector<Train> mTrains;
    QVector<Color> mColors;
    Point mTime;
    QVector<int> mStationRadius;
};

#endif // MAP_H
