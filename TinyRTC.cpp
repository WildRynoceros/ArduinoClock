// Original code by JeeLabs http://news.jeelabs.org/code/
// Modified by WildRynoceros

#include <Wire.h>
#include "TinyRTC.h"

#define DS1307_ADDRESS 0x68     // Defined in datasheet
#define SECONDS_PER_DAY 86400L

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

static uint8_t RTC_Time::conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

RTC_Time:: RTC_Time() {
    setYear(2000);
    setMonth(1);
    setDay(1);
    setDayOfWeek();
    setHour(0);
    setMinute(0);
    setSecond(0);
}
// RTC_Time ////////////////////////////////////////////////////////////////////
// RTC_Time:RTC_Time(uint16_t year = 2000, uint8_t month = 1, uint8_t day = 1,
//                   uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
// Input:
//      year - year to initialize with (should be >= 2000 and < 2100, will default to 2000 otherwise)
//      month - month to initialize with (should be 1-12, will default to 1 otherwise)
//      day - date to initialize with (should be 1-31, will default to 1 otherwise)
//      hour - hour to initialize with (should be 0-23, will default to 0 otherwise)
//      min - minute to initialize with (should be 0-59, will default to 0 otherwise)
//      sec - second to initialize with (should be 0-59, will default to 0 otherwise)
// Effect:
//      Constructs a RTC_Time object
RTC_Time::RTC_Time (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    setYear(year);
    setMonth(month);
    setDay(day);
    setDayOfWeek();
    setHour(hour);
    setMinute(min);
    setSecond(sec);
}

RTC_Time::RTC_Time (uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t min, uint8_t sec) {
    setYear(year);
    setMonth(month);
    setDay(day);
    setDayOfWeek(dow);
    setHour(hour);
    setMinute(min);
    setSecond(sec);
}

// DateTime:DateTime(const char* date, const char* time)
// Input:
//      date - A date string
//      time - A time string
// Effect:
//      Constructs a DateTime object
// Notes:
//      Date string formatted like "MMM DD YYYY"
//      where months are this:
//      Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
//
//      Time string formatted like "HH:MM:SS"
//
//      You can use compiler time as so:
//      DateTime t(__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
RTC_Time::RTC_Time (char* date, char* time) {
    Y = conv2d(date + 9);
    switch (date[0]) {
        case 'J': M = date[1] == 'a' ? 1 : M = date[2] == 'n' ? 6 : 7; break;
        case 'F': M = 2; break;
        case 'A': M = date[2] == 'r' ? 4 : 8; break;
        case 'M': M = date[2] == 'r' ? 3 : 5; break;
        case 'S': M = 9; break;
        case 'O': M = 10; break;
        case 'N': M = 11; break;
        case 'D': M = 12; break;
    }
    setDay(conv2d(date + 4));
    setDayOfWeek();
    setHour(conv2d(time));
    setMinute(conv2d(time + 3));
    setSecond(conv2d(time + 6));
}

void RTC_Time::setYear(uint16_t y) {
    if(y < 2000 || y >= 2100){
        y = 2000;
    }
    y -= 2000;
    Y = y;
    if(Y % 4 == 0) {
        DaysInMonth[1] = 29;
    }
    else {
        DaysInMonth[1] = 28;
    }
    setDayOfWeek();
}

void RTC_Time::setMonth(uint8_t m) {
    if(m < 1 || m > 12) {
        m = 1;
    }
    M = m;
    if(D > DaysInMonth[M-1]) {
        D = DaysInMonth[M-1];
    }
}

void RTC_Time::setDay(uint8_t d) {
    if(d < 1 || d > DaysInMonth[M-1]) {
        d = 1;
    }
    D = d;
}

void RTC_Time::setDayOfWeek(void) {
    DOW = getDDayOfWeek();
}

void RTC_Time::setDayOfWeek(uint8_t dow) {
    if(dow < 1 || dow > 7) {
        dow = 1;
    }
    DOW = dow;
}

void RTC_Time::setHour(uint8_t h) {
    if(h < 0 || h > 23){
        h = 0;
    }
    HH = h;
}

void RTC_Time::setMinute(uint8_t m) {
    if(m < 0 || m > 59){
        m = 0;
    }
    MM = m;
}

void RTC_Time::setSecond(uint8_t s) {
    if(s < 0 || s > 59){
        s = 0;
    }
    SS = s;
}

uint16_t RTC_Time::getDDayOfWeek(uint16_t y, uint8_t m, uint8_t d) const {
    static uint16_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return 1 + ((y + y/4 - y/100 + y/400 + t[m-1] + (uint16_t)d) % 7);
}

