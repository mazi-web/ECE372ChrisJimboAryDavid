#include "i2c.h"
#include <avr/io.h>
#include <Arduino.h>

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C(){
  PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560 power management register
  TWSR |= (1 << TWPS0);  // prescaler power = 1
  TWSR &= ~(1 << TWPS1); // prescaler power = 1 
  //πππ΅π=((πΆππ πΆππππ πππππ’ππππ¦)/(ππΆπΏ πππππ’ππππ¦)β16)/(2βγ(4)γ^ππππ )
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)

  TWCR |= (1 << TWINT)| (1 << TWEN); // enable two wire interface
}

void StartI2C_Trans(unsigned char SLA){
//Start Condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    //Serial.println("Trigger Action Complete");
    wait_for_completion;
    //Serial.println("Wait complete");
    //delay(150);
    TWDR = (SLA << 1);
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;
    //Serial.println("Start Trans Complete");
    //delay(50);
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

