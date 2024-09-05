#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Adafruit_NeoPixel.h>

// Color definitions
extern const uint32_t RED;       // Red color
extern const uint32_t GREEN;     // Green color
extern const uint32_t BLUE;      // Blue color
extern const uint32_t WHITE;     // White color
extern const uint32_t BLACK;     // Black color
extern const uint32_t CYAN;      // Cyan color
extern const uint32_t MAGENTA;   // Magenta color
extern const uint32_t YELLOW;    // Yellow color
extern const uint32_t ORANGE;    // Orange color
extern const uint32_t PURPLE;    // Purple color
extern const uint32_t TEAL;      // Teal color
extern const uint32_t PINK;      // Pink color

// neopixels
#define NUMPIXELS 24
#define NEO_PIN 6
#define ANIMATION_INTERVAL 50

// passwords
#define R true
#define B false

// Animation types
#define ANIM_NONE 0
#define ANIM_RAINBOW 1
#define ANIM_ARROW 2
#define ANIM_WIPE 3
#define ANIM_TIMER 4

#endif // CONSTANTS_H
