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
public:
    LightShowController(LightHandler *lights);
    ~LightShowController();
    void startAnimation(unsigned long duration); // Duration is is milliseconds
    void animationUpdate();
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
}

void LightShowController::animationUpdate()
{
    unsigned long elapsed = millis() - this->start_time;


}
#endif