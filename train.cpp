#include "train.h"

Train::Train(int N)
    : mNumber(0)
    , mStatus(Status::Fast)
    , mStartTime(Point(12, 0))
{
    mStations.reserve(N);
}
