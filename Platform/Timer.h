#pragma once
#ifndef TIMER_H
#define TIMER_H

// A high-frequency, low-level timer
class Timer
{
public:
    Timer()
    {
        _tickrate = 0;
        _lastTime = 0;
    }
    // Start timing
    void        start();

    void        setTickRate( double tickrate );

    // Returns time passed since last update
    double      update();

private:
    double              _tickrate;
    unsigned __int64    _lastTime;
};

#endif