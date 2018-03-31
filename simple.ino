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
#define DEBUG 0

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
long counter = 0;
int h,m,m1,s;
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
  h = int(counter / 60 / 60)%12;
  m = int(counter / 60)%60;
  s = counter%60;

  if (DEBUG) {
    Serial.println("counter="+String(counter));
    Serial.println("hours="+String(h));
    Serial.println("mins=" +String(m));
    Serial.println("secs=" +String(s));
  }

  m1 = m%5;
  m = int(m/5);
  s = int(s/5);

  if (DEBUG) {
    Serial.println("hours2="+String(h));
    Serial.println("mins2=" +String(m));
    Serial.println("m1=" +String(m1));
    Serial.println("secs2=" +String(s));
  }

  /*for(int h=0;h<NUMPIXELS;h++){
    for(int m=0;m<NUMPIXELS;m++){
      for(int s=0;s<NUMPIXELS;s++){*/

        //set seconds
        if (s==0) ledTurnOffSecs=11; else ledTurnOffSecs=s-1;
        pixels.setPixelColor(ledTurnOffSecs, pixels.Color(0,0,0));

        //sets minutes
        if (m==0) 
          ledTurnOffMins=11;
        else 
          ledTurnOffMins=m-1;
        pixels.setPixelColor(ledTurnOffMins, pixels.Color(0,0,0));
          

        //sets hours
        if (h==0) ledTurnOffHour=11; else ledTurnOffHour=h-1;
        pixels.setPixelColor(ledTurnOffHour, pixels.Color(0,0,0));
          
        pixels.setPixelColor(s, pixels.Color(0,0,brightness));
        pixels.setPixelColor(m, pixels.Color(0,brightness-brightness*m1/5,0));
        if (m1>0 and m!=11)
          pixels.setPixelColor(m+1, pixels.Color(0,brightness*m1/5,0));
        if (m1>0 and m==11)
          pixels.setPixelColor(0, pixels.Color(0,brightness*m1/5,0));
        pixels.setPixelColor(h, pixels.Color(brightness,0,0));
    
        pixels.show();
    
        //delay(delayval); 
        
  /*    }
    }
  }*/
  
  counter += 1;
  if (counter==86400) counter = 0;
  delay(1000);
}
