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
const int LED_PIN=7;

// constants
#define BAUD_RATE 9600
#define MAX_DISTANCE 600        // in meters (fix me)
#define PASSWORD_DELAY 2000

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
#define MODE_ENTER_PASSWORD 3

int mode = MODE_SEARCHING_FOR_GPS;
int prevMode=-1;
int target=0;
long lastRedPress=0;
long lastBluePress=0;

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

void updateMode(int newMode) {
  Serial.print("New mode: ");
  Serial.println(mode);
  prevMode=mode;
  mode=newMode;
}

// Define callback functions
void onRedButtonClick() {
    if (blueButton.isClicked()) digitalWrite(LED_PIN, HIGH);
    float north = compass.getNorth();
    switch (mode) {
      case MODE_ENTER_PASSWORD: pixels.startAnimation(ANIM_FLASH, RED,0,100,true); break;
      case MODE_SEARCHING_FOR_GPS: pixels.startAnimation(ANIM_PULSE_ARROW, locations[target].color, north, 25); break;
      case MODE_HAS_VALID_GPS: {
        double heading = gps.getDirection(locations[target]);
        double distanceBetween = gps.getDistance(locations[target]);
        int distancePercent = 100-(distanceBetween/MAX_DISTANCE*100); 
        float course=north-heading;
        pixels.startAnimation(ANIM_PULSE_ARROW, locations[target].color, course, distancePercent);
        break;
      }
      default: break;
    }
}

void onBlueButtonClick() {
    if (redButton.isClicked()) digitalWrite(LED_PIN, HIGH);
    switch (mode) {
      case MODE_ENTER_PASSWORD: pixels.startAnimation(ANIM_FLASH, BLUE,0,100,true); break;
      case MODE_SEARCHING_FOR_GPS:
      case MODE_HAS_VALID_GPS: {
        target = (target + 1) % MAX_TARGETS;
        pixels.startAnimation(ANIM_WIPE,locations[target].color,0.0,1,false);
        break;
      }
      default: break;
    }
}

void startPasswordMode() {
      pixels.clear();
      pixels.startAnimation(ANIM_WIPE, WHITE, 0.0, 6);
      updateMode(MODE_ENTER_PASSWORD);
}

void onRedButtonRelease(unsigned long pressDuration) {
  Serial.println("Red button released");
  lastRedPress=pressDuration;
  if (!blueButton.isClicked()) digitalWrite(LED_PIN, LOW);
  if (lastRedPress > PASSWORD_DELAY && lastBluePress > PASSWORD_DELAY) {
    lastRedPress, lastBluePress = 0;
    if (mode!=MODE_ENTER_PASSWORD) {
      startPasswordMode();
      }
  }
}

void onBlueButtonRelease(unsigned long pressDuration) {
  Serial.println("Blue button released");
  lastBluePress=pressDuration;
  if (!redButton.isClicked()) digitalWrite(LED_PIN, LOW);
  if (lastRedPress > PASSWORD_DELAY && lastBluePress > PASSWORD_DELAY) {
    lastRedPress, lastBluePress = 0;
    if (mode!=MODE_ENTER_PASSWORD) {
      startPasswordMode();
      }
  }  
}

void setup(void) {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
  Serial.println("Hello world how are you!");
  redButton.begin();
  redButton.onClick(onRedButtonClick);
  redButton.onRelease(onRedButtonRelease);
  blueButton.begin();
  blueButton.onClick(onBlueButtonClick);
  blueButton.onRelease(onBlueButtonRelease);
  compass.begin();
  pixels.begin();
  pixels.startAnimation(ANIM_RAINBOW);
  gps.begin();
}


void loop(void) {
      blueButton.update();
      redButton.update();
      gps.update();
//      gps.debug();
      bool playing = pixels.update();
    if (!playing) { // If not playing, find a fallback animation
      switch (mode) {
        case MODE_SEARCHING_FOR_GPS: // MODE_SEARCHING_FOR_GPS
          
          if (!gps.isValid()) {
            //pixels.startAnimation(ANIM_RAINBOW);
            navigateToNorth();
          } else {
           updateMode(MODE_HAS_VALID_GPS);
          }
          break;
        case MODE_HAS_VALID_GPS:
          if (gps.isValid()) {
            navigateToTarget();
          } else {
            updateMode(MODE_SEARCHING_FOR_GPS);
          }
          break;
        case MODE_ENTER_PASSWORD:
          //if nothing is playing the timer has ended
          break;
        default:
          break;
      }
    }
  }

