#ifndef MAP_H
#define MAP_H

#include "TimePoint.h"
#include "Station.h"
#include "Train.h"

#include <qvector.h>
#include <QSharedPointer>

class Map
{
public:
    enum Color {LavenderBlush, Lavender, Honeydew, MistyRose};
public:
    Map();
    Map(int aXd, int aYd, int aXq, int aYq, int aQ);

    QVector<Color> getColors() const;
    QVector<Station> getStations() const;
    QVector<Train> getTrains() const;
    QVector<Point> getWays() const;
    Point getDimention() const;
    int getDistrictsQuantity() const;
    void generate();
    Point findTrainPosition(const Train &aTrain, const TimePoint &aTime) const;

private:
    void init(int aXd, int aYd, int aXq, int aYq, int aQ);
    void generateStations();
    void globalConnection();
    void updateStationsStatuses();
    void fillColors();

private:
    Point mDimention;
    Point mDistrictQuantity;
    int mDistrictStationsQuantity;
    QVector<Station> mStations;
    QVector<Point> mWays;
    QVector<Train> mTrains;
    QVector<Color> mColors;
    Point mTime;
};

#endif // MAP_H
