#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point();
    Point(int aX, int aY);
    int getX() const;
    int getY() const;
    int size() const;
    Point operator*(Point &p) const;
    Point operator/(Point &p) const;
    int distance(Point &p) const;

private:
    int mX;
    int mY;
};

#endif // POINT_H
