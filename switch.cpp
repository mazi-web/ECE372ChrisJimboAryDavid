// Author:         James Fulton
// Net ID:         23413392
// Date:           1 May 2022
// Assignment: ECE-372 Final Project
//
// File: switch.cpp

#include "switch.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void initSwitch(){

    // D2 -> -
    // D3 -> +

    DDRK &=  0b11111110; // ~((1 << PORTD2) | (1 << PORTD3));


    PORTK |= 0b00000001; //(1 << PORTD2) | (1 << PORTD3);

    PCICR |= 0b00000100;
    PCMSK2 |= 0b00000001;
}
