// Author:
// Net ID:
// Date:
// Assignment:     Lab 4
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"
#include "adc.h"
#include "sevsegdisp.h"
#include "seven_segment.h"
#include "switch.h"
#include "timer.h"
#include <util/delay.h>
#define SHORT_DELAY 100
#define LONG_DELAY 200

/*
 * Define a set of states that can be used in the state machine using an enum.
 */
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
  timer timer0_ms;
  timer timer1_ms;
  timer0_ms.init_timer(0, 2, 2);
  timer1_ms.init_timer(1, 2, 2);
  initPWM3();
  initPWM4();
  initADC();
  //initSevSegDisp();
  seven_segment seg;
  seg.init_SevenSegment(&timer1_ms);
  initSwitch();
  //initTimer0();
  //initTimer1();

  /*
   * Implement a state machine in the while loop which achieves the assignment
   * requirements.
   */
  sei();
  int delay = SHORT_DELAY;
  int result;
  int motorRun = 1;
  while (1)
  {

    if (motorRun == 1) //Triggers motor turning on if flag is turned on
    {
      result = ADCL;
      result += (((unsigned int) ADCH) << 8);
      changeDutyCycle(result);
    }
    else
    {
      changeDutyCycle(-1);
    }
    

    switch (state)
    {
    case wait_press:
      timer0_ms.delay_timer(delay, true);
      break;
    case debounce_press:
      state = wait_release;
      break;
    case wait_release:
      timer0_ms.delay_timer(delay, true);
      break;
    case debounce_release:
      // if (delay == SHORT_DELAY)
      // {
      //   delay = LONG_DELAY;
      // }
      // else if (delay == LONG_DELAY)
      // {
      //   delay = SHORT_DELAY;
      // }

      //Turn off the motor here using the seven segment display
      state = countdown;
      break;
    case countdown:
      //Insert seven segment stuff here
      seg.countDown_Seconds(9);
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