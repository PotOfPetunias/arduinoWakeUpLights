#ifndef LIGHT_SHOW_CONTROLLER
#define LIGHT_SHOW_CONTROLLER

#include "LightHandler.h"
#include "sunRiseData.h"


class LightShowController
{
private:
    /* data */
    LightHandler *lights;
public:
    LightShowController(LightHandler *lights);
    ~LightShowController();
    void startAnimation(int iterations, byte start_color[3], byte end_color[3]);
    void animationUpdate();
};

LightShowController::LightShowController(LightHandler *lights)
{
    this->lights = lights;
}

LightShowController::~LightShowController()
{
}

void LightShowController::startAnimation(int iterations, byte start_color[3], byte end_color[3])
{
    this->lights->setAllToColor(sun_data[1]);
}

void LightShowController::animationUpdate()
{

}
#endif