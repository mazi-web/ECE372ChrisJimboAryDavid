// Authors: Ary Nath, James Fulton, David Mazi, Christopher Bremser
// Date: 1 April 2022
// Assignment: 372 Group Lab 4
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//


#include <avr/io.h>
#include <avr/interrupt.h>

#include "pwm.h"
#include "switch.h"
#include "timer.h"


#define SHORT_DELAY 100
#define LONG_DELAY 200

typedef enum state
{
  wait_press,
  debounce_press,
  wait_release,
  debounce_release,
  countdown
} stateType;

volatile stateType state = wait_press;



int main()
{
  cli();
  timer timer0_ms(TIMER0, 2, MILISECONDS);
  timer timer1_ms(TIMER1, 2, MILISECONDS);
  //SevenSegment seg7_1(&timer1_ms);

  initPWM3();
  initPWM4();
  //initADC();
  initSwitch();
  sei();

  uint8_t delay = SHORT_DELAY;
  bool motorRun = true;

  while(true)
  {
    if (motorRun) //Triggers motor turning on if flag is turned on
    {
      changeDutyCycle(ADCL + (ADCH << 8));
    }
    else
    {
      changeDutyCycle(525); // Set to 525 to stop motor (Between 500 & 525)
    }
    

    switch (state)
    {
      case wait_press:
        timer0_ms.delay_timer(delay);
        break;

      case debounce_press:
        state = wait_release;
        break;

      case wait_release:
        timer0_ms.delay_timer(delay);
        break;

      case debounce_release:
        motorRun = false;
        state = countdown;
        break;

      case countdown:
        //seg7_1.countDown_Seconds(9);
        motorRun = true;
        state = wait_press;
        break;
    }
  }
  return 0;
}


ISR(INT0_vect)
{
  if (state == wait_press)
  {
    state = debounce_press;
  }
  else if (state == wait_release)
  {
    state = debounce_release;
  }
}