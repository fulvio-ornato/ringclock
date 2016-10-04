// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
 
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define DEBUG          0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12
int lightPin = 0;
RTC_DS1307 RTC;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int ora,minuto,secondo,intensita=150,miaIntensita;
uint32_t hourRedColor;
uint32_t minuteGreenColor;
uint32_t secondBlueColor;
uint32_t blackColor = strip.Color(0, 0, 0);

DateTime now;
 
void setup () {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code  
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  
  strip.begin(); // This initializes the NeoPixel library.
  strip.show(); // Initialize all pixels to 'off'
}

void setIntensitaLuce() {
    if (DEBUG) {
      Serial.print("luce ");
      Serial.println(analogRead(lightPin));
    }
    miaIntensita=intensita - analogRead(lightPin)/5;
    if (DEBUG) {
      Serial.print("intensita ");
      Serial.println(miaIntensita);
    }
    if (miaIntensita < 5) {miaIntensita=5;}
    hourRedColor = strip.Color(miaIntensita, 0, 0);
    minuteGreenColor = strip.Color(0, miaIntensita, 0);
    secondBlueColor = strip.Color(0, 0, miaIntensita);
}

void setSecondi() {
    secondo = ((int)now.second())/5;
    strip.setPixelColor(secondo, secondBlueColor);
    if (secondo>=1) {
       strip.setPixelColor(secondo-1, blackColor);
    } else {
       strip.setPixelColor(11, blackColor);
    }
}

void setMinuti(){
    minuto = ((int)now.minute())/5;
    strip.setPixelColor(minuto, minuteGreenColor);
    if (minuto>=1) {
       strip.setPixelColor(minuto-1, blackColor);
    } else {
       strip.setPixelColor(11, blackColor);
    }
}

void setOre(){
    ora = ((int)now.hour())%12;
    strip.setPixelColor(ora, hourRedColor);
    strip.show();
}

void loop () {
    setIntensitaLuce();
    
    now = RTC.now();

    setSecondi();
    setMinuti();
    setOre();
  
    strip.show();

    if (DEBUG) {      
      Serial.println();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
       
      Serial.println();
    }
    
    delay(1000);
}


