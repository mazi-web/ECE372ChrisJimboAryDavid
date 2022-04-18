// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitch(){
//Set Data direction for input
    DDRD &= ~(1 << PORTD0);

    //Enable pullup resistor for stable reading between 0 and 1
    PORTD |= (1 << PORTD0);

    //Enable the pins for interrupt
    //attachInterrupt(digitalPinToInterrupt(PORTD0), ISR, CHANGE);
    //detachInterrupt(PORTD0)
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);
    EIMSK |= (1 << INT0);
}
