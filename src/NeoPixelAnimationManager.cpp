#include "NeoPixelAnimationManager.h"

// Color library definitions
const uint32_t RED = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t GREEN = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t BLUE = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t WHITE = Adafruit_NeoPixel::Color(255, 255, 255);
const uint32_t BLACK = Adafruit_NeoPixel::Color(0, 0, 0);
const uint32_t YELLOW = Adafruit_NeoPixel::Color(255, 255, 20);

const uint8_t ANIM_WIPE = 0;
const uint8_t ANIM_RAINBOW = 1;
const uint8_t ANIM_ARROW = 2;
const uint8_t ANIM_PULSE_ARROW = 3;
const uint8_t ANIM_FLASH = 4;

// Constructor implementation
Animation::Animation(uint8_t type, uint32_t col, uint8_t pos, uint8_t val, uint32_t start, uint16_t duration, bool restore)
    : type(type), color(col), position(pos), value(val), start(start), duration(calculateDuration(type, val)), step(0), restoreOnEnd(restore) {
    isInterruptable = true; // Adjust logic for `isInterruptable` as needed
}

uint16_t Animation::calculateDuration(uint8_t type, uint8_t val) {
    switch(type) {
        case ANIM_WIPE: return NUMPIXELS * ANIMATION_INTERVAL * val;
        case ANIM_RAINBOW: return NUMPIXELS * ANIMATION_INTERVAL;
        case ANIM_ARROW: return ANIMATION_INTERVAL;
        case ANIM_PULSE_ARROW:
            Serial.print("duration ");
            Serial.println(ANIMATION_INTERVAL * (NUMPIXELS * val / 100));
            return ANIMATION_INTERVAL * (NUMPIXELS * val / 100);
        case ANIM_FLASH: return 100;
        default: break;
    }
    return 1000; // Default duration
}

// Constructor for the NeoPixelAnimationManager class
NeoPixelAnimationManager::NeoPixelAnimationManager()
    : strip(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), currentAnim(), prevAnimation() {
    memset(state, 0, sizeof(state));
}

// Initialize the NeoPixel strip
void NeoPixelAnimationManager::begin() {
    strip.begin();
    strip.setBrightness(30);
    strip.show(); // Initialize all pixels to 'off'
}

bool NeoPixelAnimationManager::update() {
    uint32_t currentTime = millis();
    
    if (currentTime - lastUpdateTime >= ANIMATION_INTERVAL) {
        lastUpdateTime = currentTime;
        runAnimation();
    }

    if (currentAnim.start + currentAnim.duration <= currentTime) {
        if (currentAnim.restoreOnEnd) {
            restore(); // Restore the previous animation
            return true;
        }
        return false;
    }
    
    return true;
}


uint16_t NeoPixelAnimationManager::startAnimation(uint8_t animationType, uint32_t col, float angle, uint8_t val, boolean restoreOnEnd) {
    if (!prevAnimation.restoreOnEnd) {
        strip.clear(); // Clear the strip only if `restoreOnEnd` is false for the previous animation
    }
    
    for (int i = 0; i < NUMPIXELS; i++) {
        state[i] = strip.getPixelColor(i);
    }
    
    if (animationType!=prevAnimation.type) {    
        Serial.print("Starting new animation... Dur:");
        Serial.println(currentAnim.duration);
    }

    prevAnimation = currentAnim; // Save the current animation for restoration
    prevAnimation.duration=prevAnimation.duration-(millis()-prevAnimation.start);
    currentAnim = Animation(animationType, col, angle, val, millis(),restoreOnEnd);
    
    
    return currentAnim.start + currentAnim.duration;
}


// Try and interrupt the current animation, returns true if interruptible, false if failed to interrupt
bool NeoPixelAnimationManager::interrupt() {
    if (currentAnim.isInterruptable) {
        // Stop the current animation and reset strip
        strip.clear();
        strip.show();
        return true;
    }
    return false;
}

void NeoPixelAnimationManager::clear() {
    strip.clear();
}

// Restore the previous state of the strip
void NeoPixelAnimationManager::restore() {
    // Restore the pixel states from the saved array
    for (int i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, state[i]);
    }
    strip.show();
    Serial.println("RESTORING");
    // Restore the previous animation state
    currentAnim = prevAnimation;
    currentAnim.start=millis();
    prevAnimation = Animation(); // Clear previous animation
}

