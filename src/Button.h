#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
    Button(int pin);

    void begin();
    void update();
    bool isClicked();
    unsigned long getPressDuration(); // Get the duration the button was pressed

    // Callback function
    void onClick(void (*callback)());

private:
    int pin;
    unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50; // debounce delay in milliseconds

    bool buttonState = false;
    bool lastButtonState = false;
    unsigned long pressStartTime = 0; // Time when button is first pressed
    unsigned long pressDuration = 0; // Duration of button press
    void (*clickCallback)() = nullptr;

    void handleButtonPress();
};

#endif
