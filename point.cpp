#include "point.h"
#include <cmath>

Point::Point()
    : mX(0)
    , mY(0)
{
}

Point::Point(int aX, int aY)
    : mX(aX)
    , mY(aY)
{
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

Point Point::operator*(Point &p) const
{
    return Point(mX * p.mX, mY * p.mY);
}

Point Point::operator/(Point &p) const
{
    return Point(mX / p.mX, mY / p.mY);
}

int Point::distance(Point &p) const
{
    int dx = mX - p.mX;
    int dy = mY - p.mY;
    return static_cast<int>(round(sqrt(dx * dx + dy * dy)));
}
