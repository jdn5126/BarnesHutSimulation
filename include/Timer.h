/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Timer.h
 */

#ifndef _TIMER_DEFINED
#define _TIMER_DEFINED

#include <iostream>
#include <chrono>

typedef std::chrono::system_clock::time_point time_point_t;
typedef std::chrono::microseconds microseconds_t;

class Timer {

public:
    time_point_t execStart;
    time_point_t execStop;
    microseconds_t execDuration;

    void start();
    void stop();
    long long duration();

    friend std::ostream& operator<<(std::ostream& out, const Timer& t);

};

#endif // _TIMER_DEFINED
