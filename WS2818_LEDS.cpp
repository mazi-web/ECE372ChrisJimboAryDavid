// Author:         James Fulton
// Net ID:         23413392
// Date:           1 May 2022
// Assignment: ECE-372 Final Project
//
// File: WS2818_LEDS.cpp

#include "WS2818_LEDS.h"
//#include "Arduino.h"

WS2818_LEDS::WS2818_LEDS(timer *new_timer, uint16_t new_numLEDS){
    init_WS2818(new_timer, new_numLEDS);
}

void WS2818_LEDS::init_WS2818(timer *new_timer, uint16_t new_numLEDS){
    this->timer_us = new_timer;
    this->numLEDS = new_numLEDS;

    DDR |= (1 << DATA_BIT);  // Set datBit to output

    maxBrightness = 100;
}

void WS2818_LEDS::show(){
    timer_us->delay_timer((RES / 1000UL) + 1);
}

void WS2818_LEDS::setMaxBrightness(uint8_t newBright){
    this->maxBrightness = newBright;
}

void WS2818_LEDS::allLEDs_OFF(){
    allLEDs_RGB(0, 0, 0);
}

void WS2818_LEDS::allLEDs_RGB(uint8_t red, uint8_t green, uint8_t blue){
    cli();
    for(unsigned int i = 0; i < numLEDS; i++){
        sendColor(red, green, blue);
    }
    sei();
    show();
}

                                                                            
void WS2818_LEDS::rainbowCycle(unsigned int frames , unsigned int frameAdvance, unsigned int pixelAdvance ) {
  
  // Hue is a number between 0 and 3*256 than defines a mix of r->g->b where
  // hue of 0 = Full red
  // hue of 128 = 1/2 red and 1/2 green
  // hue of 256 = Full Green
  // hue of 384 = 1/2 green and 1/2 blue
  // ...
  
  unsigned int firstPixelHue = 0;     // Color for the first pixel in the string
  
  for(unsigned int j=0; j<frames; j++) {                                  
    
    unsigned int currentPixelHue = firstPixelHue;
       
    cli();    
        
    for(unsigned int i=0; i < numLEDS; i++) {
      
      if (currentPixelHue>=(3*256)) {                  // Normalize back down incase we incremented and overflowed
        currentPixelHue -= (3*256);
      }
            
      unsigned char phase = currentPixelHue >> 8;
      unsigned char step = currentPixelHue & 0xff;
                 
      switch (phase) {
        
        case 0: 
          sendColor(~step, step, 0);
          break;
          
        case 1: 
          sendColor(0, ~step, step);
          break;

        case 2: 
          sendColor(step, 0, ~step);
          break;
          
      }
      
      currentPixelHue += pixelAdvance;                                      
      
                          
    } 
    
    sei();
    
    show();
    
    firstPixelHue += frameAdvance;
           
  }
}


/*
uint32_t colorValues[50] = {0x610061,   // 0
                            0x5d00ff,
                            0x0092ff,
                            0x00eaff,
                            0x00ffcb,
                            0x00ff87,
                            0x00ff46,
                            0x1fff00,
                            0x70ff00,
                            0x92ff00,
                            0xa0ff00,
                            0xa3ff00,
                            0xa9ff00,
                            0xb0ff00,
                            0xbdff00,
                            0xc3ff00,
                            0xcfff00,
                            0xd8ff00,
                            0xe7ff00,
                            0xf6ff00,
                            0xffff00,
                            0xfff900,
                            0xffef00,
                            0xffe600,
                            0xffd900,
                            0xffcf00,
                            0xffbe00,
                            0xffad00,
                            0xff9b00,
                            0xff8900,
                            0xff7700,
                            0xff6f00,
                            0xff6700,
                            0xff5f00,
                            0xff5700,
                            0xff4b00,
                            0xff4600,
                            0xff3e00,
                            0xff3900,
                            0xff3400,
                            0xff2b00,
                            0xff2100,
                            0xff1600,
                            0xff1000,
                            0xff0900,
                            0xff0000,
};
*/

