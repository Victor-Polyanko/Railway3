#ifndef TIMEPOINT_H
#define TIMEPOINT_H

#include "point.h"

class TimePoint : public Point
{
public:
    TimePoint();
    TimePoint(int aHours, int aMinutes);

    void addMinutes(int minutes);
    void substractMinutes(int minutes);
    bool isEarlierThan(const TimePoint &aTime) const;
    int getMinutesTo(const TimePoint &aTime) const;
    static bool isLessThanHour(int minutes);
    static void correctMinutes(int &minutes);

    TimePoint operator+(const int &aMinutes) const;
};

#endif // TIMEPOINT_H
