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
    bool isPlaying();

    // Animation functions
    void start(uint8_t type, uint32_t col=0x000000); // Starts the wipe animation
    void startTimer(uint32_t dur);
    void interrupt();      // Interrupts the rainbow pinwheel animation

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


    // Private animation update functions
    void updateWipe();
    void updateRainbowPinwheel();
    void updateTimer();
};

#endif // NEOPIXELMANAGER_H
