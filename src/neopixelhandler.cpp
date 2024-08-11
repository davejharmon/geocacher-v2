// //neopixelhandler.cpp
// #include "neopixelhandler.h"

// // Color definitions
// const uint32_t RED    = Adafruit_NeoPixel::Color(255, 0, 0);
// const uint32_t GREEN  = Adafruit_NeoPixel::Color(0, 255, 0);
// const uint32_t BLUE   = Adafruit_NeoPixel::Color(0, 0, 255);
// const uint32_t WHITE  = Adafruit_NeoPixel::Color(255, 255, 255);
// const uint32_t BLACK  = Adafruit_NeoPixel::Color(0, 0, 0);
// const uint32_t YELLOW = Adafruit_NeoPixel::Color(255, 255, 20);

// // Constructor
// NeopixelHandler::NeopixelHandler()
//     : pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800) {}

// // Initialize the Neopixels
// void NeopixelHandler::begin() {
//     pixels.begin();
//     pixels.setBrightness(25);
//     setColor(BLACK);
// }

// // Set all pixels to a specific color
// void NeopixelHandler::setColor(uint32_t color) {
//     for (int i = 0; i < NUMPIXELS; i++) {
//         pixels.setPixelColor(i, color);
//     }
//     pixels.show();
// }

// // Fill the entire strip with a color one pixel at a time
// void NeopixelHandler::colorWipe(uint32_t color, int wait) {
//     for (int i = 0; i < NUMPIXELS; i++) {
//         pixels.setPixelColor(i, color);
//         pixels.show();
//         delay(wait);
//     }
// }

// // Draw a line of color on the Neopixel strip
// void NeopixelHandler::drawLine(uint32_t color, int heading, int percent) {
//     // Correctly map the heading to a pixel index
//     int center = map(heading, 0, 359, 0, NUMPIXELS - 1);
//     int length = NUMPIXELS*percent/100;
//     int start = center - (length / 2);

//     // Clear the pixels before drawing the line
//     setColor(BLACK);

//     for (int i = 0; i < length; i++) {
//         int index = (start + i) % NUMPIXELS;
//         if (index < 0) index += NUMPIXELS; // Handle negative indices
//         pixels.setPixelColor(index, color);
//     }
//     pixels.show(); // Show the updated colors
// }

// void NeopixelHandler::chase(uint32_t color, int frequency) {
//     // Clear all pixels first
//     pixels.clear();

//     // Illuminate every third pixel based on chaseIndex
//     for (int i = 0; i < NUMPIXELS; i++) {
//         if ((i + chaseIndex) % frequency == 0) {
//             pixels.setPixelColor(i, color);
//         }
//     }

//     // Update chaseIndex and wrap around if necessary
//     chaseIndex = (chaseIndex + 1) % frequency;

//     // Show the updated colors on the strip
//     pixels.show();
// }

// void NeopixelHandler::pulseArrow(uint32_t color, int heading) {
//     pixels.clear();
    
//     // Correctly map the heading to a pixel index
//     int center = map(heading, 0, 359, 0, NUMPIXELS - 1);
    
//     // Set the color of the pixel at the mapped position
//     pixels.setPixelColor(center, color);
    
//     // Show the updated colors on the strip
//     pixels.show();
// }
