/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Timer.cpp
 */

#include "Timer.h"

void
Timer::start() {
    this->execStart = std::chrono::system_clock::now();
}

void
Timer::stop() {
    this->execStop = std::chrono::system_clock::now();
    this->execDuration = std::chrono::duration_cast<microseconds_t>(
            this->execStop - this->execStart);
}

long long
Timer::duration() {
    return execDuration.count();
}

std::ostream& operator<<(std::ostream& out, const Timer& t) {
    out << "Timer: " << t.execDuration.count() << " microseconds" << std::endl;
    return out;
}
