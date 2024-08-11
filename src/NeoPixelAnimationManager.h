// NeoPixelAnimationManager.h
#ifndef NEOPIXELANIMATIONMANAGER_H
#define NEOPIXELANIMATIONMANAGER_H

#include <Adafruit_NeoPixel.h>

class NeoPixelAnimationManager {
public:
    NeoPixelAnimationManager();
    void begin();
    void update();
    void startAnimation(int animationType);
    
private:
    static const uint8_t NEO_PIN = 6;
    static const uint16_t NUMPIXELS = 24;
    Adafruit_NeoPixel strip;
    uint32_t lastUpdateTime;
    uint16_t animationDuration;
    int currentAnimation;

    void runAnimation();
    void animationChase();
    void animationPulse();
    // Add other animations here

    static const uint32_t ANIMATION_INTERVAL = 50; // Time between updates in milliseconds
};

#endif
