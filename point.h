#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point();
    Point(int aX, int aY);
    bool isSet() const;
    int getX() const;
    int getY() const;
    int size() const;
    Point operator*(const Point &p) const;
    Point operator/(const Point &p) const;
    int distance(const Point &p) const;

protected:
    int mX;
    int mY;
    bool mIsSet;
};

#endif // POINT_H
