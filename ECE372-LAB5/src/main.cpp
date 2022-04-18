// Authors: Ary Nath, James Fulton, David Mazi, Christopher Bremser
// Date: 1 April 2022
// Assignment: 372 Group Lab 4
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "pwm.h"
#include "switch.h"
#include "timer.h"
#include "i2c.h"
#include "spi.h"

#define SHORT_DELAY 100
#define LONG_DELAY 200

typedef enum state
{
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
} stateType;

typedef enum accelerometerState
{
  initialState,
  trippedState
} stateTypeAcceleroMeterState;

volatile stateType state = wait_press;
volatile stateTypeAcceleroMeterState accelState = initialState;

boolean buttonPressed = false;

volatile int x, y, z = 0;

int main()
{
  cli();
  timer timer1_ms(TIMER_1, 2, MILISECONDS);

  initI2C(); //I2C being used by accelerometer
  initSwitch(); //Switch to turn off buzzer
  SPI_MASTER_Init(); //SPI being used by 8x8 Matrix display
  initPWM_Pins(); //Piezo buzzer
  sei();

  uint8_t delay = SHORT_DELAY;
  while (true)
  {
    //Initialize accelerometer SPI communication
    StartI2C_Trans(0x68); //based off pg 46 of Who Am I reg

    //Wake up accelerometer
    //Already done in the I2C initialization stage

    Read_from(0x68, 0x3B); //XOut high
    x |= (Read_data() << 8); //Getting upper half into x
    Read_from(0x68, 0x3C); //XOut low
    x |= (Read_data()); //Getting lower half into x
    
    Read_from(0x68, 0x3D); //YOut high
    y |= (Read_data() << 8); //Getting upper half into y
    Read_from(0x68, 0x3E); //YOut lower
    y |= (Read_data()); //Getting lower half into y

    Read_from(0x68, 0x3F); //ZOut high
    z |= (Read_data() << 8); //Getting upper half into z
    Read_from(0x68, 0x40); //Zout low
    z |= (Read_data()); //Getting lower half into z

    Serial.println("X Data: " + x);
    Serial.println("Y Data" + y);
    Serial.println("Z Data: " + z);

    //Stop I2C transmission
    StopI2C_Trans();

    switch (state)
    {
    case wait_press:
      timer1_ms.delay_timer(delay);
      break;

    case debounce_press:
      state = wait_release;
      break;

    case wait_release:
      timer1_ms.delay_timer(delay);
      //PWM change frequency stuff
      buttonPressed = true;
      break;

    case debounce_release:
      state = wait_press;
      buttonPressed = false;
      break;
    }

    switch (accelState)
    {
    case initialState:
      spi_smile_maker(true);
      if (x == 150 && y == 150 && z == 150)
      {
        accelState = trippedState;
        IncFrequency(1000);
      }
      break;
    case trippedState:
      spi_smile_maker(false);
      if (buttonPressed == true)
      {
        accelState = initialState;
        IncFrequency(0);
      }
      
      break;
    default:
      break;
    }
  }
  return 0;
}

ISR(INT0_vect)
{
  if (state == wait_press)
  {
    state = debounce_press;
  }
  else if (state == wait_release)
  {
    state = debounce_release;
  }
}