#ifndef MAP_H
#define MAP_H

#include "TimePoint.h"
#include "Station.h"
#include "Train.h"

#include <qvector.h>

class Map
{
public:
    enum Color {LavenderBlush, Lavender, Honeydew, MistyRose};
public:
    Map();
    Map(int aXd, int aYd, int aXq, int aYq, int aQ);

    QVector<Color> getColors() const;
    QVector<Train> getTrains() const;
    QVector<Point> getWays() const;
    void newGame();
    int distance(int aI, int aJ) const;
    void fillColors();
    Point findTrainPosition(const Train &aTrain, const TimePoint &aTime) const;

private:
    void init(int aXd, int aYd, int aXq, int aYq, int aQ);
    void globalConnection(int aI0, int aI1);
    bool foundConnection(int aI, int aJ, int aK);
    bool madeConnection(int &i, int j, int &d);
    int getMinutes(const Point &aFirst, const Point &aLast) const;

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
