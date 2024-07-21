// Colors.h
#ifndef COLORS_H
#define COLORS_H

#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel pixels;

static uint32_t RED;
static uint32_t GREEN;
static uint32_t BLUE;
static uint32_t YELLOW;
static uint32_t WHITE;

void initializeColors() {
    RED = pixels.Color(255, 0, 0);
    GREEN = pixels.Color(0, 255, 0);
    BLUE = pixels.Color(0, 0, 255);
    YELLOW = pixels.Color(255, 219, 0);
    WHITE = pixels.Color(255, 255, 255);
}

#endif
