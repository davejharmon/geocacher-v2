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

#define BTN_RED 2
#define BTN_BLU 5
#define GPS_RX 4
#define GPS_TX 3
#define BAUD_RATE 9600
#define MAX_DISTANCE 600        // in meters (fix me)
const float ALPHA = 0.2;        // smoothing factor (for compass)

// Include breakout files
#include "GPS_Coordinates.h"  // Catalog of hard-coded GPS locations
#include "neopixelhandler.h"

NeopixelHandler neopixels;
LSM303 compass;
TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX, GPS_TX);  

unsigned static long TICK = 200;
unsigned long tock = 0;     // Variable to store the time of the last heartbeat
int prevRedBtnState = HIGH;
int prevBluBtnState = HIGH;
int dest = 0;
int course = 0;
int spread = 1;
float heading = 0;
int mode = 0;                 // 0: Searching for GPS, 1: Showing course, -1: Error

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available()) gps.encode(ss.read());
  } while (millis() - start < ms);
}

void updatePixels() {
  switch (mode) {
    case 0:
      neopixels.chase(BLUE);
      break;
    case 1:
      neopixels.drawLine(TARGETS[dest].col, course, spread);
      break;
    default:
      // Default behavior
      break;
  }
}

void updateDestination() {
  dest++;
  if (dest >= TARGET_COUNT) {
    dest = 0;
  }
  neopixels.colorWipe(TARGETS[dest].col, 50);
}

void pulseDestination(uint8_t wait) {
  neopixels.drawLine(WHITE, course, gps.satellites.isValid() + 1);
  smartDelay(25);
  neopixels.setColor(BLACK);
  for (int i = 1; i <= spread; i++) {
    neopixels.drawLine(TARGETS[dest].col, course, i);
    smartDelay(wait);
  }
}

void manageButtons() {
  int btn_red_state = digitalRead(BTN_RED);
  int btn_blu_state = digitalRead(BTN_BLU);

  if (btn_red_state == LOW && prevRedBtnState == HIGH) {  // handle button clicks
    Serial.println("click red!");
    updateDestination();
  }

  if (btn_blu_state == LOW && prevBluBtnState == HIGH) {
    Serial.println("click blue!");
    pulseDestination(50);
  }

  prevBluBtnState = btn_blu_state;
  prevRedBtnState = btn_red_state;
}

void setup(void) {
  
  neopixels.begin();
  neopixels.setColor(BLACK);
  Serial.begin(BAUD_RATE);
  ss.begin(BAUD_RATE);
  Serial.println("Setup online, starting serial...");

  pinMode(BTN_RED, INPUT_PULLUP);  // initialize push buttons
  pinMode(BTN_BLU, INPUT_PULLUP);

  compass.init();  // initialize LSM303
  compass.enableDefault();
  compass.m_min = (LSM303::vector<int16_t>){-608, -544, -495};  // get values using this: https://github.com/pololu/lsm303-arduino/tree/master/examples/Calibrate
  compass.m_max = (LSM303::vector<int16_t>){+369, +478, +418};

  Serial.println("Hello world how are you!");

  tock = millis();
}

void updateHeading(double lat, double lng) {
    heading = ALPHA * compass.heading() + (1 - ALPHA) * heading;  // Calculate course and spread
    int north = static_cast<int>(heading);
    double courseTo = gps.courseTo(lat, lng, TARGETS[dest].lat, TARGETS[dest].lng);
    double distanceTo = gps.distanceBetween(lat, lng, TARGETS[dest].lat, TARGETS[dest].lng);
    spread = map(distanceTo, MAX_DISTANCE, 0, 0, 100);
    course = abs(north - int(courseTo));

    char buffer[50];
    sprintf(buffer, "course: %d, spread: %d\n", course, spread);
    Serial.print(buffer);
}

void loop(void) {
  manageButtons();  // Monitor buttons & interrupt on click

  if (millis() >= TICK + tock) {  // Update compass and GPS at a comfortable cadence
    compass.read();
    tock = millis();

    mode = gps.location.isValid() ? 1 : 0;
    
    if (mode==0) {
      // lat = DUMMY.lat;
      // lng = DUMMY.lng;
      Serial.print("Reading from GPS... sentences: ");
      Serial.print(gps.sentencesWithFix());
      Serial.print(" chars: ");
      Serial.println(gps.charsProcessed());
    } else if (mode ==1) {
      updateHeading(gps.location.lat(), gps.location.lng());

    }
    Serial.println(tock);
    updatePixels();
  }

  smartDelay(10);  // Ensure GPS data is processed continuously
}