void RTC_Time::operator++() {
    if(++SS >= 60) {
        SS = 0;
        if(++MM >= 60) {
            MM = 0;
            if(++HH >= 24) {
                HH = 0;
                if(++DOW > 7) {
                    DOW = 1;
                }
                if(++D > DaysInMonth[M-1]) {
                    D = 1;
                    if(++M > 12) {
                        M = 1;
                        if(++Y % 4 == 0) {
                            DaysInMonth[1] = 29;
                        }
                        else {
                            DaysInMonth[1] = 28;
                        }
                    }
                }
            }
        }
    }
}

void RTC_Time::operator=(const RTC_Time &T) {
    Y = T.Y;
    M = T.M;
    D = T.D;
    DOW = T.DOW;
    HH = T.HH;
    MM = T.MM;
    SS = T.SS;
}
uint16_t RTC_Time::getDDayOfWeek(void) const {
    return getDDayOfWeek(getYear(), getMonth(), getDay());
}

void RTC_Time::printTime(void) {
    Serial.print(getDayOfWeek());
    Serial.print(" ");
    Serial.print(getMonth());
    Serial.print("/");
    Serial.print(getDay());
    Serial.print("/");
    Serial.print(getYear());
    Serial.print(" ");
    Serial.print(getHour());
    Serial.print(":");
    Serial.print(getMinute());
    Serial.print(":");
    Serial.print(getSecond());
    Serial.println();
}
static uint8_t RTC_DS1307::bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t RTC_DS1307::bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

#if (ARDUINO >= 100)

RTC_DS1307::RTC_DS1307(void) {
    Wire.begin();
}

bool RTC_DS1307::enable(void) {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00); //Set the register pointer
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 1); //Request from 0x00
    uint8_t ss = bcd2bin(Wire.read());
    ss = ss & 0x7F; //Clear CH bit but preserve seconds

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00);
    Wire.write(bin2bcd(ss));
    return (Wire.endTransmission() == 0);
}

bool RTC_DS1307::disable(void) {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00); //Set the register pointer
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 1);
    uint8_t ss = bcd2bin(Wire.read());

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00);
    ss = ss | 0x80; //Enable the CH bit at 00h but preserve the seconds
    Wire.write(bin2bcd(ss)); //Enable the CH bit at 00h
    return (Wire.endTransmission() == 0);
}

bool RTC_DS1307::isRunning(void) {
    setPointer(0x00);

    Wire.requestFrom(DS1307_ADDRESS, 1); //Request from 0x00
    uint8_t ss = bcd2bin(Wire.read());
    ss = ss >> 7;
    return !ss;
}

uint8_t RTC_DS1307::setTime(const RTC_Time& t) {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00); //Set address pointer
    Wire.write(bin2bcd(t.getSecond())); //00h
    Wire.write(bin2bcd(t.getMinute())); //01h
    Wire.write(bin2bcd(t.getHour())); //02h
    Wire.write(bin2bcd(t.getDayOfWeek())); //03h
    Wire.write(bin2bcd(t.getDay()));  //04h
    Wire.write(bin2bcd(t.getMonth()));  //05h
    Wire.write(bin2bcd(t.getYear() - 2000));  //06h
    return Wire.endTransmission();
}

uint8_t RTC_DS1307::setPointer(uint8_t addr) {
    if(addr > 0x3F) {
        return -1;
    }
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(addr);
    return Wire.endTransmission();
}

uint8_t RTC_DS1307::write(uint8_t addr, uint8_t size, uint8_t* data) {
    if(addr > 0x3F) {
        return -1;
    }
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(addr);
    for(uint8_t i = 0; i < size; i++) {
        Wire.write(data[i]);
    }
    return Wire.endTransmission();
}

void RTC_DS1307::read(uint8_t addr, uint8_t size, uint8_t* data) {
    setPointer(addr);

    Wire.requestFrom(DS1307_ADDRESS, size);
    for(uint8_t i = 0; i < size; i++) {
        data[i] = Wire.read();
    }
}

RTC_Time RTC_DS1307::getTime(void) {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 7);
    uint8_t ss = bcd2bin(Wire.read() & 0x7F);
    uint8_t mm = bcd2bin(Wire.read());
    uint8_t hh = bcd2bin(Wire.read());
    uint8_t dow = bcd2bin(Wire.read());
    uint8_t d = bcd2bin(Wire.read());
    uint8_t m = bcd2bin(Wire.read());
    uint16_t y = bcd2bin(Wire.read()) + 2000;

    return RTC_Time(y, m, d, dow, hh, mm, ss);
}

void RTC_DS1307::enableOscillator(uint8_t f) {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x07);
    switch(f) {
        case 0:
            Wire.write(0x10);
            break;
        case 1:
            Wire.write(0x11);
            break;
        case 2:
            Wire.write(0x12);
            break;
        case 3:
            Wire.write(0x13);
            break;
    }
    Wire.endTransmission();
}

void RTC_DS1307::disableOscillator(uint8_t val) {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x07);
    switch(val) {
        case 0:
            Wire.write(0x00);
            break;
        case 1:
            Wire.write(0x80);
            break;
    }
    Wire.endTransmission();
}
#endif
