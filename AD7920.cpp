// Author:         James Fulton
// Net ID:         23413392
// Date:           1 May 2022
// Assignment: ECE-372 Final Project
//
// File: AD7920.cpp

#include "AD7920.h"

void AD7920::init_AD7920(){
    DDR_SPI |= (1 << DD_SS); // Set SS Pin to Output
    SPI_PORT |= (1 << SPI_SS_BIT); // SS Pin High

    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE2));
    SPI.begin();
}