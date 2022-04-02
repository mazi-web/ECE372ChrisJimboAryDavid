// Author:         James Fulton
// Net ID:         23413392
// Date:           1 April 2022
// Assignment: ECE-372 Lab 4
//
// File: seven_segment.h

#ifndef seven_segment_h
#define seven_segment_h
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

class seven_segment{
private:
    timer *timer_ms;

public:
    void init_SevenSegment(timer *new_timer_s);
    bool countDown_Seconds(unsigned int startTime);
    bool sendDigit(unsigned int number);
};

#endif