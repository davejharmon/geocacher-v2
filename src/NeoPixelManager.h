// neopixelmanager.h
#ifndef NEOPIXELMANAGER_H
#define NEOPIXELMANAGER_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "constants.h"

class NeoPixelManager {
public:
    NeoPixelManager();
    void begin();
    bool update();
    void setPixel(uint32_t col, uint8_t index);
    void setStrip(const uint32_t newState[NUMPIXELS]);
    void drawLine(uint32_t col, float angle, uint8_t percent, bool centered=true);
    void clear();
    void fill(uint32_t col);
    bool isPlaying();

    // Animation functions
    void start(uint8_t type, uint32_t col=0x000000, uint8_t sty=0); // Starts the wipe animation
    void startTimer(uint32_t dur);
    void interrupt(uint8_t type=ANIM_NONE); // Interrupt any animation (or a specific type)

private:
    Adafruit_NeoPixel strip;
    uint32_t lastUpdateTime;
    uint32_t state[NUMPIXELS];
    uint32_t oldState[NUMPIXELS];

    // Animation state variables
    uint8_t playing;
    uint8_t step;
    uint32_t color;
    uint32_t started;
    uint32_t endsAt;
    uint8_t style;

    // Private animation update functions
    void updateWipe();
    void updateRainbowPinwheel();
    void updateTimer();
};

#endif // NEOPIXELMANAGER_H
