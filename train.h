#ifndef TRAIN_H
#define TRAIN_H

#include "Point.h"
#include "Schedule.h"

#include <qvector.h>

class Train
{
public:
    enum Status {Fast, Passenger, Local};

public:
    Train();
    Train(int N);

private:
    int mNumber;
    Status mStatus;
    Point mStartTime;
    int mStationQuantity;
    QVector<Schedule> mStations;
};

#endif // TRAIN_H
