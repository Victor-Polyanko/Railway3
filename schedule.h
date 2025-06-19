#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "TimePoint.h"

struct Schedule
{
    Schedule();
    Schedule(int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes, int aNumber);

    TimePoint arrive;
    int wait;
    TimePoint depart;
    int number;
};

#endif // SCHEDULE_H
