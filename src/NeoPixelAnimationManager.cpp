// NeoPixelAnimationManager.cpp
#include "NeoPixelAnimationManager.h"

NeoPixelAnimationManager::NeoPixelAnimationManager()
    : strip(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), animationDuration(1000), currentAnimation(-1), animationAngle(0) {
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

void NeoPixelAnimationManager::startAnimation(int animationType, float angle) {
    currentAnimation = animationType;
    animationAngle = angle; // Store angle for animations that require it
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
        case 2:
            animationRainbow();
            break;
        case 3:
            animationArrow(animationAngle);
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

void NeoPixelAnimationManager::animationRainbow() {
    static uint16_t startTime = millis();
    uint16_t elapsedTime = millis() - startTime;
    uint16_t position = elapsedTime / RAINBOW_DELAY;

    for (int i = 0; i < strip.numPixels(); i++) {
        float hue = (i + position) * 65536 / strip.numPixels();
        strip.setPixelColor(i, strip.ColorHSV(hue));
    }
    strip.show();
}

void NeoPixelAnimationManager::animationArrow(float angle) {
    static uint32_t lastUpdateTime = 0;
    static uint8_t brightness = 255;
    static int8_t fadeAmount = -5;
    static uint16_t northPixel = 0;

    // Update the animation every 50 milliseconds
    if (millis() - lastUpdateTime >= ANIMATION_INTERVAL) {
        lastUpdateTime = millis();

        // Find the pixel closest to north
        northPixel = map(angle, 0, 360, 0, strip.numPixels() - 1);
        Serial.print("North Pixel: ");
        Serial.println(northPixel);

        // Update brightness
        brightness += fadeAmount;
        if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount; // Reverse the direction of the fade
        }
        Serial.print("Brightness: ");
        Serial.println(brightness);

        // Clear the strip
        strip.clear();

        // Set pixels in the arrow shape
        for (int i = -5; i <= 5; i++) {
            int pixel = (northPixel + i + strip.numPixels()) % strip.numPixels();
            strip.setPixelColor(pixel, strip.Color(0, 0, 255, brightness)); // Blue color
        }

        // Show the updated strip
        strip.show();
    }
}