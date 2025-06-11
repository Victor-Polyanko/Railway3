#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "Point.h"

class Schedule
{
public:
    Schedule();

private:
    Point mArrive;
    int mWait;
    Point mDepart;
    int mNumber;
};

#endif // SCHEDULE_H
