#ifndef DISPLAY_H
#define DISPLAY_H

#include "Map.h"
#include "Point.h"


#include <QBrush>
#include <QPainter>

class Display
{
public:
    Display();

    void generate();
    void buildParts();
    //void showTrains(Graphics g, int aWidth, int aHeight, Point &aTime);
    void showDistricts(QPainter &aPainter) const;
    void showStationsAndWays(QPainter &aPainter) const;
    Map mMap;
private:
    //Map mMap;
    QVector<QBrush> mParts;
};

#endif // DISPLAY_H
