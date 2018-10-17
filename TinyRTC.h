/*
    TinyRTC
    Written by Ryan Alvaro (WildRynoceros)

    Description:
    A library meant to use the DS1307 RTC to its fullest

    About the DS1307:
    The DS1307 is a relatively inexpensive RTC that can be used for timekeeping.
    However, it has a few other features up its sleeve that do not appear to be
    utilized by other libraries. Two examples include not using the control
    register for its square wave output, or accessing the rest of the 56x8 RAM
    availiable.
    The 1307 internally is able to correct for leap years, and it has an accurate
    calendar from the year 2000 through 2100 (and therefore can be used reliably
    during these years). It is equipped with 64x8 of non-volatile memory, with
    56x8 RAM appearing to be usable for other purposes after the 7 timekeeping
    and 1 control register.

    Credits:
    Modified from JeeLabs original code http://news.jeelabs.org/code/

    Modified:
    6/10/2017
*/

#ifndef TinyRTC_H
#define TinyRTC_H


// General-purpose DateTime for DS1307
class RTC_Time {
    public:
        RTC_Time();
        RTC_Time (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
        RTC_Time (uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t min, uint8_t sec);
        RTC_Time (char* date, char* time);

        void setYear(uint16_t y);
        void setMonth(uint8_t m);
        void setDay(uint8_t d);
        void setHour(uint8_t h);
        void setMinute(uint8_t m);
        void setSecond(uint8_t s);
        void setDayOfWeek(void);
        void setDayOfWeek(uint8_t dow);
        uint8_t setTime(const RTC_Time& t);

        uint16_t getYear(void) const    { return 2000 + Y; }
        uint8_t getMonth(void) const    { return M; }
        uint8_t getDay(void) const      { return D; }
        uint8_t getHour(void) const     { return HH; }
        uint8_t getMinute(void) const   { return MM; }
        uint8_t getSecond(void) const   { return SS; }
        uint8_t getDayOfWeek(void) const { return DOW; }
        uint16_t getDDayOfWeek(uint16_t y, uint8_t m, uint8_t d) const;
        uint16_t getDDayOfWeek(void) const;

        void printTime(void);
        void operator++();
        void operator =(const RTC_Time &T);
        uint8_t DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    private:
        static uint8_t conv2d(const char* p);
        uint8_t Y, M, D, DOW, HH, MM, SS;
};

// RTC based on the DS1307 chip connected via I2C and the Wire library
class RTC_DS1307 {
    public:
        RTC_DS1307(void);
        bool enable(void);
        bool disable(void);
        bool isRunning(void);

        static uint8_t setTime(const RTC_Time& t);
        static RTC_Time getTime(void);

        void enableOscillator(uint8_t f);
        void disableOscillator(uint8_t val);

        uint8_t setPointer(uint8_t addr);
        uint8_t write(uint8_t addr, uint8_t size, uint8_t* data);
        void read(uint8_t addr, uint8_t size, uint8_t* data);
    private:
        static uint8_t bcd2bin(uint8_t val);
        static uint8_t bin2bcd(uint8_t val);
};

#endif
