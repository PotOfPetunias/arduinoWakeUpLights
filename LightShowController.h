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
    unsigned long duration;
    unsigned long interval;
    unsigned long animate_size;
    unsigned long animate_interval; 
    unsigned long light_size;
    bool animation_started = false;
public:
    LightShowController(LightHandler *lights);
    ~LightShowController();
    void startAnimation(unsigned long duration); // Duration is is milliseconds
    void animationUpdate();
    bool isAnimationRunning();
    bool stopAnimation();
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
    this->duration = duration;
    // this->lights->setAllToColor(sun_data[0]);
    this->lights->turnOff();
    this->animate_size = sizeof(sun_data) / sizeof(sun_data[0]);
    this->light_size = sizeof(fixed_random_order) / sizeof(fixed_random_order[0]);
    this->interval = duration / (animate_size * light_size);
    this->animate_interval = duration / animate_size;
    this->animation_started = true;

    Serial.print("duration: ");
    Serial.println(duration);
    Serial.print("Animate size: ");
    Serial.println(animate_size);
    Serial.print("Light size: ");
    Serial.println(light_size);
    Serial.print("interval: ");
    Serial.println(interval);
}

void LightShowController::animationUpdate()
{
    if (this->animation_started)
    {
        unsigned long elapsed = millis() - this->start_time;
        unsigned long total_index = elapsed / this->interval; 
        unsigned long animate_index = elapsed / this->animate_interval; 
        unsigned long light_index = total_index % this->light_size; 

        Serial.print("start time: ");
        Serial.println(this->start_time);
        Serial.print("time: ");
        Serial.println(millis());
        Serial.print("elapsed: ");
        Serial.println(elapsed);
        Serial.print("total index: ");
        Serial.println(total_index);
        Serial.print("animate_index: ");
        Serial.println(animate_index);
        Serial.print("light index: ");
        Serial.println(light_index);

        if (elapsed < this->duration)
        {
            // this->lights->setAllToColor(sun_data[animate_index]);
            this->lights->setColorAt(fixed_random_order[light_index], sun_data[animate_index]);
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

bool LightShowController::stopAnimation()
{
    this->animation_started = false;
}
#endif