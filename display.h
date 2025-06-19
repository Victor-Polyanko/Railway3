#ifndef DISPLAY_H
#define DISPLAY_H

#include "Map.h"
#include "TimePoint.h"

#include <QColor>
#include <QPainter>

class Display
{
public:
    Display();

    void generate();
    QString load(const QString &aFileName);

    void buildParts();
    void showTrains(QPainter &aPainter, const TimePoint &aTime) const;
    void showDistricts(QPainter &aPainter) const;
    void showStationsAndWays(QPainter &aPainter) const;
    void updateLastTime(const TimePoint &aTime);

private:
    void convertColors();

private:
    Map mMap;
    QVector<QColor> mColors;
    TimePoint mLastTime;
};

#endif // DISPLAY_H
