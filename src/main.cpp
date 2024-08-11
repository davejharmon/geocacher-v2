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
#define BTN_RED 5
#define BTN_BLU 2
#define GPS_RX 4
#define GPS_TX 3

// constants
#define BAUD_RATE 9600
#define MAX_DISTANCE 600        // in meters (fix me)

// breakout files & handlers
#include "Button.h"
#include "CompassHandler.h"
#include "NeoPixelAnimationManager.h"

// sensors & devices
Button redButton(BTN_RED);
Button blueButton(BTN_BLU);
CompassHandler compass;
NeoPixelAnimationManager pixels;

// modes
#define MODE_SEARCHING_FOR_GPS 0
#define MODE_HEADING_TO_TARGET 1
#define MODE_ERROR -1
int mode, prevMode = MODE_SEARCHING_FOR_GPS;

// Define callback functions
void onRedButtonClick() {
    Serial.println("Red button clicked");
    pixels.startAnimation(0); // Start chase animation
}

void onBlueButtonClick() {
    Serial.println("Blue button clicked");
    pixels.startAnimation(1); // Start pulse animation
}

void setup(void) {
  Serial.begin(BAUD_RATE);
  Serial.println("Hello world how are you!");
  redButton.begin();
  redButton.onClick(onRedButtonClick);
  blueButton.begin();
  blueButton.onClick(onBlueButtonClick);
  compass.begin();
  pixels.begin();
}

void loop(void) {
  blueButton.update();
  redButton.update();
  float north = compass.getNorth();
  Serial.print("North: ");
  Serial.println(north);
  pixels.update();
}
