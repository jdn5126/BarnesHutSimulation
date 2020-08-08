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

    void start(void) {
        this->execStart = std::chrono::high_resolution_clock::now();
    }

    void stop(void) {
        this->execStop = std::chrono::high_resolution_clock::now();
        this->execDuration = std::chrono::duration_cast<microseconds_t>(this->execStop - this->execStart);
    }

    long long duration(void) {
        return execDuration.count();
    }

    friend std::ostream& operator<<(std::ostream& out, const Timer& t) {
        out << "Timer: " << t.execDuration.count() << " microseconds" << std::endl;
        return out;
    }

};

#endif // _TIMER_DEFINED
