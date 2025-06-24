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
    Station(int aX, int aY, int aConnections, Status &aStatus, QString aName = "");
    Station(Point aPosition, int aConnections, Status &aStatus, QString aName = "");
    int getConnectionsSize() const;
    void incConnectionsSize();
    void setStatus(int s);
    int getStatus() const;
    QString getName() const;

private:
    Status mStatus;
    int mConnectionsSize;
    QString mName;
    QVector<Schedule> mTrains;

};

#endif // STATION_H
