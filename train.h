#ifndef TRAIN_H
#define TRAIN_H

#include "Point.h"
#include "Schedule.h"

#include <qvector.h>

class Train
{
public:
    enum Type {None, Fast, Passenger, Local};

public:
    Train();
    Train(int N);
    int getStationsQuantity() const;
    Schedule getStation(int i) const;

private:
    int mNumber;
    Type mType;
    Point mStartTime;
    QVector<Schedule> mStations;
};

#endif // TRAIN_H
