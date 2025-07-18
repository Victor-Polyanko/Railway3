#include "point.h"
#include <cmath>

Point::Point()
    : mX(cNotSet)
    , mY(cNotSet)
{
}

Point::Point(int aX, int aY)
    : mX(aX)
    , mY(aY)
{
}

bool Point::isSet() const
{
    return mX != cNotSet && mY != cNotSet;
}

int Point::getX() const
{
    return mX;
}

int Point::getY() const
{
    return mY;
}

int Point::size() const
{
    return mX * mY;
}

Point Point::operator*(const Point &p) const
{
    return Point(mX * p.mX, mY * p.mY);
}

Point Point::operator/(const Point &p) const
{
    return Point(mX / p.mX, mY / p.mY);
}

int Point::distance(const Point &p) const
{
    int dx = mX - p.mX;
    int dy = mY - p.mY;
    return static_cast<int>(round(sqrt(dx * dx + dy * dy)));
}
