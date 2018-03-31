// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int ledTurnOffSecs = 0;
int ledTurnOffMins = 0;
int ledTurnOffHour = 0;
int brightness = 50;
DateTime now;


void setup() {
  Serial.begin(9600);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.setBrightness(brightness);
  pixels.begin();
}

void loop() {
  Serial.println(int(millis()/1000));
  for(int h=0;h<NUMPIXELS;h++){
    for(int m=0;m<NUMPIXELS;m++){
      for(int s=0;s<NUMPIXELS;s++){
        if (s==0) ledTurnOffSecs=11; else ledTurnOffSecs=s-1;
        if (m==0) ledTurnOffMins=11; else ledTurnOffMins=m-1;
        if (h==0) ledTurnOffHour=11; else ledTurnOffHour=h-1;
    
        pixels.setPixelColor(ledTurnOffSecs, pixels.Color(0,0,0));
        pixels.setPixelColor(ledTurnOffMins, pixels.Color(0,0,0));
        pixels.setPixelColor(ledTurnOffHour, pixels.Color(0,0,0));
          
        pixels.setPixelColor(s, pixels.Color(0,0,brightness));
        pixels.setPixelColor(m, pixels.Color(0,brightness,0));
        pixels.setPixelColor(h, pixels.Color(brightness,0,0));
    
        pixels.show();
    
        delay(delayval); 
      }
    }
  }
}
