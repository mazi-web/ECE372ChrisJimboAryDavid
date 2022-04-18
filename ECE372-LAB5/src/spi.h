
#ifndef spi_h
#define spi_h
#include <avr/io.h>
#include <avr/interrupt.h>


void SPI_MASTER_Init();
void write_execute(uint8_t CMD, uint8_t data);
void spi_smile_maker(bool smile);


#endif