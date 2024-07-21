#include "neopixelhandler.h"

// Color definitions
const uint32_t RED    = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t GREEN  = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t BLUE   = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t WHITE  = Adafruit_NeoPixel::Color(255, 255, 255);
const uint32_t BLACK  = Adafruit_NeoPixel::Color(0, 0, 0);

#define PIN 6                   // pinouts
#define NUMPIXELS 24            // neopixel ring size


// Constructor
NeopixelHandler::NeopixelHandler()
    : pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800) {}

// Initialize the Neopixels
void NeopixelHandler::begin() {
    pixels.begin();
    pixels.setBrightness(50);
    setColor(BLACK);
}

// Set all pixels to a specific color
void NeopixelHandler::setColor(uint32_t color) {
    for(int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

// Fill the entire strip with a color one pixel at a time
void NeopixelHandler::colorWipe(uint32_t color, int wait) {
    for(int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, color);
        pixels.show();
        delay(wait);
    }
}

// Theater chase animation with rainbow colors
void NeopixelHandler::theaterChaseRainbow(int wait) {
    int firstPixelHue = 0;
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i < pixels.numPixels(); i += 3) {
            pixels.setPixelColor(i + 0, pixels.ColorHSV(firstPixelHue + (i * 65536L / pixels.numPixels()), 255, 255));
            pixels.setPixelColor(i + 1, pixels.ColorHSV(firstPixelHue + (i * 65536L / pixels.numPixels()), 255, 255));
            pixels.setPixelColor(i + 2, pixels.ColorHSV(firstPixelHue + (i * 65536L / pixels.numPixels()), 255, 255));
        }
        pixels.show();
        delay(wait);
        firstPixelHue += 65536 / 90;
    }
}

// Draw a line of color on the Neopixel strip
void NeopixelHandler::drawLine(uint32_t color, int heading, int length) {
    int center=map(heading, 359, 0, 0, NUMPIXELS-1);    // LSM mounted ups
    int start = center - (length / 2); 

    // Clear the pixels before drawing the line
    setColor(BLACK);

    for (int i = 0; i < length; i++) {
        int index = (start + i) % NUMPIXELS;
        if (index < 0) index += NUMPIXELS; // Handle negative indices
        pixels.setPixelColor(index, color);
    }
    pixels.show(); // Show the updated colors
}
