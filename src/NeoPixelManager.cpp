// neopixelmanager.cpp
#include "NeoPixelManager.h"
#include "constants.h"

NeoPixelManager::NeoPixelManager()
    : strip(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800), // Initialize with number of pixels, pin, and LED type
      lastUpdateTime(0)
{
    // Initialize state arrays to black (off)
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        state[i] = 0;
        oldState[i] = 0;
    }
}

void NeoPixelManager::begin() {
    strip.begin();
    strip.setBrightness(25);
    strip.show(); // Initialize all pixels to off
}

void NeoPixelManager::setPixel( uint32_t col,uint8_t index) {
    if (index < NUMPIXELS) {
        state[index] = col;
    }
}

void NeoPixelManager::setStrip(const uint32_t newState[NUMPIXELS]) {
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        state[i] = newState[i];
    }
}

void NeoPixelManager::drawLine(uint32_t col, float angle, uint8_t percent, bool centered) {
    clear();
    
    // Normalize angle to the range [0, 360)
    float normalizedAngle = fmod(angle, 360.0f);
    if (normalizedAngle < 0) {
        normalizedAngle += 360.0f; // Ensure angle is positive
    }
    
    // Map normalized angle to pixel index
    int baselinePixel = static_cast<int>(normalizedAngle / 360.0f * NUMPIXELS) % NUMPIXELS;
    
    // Calculate length based on percent
    int length = max(1, map(percent, 0, 100, 0, NUMPIXELS));

    // Calculate start and end pixels
    int startPixel = baselinePixel;
    if (centered) {
        startPixel = (baselinePixel - ((length - 1) / 2) + NUMPIXELS) % NUMPIXELS;
    }

    int endPixel = (startPixel + length - 1) % NUMPIXELS;

    // Draw line with wrapping if needed
    if (startPixel <= endPixel) {
        // No wrapping needed
        for (int i = startPixel; i <= endPixel; ++i) {
            state[i] = col;
        }
    } else {
        // Wrapping needed
        for (int i = startPixel; i < NUMPIXELS; ++i) {
            state[i] = col;
        }
        for (int i = 0; i <= endPixel; ++i) {
            state[i] = col;
        }
    }
}

void NeoPixelManager::clear(){
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        state[i] = BLACK;
    }    
}

void NeoPixelManager::fill(uint32_t col){
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        state[i] = col;
    }    
}

// animation functions

bool NeoPixelManager::isPlaying(){
    return playing!=ANIM_NONE;
}

void NeoPixelManager::start(uint8_t type, uint32_t col, uint8_t sty) {
    playing=type;
    step = 0;
    color = col;
    style = sty;
}

void NeoPixelManager::updateWipe() {
    if (playing==ANIM_WIPE && step < NUMPIXELS) {
        setPixel(color, step); // Set each pixel to white as the wipe progresses
        step++;
        if (step >= NUMPIXELS) {
            playing = ANIM_NONE; // Stop when the wipe is complete
        }
    }
}


void NeoPixelManager::updateRainbowPinwheel() {
    if (playing==ANIM_RAINBOW) {
        for (int i = 0; i < NUMPIXELS; ++i) {
            int pixelIndex = (i + step) % NUMPIXELS;
            uint16_t hueRange;
            switch (style) {
                case 1: hueRange = 7330 + (pixelIndex * (9830 - 7330) / NUMPIXELS);break;           // day
                case 2: hueRange = 29000 + (pixelIndex * (43690 - 29000) / NUMPIXELS);break;        // night
                default: hueRange = 0  + (pixelIndex * 65536 / NUMPIXELS);break;                    // rainbow
            }
            uint32_t rainbowColor = strip.ColorHSV(hueRange, 255, 255);
            setPixel(rainbowColor, i);
        }
        step++;
    }
}

void NeoPixelManager::startTimer(uint32_t dur) {
    playing=ANIM_TIMER;
    step=0;
    started = uint32_t(millis());
    endsAt = started+dur; 
}

void NeoPixelManager::updateTimer() {
    // Check if the timer has ended
    if (millis() > endsAt) {
        Serial.println("Timer elapsed!");
        playing = ANIM_NONE; // Stop the animation if time has expired
        return;
    }

    // Calculate the elapsed time
    uint32_t elapsedTime = millis() - started;
    float percentageElapsed = (float)elapsedTime / (endsAt - started);

    // Calculate the number of pixels to paint white based on the percentage elapsed
    uint16_t numPixelsToPaint = NUMPIXELS * percentageElapsed;

    // Update each pixel
    for (uint16_t i = 0; i < NUMPIXELS; ++i) {
        if (i < numPixelsToPaint) {
            if (i % 3 == 0) setPixel(WHITE, i); // Paint the pixel white
        }
    }

    // Show the updated pixels
    strip.show();
}


void NeoPixelManager::interrupt(uint8_t type) {
    if (type==ANIM_NONE || playing==type) {
        playing=ANIM_NONE;
        step=0;
        style=0;
    }
}

bool NeoPixelManager::update() {
 
    bool hasChanged = false;
    uint32_t currentTime = millis();

    // Check if enough time has passed since the last update
    if (currentTime - lastUpdateTime >= ANIMATION_INTERVAL) {

        // Update animations
        if (playing==ANIM_WIPE) {
            updateWipe();
            hasChanged = true;
        }

        if (playing==ANIM_RAINBOW) {
            updateRainbowPinwheel();
            hasChanged = true;
        }
        
        if (playing==ANIM_TIMER) {
            updateTimer();
        }

        // If there's a change in state, set hasChanged to true
        for (int i = 0; i < NUMPIXELS; ++i) {
            if (state[i] != oldState[i]) {
                hasChanged = true;
                break;
            }
        }
        
        // Update oldState to current state
        memcpy(oldState, state, sizeof(state));

        if (hasChanged) {
            for (int i = 0; i < NUMPIXELS; ++i) {
                strip.setPixelColor(i, state[i]);
            }
            strip.show();
        }

        lastUpdateTime = currentTime;
    }
    return hasChanged;
}