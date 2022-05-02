// Author:         James Fulton
// Net ID:         23413392
// Date:           1 May 2022
// Assignment: ECE-372 Final Project
//
// File: main.cpp

#include "WS2818_LEDS.h"
#include "AD7920.h"
#include "timer.h"
#include "fix_fft.h"
#include "switch.h"

//----Start FFT Parameters
#define SAMPLES 64
#define SAMPLE_FREQ 40000

unsigned int sampling_period_us;

int8_t vReal[SAMPLES];
int8_t vImag[SAMPLES];
uint16_t barht[SAMPLES];

bool pauseProgram = false;
//----End FFT Parameters


// ---- Start GLOBAL SETUP ----
//TIMERS
timer timer1_us(TIMER_1, 2, MICROSECONDS);
timer timer3_ms(TIMER_3, 2, MILISECONDS);

//ADC
AD7920 ADC_1;

//LED STRIP
WS2818_LEDS ledStrip(&timer1_us, 300);

//FFT
// ---- END GLOBAL SETUP ----


void sampleFFT(){
  for(unsigned int i = 0; i < SAMPLES; i++){
    int16_t temp = floor(ADC_1.getData()/16) - 127;
    if(temp > 127){
      vReal[i] = 127;
    }
    else{
      vReal[i] = temp;
    }
    vImag[i] = 0;
    //timer1_us.delay_timer(sampling_period_us); // Not entirely needed due to large enough delay from SPI
						 // Not entirely needed unless sampling rate lowers from 44.1kHz
  }

  fix_fft(vReal, vImag, 6, 0);

  for(unsigned int i = 0; i < SAMPLES; i++){
    barht[i] = sqrt(pow(vReal[i], 2) + pow(vImag[i], 2));
  }
  
  /*
    Serial.println("Freq(Hz),Magnitude");
  for(uint16_t i = 0; i < SAMPLES/2; i++){
    
    Serial.print(round(i * 1.0 * SAMPLE_FREQ / SAMPLES));
    Serial.print(",");
    Serial.print(barht[i]);
    Serial.println();
  }
  */

}


int main() {
  // ---- START MAIN SETUP ----

  //SERIAL OUTPUT
  //Serial.begin(115200);
  //while(!Serial);
  //Serial.println("Ready");

  //LED STRIP
  WS2818_LEDS ledStrip(&timer1_us, 300);
  ledStrip.setMaxBrightness(255);

  //ADC
  ADC_1.init_AD7920();

  //FFT
  sampling_period_us = round(1000000*(1.0 / SAMPLE_FREQ)/1.28/100); // Not entirely needed unless sampling rate lowers; read other similar comment
  Serial.println(sampling_period_us);

  //SWITCH
  initSwitch();

  // ---- END MAIN SETUP ----




  // ---- START LOOP ----
  timer3_ms.delay_timer(1, PAUSE); // For ADC

  //ledStrip.rainbowCycle(1 , 20 , 5);

  //temp = ADC_1.getData();

  //Serial.println(temp);
  //Serial.println(5 * (1.0*temp)/4095);
  sampleFFT();

  while(true){
    if(!pauseProgram){
      sampleFFT();
      ledStrip.FFT_Pattern(barht, SAMPLES);
    }
  }
  

  // --- END LOOP ---

  return 0;
}


ISR(PCINT2_vect){
  if(!(PINK & 0x01)){
    //sampleFFT();
    pauseProgram  = !pauseProgram;
  }
  
  timer3_ms.delay_timer(300);
}
