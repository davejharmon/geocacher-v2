// #ifndef NEOPIXEL_STATE_MACHINE_HANDLER_H
// #define NEOPIXEL_STATE_MACHINE_HANDLER_H

// #include <Adafruit_NeoPixel.h>

// class NeoPixelStateMachineHandler {
// public:
//     static const uint8_t PIN = 6;
//     static const uint16_t NUMPIXELS = 30;

//     // Declare color definitions
//     static extern const uint32_t RED;
//     static extern const uint32_t GREEN;
//     static extern const uint32_t BLUE;
//     static extern const uint32_t WHITE;
//     static extern const uint32_t BLACK;
//     static extern const uint32_t YELLOW;

//     NeoPixelStateMachineHandler();

//     void begin();
//     void setAnimationState(AnimationState state);
//     void update();

// private:
//     Adafruit_NeoPixel strip;
//     AnimationState currentState;
//     unsigned long lastUpdate;
//     uint16_t index;
//     uint32_t color;

//     void pulse();
//     void rainbowChase();
//     void colorWipe(uint32_t color);
//     void theaterChase(uint32_t color);

//     uint32_t wheel(byte pos);
// };

// #endif
