#ifndef TRAIN_H
#define TRAIN_H

#include "TimePoint.h"
#include "Schedule.h"

#include <qvector.h>

class Train
{
public:
    enum Type {Fast, Passenger, Local};

public:
    Train();
    Train(int N);
    Train(int aNumber, Type aType, int aHours, int aMinutes = 0, int aStations = 0);
    int getNumber() const;
    Type getType() const;
    TimePoint getStartTime() const;
    void setStartTime(TimePoint aTime);
    int getStationsQuantity() const;
    QVector<Schedule> getStations() const;
    void addStation(const Schedule &aSchedule);
    void addStation(int aStationId, const TimePoint &aArrive, int aWait, const TimePoint &aDepart);
    void addStation(int aStationId, int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes);

private:
    int mNumber;
    Type mType;
    TimePoint mStartTime;
    QVector<Schedule> mStations;
};

#endif // TRAIN_H
