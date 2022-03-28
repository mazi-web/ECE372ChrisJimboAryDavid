// Description: This file implements the initialization of PWM
//----------------------------------------------------------------------//

#include "pwm.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initPWM3(){
    //Pin Initialization
    DDRE |= (1 << PORTE3);

    //Mode Selection for 10-bit
    TCCR3A |= (1 << WGM30) | (1 << WGM31);
    TCCR3A |= (1 << WGM32);
    TCCR3A &= ~(1 << WGM33);

    //Set Compare Output Mode - Non Inverting
    TCCR3A |= (1 << COM3A1);
    TCCR3A &= ~(1 << COM3A0);

    //Set Prescaler to 1
    TCCR3B |= (1 << CS30);
    TCCR3B &= ~((1 << CS31) | (1 << CS32));

    //Duty Cycle Set to 0
    //255 for 25% 511 for 50% 767 for 75% and 1023 for 100%
    OCR3A = 0;
}

void initPWM4(){
    //Pin Initialization Reverse Motor Direction
    DDRH |= (1 << PORTH3);

    //Mode Selection for 10-bit
    TCCR4A |= (1 << WGM40) | (1 << WGM41);
    TCCR4A |= (1 << WGM42);
    TCCR4A &= ~(1 << WGM43);

    //Set Compare Output Mode - Non Inverting
    TCCR4A |= (1 << COM4A1);
    TCCR4A |= (1 << COM4A0);

    //Set Prescaler to 1
    TCCR4B |= (1 << CS40);
    TCCR4B &= ~((1 << CS41) | (1 << CS42));

    //Duty Cycle Set to 0
    //255 for 25% 511 for 50% 767 for 75% and 1023 for 100%
    OCR4A = 1023;
}

void changeDutyCycle(int result){
    //Trucate the 10-bit value which has a range of 0-1023 in decimal to have a range of 0-5V and use that as the input here
    //Consider left shift by 8
    //Incompletre for now
    if(result == -1){
        OCR3A = 0;
        OCR4A = 1023;
    }

    else if(result == 0){
        OCR3A = 1023;
        OCR4A = 1023;
    }

    else if(result < 512){
        OCR3A = result/1023;
        OCR4A = 0;
    }

    else if(result == 512){
        OCR3A = 0;
        OCR4A = 0;
    }

    else if(result == 512){
        OCR3A = 0;
        OCR4A = 1023;//Zero for timer 4, reverse
    }

    else if(result == 1023){
        OCR3A = 0;
        OCR4A = 1023;
    }
}