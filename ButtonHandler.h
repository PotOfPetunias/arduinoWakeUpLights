
#ifndef BUTTON_HANDLER
#define BUTTON_HANDLER

class ButtonHandler
{
private:
    uint8_t pin;
    bool inDownState;
public:
    ButtonHandler();
    ButtonHandler(uint8_t pin);
    ~ButtonHandler();
    bool beenPressed();
};

ButtonHandler::ButtonHandler()
{
    this->pin = 0;
    this->inDownState = false;
}
ButtonHandler::ButtonHandler(uint8_t pin)
{
    this->pin = pin;
    this->inDownState = false;
    pinMode(this->pin, INPUT_PULLUP); 
}

ButtonHandler::~ButtonHandler()
{
}

bool ButtonHandler::beenPressed()
{
    if (inDownState)
    {
        if (digitalRead(this->pin) == LOW)
        {
            return false;
        }
        else
        {
            this->inDownState = false;
            return false;
        }
    }
    else
    {
        if (digitalRead(this->pin) == LOW)
        {
            this->inDownState = true;
            return true;
        }
        else
        {
            return false;
        }
    }
    
}

#endif
