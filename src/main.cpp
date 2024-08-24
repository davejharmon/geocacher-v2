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
#define LOW_BAT 22;
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
NeoPixelAnimationManager pixels;
GPSHandler gps(GPS_RX, GPS_TX);

// modes
#define MODE_SEARCHING_FOR_GPS 0
#define MODE_HAS_VALID_GPS 1
#define MODE_HEADING_TO_TARGET 2
#define MODE_ERROR -1


int mode = MODE_SEARCHING_FOR_GPS;
int target=0;

void navigateToNorth() {
    float north = compass.getNorth();
    int distance = 25;
    pixels.startAnimation(ANIM_ARROW,BLUE,north,distance);
}

void navigateToTarget() {  
    float north = compass.getNorth();
    double heading = gps.getDirection(locations[target]);
    double distanceBetween = gps.getDistance(locations[target]);
    int distancePercent = 100-(distanceBetween/MAX_DISTANCE*100); 
    float course=north-heading;
    if (course < 0) course+=360.0;
      pixels.startAnimation(ANIM_ARROW,locations[target].color,course,distancePercent);
}

// Define callback functions
void onRedButtonClick() {
    Serial.println("Red button clicked");
    float north = compass.getNorth();
    // double heading = gps.getDirection(locations[target]);
    // double distanceBetween = gps.getDistance(locations[target]);
    // int distancePercent = 100-(distanceBetween/MAX_DISTANCE*100); 
    // float course=north-heading;
    pixels.startAnimation(ANIM_PULSE_ARROW, locations[target].color, north, 25);
  //pixels.startAnimation(ANIM_RAINBOW);
}

void onBlueButtonClick() {
    Serial.println("Blue button clicked");
    target = (target + 1) % MAX_TARGETS;
    pixels.startAnimation(ANIM_WIPE,locations[target].color);
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
  pixels.startAnimation(ANIM_RAINBOW);
  gps.begin();
}



void loop(void) {
      blueButton.update();
      redButton.update();
      gps.update();
      gps.debug();
      bool playing = pixels.update();
    if (!playing) { // If not playing, find a fallback animation
      switch (mode) {
        case MODE_SEARCHING_FOR_GPS: // MODE_SEARCHING_FOR_GPS
          
          if (!gps.isValid()) {
            //pixels.startAnimation(ANIM_RAINBOW);
            navigateToNorth();
          } else {
            mode = MODE_HAS_VALID_GPS;
          }
          break;
        case MODE_HAS_VALID_GPS:
          if (gps.isValid()) {
            navigateToTarget();
          } else {
            mode = MODE_SEARCHING_FOR_GPS;
          }
          break;
        default:
          break;
      }
    }

  }

