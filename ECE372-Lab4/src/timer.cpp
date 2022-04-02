// Author: 
// Net ID: 
// Date: 
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
	TCCR1A &= ~(1 << WGM11);
    TCCR1A &= ~(1 << WGM10);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
    // enable timer0 interrupt mask register bit using CTC mode
    TCCR1B |= (1 << CS10) | (1 << CS12); // Sets the prescaler to 1024
    OCR1A = 15625;
}

/* This delays the program an amount of seconds specified by unsigned int delay.
*/
void delayS(unsigned int delay){
    unsigned int delayCnt = 0;
    TCNT1 = 0;             //starting the timer at 0 instead of some random junk number
    TIFR1 |= (1 << OCF1A); // set compare flag to start timer

    while (delayCnt < delay)
    {
        if (TIFR1 & (1 << OCF1A))
        { //increment only while the flag is set (timer reached
            delayCnt++;
            TIFR1 |= (1 << OCF1A); //re-start timer. will go to 0 before reaching the if statement above
        }
    }
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
{
    TCCR0A |= (1 << WGM01);
    TCCR0A &= ~(1 << WGM00);
    TCCR0B &= ~(1 << WGM02);
    // enable timer0 interrupt mask register bit using CTC mode
    TCCR0B |= (1 << CS01) | (1 << CS00); // Sets the prescaler to 64
    OCR0A = 249;
}
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
   unsigned int delayCnt = 0;
    TCNT0 = 0;             //starting the timer at 0 instead of some random junk number
    TIFR0 |= (1 << OCF0A); // set compare flag to start timer

    while (delayCnt < delay)
    {
        if (TIFR0 & (1 << OCF0A))
        { //increment only while the flag is set (timer reached
            delayCnt++;
            TIFR0 |= (1 << OCF0A); //re-start timer. will go to 0 before reaching the if statement above
        }
    }
}