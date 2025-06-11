#include "station.h"

Station::Station()
    : mStatus(Status::None)
    , mConnectionsSize(0)
    , mName("місто N")
{
}

Station::Station(int aX, int aY, Status &aStatus, int aN) : Point(aX, aY)
    , mStatus(aStatus)
    , mConnectionsSize(0)
    , mName("місто N")
{
    mTrains.reserve(aN);
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
