// NeoPixelAnimationManager.h
#ifndef NEOPIXELANIMATIONMANAGER_H
#define NEOPIXELANIMATIONMANAGER_H

#include <Adafruit_NeoPixel.h>

// Color definitions
extern const uint32_t RED;
extern const uint32_t GREEN;
extern const uint32_t BLUE;
extern const uint32_t WHITE;
extern const uint32_t YELLOW;
extern const uint32_t BLACK;

// Animation definitions
extern const uint8_t ANIM_WAKE_UP;
extern const uint8_t ANIM_IDLE;
extern const uint8_t ANIM_ARROW;
extern const uint8_t ANIM_NAVIGATE_TO;

class Animation {
public:
    // Constructor
    Animation(uint32_t start, uint16_t duration, uint8_t type, uint32_t col=WHITE, int pos=0, int val=20);
    bool isPlaying();
    uint8_t type;
    uint32_t color;
    int position;
    int value;
private:
    uint32_t start;
    uint16_t duration;
};

class NeoPixelAnimationManager {
public:
    NeoPixelAnimationManager();
    void begin();
    bool update();
    void startAnimation(uint8_t animationType, uint32_t col=WHITE, float angle = 0.0, int val = 50);
    
private:
    static const uint8_t NEO_PIN = 6;
    static const uint16_t NUMPIXELS = 24;
    Adafruit_NeoPixel strip;
    uint32_t lastUpdateTime;
    Animation currentAnim;
    void runAnimation();
    void animateRainbowChase();
    void animateArrow();
    void animateNavigator();

    static const uint32_t ANIMATION_INTERVAL = 200; // Time between updates in milliseconds
    int step;
};

#endif
