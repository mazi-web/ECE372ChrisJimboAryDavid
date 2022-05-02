// Author:         James Fulton
// Net ID:         23413392
// Date:           21 March 2022
// Assignment: ECE-372 Lab 4
//
// File: timer.cpp

#include "timer.h"

// 8-bit timer (0, 2)
// Mode:           TOP    Update     TOV_Flag_Set
// 0->normal       0xFF   Immediate  Max: 0xFF
// 1->PWM_C        0xFF   Top        Bottom: 0x00
// 2->CTC          OCRA   Immediate  Max: 0xFF
// 3->FAST_PWM     0xFF   Top        Max: 0xFF
// 4->PWM_C        OCRA   Top        Bottom: 0x00
// 5->FAST_PWM     OCRA   Bottom     Top: 0xFF

// 16-bit timer (1, 3, 4, 5)
// Mode:                        WGMn    TOP     Update of OCRnX     TOVn Flag Set
// 0->normal                    0000    FFFF    Immediate           Max
// 1->PWM_Phase_Correct_8-bit   0001    00FF    Top                 Bottom
// 2->PWM_Phase_Correct_9-bit   0010    01FF    Top                 Bottom
// 3->PWM_Phase_Correct_10-bit  0011    03FF    Immediate           Bottom
// 4->CTC                       0100    OCRnA   Bottom              Max
// 5->PWM_Fast_8-bit            0101    00FF    Bottom              Top
// 6->PWM_Fast_9-bit            0110    01FF    Bottom              Top
// 7->PWM_Fast_10-bit           0111    03FF    Bottom              Top
// 8->PWM_Phase&Freq_Correct    1000    ICRn    Bottom              Bottom
// 9->PWM_Phase&Freq_Correct    1001    OCRnA   Bottom              Bottom
// 10->PWM_Phase_Correct        1010    ICRn    Top                 Bottom
// 11->PWM_Phase_Correct        1011    OCRnA   Top                 Bottom
// 12->CTC                      1100    ICRn    Immediate           Max
// 13 NULL                      1101
// 14 PWM_Fast                  1110    ICRn    Bottom              Top
// 15 PWM_Fast                  1111    OCRnA   Bottom              Top


// newTimerNum     ->  TIMER1, TIMER2, etc..
// newMode         ->  mode designated from the datasheet (also in timer.cpp)
// newTimerPrefix  ->  SECONDS, MILISECONDS, MILISECONDS_HIFI (Limited to 262.144ms), MICROSECONDS
timer::timer(uint8_t newTimerNum, uint8_t newMode, uint8_t newTimerPrefix){
    init_timer(newTimerNum, newMode, newTimerPrefix);
}

// Currently designed for no external source or output
// Modes 0 & 2 only avaliable (0 -> normal & 2-> CTC)
// 16-bit timers actually use mode 4 CTC as above: 8-bit timers use mode 2 CTC as above
// newTimerNum     ->  TIMER1, TIMER2, etc..
// newMode         ->  mode designated from the datasheet (also in timer.cpp)
// newTimerPrefix  ->  SECONDS, MILISECONDS, MILISECONDS_HIFI (Limited to 262.144ms), MICROSECONDS
bool timer::init_timer(uint8_t newTimerNum, uint8_t newMode, uint8_t newTimerPrefix){

    this->timerNum = newTimerNum;
    this->mode = newMode;
    this->timerPrefix = newTimerPrefix;

    // Timer Mode Initialization
    switch(this->timerNum){
        case 0:{
            switch(this->mode){
                case 0:{
                    TCCR0A = 0x00;
                    TCCR0B &= 0b11110111; 
                    return true;
                }
                case 2:{
                    TCCR0A = 0b00000010;
                    TCCR0B &= 0b11110111;
                    return true;
                }
            }

        }

        case 1:{
            switch(this->mode){
                case 0:{
                    TCCR1A = 0x00;
                    TCCR1B &= 0b00000111;
                    TCCR1C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR1A = 0x00;
                    TCCR1B = (TCCR1B & 0b00000111) | 0b00001000;
                    TCCR1C = 0x00;
                    return true;
                }
            }           
        }

        case 2:{
            switch(this->mode){
                case 0:{
                    TCCR2A = 0x00;
                    TCCR2B &= 0b11110111;
                    return true;
                }
                case 2:{
                    TCCR2A = 0b00000010;
                    TCCR2B &= 0b11110111;
                    return true;
                }

            }
        }

        case 3:{
            switch(this->mode){
               case 0:{
                    TCCR3A = 0x00;
                    TCCR3B &= 0b00000111;
                    TCCR3C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR3A = 0x00;
                    TCCR3B = (TCCR3B & 0b00000111) | 0b00001000;
                    TCCR3C = 0x00;
                    return true;
                }
            }
        }
        case 4:{
            switch(this->mode){
               case 0:{
                    TCCR4A = 0x00;
                    TCCR4B &= 0b00000111;
                    TCCR4C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR4A = 0x00;
                    TCCR4B = (TCCR4B & 0b00000111) | 0b00001000;
                    TCCR4C = 0x00;
                    return true;
                }
            }
        }
        case 5:{
            switch(this->mode){
               case 0:{
                    TCCR5A = 0x00;
                    TCCR5B &= 0b00000111;
                    TCCR5C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR5A = 0x00;
                    TCCR5B = (TCCR5B & 0b00000111) | 0b00001000;
                    TCCR5C = 0x00;
                    return true;
                }
            }
        }
    }

    return false;
}



