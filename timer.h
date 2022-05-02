// Author:         James Fulton
// Net ID:         23413392
// Date:           21 March 2022
// Assignment: ECE-372 Lab 4
//
// File: timer.h

#ifndef timer_h
#define timer_h
#include <avr/io.h>
#include <avr/interrupt.h>

#define SECONDS 1
#define MILISECONDS 2
#define MILISECONDS_HIFI 3
#define MICROSECONDS 4

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3
#define TIMER_4 4
#define TIMER_5 5

#define INTERRUPT 0
#define PAUSE 1

#define NOP asm volatile("nop"); // Apparently improves performance for no-interupt delays (unproven, testing)

class timer{
private:
    unsigned int timerPrefix;
    uint8_t timerNum;
    uint8_t mode;
    unsigned int countValue;
    unsigned int prescaler;

public:
    // newTimerNum     ->  TIMER1, TIMER2, etc..
    // newMode         ->  mode designated from the datasheet (also in timer.cpp)
    // newTimerPrefix  ->  SECONDS, MILISECONDS, MILISECONDS_HIFI (Limited to 262.144ms), MICROSECONDS
    timer(uint8_t newTimerNum, uint8_t newMode, uint8_t newTimerPrefix);

    // Currently designed for no external source or output: timers 0, 1, 2, 3 only avaliable
    // Modes 0 & 2 only avaliable (only timer0 modes are listed above)
    // newTimerNum     ->  TIMER1, TIMER2, etc..
    // newMode         ->  mode designated from the datasheet (also in timer.cpp)
    // newTimerPrefix  ->  SECONDS, MILISECONDS, MILISECONDS_HIFI (Limited to 262.144ms), MICROSECONDS
    bool init_timer(uint8_t newTimerNum, uint8_t newMode, uint8_t newTimerPrefix);
    
    // Timer Letter for PWM Mode only (specify specific timer counter registers by letter)
    bool stop_timer(char timerLetter);

    // Use for non PWM modes to completely stop timer
    bool stop_timer();

    // delay -> delay value
    // usage -> PAUSE, INTERRUPT
    bool delay_timer(unsigned int delay, uint8_t usage);
    
    // Make timer delay CPU; same usage as Arduino's delay(ms)
    bool delay_timer(unsigned int delay);
};

#endif