#ifndef TIMEPOINT_H
#define TIMEPOINT_H

#include "point.h"

#include <QString>

class TimePoint : public Point
{
public:
    TimePoint();
    TimePoint(int aHours, int aMinutes);

    bool isBetween(const TimePoint &aFirst, const TimePoint &aSecond) const;
    int getMinutesTo(const TimePoint &aTime) const;
    QString showAsString() const;

    TimePoint operator+(const TimePoint &aTime) const;
    TimePoint operator+(int aMinutes) const;
    TimePoint& operator+=(const TimePoint &aTime);
    TimePoint operator-(const TimePoint &aTime) const;
    TimePoint& operator-=(const TimePoint &aTime);
    bool operator<(const TimePoint &aTime) const;
};

#endif // TIMEPOINT_H