// Timer Letter for PWM Mode only (specify specific timer counter registers by letter)
bool timer::stop_timer(char newTimerLetter){
    switch(this->timerNum){
        case 0:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK0 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK0 &= 0b11111011; }
            else{ TIMSK0 = 0x00; }

            if(TIMSK0 == 0x00 || mode == 2){
                TCCR0B &= 0b11111000; // Stop timer by setting no clock source
                OCR0A = 0x00;
                OCR0B = 0x00;
                return true;
            }
        }
        case 1:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK1 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK1 &= 0b11111011; }
            else if(newTimerLetter == 'C' || newTimerLetter == 'c'){ TIMSK1 &= 0b11110111; }
            else{ TIMSK1 = 0x00; }

            if(TIMSK1 == 0x00 || mode == 2){
                TCCR1B &= 0b11111000; // Stop timer by setting no clock source
                OCR1A = 0x00;
                OCR1B = 0x00;
                OCR1C = 0x00;
                return true;
            }
        }
        case 2:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK2 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK2 &= 0b11111011; }
            else{ TIMSK2 = 0x00; }

            if(TIMSK2 == 0x00 || mode == 2){
                TCCR2B &= 0b11111000; // Stop timer by setting no clock source
                OCR2A = 0x00;
                OCR2B = 0x00;
                return true;
            }
        }
        case 3:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK3 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK3 &= 0b11111011; }
            else if(newTimerLetter == 'C' || newTimerLetter == 'c'){ TIMSK3 &= 0b11110111; }
            else{ TIMSK3 = 0x00; }

            if(TIMSK3 == 0x00 || mode == 2){
                TCCR3B &= 0b11111000; // Stop timer by setting no clock source
                OCR3A = 0x00;
                OCR3B = 0x00;
                OCR3C = 0x00;
                return true;
            }
        }
        case 4:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK4 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK4 &= 0b11111011; }
            else if(newTimerLetter == 'C' || newTimerLetter == 'c'){ TIMSK4 &= 0b11110111; }
            else{ TIMSK4 = 0x00; }

            if(TIMSK4 == 0x00 || mode == 2){
                TCCR4B &= 0b11111000; // Stop timer by setting no clock source
                OCR4A = 0x00;
                OCR4B = 0x00;
                OCR4C = 0x00;
                return true;
            }
        }
        case 5:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK5 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK5 &= 0b11111011; }
            else if(newTimerLetter == 'C' || newTimerLetter == 'c'){ TIMSK5 &= 0b11110111; }
            else{ TIMSK5 = 0x00; }

            if(TIMSK5 == 0x00 || mode){
                TCCR5B &= 0b11111000; // Stop timer by setting no clock source
                OCR5A = 0x00;
                OCR5B = 0x00;
                OCR5C = 0x00;
                return true;
            }
        }
    }

    return false;
}

// Use for non PWM modes to completely stop timer
bool timer::stop_timer(){ return stop_timer('Z'); }



