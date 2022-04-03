// Author:         James Fulton
// Net ID:         23413392
// Date:           1 April 2022
// Assignment: ECE-372 Lab 4
//
// File: SevenSegment.cpp

#include "SevenSegment.h"

// Pins Used: PORTC Pins: 35-37
// 37 -> RCLK (12)
// 36 -> SRCLK (11)
// 35 -> SER (14)

SevenSegment::SevenSegment(){
    timer_ms = nullptr;
}

SevenSegment::SevenSegment(timer *new_timer_ms){
    init_SevenSegment(new_timer_ms);
}

void SevenSegment::init_SevenSegment(timer *new_timer_ms){
    shiftreg1.init_ShiftRegister(2, 1, 0); // bits 0, 1, 2 used
    shiftreg1.shiftData_Out(MSBFIRST, 0x00);  // Turn off
    timer_ms = new_timer_ms;
}

bool SevenSegment::countDown_Seconds(unsigned int start){
    start++; // Did this to use unsigned variable ^

    while(start > 0){
        if(!sendDigit(start-1)){ return false; } //if sendDigit has a failure, quit

        if(start > 1){ timer_ms->delay_timer(1000, PAUSE); }
        else{ timer_ms->delay_timer(750, PAUSE); }              // Decided to reduce the delay for 0; "feels" better

        start--;
    }
    
    shiftreg1.shiftData_Out(MSBFIRST, 0x00); // turn off display
    return true;
}

// digit -> 0:1:9
// 0b 0000 0000
//     GFE DCBA -> Single Digit 7-Segment Display
bool SevenSegment::sendDigit(unsigned int digit){
    switch(digit){     //  gfedcba
        case 0: shiftreg1.shiftData_Out(MSBFIRST, 0b00111111); return true; // a b c d e f
        case 1: shiftreg1.shiftData_Out(MSBFIRST, 0b00000110); return true; // b c
        case 2: shiftreg1.shiftData_Out(MSBFIRST, 0b01011011); return true; // a b d e g
        case 3: shiftreg1.shiftData_Out(MSBFIRST, 0b01001111); return true; // a b c d g
        case 4: shiftreg1.shiftData_Out(MSBFIRST, 0b01100110); return true; // b c f g
        case 5: shiftreg1.shiftData_Out(MSBFIRST, 0b01101101); return true; // a c d f g
        case 6: shiftreg1.shiftData_Out(MSBFIRST, 0b01111101); return true; // a c d e f g
        case 7: shiftreg1.shiftData_Out(MSBFIRST, 0b00000111); return true; // a b c
        case 8: shiftreg1.shiftData_Out(MSBFIRST, 0b01111111); return true; // a b c d e f g
        case 9: shiftreg1.shiftData_Out(MSBFIRST, 0b01101111); return true; // a b c d f g
    }
    return false;
}