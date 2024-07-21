#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_LSM303.h>
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
#define TICK 10                 // heartbeat in milliseconds
const float ALPHA = 0.2;        // smoothing factor (for compass)

// Include breakout files
// #include "Colors.h"           // Constants for main colours
// #include "GPS_Coordinates.h"  // Catalog of hard-coded GPS locations
// #include "NeoPixelEffects.h"  // NeoPixel animations & effects
#include "neopixelhandler.h"

NeopixelHandler neopixels;

DFRobot_LSM303 compass;
TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX, GPS_TX);  

unsigned long TOCK = 2;     // Variable to store the time of the last heartbeat
int prevRedBtnState = HIGH;
int prevBluBtnState = HIGH;
int dest = 0;
int adjustedCourse=0;
int spread=0;
float smoothedHeading=0;
int mode=1;                 // 0: Searching for GPS, 1: Showing course, -1: Error

static void smartDelay(unsigned long ms)                                                                            // a custom version of delay() that ensures the gps is being fed
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);

}

void updatePixels() {
  if (millis() - TOCK >= TICK) {
    switch (mode) {
      case 0:
        neopixels.theaterChaseRainbow(500);
        break;
      case 1:
        neopixels.drawLine(RED,adjustedCourse,spread);
        break;
      default:
        // Default behavior
        break;
    }
    TOCK = millis();
  }
}

// void updateDestination() {
//   dest++;
//   pixels.colorWipe(TARGETS[dest].col, 50);
//   if (dest >= static_cast<int>(sizeof(TARGETS) / sizeof(TARGETS[0]))) dest = 0;
// }

// void pulseDestination(uint8_t wait) {
//   setColor(0);
//   drawLine(WHITE, adjustedCourse, gps.satellites.isValid()+1);
//   smartDelay(300);
//   setColor(0);
//   for (int i = 1; i <= spread; i++) {
    
//     drawLine(TARGETS[dest].col,adjustedCourse,i);
//     smartDelay(wait);
//   }
// }

void setup(void) {
  Serial.begin(BAUD_RATE);
  ss.begin(BAUD_RATE);

  pinMode(BTN_RED, INPUT_PULLUP);                                                                                   // initialise push buttons
  pinMode(BTN_BLU, INPUT_PULLUP);

  compass.init();                                                                                                   // initialise LSM303
  compass.enable();
  neopixels.begin();

  Serial.println("Hello world how are you!");
  Serial.println("");
}


void loop(void) {
  // while (ss.available()) gps.encode(ss.read());

  compass.read();                                         // read inputs                        
  int btn_red_state = digitalRead(BTN_RED);
  int btn_blu_state = digitalRead(BTN_BLU);

  if (btn_red_state==LOW && prevRedBtnState==HIGH) {      // handle button clicks
    Serial.println("click red!");
    // updateDestination();
  }
  prevRedBtnState=btn_red_state;

  if (btn_blu_state==LOW && prevBluBtnState==HIGH) {
    Serial.println("click blue!");
    // pulseDestination(50);
  }
  prevBluBtnState=btn_blu_state;

  // double lat = DUMMY.lat;                                                                                             // dummy GPS for indoor code
  // double lng = DUMMY.lng; 
  if (true) {

//  double lat = gps.location.lat();
//  double lng = gps.location.lng();
//  if (gps.location.isValid() == true) {                                                                                // if gps has a lock... display heading and distance
    float heading = compass.getNavigationAngle();
    // smoothedHeading = ALPHA * heading + (1 - ALPHA) * smoothedHeading;                                               // apply smoothing
    int north = static_cast<int>(heading);
    //double courseTo = gps.courseTo(lat, lng, TARGETS[dest].lat, TARGETS[dest].lng);
    //double distanceTo = gps.distanceBetween(lat, lng, TARGETS[dest].lat, TARGETS[dest].lng);
    //adjustedCourse=map(abs(north-int(courseTo)),0,360,NUM_PIXELS,0);
    //spread = map(distanceTo, MAX_DISTANCE,0,1,NUM_PIXELS);
    // smartDelay(25);
    adjustedCourse=north;
    spread=1;
    Serial.print(north);
    Serial.print("-");
    Serial.println(spread);
  } else {                                                                                                            // or else, display 
    updatePixels();
    Serial.print("reading from gps... sent: ");
    Serial.print(gps.sentencesWithFix());
    Serial.print(" chars: ");
    Serial.println(gps.charsProcessed());
  }
    updatePixels();   
}