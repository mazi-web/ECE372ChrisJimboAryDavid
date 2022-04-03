// Author:         James Fulton
// Net ID:         23413392
// Date:           2 April 2022
// Assignment: ECE-372 Lab 4
//
// File: ShiftRegister.h

#ifndef ShiftRegister_h
#define ShiftRegister_h

#include <avr/io.h>

#define LSBFIRST 0
#define MSBFIRST 1

class ShiftRegister{
private:
    char portName;
    uint8_t dataPin;
    uint8_t shiftClkPin;
    uint8_t storageClkPin;

    //TEMPORARY: Will implement a better method... eventually
    void shiftClk(bool value);
    void latchClk(bool value);
    void dataSet(bool value);

public:
    // Only uses PORTC... for now
    // dataPinBit       -> Bit within PORTC for the data line (SER - 14)
    // shiftClkPinBit   -> Bit within PORTC for the Shift In Clock (SRCLK - 11)
    // storageClkPinBit -> Bit within PORTC for the Latch/Storage Clock (RCLK - 12)
    void init_ShiftRegister(uint8_t dataPinBit, uint8_t shiftClkPinBit, uint8_t storageClkPinBit);

    // bitOrder -> LSBFIRST or MSBFIRST
    // data     -> Byte of data to send to shift register
    void shiftData_Out(uint8_t bitOrder, uint8_t data);

    // bitOrder -> LSBFIRST or MSBFIRST
    // Not Done
    uint8_t shiftData_In(uint8_t bitOrder);
};


#endif