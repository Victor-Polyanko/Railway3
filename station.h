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
    QList<int> getTrainsNumbers() const;
    void addTrainNumber(int aTrainId);
    void delTrainNumber(int aTrainId);

private:
    Status mStatus;
    QList<int> mConnectionStations;
    QString mName;
    QList<int> mTrainsNumbers;

};

#endif // STATION_H
