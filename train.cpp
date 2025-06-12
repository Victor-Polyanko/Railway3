#include "train.h"

const Point cDefaultTime(12, 0);

Train::Train() {}

Train::Train(int N)
    : mNumber(0)
    , mType(Type::None)
    , mStartTime(cDefaultTime)
{
    mStations.resize(N);
}

int Train::getStationsQuantity() const
{
    return mStations.size();
}

Schedule Train::getStation(int i) const
{
    return mStations[i];
}
