#include "schedule.h"

Schedule::Schedule()
    : mWait(-1)
    , mNumber(-1)
{
}

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
