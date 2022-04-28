// Description: This file implements the initialization of the ADC
//----------------------------------------------------------------------//

#include "adc.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initADC(){

    //Setting Reference Voltage to AVCC = 5V 
    //In order to do this Setting ADMUX REFS1 And REFS0 to 01 respectively

    ADMUX |= (1 << REFS0);
    ADMUX &= ~( 1 << REFS1);

    // Right Justified  for Admux!

    ADMUX &= ~(1 << ADLAR);

    // Auto Trigger Source Selection
    // Free Running Mode. 
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // Enable ADC and Auto Triggering

    ADCSRA |= ((1 << ADEN) | ( 1 << ADATE));

    // ADC Clock Frequency. Prescaler of 128. 

    ADCSRA |= ( 1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Disable ADC0 Pin Digital input 
    //Pin is A0

    DDRA |= (1 << ADC7D);

    //Start ADC! 

    ADCSRA |= (1 << ADSC);

}