// delay -> delay value
// usage -> PAUSE (hold/pause CPU), INTERRUPT (ISR)
bool timer::delay_timer(unsigned int delay, uint8_t usage){
    // end if current timer is active
    //if((newTimerLetter == 'A' || newTimerLetter == 'a') && (activeTimers & 0x01 == 1)){ return false; }
    //else if((newTimerLetter == 'B' || newTimerLetter == 'b') && ((activeTimers >> 1) & 0x01 == 1)){ return false; }
    //else if((newTimerLetter == 'C' || newTimerLetter == 'c') && ((activeTimers >> 2) & 0x01 == 1)){ return false; }

    countValue = 100;


    // Defining Prescaler and Max Count Values
    switch(this->timerPrefix){
        case 1:{  // seconds
            if(timerNum == 0 || timerNum == 2){ return false; }
            else{ countValue = 15625 * delay; prescaler = 1024; } // 16000000 * delay / 1024
            break;
        }
        case 2:{  // miliseconds LOW fidelity
            if(timerNum == 0 || timerNum == 2){ countValue = 16 * delay; prescaler = 1024; } // 16000000 * delay / 1024 / 1000
            else{ countValue = 62 * delay; prescaler = 256; } // 16000000 * delay / 256 / 1000
            break;
        }
        case 3:{  // miliseconds HIGH fidelity (Limited to 262.144ms)
            if(timerNum == 0 || timerNum == 2){ return false; }
            countValue = 250 * delay; prescaler = 64; // 16000000 * delay / 64 / 1000
            break;
        }        
        case 4:{ // microseconds: 
            countValue = 2 * delay; prescaler = 8; // 16000000 * delay / 8 / 1000000
            break; 
        }
    }

    // TimerN Creation and Delay
    switch(this->timerNum){
        case 0:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT0 = 256 - (countValue); //Set counter to required level

                    if(usage == INTERRUPT){
                        TIMSK0 = 0b00000001; //Enable Timer/Counter0 overflow interupt 
                    }
                    else{
                        TIFR0 |= 0b00000001; // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR0B = (TCCR0B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR0B = (TCCR0B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR0B = (TCCR0B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR0B = (TCCR0B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        NOP; // Apparently improves performance
                        while( (TIFR0 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR0A = countValue;
                    TCNT0 = 0; //Set Counter to 0

                    if(usage == INTERRUPT){
                        TIMSK0 = 0b00000010; //Enable Counter Interupt Mask Register OCIE0A
                    }
                    else{
                        TIFR0 |= (1 << OCF0A); // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR0B = (TCCR0B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR0B = (TCCR0B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR0B = (TCCR0B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR0B = (TCCR0B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        NOP; // Apparently improves performance
                        while( ((TIFR0 >> 1) & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
            }
            return false;
        }
        case 1:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT1H = (65536 - (countValue)) >> 8; //Set counter to required level
                    TCNT1L = (65536 - (countValue)) & 0x00FF;

                    if(usage == INTERRUPT){
                        TIMSK1 = 0b00000001; //Enable Timer/Counter0 overflow interupt 
                    }
                    else{
                        TIFR1 |= 0b00000001; // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR1B = (TCCR1B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR1B = (TCCR1B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR1B = (TCCR1B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR1B = (TCCR1B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        NOP; // Apparently improves performance
                        while( (TIFR1 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR1AH = countValue >> 8;
                    OCR1AL = countValue & 0x00FF;
                    TCNT1 = 0x0000; //Set Counter to 0

                    if(usage == INTERRUPT){
                        TIMSK1 = 0b00000010; //Enable Counter Interupt Mask Register OCIE0A
                    }
                    else{
                        //TIFR1 |= 0b00000010; // Clear flag bit
                        TIFR1 |= (1 << OCF1A);
                    }

                    switch(prescaler){
                        case 8: TCCR1B = (TCCR1B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR1B = (TCCR1B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR1B = (TCCR1B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR1B = (TCCR1B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        NOP; // Apparently improves performance
                        while( ((TIFR1 >> 1) & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
            }
            return false;
        }
        case 2:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT2 = 256 - (countValue); //Set counter to required level

                    if(usage == INTERRUPT){
                        TIMSK2 = 0b00000001; //Enable Timer/Counter0 overflow interupt 
                    }
                    else{
                        TIFR2 |= 0b00000001; // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR2B = (TCCR2B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR2B = (TCCR2B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR2B = (TCCR2B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR2B = (TCCR2B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        NOP; // Apparently improves performance
                        while( (TIFR2 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR2A = countValue;
                    TCNT2 = 0; //Set Counter to 0

                    if(usage == INTERRUPT){
                        TIMSK2 = 0b00000010; //Enable Counter Interupt Mask Register OCIE0A
                    }
                    else{
                        TIFR2 |= (1 << OCF2A); // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR2B = (TCCR2B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR2B = (TCCR2B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR2B = (TCCR2B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR2B = (TCCR2B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        NOP; // Apparently improves performance
                        while( (TIFR2 >> 1 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
            }
            return false;
        }
        case 3:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT3 = 65536 - (countValue); //Set counter to required level

                    if(usage == INTERRUPT){
                        TIMSK3 = 0b00000001; //Enable Timer/Counter0 overflow interupt 
                    }
                    else{
                        TIFR3 |= 0b00000001; // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR3B = (TCCR3B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR3B = (TCCR3B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR3B = (TCCR3B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR3B = (TCCR3B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        while( (TIFR3 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR3AH = countValue >> 8;
                    OCR3AL = countValue & 0x00FF;
                    TCNT3 = 0x0000; //Set Counter to 0

                    if(usage == INTERRUPT){
                        TIMSK3 = 0b00000010; //Enable Counter Interupt Mask Register OCIE0A
                    }
                    else{
                        TIFR3 |= (1 << OCF3A); // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR3B = (TCCR3B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR3B = (TCCR3B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR3B = (TCCR3B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR3B = (TCCR3B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        while( (TIFR3 >> 1 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
            }
            return false;
        }
        case 4:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT4 = 65536 - (countValue); //Set counter to required level

                    if(usage == INTERRUPT){
                        TIMSK4 = 0b00000001; //Enable Timer/Counter0 overflow interupt 
                    }
                    else{
                        TIFR4 |= 0b00000001; // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR4B = (TCCR4B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR4B = (TCCR4B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR4B = (TCCR4B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR4B = (TCCR4B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        while( (TIFR4 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR4AH = countValue >> 8;
                    OCR4AL = countValue & 0x00FF;
                    TCNT4 = 0x0000; //Set Counter to 0

                    if(usage == INTERRUPT){
                        TIMSK4 = 0b00000010; //Enable Counter Interupt Mask Register OCIE0A
                    }
                    else{
                        TIFR4 |= (1 << OCF4A); // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR4B = (TCCR4B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR4B = (TCCR4B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR4B = (TCCR4B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR4B = (TCCR4B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        while( (TIFR4 >> 1 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
            }
            return false;
        }
        case 5:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT5 = 65536 - (countValue); //Set counter to required level

                    if(usage == INTERRUPT){
                        TIMSK5 = 0b00000001; //Enable Timer/Counter0 overflow interupt 
                    }
                    else{
                        TIFR5 |= 0b00000001; // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR5B = (TCCR5B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR5B = (TCCR5B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR5B = (TCCR5B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR5B = (TCCR5B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        while( (TIFR5 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR5AH = countValue >> 8;
                    OCR5AL = countValue & 0x00FF;
                    TCNT5 = 0x0000; //Set Counter to 0

                    if(usage == INTERRUPT){
                        TIMSK5 = 0b00000010; //Enable Counter Interupt Mask Register OCIE0A
                    }
                    else{
                        TIFR5 |= (1 << OCF5A); // Clear flag bit
                    }

                    switch(prescaler){
                        case 8: TCCR5B = (TCCR5B & 0b11111000) | 0b00000010; break;
                        case 64: TCCR5B = (TCCR5B & 0b11111000) | 0b00000011; break;
                        case 256: TCCR5B = (TCCR5B & 0b11111000) | 0b00000100; break;
                        case 1024: TCCR5B = (TCCR5B & 0b11111000) | 0b00000101; break;
                    }

                    if(usage == PAUSE){
                        while( (TIFR5 >> 1 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
            }
            return false;
        }
    }

    return false;
}


// Make timer delay CPU; same usage as Arduino's delay(ms)
bool timer::delay_timer(unsigned int delay){ return delay_timer(delay, PAUSE); }