#include "schedule.h"

Schedule::Schedule()
    : mWait(-1)
    , mNumber(-1)
{
}

Schedule::Schedule(int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes, int aNumber)
    : mArrive(aArriveHours, aArriveMinutes)
    , mWait(aWait)
    , mDepart(aDepartHours, aDepartMinutes)
    , mNumber(aNumber)
{}

TimePoint Schedule::getArrive() const
{
    return mArrive;
}

TimePoint Schedule::getDepart() const
{
    return mDepart;
}

int Schedule::getNumber() const
{
    return mNumber;
}
