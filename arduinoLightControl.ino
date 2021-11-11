#include "ButtonHandler.h"
#include "LightHandler.h"
#include "LightShowController.h"
#include "ClockHandler.h"

#define BTN_UP 8
#define BTN_DOWN 9
#define BTN_STATE 10

// Time to make animation last in minutes
static int sweeptime = 30;
// Alarm Time (Time animation will start aka. sweeptime before you want to wake up)
static int alarm_hour[3] = { 5,  5,  4};
static int alarm_min[3] =  {25, 00, 30};
static int current_alarm = 0;

#define NUM_STATES 5
static uint8_t stateIndex = 0;

static uint8_t colorState = 0;
static uint8_t alarmState = 2;
unsigned long time_of_start_show_alarm = 0;

ButtonHandler *up_btn;
ButtonHandler *down_btn;
ButtonHandler *state_btn;
LightHandler *lights;
LightShowController *light_show_controller;
ClockHandler *clock;

void setup() 
{
	// delay(1000);

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

	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}
void loop()
{
	bool up_was_pressed = up_btn->beenPressed();
	bool down_was_pressed = down_btn->beenPressed();
	handleStateButton();

	if (stateIndex == 0)
	{
		// Alarm state
		alarmStateAction(up_was_pressed, down_was_pressed);
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
		// Serial.print(clock->getHour());
		// Serial.print(":");
		// Serial.print(clock->getMin());
		// Serial.print(":");
		// Serial.println(clock->getSec());
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

void handleStateButton()
{
	if (state_btn->beenPressed())
	{
		if (stateIndex >= NUM_STATES - 1) 
			stateIndex = 0;
		else
			stateIndex += 1;
		
		if (stateIndex == 0)
		{
			alarmState = 2;
			time_of_start_show_alarm = millis();
		}
		lights->turnOff();
		light_show_controller->stopAnimation();
		Serial.print("Now in state: ");
		Serial.println(stateIndex);
	}
}

void alarmStateAction(bool up, bool down)
{
	if (alarmState == 1 && light_show_controller->isAnimationRunning())
	{
		light_show_controller->animationUpdate();
	}
	else if (alarmState == 0 && clock->getHour() == alarm_hour[current_alarm] && clock->getMin() == alarm_min[current_alarm])
	{
		unsigned long second = 1000;
		unsigned long minute = 60;
		light_show_controller->startAnimation(second * minute * sweeptime);
		Serial.println("Animation Started");
		alarmState = 1;
	}
	else if(alarmState == 2)
	{
		// Show alarm index???
		lights->displayTime(alarm_hour[current_alarm], alarm_min[current_alarm], 0);
		unsigned long diff = millis() - time_of_start_show_alarm;
		if (diff > 6000)
		{
			lights->turnOff();
			alarmState = 0;
		}
	}
	if (up)
	{
		size_t last_index = (sizeof(alarm_hour) / sizeof(alarm_hour[0])) - 1;
		size_t first_index = 0;
		light_show_controller->stopAnimation();
		lights->turnOff();
		// If current alarm is the last alarm in the list 
        if (current_alarm >= last_index)
		{
			current_alarm = first_index;
		}
		else
		{
			current_alarm++;
		}
		Serial.print("Now using alarm: ");
		Serial.println(current_alarm);
		alarmState = 2;
		time_of_start_show_alarm = millis();
	}
	else if (down)
	{
		size_t last_index = (sizeof(alarm_hour) / sizeof(alarm_hour[0])) - 1;
		size_t first_index = 0;
		light_show_controller->stopAnimation();
		lights->turnOff();
		// If current alarm is the last alarm in the list 
        if (current_alarm <= first_index)
		{
			current_alarm = last_index;
		}
		else
		{
			current_alarm--;
		}
		Serial.print("Now using alarm: ");
		Serial.println(current_alarm);
		alarmState = 2;
		time_of_start_show_alarm = millis();
	}
	// else
	// {
	// 	lights->turnOff();
	// }
	
}

void sunTestStateAction(bool up, bool down)
{
	if (!light_show_controller->isAnimationRunning() || up || down)
	{
		light_show_controller->startAnimation(60000); // 1 minute
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
		byte rgb[3] = {255, 0, 0};
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
		byte rgb[3] = {0, 0, 255};
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