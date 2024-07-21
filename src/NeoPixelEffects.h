// NeoPixelEffects.h
#ifndef NEOPIXEL_EFFECTS_H
#define NEOPIXEL_EFFECTS_H

#include <Adafruit_NeoPixel.h>

// Declare NeoPixel lighting effect functions
uint32_t Wheel(byte WheelPos);
void setColor(Adafruit_NeoPixel& pixels, uint32_t c);
void colorWipe(Adafruit_NeoPixel& pixels, uint32_t c, uint8_t wait);
void rainbow(Adafruit_NeoPixel& pixels, uint8_t wait);
void rainbowCycle(Adafruit_NeoPixel& pixels, uint8_t wait);
void theaterChase(Adafruit_NeoPixel& pixels, uint32_t c, uint8_t wait, uint8_t rpt=1);
void theaterChaseRainbow(Adafruit_NeoPixel& pixels, uint8_t wait);
void drawLine(Adafruit_NeoPixel& pixels, uint32_t c, int center, int length);

#endif
