// Author:         James Fulton
// Net ID:         23413392
// Date:           21 March 2022
// Assignment: ECE-372 Lab 3
//
// File: timer.h

#ifndef timer_h
#define timer_h
#include <avr/io.h>
#include <avr/interrupt.h>

class timer{
private:
    unsigned int timerPrefix;
    unsigned int timerNum;
    unsigned int mode;

public:
    // timerNum: timerX designation
    // mode: timer mode
    // delayPrescaler: multiplier designating timer prefix: 1->s, 2->ms, etc.
    // returns false if timer in use
    bool init_timer(unsigned int newTimerNum, unsigned int newMode, unsigned int newTimerPrefix);
    
    // returns false if timer is not in use
    // also resets timer
    bool stop_timer(char timerLetter);
    bool stop_timer();

    // isTemporary: true -> timer will hold CPU for delay. False -> timer runs in background
    bool delay_timer(unsigned int delay, bool isTemporary);
};

#endif