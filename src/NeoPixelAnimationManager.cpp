// NeoPixelAnimationManager.cpp
#include "NeoPixelAnimationManager.h"

// Color definitions
const uint32_t RED    = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t GREEN  = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t BLUE   = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t WHITE  = Adafruit_NeoPixel::Color(255, 255, 255);
const uint32_t BLACK  = Adafruit_NeoPixel::Color(0, 0, 0);
const uint32_t YELLOW = Adafruit_NeoPixel::Color(255, 255, 20);

// Animation definitions
const uint8_t ANIM_WAKE_UP = 0;
const uint8_t ANIM_IDLE = 1;
const uint8_t ANIM_ARROW = 2;

// Constructor Implementation
Animation::Animation(uint32_t start, uint16_t duration, uint8_t type, uint32_t col, int pos, int val) 
    : type(type), color(col), position(pos), value(val), start(start), duration(duration)
{
    // Constructor body (if needed)
}

// Method to check if the animation is currently playing
bool Animation::isPlaying() {
    uint32_t currentTime = millis();
    return (duration==0 || (currentTime - start) < duration);
}

// NeoPixelAnimationManager Constructor
NeoPixelAnimationManager::NeoPixelAnimationManager()
    : strip(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), currentAnim(0, 0, ANIM_IDLE,WHITE,0,0)
{
}

// Initialize the NeoPixel strip
void NeoPixelAnimationManager::begin() {
    strip.begin();
    strip.setBrightness(30);
    strip.show(); // Initialize all pixels to 'off'
}

// Update the animations, should be called in the loop function
bool NeoPixelAnimationManager::update() {
    uint32_t currentTime = millis();
    if (currentTime - lastUpdateTime >= ANIMATION_INTERVAL) {
        if (currentAnim.isPlaying()) {
            runAnimation();
        }
        lastUpdateTime = currentTime;
        Serial.print("Millis:");
        Serial.println(currentTime);
    }
    return currentAnim.isPlaying();
}

// Start a specific animation based on type and parameters
void NeoPixelAnimationManager::startAnimation(uint8_t animationType, uint32_t col, float angle, int val) {
    uint32_t startTime = millis();
    step=0;
    uint8_t pos = static_cast<int>(angle / 360.0 * strip.numPixels());
    uint32_t duration=0;
    switch (animationType) {
        case ANIM_WAKE_UP:
            duration=2000;
            break;
        case ANIM_IDLE:
            duration=0;
            break;
        case ANIM_ARROW:
            int steps = static_cast<int>(val / 100.0 * strip.numPixels());
            duration=steps*ANIMATION_INTERVAL;
            break;
        default:
            break;
    }
    currentAnim = Animation(startTime, duration, animationType, col, pos, val);
}

// Private method to run the current animation
void NeoPixelAnimationManager::runAnimation() {
    strip.clear();
    switch (currentAnim.type) {
        case ANIM_WAKE_UP:
            // Implement wake up animation logic
            break;
        case ANIM_IDLE:
            animateRainbowChase();
            break;
        case ANIM_ARROW:
            animateArrow();
            break;
        default:
            break;
    }

}

void NeoPixelAnimationManager::animateRainbowChase() {
    uint16_t hue = (65535/24)*step;    
    strip.rainbow(hue);
    strip.show();
    step+=1;
    if (step>=24) {step=0;}
}

void NeoPixelAnimationManager::animateArrow() {
    int startPos=currentAnim.position-step/2;
    for (int i=0;i<step;i++) {
        int pos = startPos+i;
        if (pos < 0) pos +=strip.numPixels();
        if (pos >strip.numPixels()-1) pos-=strip.numPixels();
        strip.setPixelColor(pos,currentAnim.color);
    }
    strip.show();
    step+=1;
}