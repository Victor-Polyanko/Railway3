#include "timePoint.h"
#include "point.h"

const int cMinutesInHour = 60;
const int cHoursInDay = 24;

TimePoint::TimePoint() : Point()
{}

TimePoint::TimePoint(int aHours, int aMinutes) : Point(aHours, aMinutes) {}

TimePoint& TimePoint::operator+=(const TimePoint &aTime)
{
    mY += aTime.mY;
    if (mY >= cMinutesInHour)
    {
        mY -= cMinutesInHour;
        mX++;
    }
    mX += aTime.mX;
    if (mX >= cHoursInDay)
        mX -= cHoursInDay;
    return *this;
}

TimePoint& TimePoint::operator-=(const TimePoint &aTime)
{
    mY -= aTime.mY;
    if (mY < 0)
    {
        mY += cMinutesInHour;
        mX--;
    }
    mX -= aTime.mX;
    if (mX < 0)
        mX += cHoursInDay;
    return *this;
}

bool TimePoint::operator<(const TimePoint &aTime) const
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

QString TimePoint::showAsString() const
{
    auto toString = [](int i) { return (i < 10 ? "0" : "") + QString::number(i); };
    return toString(mX) + ":" + toString(mY);
}

TimePoint TimePoint::operator+(const TimePoint &aTime) const
{
    auto hours = mX + aTime.mX;
    auto minutes = mY + aTime.mY;
    if (minutes > cMinutesInHour)
    {
        minutes -= cMinutesInHour;
        ++hours;
    }
    if (hours > cHoursInDay)
        hours -= cHoursInDay;
    return TimePoint(hours, minutes);
}

TimePoint TimePoint::operator-(const TimePoint &aTime) const
{
    auto hours = mX - aTime.mX;
    auto minutes = mY - aTime.mY;
    if (minutes < 0)
    {
        minutes += cMinutesInHour;
        --hours;
    }
    if (hours < 0)
        hours += cHoursInDay;
    return TimePoint(hours, minutes);
}
