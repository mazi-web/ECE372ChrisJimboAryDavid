#include "i2c.h"
#include <avr/io.h>
#include <Arduino.h>

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C(){
  PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560 power management register
  TWSR |= (1 << TWPS0);  // prescaler power = 1
  TWSR &= ~(1 << TWPS1); // prescaler power = 1 
  //𝑇𝑊𝐵𝑅=((𝐶𝑃𝑈 𝐶𝑙𝑜𝑐𝑘 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)/(𝑆𝐶𝐿 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)−16)/(2∗〖(4)〗^𝑇𝑊𝑃𝑆 )
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)

  TWCR |= (1 << TWINT)| (1 << TWEN); // enable two wire interface
}

void StartI2C_Trans(unsigned char SLA){
//Start Condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    wait_for_completion;
    TWDR = (SLA << 1);
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;
}

void StopI2C_Trans(){
    //Stop Condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void write(unsigned char data){
    //Write Funct
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
    //Read Funct
    StartI2C_Trans(SLA);
    write(MEMADDRESS);

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    wait_for_completion;
    TWDR = (SLA << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);
    wait_for_completion;
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

unsigned char Read_data() // Returns the last byte  from the data register
{
  return TWDR;
}