#include "station.h"

const QString cDefaultName("Місто N");

Station::Station()
    : mStatus(Status::None)
    , mConnectionsSize(0)
    , mName(cDefaultName)
{
}

Station::Station(int aX, int aY, int aConnections, Status &aStatus, QString aName) : Point(aX, aY)
    , mStatus(aStatus)
    , mConnectionsSize(aConnections)
    , mName(aName.isEmpty() ? cDefaultName : aName)
{
}

int Station::getConnectionsSize() const
{
    return mConnectionsSize;
}

void Station::incConnectionsSize()
{
    ++mConnectionsSize;
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
