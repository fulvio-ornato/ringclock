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
#define DEBUG          0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12
int lightPin = 0;
int ledPin = 13; // test per la programmazione
RTC_DS1307 RTC;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int ora,minuto,minuto2,secondo,intensita=150,miaIntensita;
int buzzer = 32;     //buzzer connesso al pin 3


uint32_t hourRedColor;
uint32_t minuteGreenColor,minuteGreenColor2;
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
    minuteGreenColor  = strip.Color(0, miaIntensita/(now.minute()-minuto*5+1), 0);
    Serial.print("intensita green led 1=");Serial.println(miaIntensita/(now.minute()-minuto*5+1));
    strip.setPixelColor(minuto, minuteGreenColor);
    //accendo il secondo green solo se non sono sul 5,10,15 ....
    if (now.minute()%5 != 0) {
      minuto2 = (minuto+1)%12;
      minuteGreenColor2 = strip.Color(0, miaIntensita/(minuto2*5-now.minute()), 0);
      Serial.print("intensita green led 2=");Serial.println(miaIntensita/(minuto2*5-now.minute()));
      strip.setPixelColor(minuto2, minuteGreenColor2);
    } 
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
  //Serial.println("sto in verifica");
  int ore = 1;
  int min=0;
  if (digitalRead(btnProgramma) == LOW) {
    delay(100);
      if (digitalRead(btnProgramma) == LOW) {
        Serial.println("stai programmando");
        beep();
        spegniTutto();
        delay(1000);
        while(1) {
          if (digitalRead(btnAvanza) == LOW) {
            delay(100);
            if (digitalRead(btnAvanza) == LOW) {
              delay(500);
              strip.setPixelColor(ore, hourRedColor);
              if (ore==0) {
                strip.setPixelColor(11, blackColor);
              } else {
                strip.setPixelColor(ore-1, blackColor);
              }
              strip.show();
              ore++;
              ore=ore%12;
            }
          }
         if (digitalRead(btnProgramma) == LOW) {
            if (ore==0) {ore=12;}
            delay(100);
              if (digitalRead(btnProgramma) == LOW) {
                min=settaMinuti();
                //RTC.adjust(DateTime(_year, _month, _day, _hour, _min, _sec));
                RTC.adjust(DateTime(2016, 1, 1, ore-1, min, 0));
                Serial.println("imposto ora");
                Serial.println(ore-1);
                spegniTutto();           
                return;
              }
          }
        }
        //digitalWrite(ledPin, HIGH);           // turn on LED
      }
  }
}

int settaMinuti() {
  beep();
  spegniTutto();
  delay(1000);
  Serial.println("sto in settaminuti");
  int min = 1;
        while(1) {
          if (digitalRead(btnProgramma) == LOW) {
            delay(100);
              if (digitalRead(btnProgramma) == LOW) {
                Serial.println("imposto minuti");
                if (min<=0) {min=59;}
                Serial.println(min-1);
                return min-1;
              }
          }
          if (digitalRead(btnAvanza) == LOW) {
            delay(100);
            if (digitalRead(btnAvanza) == LOW) {
              delay(500);
              strip.setPixelColor(min%12, minuteGreenColor);
              if (min%12==0) {
                strip.setPixelColor(11, blackColor);
              } else {
                strip.setPixelColor((min-1)%12, blackColor);
              }
              strip.show();
              min++;
              Serial.println("sono arrivato a contare minuti:");
              Serial.println(min);
              min=min%60;
            }
          }
        }
}

void hourlyBeep(){
//beep every hour using buzzer
  if (now.minute() == 0 && 
      now.second() == 0 ) {
    beep();        
  }
  if (now.minute()%12 == 0 &&
      now.minute() == 0 && 
      now.second() == 0 ) {
    beep();        
  }
}

void beep() {
  tone(buzzer,2000,50);
}

void spegniTutto() {
  for (int st=0; st<12; st++) {
    strip.setPixelColor(st, strip.Color(0,0,0));
  }
  strip.show();
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

    if (DEBUG || 1>0) {      
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

