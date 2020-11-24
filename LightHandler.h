#ifndef LIGHT_HANDLER
#define LIGHT_HANDLER

#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 62 // full string is 67 test string is 9
// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// What pin is the light data wire in?
#define DATA_PIN 5
//#define CLOCK_PIN 13

class LightHandler
{
private:
    CRGB leds[NUM_LEDS] = {0,0,0,0,255,0,0,0,0,0};
    // byte light_data[NUM_LEDS][3];
    unsigned long rgb_to_long(byte rgb[3]);
	void display6bit(uint8_t num, uint8_t bigEndLoc, unsigned long mc_l);
public:
    LightHandler() {};
    LightHandler(int nothing);
    ~LightHandler() {};
    void light_data_out(byte l_data[NUM_LEDS][3]);
	void light_data_out(unsigned long l_data[NUM_LEDS]);
    void setAllToColor(byte color[3]);
	void setAllToColor(unsigned long color);
    void turnOff();
    void displayTime(uint8_t hour, uint8_t min, uint8_t sec);
};

LightHandler::LightHandler(int nothing)
{
    pinMode(LED_BUILTIN, OUTPUT);
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    this->turnOff();
}

void LightHandler::setAllToColor(unsigned long color)
{
    for ( int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color;
    }
    FastLED.show();
}

void LightHandler::setAllToColor(byte color[3])
{
    unsigned long color_l = rgb_to_long(color);
    for ( int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color_l;
    }
    FastLED.show();
}

void LightHandler::turnOff()
{
    byte rgb[3] = {0, 0, 0};
    this->setAllToColor(rgb);
}

void LightHandler::displayTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	byte hColor[3] = {0, 255, 0};
	byte mColor[3] = {255, 0, 0};
	byte sColor[3] = {0, 0, 255};
	byte sepColor[3] = {255, 255, 255};
	unsigned long hc_l = rgb_to_long(hColor);
	unsigned long mc_l = rgb_to_long(mColor);
	unsigned long sc_l = rgb_to_long(sColor);
	unsigned long sepc_l = rgb_to_long(sepColor);


	this->turnOff();
	if (NUM_LEDS < 6)
		return;
	else if (NUM_LEDS < 13)
		this->display6bit(hour, 5, hc_l);
	else if (NUM_LEDS < 20)
	{
		this->display6bit(hour, 12, hc_l);
		leds[6] = sepc_l;
		this->display6bit(min, 5, mc_l);
	}
	else if (NUM_LEDS < 40)
	{
		this->display6bit(hour, 19, hc_l);
		leds[13] = sepc_l;
		this->display6bit(min, 12, mc_l);
		leds[6] = sepc_l;
		this->display6bit(sec, 5, sc_l);
	}
	else
	{
		this->display6bit(hour, 39, hc_l);
		leds[33] = sepc_l;
		this->display6bit(min, 32, mc_l);
		leds[26] = sepc_l;
		this->display6bit(sec, 25, sc_l);
	}
}

void LightHandler::display6bit(uint8_t num, uint8_t bigEndLoc, unsigned long color_l)
{
	if (bigEndLoc < 5 || bigEndLoc >= NUM_LEDS)
		return;
	uint8_t temp_num = num;
	if (temp_num >= 32) 
	{
		leds[bigEndLoc] = color_l;
		temp_num -= 32;
	}
	if (temp_num >= 16) 
	{
		leds[bigEndLoc - 1] = color_l;
		temp_num -= 16;
	}
	if (temp_num >= 8) 
	{
		leds[bigEndLoc - 2] = color_l;
		temp_num -= 8;
	}
	if (temp_num >= 4) 
	{
		leds[bigEndLoc - 3] = color_l;
		temp_num -= 4;
	}
	if (temp_num >= 2) 
	{
		leds[bigEndLoc - 4] = color_l;
		temp_num -= 2;
	}
	if (temp_num >= 1) 
	{
		leds[bigEndLoc - 5] = color_l;
	}
	FastLED.show();
}

void LightHandler::light_data_out(unsigned long l_data[NUM_LEDS])
{
	for ( int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = l_data[i];
	}
	FastLED.show();
}

void LightHandler::light_data_out(byte l_data[NUM_LEDS][3])
{
	for ( int i = 0; i < NUM_LEDS; i++)
	{
		unsigned long color = rgb_to_long(l_data[i]);
		leds[i] = color;
	}
	FastLED.show();
	// FastLED.delay(100);
}

unsigned long LightHandler::rgb_to_long(byte rgb[3])
{
	unsigned long rsh = rgb[0];
	word gsh = rgb[1];
	rsh = rsh << 16;
	gsh = gsh << 8;
	unsigned long color = rsh | gsh | rgb[2];
	return color;
}

#endif