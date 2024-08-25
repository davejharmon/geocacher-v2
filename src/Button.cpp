#include "Button.h"

Button::Button(int pin) : pin(pin) {}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP);
}

void Button::update() {
    handleButtonPress();
}

bool Button::isClicked() {
    return buttonState; // Active-low button: return true when pressed
}

unsigned long Button::getPressDuration() {
    if (buttonState) {
        return millis() - pressStartTime; // If button is still pressed, calculate duration
    } else {
        return 0; // If button is released, return 0
    }
}

void Button::onClick(void (*callback)()) {
    clickCallback = callback;
}

void Button::onRelease(void (*callback)(unsigned long)) {
    releaseCallback = callback;
}

void Button::handleButtonPress() {
    bool reading = digitalRead(pin) == LOW; // Assuming button is active-low

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;

            if (buttonState == HIGH) { // Button just pressed down
                pressStartTime = millis(); // Record the time when button is pressed
                if (clickCallback != nullptr) {
                    clickCallback(); // Call the click callback when button is pressed down
                }
            } else if (buttonState == LOW) { // Button just released
                pressDuration = millis() - pressStartTime; // Calculate and store the duration
                if (releaseCallback != nullptr) {
                    releaseCallback(pressDuration); // Call the release callback with the press duration
                }
            }
        }
    }

    lastButtonState = reading;
}
