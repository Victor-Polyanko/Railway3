#ifndef DISPLAY_H
#define DISPLAY_H

#define DEBUG_MODE //comment for release

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
    QString save(const QString &aFileName);

    void buildParts();
    void showTrains(QPainter &aPainter, const TimePoint &aTime) const;
    void showDistricts(QPainter &aPainter) const;
    void showStationsAndWays(QPainter &aPainter) const;
    void updateLastTime(const TimePoint &aTime);

private:
    void convertColors();

#ifdef DEBUG_MODE
public:
#else
private:
#endif
    Map mMap;

private:
    QVector<QColor> mColors;
    TimePoint mLastTime;
};

#endif // DISPLAY_H
