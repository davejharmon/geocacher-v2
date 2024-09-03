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
extern const uint32_t CYAN;
extern const uint32_t MAGENTA;
extern const uint32_t YELLOW;
extern const uint32_t ORANGE;
extern const uint32_t PURPLE;
extern const uint32_t TEAL;
extern const uint32_t PINK;

extern const uint8_t ANIM_WIPE;
extern const uint8_t ANIM_RAINBOW;
extern const uint8_t ANIM_ARROW;
extern const uint8_t ANIM_PULSE_ARROW;
extern const uint8_t ANIM_FLASH;

// Constants
const uint16_t NUMPIXELS = 24;           // Number of NeoPixels
const uint8_t NEO_PIN = 6;               // Pin where NeoPixels are connected
const uint16_t ANIMATION_INTERVAL = 250;  // Interval between animation steps in milliseconds

class Animation {
public:
    // Constructor
    Animation(uint8_t type = 0, uint32_t col = 0, uint8_t pos = 0, uint8_t val = 0, uint32_t start = 0, uint16_t duration = 0, bool restore = false);

    // Member variables
    uint8_t type;         // Animation type
    uint32_t color;       // Animation color
    uint8_t position;     // Position for the animation
    uint8_t value;        // Value related to the animation
    uint32_t start;       // Start time of the animation
    uint16_t duration;    // Duration of the animation
    bool isInterruptable; // Flag to indicate if the animation is interruptible
    uint8_t step;         // Step counter for current animation
    bool restoreOnEnd;    // Flag to indicate if previous animation should be restored when done

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
    uint16_t startAnimation(uint8_t animationType, uint32_t col=0xFFFF00, float angle = 0.0, uint8_t val = 100, bool restoreOnEnd=false); // Start a specific animation, returns end time
    bool interrupt();   // Try and interrupt the current animation, returns true if interruptible, false if failed to interrupt
    void clear();       // clear the pixel strip
    void restore();     // restore the previous state of the strip
    uint32_t getPixel(uint8_t index);   // Get the pixel colour at a point
    void setPixel(uint8_t index, uint32_t col);
    void setAllPixels(const uint32_t newState[NUMPIXELS]);


private:
    Adafruit_NeoPixel strip; // NeoPixel strip object
    uint32_t lastUpdateTime; // Last update time for animations
    Animation currentAnim;   // Current animation being played
    Animation prevAnimation; // Previous animation for restoration
    uint32_t state[NUMPIXELS];

    void runAnimation();     // Run the current animation

    // Private methods for different animations
    void animateWipe();         // wipe the strip with one colour
    void animateRainbowChase(); // loopable rainbow chase
    void animateArrow();        // draw a line centred on a position
    void animatePulseArrow();   // draw a line that pulses
    void animateFlash();        // flash a colour
};

#endif // NEOPIXELANIMATIONMANAGER_H
