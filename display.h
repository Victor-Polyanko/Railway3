#ifndef DISPLAY_H
#define DISPLAY_H

#include "Map.h"
#include "Point.h"

class Display
{
public:
    Display();

    void buildParts();
    //void showTrains(Graphics g, int aWidth, int aHeight, Point &aTime);
    //void show(Graphics g, int aWidth, int aHeight);
    //void quickShow(Graphics g, int aWidth, int aHeight);

private:
    Map mMap;
    //QVector<SolidBrush> mParts;
};

#endif // DISPLAY_H
