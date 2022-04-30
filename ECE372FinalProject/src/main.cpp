// MSGEQ7 I/O pins
#define msg7RESET 6
#define msg7Strobe 7
#define msg7DCout A0
#define potentiometer 1

// WS2811 compatible NeoPixel library is used
#include <Adafruit_NeoPixel.h>
#define LED_COUNT 50
#define PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// Defining RGB colors, to be used later
uint32_t blue = strip.Color(0, 0, 255);
uint32_t red = strip.Color(0, 255, 0);
uint32_t white = strip.Color(50, 50, 50);
uint32_t pink = strip.Color(0, 255, 255);
uint32_t orange = strip.Color(153, 255, 51);
uint32_t green = strip.Color(153, 0, 76);
uint32_t light_blue = strip.Color(50, 50, 255);
uint32_t random_color;

uint32_t color_array_chooser[6] = {blue, red, pink, orange, green, light_blue};

// The two frequency measure variables are the 'bands' that are to be read by the msgeq7;
// bands 1 and 0 are the lowest frequency bands, with bands 6 and 7 being the highest frequency bands.
int frequencyMeasure = 0;
int frequencyMeasure2 = 1;
int n = 0;

// Counter variables used to create the 'moving' effects of the lights changing color
int counter_index = 0;
//int time_index = 0;
uint32_t color_arr[50];
//int high = 0;


//SETUP//
void setup() {       
  // Set the I/O pins to output         
  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show();

  for (int i = 0; i < 50; i++) {
    color_arr[i] = white;
  }

  random_color = color_array_chooser[random(sizeof(color_array_chooser) - 1)];
 }


//LOOP//
void loop() {
  
  digitalWrite(msg7RESET, HIGH);      // reset the MSGEQ7's counter
  delay(5);
  digitalWrite(msg7RESET, LOW);       // send the reset signal to the MSGEQ7
    int magnitudes[7];                  // array that stores the 7 frequency band values
  
  
    //READING THE MSGEQ7//
    for (int x = 0; x < 7; x++)         // save each frequency band value to its respective array location
    {
    digitalWrite(msg7Strobe, LOW);    // output each DC value for each freq band
    delayMicroseconds(35);            // to allow the output to settle
    int spectrumRead = analogRead(msg7DCout);
    digitalWrite(msg7Strobe, HIGH);
    magnitudes[x]=spectrumRead;
    Serial.print(spectrumRead);       // print the frequency band value to the serial monitor
    Serial.print(" ");                // print a space inbetween each value to the serial monitor 
  }

  //CALCULATING HOW MANY LIGHTS NEED TO LIGHT UP
  n = pow(((magnitudes[frequencyMeasure] + magnitudes[frequencyMeasure2]) / 2), 2) * 25 / pow(1023.0, 2); //this transforms the magnitude of the signal into a number between 0 and 25 which is also the number of lights on either side of the center
  //n = ((magnitudes[frequencyMeasure] + magnitudes[frequencyMeasure2]) / 2) * 25 / 1023;
  
  //TRANSFORMING USING POTENTIOMETER
  
  double potentReading = analogRead(potentiometer);
  
  potentReading *= (10.0 / 1024);
  /*
  n *= potentReading;
  */
  

  // Transform n
  if (n < potentReading) {
    n = 0;
    //high = 0;
  }
  else {
    n = 3;
    //high = 1;
  }
 


  
  //SET LIGHTS TO NEW VALUES
  for (int i = 0; i < 2; i++) {
    color_arr[25 - i] = white;
    color_arr[25 + i] = white;
  }
  for (int i = 0; i < 25; i++) {
    strip.setPixelColor(i, color_arr[i]);
    if (i == 24) {
      color_arr[i] = white;
    }
    else {
      color_arr[i] = color_arr[i + 1];
    }
  }
  for (int i = 49; i > 24; i--) {
    strip.setPixelColor(i, color_arr[i]);
    if (i == 25) {
      color_arr[i] = white;
    }
    else {
      color_arr[i] = color_arr[i - 1];
    }
  }
  //random_color = color_array_chooser[random(sizeof(color_array_chooser) - 1)];
  for (int i = 0; i < n; i++) {
    color_arr[25 - i] = blue;
    strip.setPixelColor(25 - i, blue);
    color_arr[25 + i] = blue;
    strip.setPixelColor(25 + i, blue);
  }
 
  //UPDATING THE LIGHTS
   strip.show();
  
   
  //PRINT TO SERIAL MONITOR//


  // UPDATE INCREMENTERS
  //time_index++;
  //if (time_index < 550) {
    //time_index++;
  //}
  counter_index++;
  /*
  if (counter_index > 500) {
    counter_index = 0;
    random_color = color_array_chooser[random(sizeof(color_array_chooser) - 1)];
  }
  */
  delay(10);

}