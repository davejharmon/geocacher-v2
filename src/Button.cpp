#include "Button.h"

Button::Button(int pin) : pin(pin) {}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP);
}

void Button::update() {
    handleButtonPress();
}

bool Button::isClicked() {
    return !buttonState; // Active-low button
}

void Button::onClick(void (*callback)()) {
    clickCallback = callback;
}

void Button::handleButtonPress() {
    bool reading = digitalRead(pin) == LOW; // Assuming button is active low

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;

            if (buttonState == LOW && clickCallback != nullptr) {
                clickCallback();
            }
        }
    }

    lastButtonState = reading;
}
