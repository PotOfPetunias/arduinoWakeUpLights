#ifndef LIGHT_SHOW_CONTROLLER
#define LIGHT_SHOW_CONTROLLER

#include "LightHandler.h"
#include "sunRiseData.h"


class LightShowController
{
private:
    /* data */
    LightHandler *lights;
    unsigned long start_time;
    unsigned long interval;
    bool animation_started = false;
public:
    LightShowController(LightHandler *lights);
    ~LightShowController();
    void startAnimation(unsigned long duration); // Duration is is milliseconds
    void animationUpdate();
    bool isAnimationRunning();
};

LightShowController::LightShowController(LightHandler *lights)
{
    this->lights = lights;
}

LightShowController::~LightShowController()
{
}

void LightShowController::startAnimation(unsigned long duration)
{
    this->start_time = millis();
    this->lights->setAllToColor(sun_data[0]);
    this->interval = duration / (sizeof(sun_data) / sizeof(sun_data[0]));
    this->animation_started = true;
}

void LightShowController::animationUpdate()
{
    if (this->animation_started)
    {
        unsigned long elapsed = millis() - this->start_time;
        unsigned long animate_index = elapsed / this->interval; 
        Serial.print("start time: ");
        Serial.println(this->start_time);
        Serial.print("time: ");
        Serial.println(millis());
        Serial.print("elapsed: ");
        Serial.println(elapsed);
        Serial.print("animate_index: ");
        Serial.println(animate_index);
        if (animate_index < (sizeof(sun_data) / sizeof(sun_data[0])) - 1)
        {
            this->lights->setAllToColor(sun_data[animate_index]);
        }
        else
        {
            this->animation_started = false;
        }
        
    }
}

bool LightShowController::isAnimationRunning()
{
    return this->animation_started;
}
#endif