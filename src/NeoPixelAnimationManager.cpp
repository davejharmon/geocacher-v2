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
const uint8_t ANIM_NAVIGATE_TO = 3;

// Constructor Implementation
Animation::Animation(uint32_t start, uint16_t duration, uint8_t type, uint32_t col, int pos, int val) 
    : type(type), color(col), position(pos), value(val), start(start), duration(duration)
{
    // Constructor body (if needed)
}

// Method to check if the animation is currently playing
int Animation::isPlaying() {
    uint32_t currentTime = millis();
    if (duration==0) return -1;
    return (currentTime-start) < duration ? 0 : 1;
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
    }
    return currentAnim.isPlaying();
}

// Start a specific animation based on type and parameters
void NeoPixelAnimationManager::startAnimation(uint8_t animationType, uint32_t col, float angle, int val) {
    uint32_t startTime = millis();
    step=0;
    uint8_t pos = static_cast<uint8_t>((angle / 360.0) * strip.numPixels());
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
        case ANIM_NAVIGATE_TO:
            duration=0;
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
            animateRainbowChase();  // TODO: Busted
            // Implement wake up animation logic
            break;
        case ANIM_IDLE:
            animateRainbowChase();
            break;
        case ANIM_ARROW:
            animateArrow();
            break;
        case ANIM_NAVIGATE_TO:
            animateNavigator();
            break;
        default:
            break;
    }

}

// Check if an animation is playing
bool NeoPixelAnimationManager::isInterruptible() {
    return currentAnim.isPlaying() < 1; // interruptible or not playing
}

void NeoPixelAnimationManager::animateRainbowChase() {
    uint16_t hue = (65535 / 24) * step;
    strip.rainbow(hue);
    strip.show();
    step = (step + 1) % 24;
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

void NeoPixelAnimationManager::animateNavigator() {
    // Calculate the length of the segment to be illuminated
    int length = static_cast<int>(currentAnim.value / 100.0 * strip.numPixels());
    // Calculate start position, ensuring it is within the range [0, strip.numPixels()-1]
    int startPos = (currentAnim.position - length / 2 + strip.numPixels()) % strip.numPixels();
    // Illuminate the segment
    for (int i = 0; i < length; i++) {
        // Calculate the pixel position with wrapping
        int pos = (startPos + i) % strip.numPixels();
        strip.setPixelColor(pos, currentAnim.color);
    }
    
    // Show the updated strip
    strip.show();
}
