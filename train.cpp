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

int Train::getStationsQuantity() const
{
    return mStations.size();
}

QVector<Schedule> Train::getStations() const
{
    return mStations;
}

void Train::addStation(int aArriveHours, int aArriveMinutes,
                       int aWait, int aDepartHours, int aDepartMinutes, int aNumber)
{
    mStations.emplace_back(Schedule(aArriveHours, aArriveMinutes, aWait,
                             aDepartHours, aDepartMinutes, aNumber));
}
