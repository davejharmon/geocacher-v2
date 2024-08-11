// #include "NeoPixelStateMachineHandler.h"

// // Define color constants
// const uint32_t NeoPixelStateMachineHandler::RED    = Adafruit_NeoPixel::Color(255, 0, 0);
// const uint32_t NeoPixelStateMachineHandler::GREEN  = Adafruit_NeoPixel::Color(0, 255, 0);
// const uint32_t NeoPixelStateMachineHandler::BLUE   = Adafruit_NeoPixel::Color(0, 0, 255);
// const uint32_t NeoPixelStateMachineHandler::WHITE  = Adafruit_NeoPixel::Color(255, 255, 255);
// const uint32_t NeoPixelStateMachineHandler::BLACK  = Adafruit_NeoPixel::Color(0, 0, 0);
// const uint32_t NeoPixelStateMachineHandler::YELLOW = Adafruit_NeoPixel::Color(255, 255, 20);

// NeoPixelStateMachineHandler::NeoPixelStateMachineHandler()
// : strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800), currentState(ANIMATION_OFF), lastUpdate(0), index(0), color(RED) {}

// void NeoPixelStateMachineHandler::begin() {
//     strip.begin();
//     strip.setBrightness(25);
//     strip.show();  // Initialize all pixels to 'off'
// }

// // ... other methods ...

// void NeoPixelStateMachineHandler::setAnimationState(AnimationState state) {
//     currentState = state;
//     index = 0;  // Reset index for new animation
// }

// void NeoPixelStateMachineHandler::update() {
//     unsigned long currentMillis = millis();

//     switch (currentState) {
//         case ANIMATION_PULSE:
//             if (currentMillis - lastUpdate >= 50) {
//                 lastUpdate = currentMillis;
//                 pulse();
//             }
//             break;
//         case ANIMATION_RAINBOW_CHASE:
//             if (currentMillis - lastUpdate >= 50) {
//                 lastUpdate = currentMillis;
//                 rainbowChase();
//             }
//             break;
//         case ANIMATION_COLOR_WIPE:
//             if (currentMillis - lastUpdate >= 50) {
//                 lastUpdate = currentMillis;
//                 colorWipe(color);
//             }
//             break;
//         case ANIMATION_THEATER_CHASE:
//             if (currentMillis - lastUpdate >= 100) {
//                 lastUpdate = currentMillis;
//                 theaterChase(color);
//             }
//             break;
//         default:
//             strip.clear();
//             strip.show();
//             break;
//     }
// }

// void NeoPixelStateMachineHandler::pulse() {
//     uint8_t brightness = (sin(index * 0.1) * 127) + 128;
//     strip.setBrightness(brightness);
//     strip.fill(color);
//     strip.show();
//     index++;
// }

// void NeoPixelStateMachineHandler::rainbowChase() {
//     for (int i = 0; i < strip.numPixels(); i++) {
//         strip.setPixelColor(i, wheel((i + index) & 255));
//     }
//     strip.show();
//     index++;
// }

// void NeoPixelStateMachineHandler::colorWipe(uint32_t color) {
//     if (index < strip.numPixels()) {
//         strip.setPixelColor(index, color);
//         strip.show();
//         index++;
//     } else {
//         index = 0;
//     }
// }

// void NeoPixelStateMachineHandler::theaterChase(uint32_t color) {
//     for (int i = 0; i < strip.numPixels(); i += 3) {
//         strip.setPixelColor(i + index % 3, color);
//     }
//     strip.show();
//     index++;
// }

// uint32_t NeoPixelStateMachineHandler::wheel(byte pos) {
//     pos = 255 - pos;
//     if (pos < 85) {
//         return strip.Color(255 - pos * 3, 0, pos * 3);
//     } else if (pos < 170) {
//         pos -= 85;
//         return strip.Color(0, pos * 3, 255 - pos * 3);
//     } else {
//         pos -= 170;
//         return strip.Color(pos * 3, 255 - pos * 3, 0);
//     }
// }
