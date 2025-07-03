#include "station.h"

const QString cDefaultName("Місто N");

Station::Station()
    : mStatus(Status::None)
    , mName(cDefaultName)
{
}

Station::Station(int aX, int aY, Status &aStatus, QString aName) : Point(aX, aY)
    , mStatus(aStatus)
    , mName(aName.isEmpty() ? cDefaultName : aName)
{
}

Station::Station(Point aPosition, Status &aStatus, QString aName) : Point(aPosition)
    , mStatus(aStatus)
    , mName(aName.isEmpty() ? cDefaultName : aName)
{
}

QList<int> Station::getConnections() const
{
    return mConnectionStations;
}

void Station::addConnection(int aStationId)
{
    if (!mConnectionStations.contains(aStationId))
        mConnectionStations.emplace_back(aStationId);
}

void Station::delConnection(int aStationId)
{
    if (mConnectionStations.contains(aStationId))
        mConnectionStations.removeOne(aStationId);
}

void Station::setStatus(int s)
{
    mStatus = static_cast<Status>(s);
}

int Station::getStatus() const
{
    return mStatus;
}

QString Station::getName() const
{
    return mName;
}
