#ifndef DISPLAY_H
#define DISPLAY_H

//#define DEBUG_MODE //comment for release

#include "Map.h"
#include "TimePoint.h"

#include <QColor>
#include <QPainter>

typedef QPair<double, double> Scale;

class Display
{
public:
    Display(Map *aMap);

    void generate();
    QString load(const QString &aFileName);
    QString save(const QString &aFileName);

    void buildParts();
    void calculateScale(QPainter &aPainter);
    void showTrains(QPainter &aPainter, const TimePoint &aTime) const;
    void showDistricts(QPainter &aPainter) const;
    void showStationsAndWays(QPainter &aPainter) const;

private:
    void convertColors();

private:
    QVector<QColor> mColors;
    Scale mScale;
    Map *mMap;
};

#endif // DISPLAY_H
