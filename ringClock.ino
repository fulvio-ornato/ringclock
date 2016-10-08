//http://www.mauroalfieri.it/elettronica/rtc-ds1307.html
//http://www.bristolwatch.com/arduino/arduino_ds1307.htm

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
 
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define DEBUG          1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12
int lightPin = 0;
int ledPin = 13; // test per la programmazione
RTC_DS1307 RTC;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int ora,minuto,secondo,intensita=150,miaIntensita;
int buzzer = 32;     //buzzer connesso al pin 3

uint32_t hourRedColor;
uint32_t minuteGreenColor;
uint32_t secondBlueColor;
uint32_t blackColor = strip.Color(0, 0, 0);


DateTime now;

int btnProgramma = 40;  // change to whatever you want
int btnAvanza = 42;  // change to whatever you want

void setup () {
    pinMode(btnProgramma, INPUT);    // button as input
    digitalWrite(btnProgramma, HIGH); // turns on pull-up resistor after input
    pinMode(btnAvanza, INPUT);    // button as input
    digitalWrite(btnAvanza, HIGH); // turns on pull-up resistor after input

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code  
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
    RTC.writeSqwPinMode(SquareWave1HZ);
 
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
    if (miaIntensita > 150) {miaIntensita=150;}
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
    //spengo il pixel precedente a meno che nn ci siano i secondi attivi
    if (minuto>=1 && (minuto-1)!=secondo) {
       strip.setPixelColor(minuto-1, blackColor);
    } else {
      if (secondo!=11) {
        strip.setPixelColor(11, blackColor);
      }      
    }
}

void setOre(){
    ora = ((int)now.hour())%12;
    strip.setPixelColor(ora, hourRedColor);
    strip.show();
}

void verificaProgrammazione(){
  Serial.println("sto in verifica");
  int i = 0;
  uint32_t rndColor;
  if (digitalRead(btnProgramma) == LOW) {
    delay(50);
      if (digitalRead(btnProgramma) == LOW) {
        Serial.println("stai programmando");
        while(1) {
          rndColor = strip.Color(random(0, 255), random(0, 255), random(0, 255));
        if (i==13) {
            return;
          }
          strip.setPixelColor(i, rndColor);
          if (i>0) {
            strip.setPixelColor(i-1, strip.Color(0,0,0));
          } else {
            strip.setPixelColor(11, strip.Color(0,0,0));
          }
        
          strip.show();
          i += 1;          
          delay(1000);
          }
        //digitalWrite(ledPin, HIGH);           // turn on LED
      }
  }
}

void hourlyBeep(){
//beep every hour using buzzer
    if (now.minute() == 0 && 
        now.second() == 0 ) {
      tone(buzzer,2000,50);
    }
}

void loop () {
    verificaProgrammazione();
    
    setIntensitaLuce();

    now = RTC.now();
    hourlyBeep();

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

