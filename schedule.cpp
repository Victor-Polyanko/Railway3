#include "schedule.h"

Schedule::Schedule()
    : wait(-1)
    , number(-1)
{
}

Schedule::Schedule(int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes, int aNumber)
    : arrive(aArriveHours, aArriveMinutes)
    , wait(aWait)
    , depart(aDepartHours, aDepartMinutes)
    , number(aNumber)
{}
