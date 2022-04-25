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

#define SHORT_DELAY 10
#define LONG_DELAY 20

#define PWR_MGMT 0x6B
#define WAKEUP 0x00

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

boolean turnedOn = false;

volatile unsigned int x, y, z = 0;

void turnOn()
{
  // prescaler 1
  TCCR4B |= (1 << CS40);
  TCCR4B &= ~((1 << CS41) | (1 << CS42));

  // output
  DDRH |= (1 << DDH5);
}
void turnOff()
{
  // prescaler no clock
  TCCR4B &= ~((1 << CS41) | (1 << CS42) | (1 << CS40));

  // low
  DDRH &= ~(1 << DDH5);
}

int main()
{
  Serial.begin(9600);
  turnOff();
  timer timer1_ms(TIMER_1, 2, MILISECONDS);
  Serial.println("Timer configed");
  initI2C(); // I2C being used by accelerometer
  Serial.println("I2C Configed");
  initSwitch();      // Switch to turn off buzzer
  SPI_MASTER_Init(); // SPI being used by 8x8 Matrix display
  // initialize 8 x 8 LED array (info from MAX7219 datasheet)
  write_execute(0x0A, 0x08); // brightness control
  write_execute(0x0B, 0x07); // scanning all rows and columns
  write_execute(0x0C, 0x01); // set shutdown register to normal operation (0x01)
  write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)
  Serial.println("SPI configed");
  initPWM_Pins(); // Piezo buzzer
  turnOff();
  Serial.println("PWM configed");
  sei();
  //unsigned int buzzfreq;

  DDRB |= (1 << DDB7);

  uint8_t delayA = SHORT_DELAY;
  while (true)
  {
    Serial.println("Start of while loop");
    // PORTB |= (1 << PORTB7);
    // delay(250);
    //  Initialize accelerometer SPI communication
    Serial.println("B4 I2C");
    Serial.flush();
    StartI2C_Trans(0x68); // based off pg 46 of Who Am I reg
    Serial.println("After I2C");
    Serial.flush();
    // spi_smile_maker(true);

    // Wake up accelerometer
    write(PWR_MGMT); // address on SLA for Power Management
    write(WAKEUP);   // send data to Wake up from sleep mode
    // Already done in the I2C initialization stage

    Read_from(0x68, 0x3B);  // XOut high
    x = Read_data(); // Getting upper half into x
    Read_from(0x68, 0x3C);  // XOut low
    // x |= (Read_data());    // Getting lower half into x
    x = (x << 8) | Read_data();
    Serial.print("X:");
    Serial.print(x);

    Read_from(0x68, 0x3D);   // YOut high
    y = Read_data(); // Getting upper half into y
    Read_from(0x68, 0x3E);   // YOut lower
    y = (y << 8) | Read_data();      // Getting lower half into y
    Serial.print(" Y:");
    Serial.print(y);

    Read_from(0x68, 0x3F);   // ZOut high
    z = Read_data(); // Getting upper half into z
    Read_from(0x68, 0x40);   // Zout low
    z = (z << 8) | Read_data();      // Getting lower half into z
    Serial.print(" Z:");
    Serial.println(z);

    // Serial.println("X Data: " + x);
    // Serial.println("Y Data" + y);
    // Serial.println("Z Data: " + z);

    // Stop I2C transmission
    StopI2C_Trans();

    Serial.println("Before smile");
    // spi_smile_maker(false);
    Serial.println("After smile");

    switch (state)
    {
    case wait_press:
      timer1_ms.delay_timer(delayA);
      break;

    case debounce_press:
      state = wait_release;
      break;

    case wait_release:
      timer1_ms.delay_timer(delayA);
      // PWM change frequency stuff
      //buttonPressed = true;
      break;

    case debounce_release:
      state = wait_press;
      // buttonPressed = !(buttonPressed);
      // if (buttonPressed == false)
      // {
      //   turnOff();
      // }
      // if (buttonPressed == true)
      // {
      //   turnOn();
      // }
      turnOff();
      turnedOn = false;
      break;
    }

    switch (accelState)
    {
    case initialState:
      spi_smile_maker(true);
      if (z <= 15000)
      {
        accelState = trippedState;
      }
      if (turnedOn == true)
      {
        for (unsigned int i = 500; i < 10000; i++)
        {
          IncFrequency(i);
        }
      }
      break;
    case trippedState:
      spi_smile_maker(false);
      turnOn();
      turnedOn = true;
      for (unsigned int i = 500; i < 10000; i++)
        {
          IncFrequency(i);
        }
      if (z > 15000)
      {
        accelState = initialState;
      }
    break;
  default:
    break;
  }
}
return 0;
}

//ISR(PCINT0_vect)
ISR(INT4_vect)
{
  Serial.println("ISR Hours");
  if (state == wait_press)
  {
    state = debounce_press;
    Serial.println("First If");
  }
  else if (state == wait_release)
  {
    Serial.println("Second If");
    state = debounce_release;
  }
}