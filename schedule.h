#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "TimePoint.h"

struct Schedule
{
    Schedule();
    Schedule(int aStationId, int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes);

    int stationId;
    TimePoint arrive;
    int wait;
    TimePoint depart;
};

#endif // SCHEDULE_H
