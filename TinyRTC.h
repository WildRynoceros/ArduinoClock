/*
    DS1307RTC
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

// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime {
    public:
        DateTime (uint32_t t =0);
        DateTime (uint16_t year, uint8_t month, uint8_t day,
                  uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
        DateTime (const char* date, const char* time);
        uint16_t year() const       { return 2000 + yOff; }
        uint8_t month() const       { return m; }
        uint8_t day() const         { return d; }
        uint8_t hour() const        { return hh; }
        uint8_t minute() const      { return mm; }
        uint8_t second() const      { return ss; }
        uint8_t dayOfWeek() const;

        // 32-bit times as seconds since 1/1/2000
        long secondstime() const;
        // 32-bit times as seconds since 1/1/1970
        uint32_t unixtime(void) const;

    protected:
        uint8_t yOff, m, d, hh, mm, ss;
};

// RTC based on the DS1307 chip connected via I2C and the Wire library
class RTC_DS1307 {
    public:
        static uint8_t begin(void);
        static void adjust(const DateTime& dt);
        uint8_t isrunning(void);
        static DateTime now();
};
