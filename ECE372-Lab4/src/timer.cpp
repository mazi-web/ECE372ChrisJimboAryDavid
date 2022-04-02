// Author:         James Fulton
// Net ID:         23413392
// Date:           21 March 2022
// Assignment: ECE-372 Lab 3
//
// File: timer.cpp

#include "timer.h"

// 8-bit timer
// mode:           TOP    Update     TOV_Flag_Set
// 0->normal       0xFF   Immediate  Max: 0xFF
// 1->PWM_C        0xFF   Top        Bottom: 0x00
// 2->CTC          OCRA   Immediate  Max: 0xFF
// 3->FAST_PWM     0xFF   Top        Max: 0xFF
// 4->PWM_C        OCRA   Top        Bottom: 0x00
// 5->FAST_PWM     OCRA   Bottom     Top: 0xFF

// 16-bit timer                 WGMn    TOP     Update of OCRnX     TOVn Flag Set
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

// Currently designed for no external source or output: timers 0, 1, 2, 3 only avaliable
// Modes 0 & 2 only avaliable (only timer0 modes are listed above)
bool timer::init_timer(unsigned int newTimerNum, unsigned int newMode, unsigned int newTimerPrefix){

    this->timerNum = newTimerNum;
    this->mode = newMode;
    this->timerPrefix = newTimerPrefix;

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
                    TCCR1B &= 0b11100111;
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
                    TCCR3B &= 0b11100111;
                    TCCR3C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR3A = 0x00;
                    TCCR3B = (TCCR3B & 0b11100111) | 0b00001000;
                    TCCR3C = 0x00;
                    return true;
                }
            }
        }
        case 4:{
            switch(this->mode){
               case 0:{
                    TCCR4A = 0x00;
                    TCCR4B &= 0b11100111;
                    TCCR4C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR4A = 0x00;
                    TCCR4B = (TCCR4B & 0b11100111) | 0b00001000;
                    TCCR4C = 0x00;
                    return true;
                }
            }
        }
        case 5:{
            switch(this->mode){
               case 0:{
                    TCCR5A = 0x00;
                    TCCR5B &= 0b11100111;
                    TCCR5C = 0x00;
                    return true;
                }
                case 2:{
                    TCCR5A = 0x00;
                    TCCR5B = (TCCR5B & 0b11100111) | 0b00001000;
                    TCCR5C = 0x00;
                    return true;
                }
            }
        }
    }

    return false;
}

bool timer::stop_timer(char newTimerLetter){
    switch(this->timerNum){
        case 0:{
            if(newTimerLetter == 'A' || newTimerLetter == 'a'){ TIMSK0 &= 0b11111101; }
            else if(newTimerLetter == 'B' || newTimerLetter == 'b'){ TIMSK0 &= 0b11111011; }
            else{ TIMSK0 = 0x00; }

            if(TIMSK0 == 0x00 || mode == 2){
                TCCR0B = 0x00; // Stop timer by setting no clock source
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
                TCCR1B = 0x00; // Stop timer by setting no clock source
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
                TCCR2B = 0x00; // Stop timer by setting no clock source
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

            if(TIMSK3 == 0x00 || mode){
                TCCR3B = 0x00; // Stop timer by setting no clock source
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

            if(TIMSK4 == 0x00 || mode){
                TCCR4B = 0x00; // Stop timer by setting no clock source
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
                TCCR5B = 0x00; // Stop timer by setting no clock source
                OCR5A = 0x00;
                OCR5B = 0x00;
                OCR5C = 0x00;
                return true;
            }
        }
    }

    return false;
}
bool timer::stop_timer(){ return stop_timer('Z'); }


bool timer::delay_timer(unsigned int delay, bool isTemporary){
    // end if current timer is active
    //if((newTimerLetter == 'A' || newTimerLetter == 'a') && (activeTimers & 0x01 == 1)){ return false; }
    //else if((newTimerLetter == 'B' || newTimerLetter == 'b') && ((activeTimers >> 1) & 0x01 == 1)){ return false; }
    //else if((newTimerLetter == 'C' || newTimerLetter == 'c') && ((activeTimers >> 2) & 0x01 == 1)){ return false; }

    unsigned int ocrValue = 100;
    unsigned int prescaler = 1024;


    switch(this->timerPrefix){
        case 1:{  // seconds
            if(timerNum == 0 || timerNum == 2){ return false; }
            else{ ocrValue = 15625 * delay; prescaler = 1024; } // 16000000 * delay / 1024
            break;
        }
        case 2:{  // miliseconds
            if(timerNum == 0 || timerNum == 2){ ocrValue = 62 * delay; prescaler = 256; } // 16000000 * delay / 256 / 1000
            else { ocrValue = 250 * delay; prescaler = 64;  } // 16000000 * delay / 64 / 1000
            break;
        }
        case 3: case 4: case 5:{ ocrValue = 2 * delay; prescaler = 8; break; } // microseconds: 16000000 * delay / 8 / 1000000
    }

    switch(this->timerNum){
        case 0:{
            switch(this->mode){
                case 0:{ // Normal
                    cli();
                    TCNT0 = 256 - (ocrValue); //Set counter to required level

                    if(!isTemporary){
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

                    if(isTemporary){
                        while( (TIFR0 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR0A = ocrValue;
                    TCNT0 = 0; //Set Counter to 0

                    if(!isTemporary){
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

                    if(isTemporary){
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
                    TCNT1H = (65536 - (ocrValue)) >> 8; //Set counter to required level
                    TCNT1L = (65536 - (ocrValue)) & 0x00FF;

                    if(!isTemporary){
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

                    if(isTemporary){
                        while( (TIFR1 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR1AH = ocrValue >> 8;
                    OCR1AL = ocrValue & 0x00FF;
                    TCNT1 = 0x0000; //Set Counter to 0

                    if(!isTemporary){
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

                    if(isTemporary){
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
                    TCNT2 = 256 - (ocrValue); //Set counter to required level

                    if(!isTemporary){
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

                    if(isTemporary){
                        while( (TIFR2 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR2A = ocrValue;
                    TCNT2 = 0; //Set Counter to 0

                    if(!isTemporary){
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

                    if(isTemporary){
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
                    TCNT3 = 65536 - (ocrValue); //Set counter to required level

                    if(!isTemporary){
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

                    if(isTemporary){
                        while( (TIFR3 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR3AH = ocrValue >> 8;
                    OCR3AL = ocrValue & 0x00FF;
                    TCNT3 = 0x0000; //Set Counter to 0

                    if(!isTemporary){
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

                    if(isTemporary){
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
                    TCNT4 = 65536 - (ocrValue); //Set counter to required level

                    if(!isTemporary){
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

                    if(isTemporary){
                        while( (TIFR4 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR4AH = ocrValue >> 8;
                    OCR4AL = ocrValue & 0x00FF;
                    TCNT4 = 0x0000; //Set Counter to 0

                    if(!isTemporary){
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

                    if(isTemporary){
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
                    TCNT5 = 65536 - (ocrValue); //Set counter to required level

                    if(!isTemporary){
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

                    if(isTemporary){
                        while( (TIFR5 & 0x01) == 0);
                        this->stop_timer();
                    }
                    sei();
                    return true;
                }
                case 2:{ // CTC
                    cli();
                    OCR5AH = ocrValue >> 8;
                    OCR5AL = ocrValue & 0x00FF;
                    TCNT5 = 0x0000; //Set Counter to 0

                    if(!isTemporary){
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

                    if(isTemporary){
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