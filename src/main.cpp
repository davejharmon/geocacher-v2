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
#define PASSWORD_TIMER ((uint32_t)12000)


// breakout files & handlers
#include "Button.h"
#include "CompassHandler.h"
#include "NeoPixelManager.h"
#include "GPS_Coordinates.h"
#include "GPS_Handler.h"
#include "constants.h"

// sensors & devices
Button redButton(BTN_RED);
Button blueButton(BTN_BLU);
CompassHandler compass;
NeoPixelManager pixels;
GPSHandler gps(GPS_RX, GPS_TX);

// modes
#define MODE_SEARCHING_FOR_GPS 0
#define MODE_HAS_VALID_GPS 1
#define MODE_ENTER_PASSWORD 2
#define MODE_SECRET_MENU 3

#define PASS_CHAR_SIZE 3
#define NULL_VAL 255

uint8_t mode = NULL_VAL; // impossible value, no mode entered
uint8_t prevMode=0;
uint8_t target=NULL_VAL; // impossible value, no target entered
long lastRedPress=0;
long lastBluePress=0;
uint32_t userPass[NUMPIXELS] = {0};
bool userPassBools[NUMPIXELS/3]={false};
uint8_t userPassIndex=0;
uint8_t userPassTimer=0;

void navigateToNorth() {
  if (pixels.isPlaying()) return;
  float north = compass.getNorth();
  int distance = 25;
  pixels.drawLine(RED, north, distance);
}

unsigned long lastPrintTime = 0;  // Variable to keep track of last print time

void navigateToTarget(int seconds=0) {
  if (pixels.isPlaying()) return;

  unsigned long currentTime = millis();
  float north = compass.getNorth();
  float heading = gps.getDirection(locations[target]);
  double distanceBetween = gps.getDistance(locations[target]);
  int distancePercent = constrain(map(static_cast<int>(distanceBetween), 0, MAX_DISTANCE, 100, 0), 0, 100);

  float course = north + heading;
  if (course < 0) course += 360.0;

  // Check if a second has passed
  if (currentTime - lastPrintTime >= 1000) {
    Serial.print("Course: "); Serial.print(course); Serial.print(" | ");
    Serial.print("North: "); Serial.print(north); Serial.print(" | ");
    Serial.print("Heading: "); Serial.println(heading);

    lastPrintTime = currentTime;  // Update last print time
  }

  pixels.drawLine(locations[target].color, course, distancePercent);
}

void updateMode(int newMode) {
  String modeName;
  prevMode=mode;
  mode=newMode;
  switch (mode) {
    case MODE_ENTER_PASSWORD: {
      modeName="Enter Password...";
      pixels.startTimer(PASSWORD_TIMER);
      break;
    }
    case MODE_SEARCHING_FOR_GPS: {
      modeName="Searching for GPS...";
 
      break;
    }
    case MODE_HAS_VALID_GPS: {
      modeName="Heading to target...";
      pixels.interrupt(ANIM_RAINBOW);
      break;
    }
    case MODE_SECRET_MENU: {
      modeName="Secret menu unocked!";
      pixels.interrupt();
      pixels.fill(WHITE);
    }   
    default: break;
  }
  Serial.print("New mode: ");
  Serial.println(modeName);

}

void checkPassword() {
  bool matchFound=false;
  // For each GPS_Coordinate in locations, checkPassword with userPassBools
  for (int i = 0; i < MAX_TARGETS; ++i) {
      if (locations[i].checkPassword(userPassBools)) {
          target = i;  // Update target to the index of the matching coordinate
          pixels.start(ANIM_WIPE, locations[target].color);
          matchFound=true;
          break;  // Exit the loop once a match is found
      }
  }

  if (!matchFound) pixels.start(ANIM_WIPE, BLACK);
  for (int i = 0; i < NUMPIXELS; ++i) {
    userPass[i] = 0x000000;  // Set each element to black
  }
  userPassIndex=0;
  userPassTimer=0;
  updateMode(gps.isValid() ? MODE_HAS_VALID_GPS : MODE_SEARCHING_FOR_GPS);
}

void inputPassword(uint32_t col) {
  uint8_t first = PASS_CHAR_SIZE*userPassIndex; 
  for (uint8_t i=first+1;i<first+3;i++) { // skip the last pixel
    userPass[i]=col;

  }
  userPassBools[userPassIndex]=col==RED?R:B;
  userPassIndex++;
  if (userPassIndex*PASS_CHAR_SIZE>NUMPIXELS-1) checkPassword();
}
// Define callback functions
void onRedButtonClick() {
  
    if (blueButton.isClicked()) digitalWrite(LED_PIN, HIGH);
        Serial.println("Red click!");
    switch (mode) {
      case MODE_ENTER_PASSWORD: {
        inputPassword(RED);
        break;
      }
      case MODE_SEARCHING_FOR_GPS: {
        break;
      }
      case MODE_HAS_VALID_GPS: {
        break;
      }
      default: break;
    }
}

void onBlueButtonClick() {
    if (redButton.isClicked()) digitalWrite(LED_PIN, HIGH);
    Serial.println("Blue click!");
    switch (mode) {
      case MODE_ENTER_PASSWORD: {
        inputPassword(BLUE);
        break;
      }
      case MODE_SEARCHING_FOR_GPS: 
      case MODE_HAS_VALID_GPS: {
        break;
      }
      default: break;
    }
}

void onRedButtonRelease(unsigned long pressDuration) {
  lastRedPress=pressDuration;
  if (!blueButton.isClicked()) {
    digitalWrite(LED_PIN, LOW);
    if (mode==MODE_SECRET_MENU) updateMode(MODE_ENTER_PASSWORD);
  }

}

void onBlueButtonRelease(unsigned long pressDuration) {
  lastBluePress=pressDuration;
  if (!redButton.isClicked()) {
    digitalWrite(LED_PIN, LOW);
    if (mode==MODE_SECRET_MENU) updateMode(MODE_ENTER_PASSWORD);
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
  gps.begin();
  updateMode(MODE_SEARCHING_FOR_GPS);
}


void loop(void) {
  gps.update();
  //gps.debug();
  blueButton.update();
  redButton.update();
  pixels.update();

  // if buttons are pressed for more than 2 seconds together enter password mode.
  if (mode!=MODE_SECRET_MENU && redButton.getPressDuration() > PASSWORD_DELAY && blueButton.getPressDuration() > PASSWORD_DELAY) {
    updateMode(MODE_SECRET_MENU);
  }
  switch (mode) {
    case MODE_SEARCHING_FOR_GPS:  // if GPS is found, go to valid gps mode.
      if (gps.isValid()) {
          updateMode(MODE_HAS_VALID_GPS);
      } else {
        if (!pixels.isPlaying()) pixels.start(ANIM_RAINBOW, BLACK, 2); // night mode
      }
      break;
    case MODE_HAS_VALID_GPS: // navigate while in GPS mode, or go to searching mode if GPS lost
      if (gps.isValid()) {
        if (target==NULL_VAL && !pixels.isPlaying()) {
          pixels.start(ANIM_RAINBOW, BLACK, 1);  // day mode
        } else {
          if (!pixels.isPlaying()) navigateToTarget();
        }
      } else {
        updateMode(MODE_SEARCHING_FOR_GPS);
      }
      break;
    case MODE_ENTER_PASSWORD: { // show entered password and timer
        pixels.setStrip(userPass);
        if (!pixels.isPlaying()) updateMode(gps.isValid() ? MODE_HAS_VALID_GPS : MODE_SEARCHING_FOR_GPS);
      };
      break;
    default:
      break;
    }
  }

