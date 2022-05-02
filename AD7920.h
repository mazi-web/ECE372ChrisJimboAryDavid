// Author:         James Fulton
// Net ID:         23413392
// Date:           1 May 2022
// Assignment: ECE-372 Final Project
//
// File: AD7920.h

#ifndef AD7920_h
#define AD7920_h
#include <avr/io.h>
#include <SPI.h>

#define DDR_SPI DDRB      // DDR on ATMEGA2560 for SPI is DDRB
#define DD_SS DDB0        // SS Chip Select of ATMEGA2560 / CS            (Pin 53)
//#define DD_SCK DDB1       // Clock pin on ATMEGA2560 / Clk                (Pin 52)
//#define DD_MOSI DDB2      // MOSI on ATMEGA2560                           (Pin 51)
//#define DD_MISO DDB3      // MOSI on ATMEGA2560                           (Pin 50)
#define SPI_PORT PORTB    // Port for SPI on ATMEGA2560
#define SPI_SS_BIT PORTB0 // Port Bit of Chip Select on ATMEGA2560

class AD7920{
private:


public:

    void init_AD7920();

    inline uint16_t getData(){
        uint16_t dataIn = 0x0000;
        
        SPI_PORT &= ~(1 << SPI_SS_BIT); // SS Pin Low

        //dataIn |= SPI.transfer(0) << 8; //SPI.transfer16(0xffff);
        //dataIn |= SPI.transfer(0);
        dataIn = SPI.transfer16(0);

        SPI_PORT |= (1 << SPI_SS_BIT); // SS Pin High

        return dataIn;
    }

};

#endif