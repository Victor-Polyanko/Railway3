#ifndef TRAIN_H
#define TRAIN_H

#include "TimePoint.h"
#include "Schedule.h"

#include <qvector.h>

class Train
{
public:
    enum Type {None, Fast, Passenger, Local};

public:
    Train();
    Train(int N);
    Train(int aNumber, int aHours, int aMinutes, int aStations, Type aType);
    int getNumber() const;
    Type getType() const;
    TimePoint getStartTime() const;
    int getStationsQuantity() const;
    QVector<Schedule> getStations() const;
    void setStation(int aI, int aArriveHours, int aArriveMinutes, int aWait, int aDepartHours, int aDepartMinutes, int aNumber);

private:
    int mNumber;
    Type mType;
    TimePoint mStartTime;
    QVector<Schedule> mStations;
};

#endif // TRAIN_H
