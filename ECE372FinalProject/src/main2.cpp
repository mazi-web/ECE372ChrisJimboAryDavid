// #include <Arduino.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include "switch.h"
// #include "timer.h"
// #include "adc.h"
// #include "oldadc.h"
// #include "i2c.h"
// #include "spi.h"
// #include "fft.h"
// #include "led.h"
// #define msg7RESET 6
// #define msg7Strobe 7
// #define msg7DCout A0
// #define msg7DCouto A1
// const int LEDpins[7] = {3,5,5,6,9,9,10}; // there are 5 LEDs and 7 freq bands. So, repeat LEDs

// #define pushButton 2

// //int main() {
// // initADC();
// //   Serial.begin(9600);
// //   unsigned int result = 0;
// //   float voltage = 0;

// //   while(1){
// //       // print out ADC value
// // 	// read in ADCL first then read ADCH
// //       result = ADCL;
// //       result += ((unsigned int) ADCH) << 8;
// //       voltage = result * (2.56/1024.0);
// //       Serial.println(voltage,2);
// //   }

// void setup() {
//    // initialize the digital pin as an output.
//    // Pin 13 has an LED connected on most Arduino boards:
//    //for (int x=0; x<7; x++) {
//    //   pinMode(LEDpins[x], OUTPUT);
//    //}
//    pinMode(msg7RESET, OUTPUT);
//    pinMode(msg7Strobe, OUTPUT);
//    pinMode(msg7DCout, INPUT);
//    Serial.begin(9600);

//    //pinMode(pushButton, INPUT); // never actually used in this example.
//    //digitalWrite(pushButton, HIGH); // Enable internal pull-up
// }

// void loop() {
//    digitalWrite(msg7RESET, HIGH); // reset the MSGEQ7's counter
//    delay(5);
//    digitalWrite(msg7RESET, LOW);
//    int magnitudes[7];

//    for (int x = 0; x < 7; x++){
//       digitalWrite(msg7Strobe, LOW); // output each DC value for each freq band
//       delayMicroseconds(35); // to allow the output to settle
//       int spectrumRead = analogRead(msg7DCouto);

//       // int PWMvalue = map(spectrumRead, 0, 1024, 0, 255); // scale analogRead's value to Write's 255 max
//       // if (PWMvalue < 50)
//       //    PWMvalue = PWMvalue / 2; // bit of a noise filter, so the LEDs turn off at low levels

//       // analogWrite(LEDpins[x], PWMvalue);
//       digitalWrite(msg7Strobe, HIGH);
//       magnitudes[x] = spectrumRead;
//       Serial.print(spectrumRead);
//       Serial.print(" ");
//    }
// }
// //}