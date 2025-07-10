#include "timePoint.h"
#include "point.h"

const int cMinutesInHour = 60;
const int cHoursInDay = 24;

TimePoint::TimePoint() : Point()
{}

TimePoint::TimePoint(int aHours, int aMinutes) : Point(aHours, aMinutes) {}

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

QString TimePoint::showAsString() const
{
    auto toString = [](int i) { return (i < 10 ? "0" : "") + QString::number(i); };
    return toString(mX) + ":" + toString(mY);
}

TimePoint TimePoint::operator+(const int &aMinutes) const
{
    auto hours = mX + (mY + aMinutes) / cMinutesInHour;
    auto minutes = (mY + aMinutes) % cMinutesInHour;
    if (hours >= cHoursInDay)
        hours = hours % cHoursInDay;
    return TimePoint(hours, minutes);
}
