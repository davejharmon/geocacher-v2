// NeoPixelAnimationManager.cpp
#include "NeoPixelAnimationManager.h"

NeoPixelAnimationManager::NeoPixelAnimationManager()
    : strip(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), animationDuration(1000), currentAnimation(-1) {
}

void NeoPixelAnimationManager::begin() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void NeoPixelAnimationManager::update() {
    uint32_t currentTime = millis();
    if (currentTime - lastUpdateTime >= ANIMATION_INTERVAL) {
        lastUpdateTime = currentTime;
        runAnimation();
    }
}

void NeoPixelAnimationManager::startAnimation(int animationType) {
    currentAnimation = animationType;
    lastUpdateTime = millis(); // Reset timer
}

void NeoPixelAnimationManager::runAnimation() {
    switch (currentAnimation) {
        case 0:
            animationChase();
            break;
        case 1:
            animationPulse();
            break;
        // Add other cases for different animations
        default:
            strip.clear();
            strip.show();
            break;
    }
}

void NeoPixelAnimationManager::animationChase() {
    static uint16_t position = 0;
    strip.clear();
    strip.setPixelColor(position, strip.Color(255, 0, 0)); // Red color
    strip.show();
    position = (position + 1) % strip.numPixels();
}

void NeoPixelAnimationManager::animationPulse() {
    static uint8_t brightness = 0;
    static int8_t fadeAmount = 5;
    strip.clear();
    strip.setBrightness(brightness);
    strip.fill(strip.Color(0, 255, 0)); // Green color
    strip.show();
    brightness += fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount; // Reverse the direction of the fade
    }
}
