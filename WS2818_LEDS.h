// Author:         James Fulton
// Net ID:         23413392
// Date:           1 May 2022
// Assignment: ECE-372 Final Project
//
// File: WS2818_LEDS.h

#ifndef WS2818_LEDS_h
#define WS2818_LEDS_h

#include <avr/io.h>
#include "timer.h"

#define PORT PORTE
#define DDR DDRE
#define DATA_BIT 5

#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns

#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

#define RES 250000

#define NS_PER_SEC (1000000000L)          // Signed value since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )



class WS2818_LEDS{
private:
    timer *timer_us;
    uint16_t numLEDS;
	uint8_t maxBrightness;

public:
	WS2818_LEDS(timer *new_timer, uint16_t new_numLEDS);

    void init_WS2818(timer *new_timer, uint16_t new_numLEDS);

	void show();

	void setMaxBrightness(uint8_t newBright);

    inline void sendBit(bool bit) {
    	if(bit){ // 0
			asm volatile (
		    	"sbi %[port], %[bit] \n\t"				// Set the output bit
		    	".rept %[onCycles] \n\t"                                // Execute NOPs to delay exactly the specified number of cycles
		    	"nop \n\t"
		    	".endr \n\t"
		    	"cbi %[port], %[bit] \n\t"                              // Clear the output bit
		    	".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
		    	"nop \n\t"
		    	".endr \n\t"
		    	::
		    	[port]		"I" (_SFR_IO_ADDR(PORT)),
		    	[bit]		"I" (DATA_BIT),
		    	[onCycles]	"I" (NS_TO_CYCLES(T1H) - 2),		
		    	[offCycles] 	"I" (NS_TO_CYCLES(T1L) - 2)		// Minimum interbit delay

			);
    	}
    	else{ // 1

			asm volatile (
		    	"sbi %[port], %[bit] \n\t"				// Set the output bit
		    	".rept %[onCycles] \n\t"				// Execute NOPs to delay exactly the specified number of cycles
		    	"nop \n\t"                                              
		    	".endr \n\t"
		    	"cbi %[port], %[bit] \n\t"                              // Clear the output bit
		    	".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
		    	"nop \n\t"
		    	".endr \n\t"
		    	::
		    	[port]		"I" (_SFR_IO_ADDR(PORT)),
		    	[bit]		"I" (DATA_BIT),
		    	[onCycles]	"I" (NS_TO_CYCLES(T0H) - 2),
		    	[offCycles]	"I" (NS_TO_CYCLES(T0L) - 2)		// Minimum interbit delay

			);
    	}
	}

	inline void sendByte(uint8_t data) {
    		for(unsigned int i = 0; i < 8 ; i++){
   	    		sendBit((data >> (7-i)) & 0x01);
    		}           
	}


	//---------Manual Functions:

	inline void sendColor(uint8_t red, uint8_t green, uint8_t blue){
    	// Send green, red, then blue for WS2818
		if(green > maxBrightness){
			sendByte(maxBrightness);
		}
		else{
			sendByte(green);
		}

		if(red > maxBrightness){
			sendByte(maxBrightness);
		}
		else{
			sendByte(red);
		}

		if(blue > maxBrightness){
			sendByte(maxBrightness);
		}
		else{
			sendByte(blue);
		}

 		//sendByte(green);
    	//sendByte(red);
    	//sendByte(blue);
	}

	inline void sendHexColor(uint32_t hexValue, uint8_t brightness){
		uint8_t red = ((hexValue >> 16) & 0xFF) * brightness / 255;
		uint8_t green = ((hexValue >> 8) & 0xFF) * brightness / 255;
		uint8_t blue = (hexValue & 0xFF) * brightness / 255;

		if(green > 4){ sendByte(green); }
		else{ sendByte(0x00); }
		if(red > 4){ sendByte(red); }
		else{ sendByte(0x00); }
		if(blue > 4){ sendByte(blue); }
		else{ sendByte(0x00); }

	}



	//---------Predefined Functions:

	void allLEDs_OFF();
	void allLEDs_RGB(uint8_t red, uint8_t green, uint8_t blue);
	void rainbowCycle(unsigned int frames , unsigned int frameAdvance, unsigned int pixelAdvance);

	uint32_t colorValues[32] = {0x610061,            // 0,2
                            0x8100cc,            // 625,0
                            0x6100ff,            // 1250,0
                            0x5900ff,            // 1875,0
                            0x0000ff,            // 2500,0
                            0x0061ff,            // 3125,0
                            0x00aeff,            // 3750,0
                            0x00e2ff,            // 4375,0
                            0x00ffe0,            // 5000,0
                            0x00ff54,            // 5625,0
                            0x31ff00,            // 6250,0
                            0x99ff00,            // 6875,0
                            0xe1ff00,            // 7500,0
                            0xf9ff00,            // 8125,0
                            0xfff200,            // 8750,0
                            0xffd200,            // 9375,1
                            0xffb700,            // 10000,20
                            0xffa200,            // 10625,0
                            0xff9400,            // 11250,0
                            0xff7e00,            // 11875,0
                            0xff6700,            // 12500,0
                            0xff5700,            // 13125,0
                            0xff4f00,            // 13750,0
                            0xff2b00,            // 14375,0
                            0xff1600,            // 15000,0
                            0xff0000,            // 15625,0
                            0xff0000,            // 16250,0
                            0xff0000,            // 16875,0
                            0xff0000,            // 17500,0
                            0xff0000,            // 18125,0
                            0xff0000,            // 18750,0
                            0xff0000,            // 19375,1
	};

	inline void FFT_Pattern(uint16_t Data[], uint16_t samples){
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
};

#endif