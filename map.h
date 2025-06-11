#ifndef MAP_H
#define MAP_H

#include "Point.h"
#include "Station.h"
#include "Train.h"

#include <qvector.h>

class Map
{
public:
    Map();
    Map(int aXd, int aYd, int aXq, int aYq, int aQ);

    void newGame();
    int distance(int aI, int aJ) const;
    void buildParts();
    bool isEarlier(Point aFirst, Point aLast);
    int getMinutes(Point aFirst, Point aLast);
    void showTrains(Graphics g, int aWidth, int aHeight, Point aTime);
    void show(Graphics g, int aWidth, int aHeight);
    void quickShow(Graphics g, int aWidth, int aHeight);

private:
    void init(int aXd, int aYd, int aXq, int aYq, int aQ);
    void globalConnection(int aI0, int aI1);
    bool foundConnection(int aI, int aJ, int aK);
    bool madeConnection(int &i, int j, int &d);

private:
    Point mDimention;
    Point mDistrictQuantity;
    int mDistrictStationsQuantity;
    QVector<Station> mStations;
    QVector<Point> mWays;
    QVector<Train> mTrains;
    SolidBrush[] mPart;
    Point mTime;
};

#endif // MAP_H
