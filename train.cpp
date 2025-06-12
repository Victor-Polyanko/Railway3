#include "train.h"

const Point cDefaultTime(12, 0);

Train::Train(int N)
    : mNumber(0)
    , mStatus(Status::Fast)
    , mStartTime(cDefaultTime)
{
    mStations.resize(N);
}
