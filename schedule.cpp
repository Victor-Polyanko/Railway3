#include "schedule.h"

Schedule::Schedule()
    : stationId(cNotSet)
    , wait(cNotSet)
{
}

Schedule::Schedule(int aStationId, int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes)
    : stationId(aStationId)
    , arrive(aArriveHours, aArriveMinutes)
    , wait(aWait)
    , depart(aDepartHours, aDepartMinutes)
{}
