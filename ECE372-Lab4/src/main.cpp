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
#include "switch.h"
#include "timer.h"
#include <util/delay.h>
#define SHORT_DELAY 100
#define LONG_DELAY 200


/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum state {wait_press, debounce_press, wait_release, debounce_release} stateType;
volatile stateType state = wait_press;

int main(){
  initPWM();
  initADC();
  initSevSegDisp();
  initSwitch();
  initTimer0();
  initTimer1();
  /*
  * Implement a state machine in the while loop which achieves the assignment
  * requirements.
  */
  sei();
  int delay = SHORT_DELAY;
  int count = 1;
	while (1) {
    switch(state){
      case wait_press:
        delayMs(delay);
        //_delay_ms(SHORT_DELAY);
        break;
      case debounce_press:
        state = wait_release;
        break;
      case wait_release:
        //_delay_ms(SHORT_DELAY);
        delayMs(delay);
        break;
      case debounce_release:
        if(delay == SHORT_DELAY){
          delay = LONG_DELAY;
        }
        else if(delay == LONG_DELAY){
          delay = SHORT_DELAY;
        }
        state = wait_press;
        break;
    }
    if(count == 15){
      count = 1;
    }
    else{
      count++;
    }
	}
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
ISR(INT0_vect){
  if(state == wait_press){
    state = debounce_press;
  }
  else if(state == wait_release){
    state = debounce_release;
  }
}