#ifndef _MSC_VER

#include "SimpleTimer.hpp"

namespace NSEOF {

FLOAT timeDifference(const struct timeval & t1, const struct timeval & t2) {
    long seconds, useconds;
    seconds = t2.tv_sec - t1.tv_sec;
    useconds = t2.tv_usec - t1.tv_usec;
    return (FLOAT)seconds + useconds / 1.0e6;
}

void SimpleTimer::start() {
    gettimeofday(&startingTime_, NULL);
}

FLOAT SimpleTimer::getTimeAndRestart() {
    gettimeofday(&currentTime_, NULL);
    FLOAT returnValue = timeDifference(startingTime_, currentTime_);
    startingTime_ = currentTime_;
    return returnValue;
}

FLOAT SimpleTimer::getTimeAndContinue() {
    gettimeofday(&currentTime_, NULL);
    return timeDifference(startingTime_, currentTime_);
}

} // namespace NSEOF

#endif // _MSC_VER
