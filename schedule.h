#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "TimePoint.h"

class Schedule
{
public:
    Schedule();
    Schedule(int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes, int aNumber);

    TimePoint getArrive() const;
    TimePoint getDepart() const;
    int getNumber() const;

private:
    TimePoint mArrive;
    int mWait;
    TimePoint mDepart;
    int mNumber;
};

#endif // SCHEDULE_H
