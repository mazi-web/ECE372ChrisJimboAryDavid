// Author:         James Fulton
// Net ID:         23413392
// Date:           1 April 2022
// Assignment: ECE-372 Lab 4
//
// File: SevenSegment.h

#ifndef SevenSegment_h
#define SevenSegment_h
#include "timer.h"
#include "ShiftRegister.h"

class SevenSegment{
private:
    timer *timer_ms;
    ShiftRegister shiftreg1;

public:
    SevenSegment(timer *new_timer_s);
    SevenSegment();
    
    void init_SevenSegment(timer *new_timer_s);
    bool countDown_Seconds(unsigned int startTime);
    bool sendDigit(unsigned int number);
};

#endif