// Private methods

// Run the current animation based on its type
void NeoPixelAnimationManager::runAnimation() {
    switch (currentAnim.type) {
        case ANIM_WIPE:
            animateWipe();
            break;
        case ANIM_RAINBOW:
            animateRainbowChase();
            break;
        case ANIM_ARROW:
            animateArrow();
            break;
        case ANIM_PULSE_ARROW:
            animatePulseArrow();
            break;
        case ANIM_FLASH:
            animateFlash();
            break;
        default:
            break;
    }
}

// Wipe the strip with one color
void NeoPixelAnimationManager::animateWipe() {
    int stepModifiedBySpeed = (currentAnim.step / currentAnim.value);
    if (stepModifiedBySpeed <= NUMPIXELS + 1) {
        strip.setPixelColor(stepModifiedBySpeed, currentAnim.color);
        strip.show();
        currentAnim.step++;
    } else {
        currentAnim.step = 0; // Reset the step for future wipes
    }
}

void NeoPixelAnimationManager::animateRainbowChase() {
    // Calculate the starting hue based on the step
    uint16_t startHue = (currentAnim.step * 65536L / NUMPIXELS) % 65536;
    strip.rainbow(startHue, 1, 255, 255, true);

    // Update the strip to show the changes
    strip.show();

    // Increment step and wrap around
    currentAnim.step = (currentAnim.step + 1) % NUMPIXELS;
}

// Draw a line centered on a position (arrow animation)
void NeoPixelAnimationManager::animateArrow() {
    strip.clear();
    int centerPixel = map(currentAnim.position, 0, 360, 0, NUMPIXELS);
    int length = map(currentAnim.value, 0, 100, 0, NUMPIXELS);
    int startPixel = centerPixel - length / 2;
    int endPixel = centerPixel + length / 2 - 1;

    if (length >= NUMPIXELS) {
        // If length is equal to or greater than the total number of pixels,
        // fill the entire strip
        strip.fill(currentAnim.color);
    } else {
        if (startPixel < 0) {
            // Handle wrapping if startPixel is negative
            strip.fill(currentAnim.color, NUMPIXELS + startPixel, NUMPIXELS - startPixel);
            strip.fill(currentAnim.color, 0, endPixel + 1);
        } else if (endPixel >= NUMPIXELS) {
            // Handle wrapping if endPixel exceeds NUMPIXELS
            strip.fill(currentAnim.color, startPixel, NUMPIXELS - startPixel);
            strip.fill(currentAnim.color, 0, endPixel % NUMPIXELS + 1);
        } else {
            // No wrapping needed, normal fill
            strip.fill(currentAnim.color, startPixel, length);
        }
    }
    // Update the strip to show the changes
    strip.show();
}

// Draw a line centered on a position (pulse arrow animation)
void NeoPixelAnimationManager::animatePulseArrow() {
    // Clear the strip
    strip.clear();
    // Calculate center and length
    int centerPixel = map(currentAnim.position, 0, 360, 0, NUMPIXELS);
    int length = map(currentAnim.value, 0, 100, 0, NUMPIXELS);

    // Calculate start pixel
    int startPixel = centerPixel - length / 2;

    // Draw the line
    for (int i = 0; i < length; ++i) {
        int pixel = (startPixel + i) % NUMPIXELS;
        if (pixel < 0) pixel += NUMPIXELS; // Handle negative pixel index
        uint32_t col = currentAnim.color;

        // Check if the pixel should be WHITE based on step
        if (pixel == (centerPixel - currentAnim.step / 2) || pixel == (centerPixel + currentAnim.step / 2)) {
            col = WHITE;
        }

        // Set pixel color
        strip.setPixelColor(pixel, col);
    }

    // Update the strip
    strip.show();

    // Increment step and wrap around
    currentAnim.step++;
    if (currentAnim.step >= length) currentAnim.step = 0;
}

void NeoPixelAnimationManager::animateFlash() {
    // Set all pixels to the flash color
    for (int i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, currentAnim.color);
    }
    strip.show(); // Update the display with the new color
}
