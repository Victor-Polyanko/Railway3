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
    Display();

    void generate();
    QString load(const QString &aFileName);
    QString save(const QString &aFileName);

    void buildParts();
    void calculateScale(QPainter &aPainter);
    void showTrains(QPainter &aPainter, const TimePoint &aTime) const;
    void showDistricts(QPainter &aPainter) const;
    void showStationsAndWays(QPainter &aPainter) const;
    void updateLastTime(const TimePoint &aTime);
    QStringList getAllNames() const;
    QStringList getNamesForStation(int aStationId) const;
    int getStationIdForConnection(int aStationId, int aConnectionId) const;
    QString getStationName(int aStationId) const;
    Point getStationPosition(int aStationId) const;
    int getStationStatus(int aStationId) const;
    QList<Train> getTrains() const;
    void addTrain(const Train &aTrain);
    void delTrain(int aTrainId);
    QList<Way> getWays() const;
    void addWay(const Way &aWay);
    void delWay(const Way &aWay);

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
    Scale mScale;
};

#endif // DISPLAY_H