/*
Hz       #  Color   Lambda (nm wavelength)
0-20     1  Purple  380-425
20-150   2  Blue    450-493
150-350  3  Green   495-566
350-3k   4  Yellow  570-591
3k-5k    5  Orange  595-630
5k-20k   6  Red     635-750

Hz      Wavelength (nm)   RGB              Hex      Section (Each # is 3 LEDS)
0       380               rgb(97, 0, 97)    610061  2 1 0
345     423               rgb(93, 0, 255)   5d00ff  4 3 1
689     465               rgb(0, 146, 255)  0092ff  6 5 2
1034    485               rgb(0, 234, 255)  00eaff  8 7 3
1378    495               rgb(0, 255, 203)  00ffcb  9   4
1723    501               rgb(0, 255, 135)  00ff87  10  5
2067    506               rgb(0, 255, 70)   00ff46  11  6
2412    515               rgb(31, 255, 0)   1fff00  12  7
2756    535               rgb(112, 255, 0)  70ff00  13  8
3101    545               rgb(146, 255, 0)  92ff00  14  9
3445    549               rgb(160, 255, 0)  a0ff00  15  10
3790    550               rgb(163, 255, 0)  a3ff00  16  11
4134    552               rgb(169, 255, 0)  a9ff00  17  12
4479    554               rgb(176, 255, 0)  b0ff00  18  13
4823    558               rgb(189, 255, 0)  bdff00  19  14
5168    560               rgb(195, 255, 0)  c3ff00  20  15
5513    564               rgb(207, 255, 0)  cfff00  21  16
5857    567               rgb(216, 255, 0)  d8ff00  22  17
6202    572               rgb(231, 255, 0)  e7ff00  23  18
6546    577               rgb(246, 255, 0)  f6ff00  24  19
6891    580               rgb(255, 255, 0)  ffff00  25  20  
7235    582               rgb(255, 249, 0)  fff900  26  21
7580    585               rgb(255, 239, 0)  ffef00  27  22
7924    588               rgb(255, 230, 0)  ffe600  28  23
8269    592               rgb(255, 217, 0)  ffd900  29  24
8613    595               rgb(255, 207, 0)  ffcf00  30  25
8958    600               rgb(255, 190, 0)  ffbe00  31  26
9302    605               rgb(255, 173, 0)  ffad00  32  27
9647    610               rgb(255, 155, 0)  ff9b00  33  28
9991    615               rgb(255, 137, 0)  ff8900  34  29
10336   620               rgb(255, 119, 0)  ff7700  35  30
10680   622               rgb(255, 111, 0)  ff6f00  36  31
11025   624               rgb(255, 103, 0)  ff6700  37  32
11370   626               rgb(255, 95, 0)   ff5f00  38  33
11714   628               rgb(255, 87, 0)   ff5700  39  34
12059   631               rgb(255, 75, 0)   ff4b00  40  35
12403   632               rgb(255, 70, 0)   ff4600  41  36
12748   634               rgb(255, 62, 0)   ff3e00  42  37
13092   635               rgb(255, 57, 0)   ff3900  43  38
13437   636               rgb(255, 52, 0)   ff3400  44  39
13781   638               rgb(255, 43, 0)   ff2b00  45  40
14126   640               rgb(255, 33, 0)   ff2100  46  41
14470   642               rgb(255, 22, 0)   ff1600  47  42
14815   643               rgb(255, 16, 0)   ff1000  48  43
15159   644               rgb(255, 9, 0)    ff0900  49  44
15504   645 to bottom     rgb(255, 0, 0)    ff0000  50 (45)
15848
16193
16538
16882
17227
17571
17916
18260
18605
18949
19294
19638
19983
20327
20672
21016
21361
21705
*/



#define THRESHOLD 7
#define ACTIVATED 20
#define MAX 80

/*
inline void WS2818_LEDS::FFT_Pattern(uint16_t Data[], uint16_t samples){
  // Multiplier 1 = 150 LEDS
  // Multipler  2 = 300 LEDS
  uint8_t mult = 1;
  uint8_t intensity = 0;
  uint32_t colorTemp = 0;

  cli();
  for(uint16_t i = samples/2; i > 0; i--){
    intensity = 0x00 | (Data[i-1] * 2);
    colorTemp = colorValues[i-1];

    if(i < 3){
      for(uint8_t j = 0; j < 8*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
    else if(i < 9){
      for(uint8_t j = 0; j < 6*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
    else if(i < 15){
      for(uint8_t j = 0; j < 5*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
    else{
      for(uint8_t j = 0; j < 4*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
  }
  for(uint16_t i = 0; i < samples/2; i++){
    intensity = 0x00 | (Data[i] * 2);
    colorTemp = colorValues[i];

    if(i < 3){
      for(uint8_t j = 0; j < 8*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
    else if(i < 9){
      for(uint8_t j = 0; j < 6*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
    else if(i < 15){
      for(uint8_t j = 0; j < 5*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
    else{
      for(uint8_t j = 0; j < 4*mult; j++){ sendHexColor(colorTemp, intensity); }
    }
  }
  sei();
  show();

}
*/

