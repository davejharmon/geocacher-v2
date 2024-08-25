#include "Button.h"

Button::Button(int pin) : pin(pin) {}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP);
}

void Button::update() {
    handleButtonPress();
}

bool Button::isClicked() {
    return buttonState; // Active-low button
}

unsigned long Button::getPressDuration() {
    if (buttonState) {
        return millis() - pressStartTime; // If button is still pressed, calculate duration
    } else {
        return pressDuration; // If button is released, return stored duration
    }
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

            if (buttonState == LOW) { // Button just pressed
                pressStartTime = millis(); // Record the time when button was pressed
            } else { // Button just released
                pressDuration = millis() - pressStartTime; // Calculate and store the duration
                if (clickCallback != nullptr) {
                    clickCallback();
                }
            }
        }
    }

    lastButtonState = reading;
}
