#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
    Button(int pin);

    void begin();
    void update();
    bool isClicked();

    // Callback function
    void onClick(void (*callback)());

private:
    int pin;
    unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50; // debounce delay in milliseconds

    bool buttonState = false;
    bool lastButtonState = false;
    void (*clickCallback)() = nullptr;

    void handleButtonPress();
};

#endif
