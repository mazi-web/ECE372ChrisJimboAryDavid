// Author:         James Fulton
// Net ID:         23413392
// Date:           1 April 2022
// Assignment: ECE-372 Lab 4
//
// File: seven_segment.cpp

#include "seven_segment.h"

// Pins Used: All PORTC Pins: 30-37
// 37 -> A
// 36 -> B
// 35 -> C
// 34 -> D
// 33 -> E
// 32 -> F
// 31 -> G
// 30 -> NONE


void seven_segment::init_SevenSegment(timer *new_timer_ms){
     DDRC |= 0b01111111;
    PORTC &= 0b10000000;

    timer_ms = new_timer_ms;
}

bool seven_segment::countDown_Seconds(unsigned int start){
    while(start >= 0){
        if(!sendDigit(start)){ return false; } //if sendDigit has a failure, quit

        if(start > 0){ timer_ms->delay_timer(1000, true); }
        else{ timer_ms->delay_timer(1000, true); }

        start--;
    }
    
    sendDigit(10);
    return true;
}

// 0b 0000 0000
//     GFE DCBA
bool seven_segment::sendDigit(unsigned int digit){
    switch(digit){     //  gfedcba
        case 0: PORTC = 0b00111111; return true; // a b c d e f
        case 1: PORTC = 0b00000110; return true; // b c
        case 2: PORTC = 0b00101011; return true; // a b d e g
        case 3: PORTC = 0b01001111; return true; // a b c d g
        case 4: PORTC = 0b01100110; return true; // b c f g
        case 5: PORTC = 0b01101101; return true; // a c d f g
        case 6: PORTC = 0b01111101; return true; // a c d e f g
        case 7: PORTC = 0b00000111; return true; // a b c
        case 8: PORTC = 0b01111111; return true; // a b c d e f g
        case 9: PORTC = 0b01101111; return true; // a b c d f g
        case 10: PORTC = 0b00000000; return true; // turn off display
    }
    return false;
}