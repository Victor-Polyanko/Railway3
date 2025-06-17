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

    QVector<Color> getColors() const;
    QVector<Station> getStations() const;
    QVector<Train> getTrains() const;
    QVector<Way> getWays() const;
    Point getDimention() const;
    Point getDistrictsQuantity() const;
    int getDistrictsStationsQuantity() const;
    Point findTrainPosition(const Train &aTrain, const TimePoint &aTime) const;

private:
    void init(int aXd, int aYd, int aXq, int aYq, int aQ);
    void loadStations(QDataStream &aStream, int aQuantity);
    void loadWays(QDataStream &aStream, int aQuantity);
    void loadTrains(QDataStream &aStream, int aQuantity);
    void generateStations();
    void globalConnection();
    void updateStationsStatuses();
    void fillColors();

private:
    Point mDimention;
    Point mDistrictQuantity;
    int mDistrictStationsQuantity;
    QVector<Station> mStations;
    QVector<Way> mWays;
    QVector<Train> mTrains;
    QVector<Color> mColors;
    Point mTime;
};

#endif // MAP_H
