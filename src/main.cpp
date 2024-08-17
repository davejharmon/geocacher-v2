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
NeoPixelAnimationManager pixelManager;
GPSHandler gps(GPS_RX, GPS_TX);

// modes
#define MODE_SEARCHING_FOR_GPS 0
#define MODE_HAS_VALID_GPS 1
#define MODE_HEADING_TO_TARGET 2
#define MODE_ERROR -1
int mode = MODE_SEARCHING_FOR_GPS;
int target=0;


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
  pixelManager.begin();
  pixelManager.startAnimation(ANIM_IDLE);
  gps.begin();
}

void navigateToTarget() {
  
    float north = compass.getNorth();
    double heading = gps.getDirection(locations[target]);
    float course=north-heading;
    if (course < 0) course+=360.0;
    // Serial.print("north: ");
    // Serial.print(north);
    //   Serial.print("head: ");
    // Serial.print(heading);
    // Serial.print(" course: ");
    // Serial.println(course);
    int distanceTo = 5;
    pixelManager.startAnimation(ANIM_NAVIGATE_TO,locations[target].color,course,distanceTo);
}

void loop(void) {
    blueButton.update();
    redButton.update();
    gps.update();
    switch (mode) {
      case MODE_SEARCHING_FOR_GPS: // MODE_SEARCHING_FOR_GPS
          if (gps.isValid()) {
            mode = MODE_HAS_VALID_GPS;
            navigateToTarget();
          } else {
            gps.debug();
          };
          break;
      case MODE_HAS_VALID_GPS:
        if (gps.isValid()) {
         navigateToTarget();
        }
        else {
          mode = MODE_SEARCHING_FOR_GPS;
          pixelManager.startAnimation(ANIM_IDLE);
        }
        break;
        default:
          break;
    }


    bool animIsPlaying = pixelManager.update(); // Ensure this is called frequently
    if (!animIsPlaying) pixelManager.startAnimation(ANIM_IDLE, WHITE);
}

