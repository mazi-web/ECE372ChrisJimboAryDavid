// Author:         James Fulton
// Net ID:         23413392
// Date:           2 April 2022
// Assignment: ECE-372 Lab 4
//
// File: ShiftRegister.cpp

#include "ShiftRegister.h"

// Note:
// Didn't use any delays; a 20ns pulse width is less than one CPU cycle (one cycle is 62.5ns)
// Minimum pulse duration & setup time at 20ns; no reason to use delays (unless using NOP asm instructions, but unnecessary for this application)

// Only uses PORTC... for now
// dataPinBit       -> Bit within PORTC for the data line (SER - 14)
// shiftClkPinBit   -> Bit within PORTC for the Shift In Clock (SRCLK - 11)
// storageClkPinBit -> Bit within PORTC for the Latch/Storage Clock (RCLK - 12)
void ShiftRegister::init_ShiftRegister(uint8_t dataPinBit, uint8_t shiftClkPinBit, uint8_t storageClkPinBit){
    this->portName = portName;
    this->dataPin = dataPinBit;
    this->shiftClkPin = shiftClkPinBit;
    this->storageClkPin = storageClkPinBit;


    DDRC |= (1 << dataPinBit) | (1 << shiftClkPinBit) | (1 << storageClkPinBit);
    PORTC &= ~((1 << dataPinBit) | (1 << shiftClkPinBit) | (1 << storageClkPinBit));
}

// bitOrder -> LSBFIRST or MSBFIRST
// data     -> Byte of data to send to shift register
void ShiftRegister::shiftData_Out(uint8_t bitOrder, uint8_t data){
    for(uint8_t i = 0; i < 8; i++){
        if(bitOrder == LSBFIRST){
            dataSet(data & 0x01); // Aquire first bit
            data >>= 1;
        }
        else{
            dataSet((data & 0x80) != 0x00); // Aquire last bit
            data <<= 1;
        }

        // Shift Data into Shift Register
        shiftClk(true);
        shiftClk(false);
    }

    // Latch Data Through Shift Register
    latchClk(true); //latch clock pin high
    latchClk(false); //latch clock pin low
}


// bitOrder -> LSBFIRST or MSBFIRST
// Not Done
/*
uint8_t ShiftRegister::shiftData_In(uint8_t bitOrder){
    uint8_t returnValue;

    for(uint8_t i = 0; i < 8; i++){
        shiftClk(true);

        shiftClk(false);
    }

    return returnValue;
}
*/


//TEMPORARY: Will implement a better method... eventually
// Changes Designated Pin States
void ShiftRegister::shiftClk(bool value){
    if(value){ PORTC |= (1 << shiftClkPin); }
    else{ PORTC &= ~(1 << shiftClkPin); }
}
void ShiftRegister::latchClk(bool value){
    if(value){ PORTC |= (1 << storageClkPin); }
    else{ PORTC &= ~(1 << storageClkPin); }
}
void ShiftRegister::dataSet(bool value){
    if(value){ PORTC |= (1 << dataPin); }
    else{ PORTC &= ~(1 << dataPin); }
}