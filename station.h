#ifndef STATION_H
#define STATION_H

#include "Point.h"
#include "Schedule.h"

#include <qstring.h>
#include <qvector.h>

class Station : public Point
{
public:
    enum Status {None, Town, City, Capital};

public:
    Station();
    Station(int aX, int aY, Status &aStatus, QString aName = "");
    Station(Point aPosition, Status &aStatus, QString aName = "");
    QList<int> getConnections() const;
    void addConnection(int aStationId);
    void delConnection(int aStationId);
    void setStatus(int s);
    int getStatus() const;
    QString getName() const;

private:
    Status mStatus;
    QList<int> mConnectionStations;
    QString mName;
    QVector<Schedule> mTrains;

};

#endif // STATION_H
