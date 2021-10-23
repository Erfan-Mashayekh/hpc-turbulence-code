#ifndef _MSC_VER

#ifndef __SIMPLE_TIMER_HPP__
#define __SIMPLE_TIMER_HPP__

#include "Definitions.hpp"

#include <sys/time.h>

namespace NSEOF {

/** Timer. Used for scaling measurements.
 */
class SimpleTimer {
private:
    struct timeval startingTime_;
    struct timeval currentTime_;

public:
    SimpleTimer() = default;
    ~SimpleTimer() = default;

    void start();

    FLOAT getTimeAndRestart();
    FLOAT getTimeAndContinue();
};

} // namespace NSEOF

#endif // __SIMPLE_TIMER_HPP__

#endif // _MSC_VER
