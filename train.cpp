#include "train.h"

const TimePoint cDefaultTime(12, 0);

Train::Train() {}

Train::Train(int N)
    : mNumber(0)
    , mType(Type::None)
    , mStartTime(cDefaultTime)
{
    mStations.resize(N);
}

Train::Train(int aNumber, int aHours, int aMinutes, int aStations, Type aType)
    : mNumber(aNumber)
    , mType(aType)
    , mStartTime(TimePoint(aHours, aMinutes))
{
    mStations.resize(aStations);
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

void Train::setStation(int aI, int aArriveHours, int aArriveMinutes,
                       int aWait, int aDepartHours, int aDepartMinutes, int aNumber)
{
    mStations[aI] = Schedule(aArriveHours, aArriveMinutes, aWait,
                             aDepartHours, aDepartMinutes, aNumber);
}
