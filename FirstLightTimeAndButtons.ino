#include "ButtonHandler.h"
#include "LightHandler.h"
#include "LightShowController.h"
#include "ClockHandler.h"

#define BTN_UP 8
#define BTN_DOWN 9
#define BTN_STATE 10

// Time to make animation last
//                    Min * scale for animation time
//static int sweeptime = 1 * 250;
static int sweeptime = 5;

#define NUM_STATES 5
static uint8_t stateIndex = 0;

static uint8_t colorState = 0;

ButtonHandler *up_btn;
ButtonHandler *down_btn;
ButtonHandler *state_btn;
LightHandler *lights;
LightShowController *light_show_controller;
ClockHandler *clock;

void setup() 
{
	delay(1000);

	// Setup serial output
	Serial.begin(9600);
	while (! Serial);
	Serial.println("Program start");

	// Setup clock
	clock = new ClockHandler(0);

	// Setup LEDS
	lights = new LightHandler(0);

	// Setup Light Show Controller 
	light_show_controller = new LightShowController(lights);

	// Setup Buttons
	up_btn = new ButtonHandler(BTN_UP);
	down_btn = new ButtonHandler(BTN_DOWN);
	state_btn = new ButtonHandler(BTN_STATE);

	// Signal successful start with led
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(200);                       // wait for a second
		digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
		delay(200);
	}
}
void loop()
{
	bool up_was_pressed = up_btn->beenPressed();
	bool down_was_pressed = down_btn->beenPressed();
	handleStateChange();

	if (stateIndex == 0)
	{
		// Alarm state
		alarmStateAction();
	}
	else if (stateIndex == 1)
	{
		// Color play state
		sunTestStateAction(up_was_pressed, down_was_pressed);
	}
	else if (stateIndex == 2)
	{
		// Color play state
		colorStateAction(up_was_pressed, down_was_pressed);
	}
	else if (stateIndex == 3)
	{
		// Display time state
		lights->displayTime(clock->getHour(), clock->getMin(), clock->getSec());
	}
	else if (stateIndex == 4)
	{
		// Off state (do nothing because lights are always off after state change)
	}
	else
	{
		// Unknown state
	}

}

void handleStateChange()
{
	if (state_btn->beenPressed())
	{
		if (stateIndex >= NUM_STATES - 1) 
			stateIndex = 0;
		else
			stateIndex += 1;
		lights->turnOff();
		Serial.print("Now in state: ");
		Serial.println(stateIndex);
	}
}

void alarmStateAction()
{
	if (clock->getHour() == 5)
	{
		if (clock->getMin() > 15 && clock->getMin() < 30)
		{	
			byte rgb[3] = {(clock->getMin() - 15)*7, 0, 0};
			lights->setAllToColor(rgb);
		}
		else if (clock->getMin() >= 30 && clock->getMin() < 45)
		{
			byte rgb[3] = {128, 128, 0};
			lights->setAllToColor(rgb);
		}
		else if (clock->getMin() >= 45)
		{
			byte rgb[3] = {65, 65, 255};
			lights->setAllToColor(rgb);
		}
		else
		{
			lights->turnOff();
		}
	}
	if (clock->getHour() == 6 && clock->getMin() <= 15)
	{
		if (clock->getSec() % 2 == 0)
		{
			byte rgb[3] = {65, 65, 255};
			lights->setAllToColor(rgb);
		}
		else
		{
			lights->turnOff();
		}
	}
	else
	{
		lights->turnOff();
	}
}

void sunTestStateAction(bool up, bool down)
{
	if (!light_show_controller->isAnimationRunning() || up || down)
	{
		light_show_controller->startAnimation(60000);
	}
	else
	{
		light_show_controller->animationUpdate();
	}
}

void colorStateAction(bool up, bool down)
{
	if (up)
	{
		colorState += 1;
	}
	if (down)
	{
		colorState -= 1;
	}
	if (colorState == 0)
	{
		byte rgb[3] = {0, 0, 255};
		lights->setAllToColor(rgb);
	}
	else if (colorState == 1)
	{
		byte rgb[3] = {0, 255, 0};
		lights->setAllToColor(rgb);
	}
	else if (colorState == 2)
	{
		byte rgb[3] = {0, 255, 255};
		lights->setAllToColor(rgb);
	}
	
	else if (colorState == 3)
	{
		byte rgb[3] = {255, 0, 0};
		lights->setAllToColor(rgb);
	}
	else if (colorState == 4)
	{
		byte rgb[3] = {255, 0, 255};
		lights->setAllToColor(rgb);
	}
	else if (colorState == 5)
	{
		byte rgb[3] = {255, 255, 0};
		lights->setAllToColor(rgb);
	}
	else if (colorState == 6)
	{
		byte rgb[3] = {255, 255, 255};
		lights->setAllToColor(rgb);
	}
	else
	{
		byte rgb[3] = {64, 64, 64};
		lights->setAllToColor(rgb);
	}
}

	// if (up_btn->beenPressed())
	// {
	// 	Serial.println("Up pressed: enter state 1");
	// 	stateIndex = 1;
	// 	byte rgb[3] = {255, 255, 0};
	// 	lights->setAllToColor(rgb);
	// }
	// if (down_btn->beenPressed())
	// {
	// 	Serial.println("Down pressed: enter state 2");
	// 	stateIndex = 2;
	// 	byte rgb[3] = {255, 0, 255};
	// 	lights->setAllToColor(rgb);
	// }
	// if (state_btn->beenPressed())
	// {
	// 	Serial.println("State pressed: enter state 3");
	// 	stateIndex = 3;
	// }
	// if (stateIndex == 3)
	// {
	// 	lights->displayTime(clock->getHour(), clock->getMin());
	// }