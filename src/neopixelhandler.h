#ifndef NEOPIXEL_HANDLER_H
#define NEOPIXEL_HANDLER_H

#include <Adafruit_NeoPixel.h>

// Define the pin and number of pixels
#define PIN            6
#define NUMPIXELS      60

// Color definitions
extern const uint32_t RED;
extern const uint32_t GREEN;
extern const uint32_t BLUE;
extern const uint32_t WHITE;

// Neopixel handler class
class NeopixelHandler {
public:
    NeopixelHandler();
    void begin();
    void setColor(uint32_t color);
    void colorWipe(uint32_t color, int wait);
    void theaterChaseRainbow(int wait);
    void drawLine(uint32_t color, int heading, int wait);
    
private:
    Adafruit_NeoPixel pixels;
};

#endif // NEOPIXEL_HANDLER_H
