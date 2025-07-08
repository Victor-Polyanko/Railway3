#include "train.h"

const TimePoint cDefaultTime(12, 0);

Train::Train() {}

Train::Train(int N)
    : mNumber(0)
    , mType(Type::Local)
    , mStartTime(cDefaultTime)
{
    mStations.resize(N);
}

Train::Train(int aNumber, Type aType, int aHours, int aMinutes, int aStations)
    : mNumber(aNumber)
    , mType(aType)
    , mStartTime(TimePoint(aHours, aMinutes))
{
    mStations.reserve(aStations);
}

int Train::getNumber() const
{
    return mNumber;
}

Train::Type Train::getType() const
{
    return mType;
}

TimePoint Train::getStartTime() const
{
    return mStartTime;
}

void Train::setStartTime(TimePoint aTime)
{
    mStartTime = aTime;
}

int Train::getStationsQuantity() const
{
    return mStations.size();
}

QVector<Schedule> Train::getStations() const
{
    return mStations;
}

void Train::addStation(const Schedule &aSchedule)
{
    mStations.emplace_back(aSchedule);
}

void Train::addStation(int aStationId, const TimePoint &aArrive, int aWait, const TimePoint &aDepart)
{
    mStations.emplace_back(Schedule(aStationId, aArrive.getX(), aArrive.getY(),
                                    aWait, aDepart.getX(), aDepart.getY()));
}

void Train::addStation(int aStationId, int aArriveHours, int aArriveMinutes,
                       int aWait, int aDepartHours, int aDepartMinutes)
{
    mStations.emplace_back(Schedule(aStationId, aArriveHours, aArriveMinutes,
                                    aWait, aDepartHours, aDepartMinutes));
}
