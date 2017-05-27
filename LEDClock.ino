/*
 * LEDClock.pde
 * Code to control the RTC and WS2812B LEDs to create a clock
 * 
 */

#include <FastLED.h>  // for controlling the LEDs
#include <TimeLib.h>  // for controlling the RTC
#include <DS1307RTC.h>
#include <Wire.h>

// Pin constants
#define DATA_PIN 3
#define CLOCK_PIN 13

// LED indexing constants
#define NUM_LEDS 132
#define MIN_OFFSET 60
#define HR_OFFSET 120

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

CRGB leds[NUM_LEDS];
tmElements_t tm;

void setup()
{
//  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
//  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(9600);
  while(!Serial);
  setSyncProvider(RTC.get);
  if(timeStatus() != timeSet){
    bool parse = false;
    bool config = false;

    if(getDate(__DATE__) && getTime(__TIME__)){
      parse = true;
      if(RTC.write(tm)){
        config = true;
      }
    }
  }
  else
    Serial.println("RTC has set the system time");
}

void loop()
{ 
  if(timeStatus() == timeSet){
    updateClock();
    printClock();
  }
  delay(1000);
}

void printClock()
{
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void updateClock()
{
  if(second() == 0){
    leds[59] = CRGB::Black;
    leds[0] = CRGB::Red;
    if(minute() == 0){
      leds[MIN_OFFSET + 59] = CRGB::Black;
      leds[MIN_OFFSET] = CRGB::Red;
      if(hour() % 12 == 0){
        leds[HR_OFFSET + 11] = CRGB::Black;
        leds[HR_OFFSET] = CRGB::Red;
      }
      leds[HR_OFFSET + (hour() % 12) - 1] = CRGB::Black;
      leds[HR_OFFSET + (hour() % 12)] = CRGB::Red;
    }
    leds[MIN_OFFSET + minute() - 1] = CRGB::Black;
    leds[MIN_OFFSET + minute()] = CRGB::Red; 
  }
  FastLED.show();
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
