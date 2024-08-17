// main.cpp
#include <Arduino.h>
#include <Wire.h>
#include <LSM303.h>
#include <TinyGPSPlus.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// pin definitions
#define BTN_RED 2
#define BTN_BLU 5
#define GPS_RX 4
#define GPS_TX 3

// constants
#define BAUD_RATE 9600
#define MAX_DISTANCE 600        // in meters (fix me)

// breakout files & handlers
#include "Button.h"
#include "CompassHandler.h"
#include "NeoPixelAnimationManager.h"
#include "GPS_Coordinates.h"
#include "GPS_Handler.h"

// sensors & devices
Button redButton(BTN_RED);
Button blueButton(BTN_BLU);
CompassHandler compass;
NeoPixelAnimationManager pixelManager;
GPSHandler gps(GPS_RX, GPS_TX);

// modes
#define MODE_SEARCHING_FOR_GPS 0
#define MODE_HEADING_TO_TARGET 1
#define MODE_ERROR -1
int mode, prevMode = MODE_SEARCHING_FOR_GPS;

// Animation control
#define ANIMATION_DURATION 5000 // Duration for each animation in milliseconds
unsigned long animationStartTime = 0;
bool isAnimationActive = false;

// Define callback functions
void onRedButtonClick() {
    Serial.println("Red button clicked");
    float northAngle = compass.getNorth(); // Get current north angle
    pixelManager.startAnimation(ANIM_ARROW, GREEN, northAngle, 40);
}

void onBlueButtonClick() {
    Serial.println("Blue button clicked");
    
}

void setup(void) {
  Serial.begin(BAUD_RATE);
  Serial.println("Hello world how are you!");
  redButton.begin();
  redButton.onClick(onRedButtonClick);
  blueButton.begin();
  blueButton.onClick(onBlueButtonClick);
  compass.begin();
  gps.begin();
  pixelManager.begin();
  pixelManager.startAnimation(ANIM_IDLE);
}

void loop(void) {
    blueButton.update();
    redButton.update();
    float north = compass.getNorth();
    gps.update();
    bool animIsPlaying = pixelManager.update(); // Ensure this is called frequently
    if (!animIsPlaying) pixelManager.startAnimation(ANIM_IDLE, WHITE);
}
