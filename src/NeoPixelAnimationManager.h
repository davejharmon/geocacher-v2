// NeoPixelAnimationManager.h
#ifndef NEOPIXELANIMATIONMANAGER_H
#define NEOPIXELANIMATIONMANAGER_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// Color library
extern const uint32_t RED;
extern const uint32_t GREEN;
extern const uint32_t BLUE;
extern const uint32_t WHITE;
extern const uint32_t BLACK;
extern const uint32_t YELLOW;

extern const uint8_t ANIM_WIPE;
extern const uint8_t ANIM_RAINBOW;
extern const uint8_t ANIM_ARROW;
extern const uint8_t ANIM_PULSE_ARROW;

// Constants
const uint16_t NUMPIXELS = 24;           // Number of NeoPixels
const uint8_t NEO_PIN = 6;               // Pin where NeoPixels are connected
const uint16_t ANIMATION_INTERVAL = 50;  // Interval between animation steps in milliseconds

// Animation class to hold animation properties
class Animation {
public:
    // Constructor
    Animation(uint8_t type = 0, uint32_t col = 0, uint8_t pos = 0, uint8_t val = 0, uint32_t start = 0, uint16_t duration = 0);

    // Member variables
    uint8_t type;         // Animation type
    uint32_t color;       // Animation color
    uint8_t position;     // Position for the animation
    uint8_t value;        // Value related to the animation
    uint32_t start;       // Start time of the animation
    uint16_t duration;    // Duration of the animation
    bool isInterruptable; // Flag to indicate if the animation is interruptible

private:
    // Method to calculate the duration based on type and value
    uint16_t calculateDuration(uint8_t type, uint8_t val);
};
// NeoPixelAnimationManager class to manage NeoPixel animations
class NeoPixelAnimationManager {
public:
    NeoPixelAnimationManager();

    void begin();       // Initialize the NeoPixel strip
    bool update();      // Update the animations, should be called in the loop function
    uint16_t startAnimation(uint8_t animationType, uint32_t col=0xFFFF00, float angle = 0.0, uint8_t val = 100); // Start a specific animation, returns end time
    bool interrupt();   // Try and interrupt the current animation, returns true if interruptible, false if failed to interrupt

private:
    Adafruit_NeoPixel strip; // NeoPixel strip object
    uint32_t lastUpdateTime; // Last update time for animations
    int step;                // Step counter for current animation
    Animation currentAnim;   // Current animation being played

    void runAnimation();     // Run the current animation

    // Private methods for different animations
    void animateWipe();         // wipe the strip with one colour
    void animateRainbowChase(); // loopable rainbow chase
    void animateArrow();        // draw a line centred on a position
    void animatePulseArrow();   // draw a line that pulses
};

#endif // NEOPIXELANIMATIONMANAGER_H
