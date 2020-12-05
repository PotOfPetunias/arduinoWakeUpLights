#ifndef CLOCK_HANDLER
#define CLOCK_HANDLER

#include <Wire.h>
#include <DS3231.h>

class ClockHandler
{
private:
    DS3231 clock;
    RTCDateTime dt;

public:
    ClockHandler() {};
    ClockHandler(int nothing);
    ~ClockHandler() {};

    uint8_t getHour();
    uint8_t getMin();
    uint8_t getSec();

    void printTime();
};

ClockHandler::ClockHandler(int nothing)
{
    clock.begin();
    // Set sketch compiling time
    clock.setDateTime(__DATE__, __TIME__);
}

uint8_t ClockHandler::getHour()
{
    dt = clock.getDateTime();
    return dt.hour;
}

uint8_t ClockHandler::getMin()
{
    dt = clock.getDateTime();
    return dt.minute;
}

uint8_t ClockHandler::getSec()
{
    dt = clock.getDateTime();
    return dt.second;
}

void ClockHandler::printTime()
{
    dt = clock.getDateTime();

    // For leading zero look to DS3231_dateformat example

    Serial.print("Raw data: ");
    Serial.print(dt.year);   Serial.print("-");
    Serial.print(dt.month);  Serial.print("-");
    Serial.print(dt.day);    Serial.print(" ");
    Serial.print(dt.hour);   Serial.print(":");
    Serial.print(dt.minute); Serial.print(":");
    Serial.print(dt.second); Serial.println("");
}
#endif