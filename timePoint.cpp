#include "timePoint.h"

const int cMinutesInHour = 60;
const int cHoursInDay = 24;

TimePoint::TimePoint()
{
    mX = -1;
    mY = -1;
}

void TimePoint::addMinutes(int minutes)
{
    mY += minutes % cMinutesInHour;
    if (mY >= cMinutesInHour)
    {
        mY -= cMinutesInHour;
        mX++;
    }
    if (mX >= cHoursInDay)
        mX -= cHoursInDay;
}

void TimePoint::substractMinutes(int minutes)
{
    mY -= minutes % cMinutesInHour;
    if (mY < 0)
    {
        mY += cMinutesInHour;
        mX--;
    }
    if (mX < 0)
        mX += cHoursInDay;
}

bool TimePoint::isEarlierThan(const TimePoint &aTime) const
{
    if (mX == aTime.getX())
    {
        if (mY <= aTime.getY())
            return true;
        else
            return false;
    }
    else
    {
        if (mX < aTime.getX())
            return true;
        else
            return false;
    }
}

int TimePoint::getMinutesTo(const TimePoint &aTime) const
{
    int minutes = (aTime.getX() - mX) * cMinutesInHour + aTime.getY() - mY;
    if (minutes < 0)
        minutes += cMinutesInHour * cHoursInDay;
    return minutes;
}

bool TimePoint::isLessThanHour(int aMinutes)
{
    return aMinutes < cMinutesInHour;
}

void TimePoint::correctMinutes(int &aMinutes)
{
    if (aMinutes < cMinutesInHour * 2)
        aMinutes /= 2;
    else
        aMinutes = cMinutesInHour;
}
