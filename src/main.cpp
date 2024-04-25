#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_LSM303.h>
#include <TinyGPSPlus.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NEO_PIN 6
#define BTN_PIN 2
#define GPS_RX 4
#define GPS_TX 3
#define NUM_PIXELS 24
#define BAUD_RATE 9600
#define MAX_DISTANCE 600 // in meters

class GPS_Coordinate {
  public:
    double lat;   // gps lattitude
    double lng;   // gps longitude
    uint32_t col; // neopixel colour identity

    // Constructor
    GPS_Coordinate(double latitude, double longitude, uint32_t c) : lat(latitude), lng(longitude), col(c) {}
};

DFRobot_LSM303 compass;
TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX, GPS_TX);  
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

static const uint32_t RED = pixels.Color(255,0,0);
static const uint32_t GREEN = pixels.Color(0,255,0);
static const uint32_t BLUE = pixels.Color(0,0,255);
static const uint32_t YELLOW = pixels.Color(255,219,0);
static const uint32_t WHITE = pixels.Color(255,255,255);

GPS_Coordinate BACKYARD(-33.91955388338126, 151.10610149289386, RED);
GPS_Coordinate LITTLE_CAFE(-33.91912615210476, 151.11033004286273, GREEN);
GPS_Coordinate CNR_CANTERBURY(-33.918216637305314, 151.1045469768015, BLUE);
GPS_Coordinate CNR_CROSS(-33.92124868722235, 151.10657241841565, YELLOW);
GPS_Coordinate DUMMY(-33.91885048631868, 151.10756407548317, WHITE);

static const GPS_Coordinate TARGETS[]={BACKYARD, LITTLE_CAFE, CNR_CANTERBURY, CNR_CROSS};

#define MODE_SEARCHING_FOR_GPS 0
#define MODE_ERROR 1
#define MODE_SHOW_COURSE 2
#define MODE_CLEAR_SCREEN 3
#define MODE_BLINK 4
#define TICK 10                                                                                                     // heartbeat in milliseconds
unsigned long TOCK = 2; // Variable to store the time of the last heartbeat
int prevButtonState = HIGH;
int dest = 0;

static void smartDelay(unsigned long ms)                                                                            // a custom version of delay() that ensures the gps is being fed
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

uint32_t Wheel(byte WheelPos) {                                                                                     // neopixel colour effects
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void colorWipe(uint32_t c, uint8_t wait) {                                                                            // fill the dots one after the other with a color

  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    smartDelay(wait);
  }
}
void rainbow(uint8_t wait) {                                                                                          // fil the dots with a rainbow
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    smartDelay(wait);
  }
}
void rainbowCycle(uint8_t wait) {                                                                                     // slightly different, this makes the rainbow equally distributed throughout
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    smartDelay(wait);
  }
}
void theaterChase(uint32_t c, uint8_t wait, uint8_t rpt=1) {                                                                         // theatre style crawling lights
  for (int j=0; j<rpt; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
void theaterChaseRainbow(uint8_t wait) {                                                                              // theatre-style crawling lights with rainbow effect
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
void drawLine( uint32_t c, int center, int length) {                                                                   // draw a line centred on point on the wheel
  int start = center - (length / 2);                              // Calculate the starting pixel index
  int end = center + (length / 2);                                // Calculate the ending pixel index
  
  for (int i = start; i <= end; i++) {
    int index = i;
    if (i < 0) {
      index = NUM_PIXELS + i;                                     // Wrap around for negative indices
    } else if (i >= NUM_PIXELS) {
      index = i - NUM_PIXELS;                                     // Wrap around for indices beyond NUM_LEDS
    }
    pixels.setPixelColor(index, c);                               // Set the color of the pixel at index
  }
  pixels.show();                                                  // Display the updated pixel colors
}

void updatePixels(int8_t mode) {
    if (millis()-TOCK >=TICK) {
      switch(mode) {
          case MODE_SEARCHING_FOR_GPS:                                                                                  // search for GPS signal
              static uint16_t color;
              uint16_t i;
              for(i=0; i< pixels.numPixels(); i++) {
                pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + color) & 255));
              }
              color+=5;
              if (color>255) color=0;
              break;
          case MODE_ERROR:
              // Perform blink animation
              // Example: Toggle pixels on and off
              break;
          case MODE_SHOW_COURSE:
              // Perform fade animation
              // Example: Gradually change brightness of pixels
              break;
          case MODE_CLEAR_SCREEN:
              // Perform random animation
              // Example: Randomly change colors of pixels
              break;
          case MODE_BLINK:
              // Perform random animation
              // Example: Randomly change colors of pixels
              break;
          default:
              // Default behavior
              // Example: Display pixels in a static pattern
              break;
      }
      TOCK = millis();
      pixels.show();
    }
}

void updateDestination() {
  dest++;
  colorWipe(TARGETS[dest].col, 50);
  if (dest >= static_cast<int>(sizeof(TARGETS) / sizeof(TARGETS[0]))) dest = 0;
}

void setup(void) {
  Serial.begin(BAUD_RATE);
  ss.begin(BAUD_RATE);

  pinMode(BTN_PIN, INPUT_PULLUP);                                                                                   // initialise push button

  compass.init();                                                                                                   // initialise LSM303
  compass.enable();

  pixels.begin();                                                                                                   // initialise neopixel pixels
  pixels.setBrightness(50);
  pixels.show();

  Serial.println("Hello world how are you!");
  Serial.println("");
  colorWipe(TARGETS[dest].col, 50);
}

void loop(void) {
  while (ss.available()) gps.encode(ss.read());
  compass.read();
  int btn_state = digitalRead(BTN_PIN);
  if (btn_state==LOW && prevButtonState==HIGH) {
    updateDestination();
  }
  prevButtonState=btn_state;

//  double lat = DUMMY.lat;                                                                                             // dummy GPS for indoor code
//  double lng = DUMMY.lng; 
//  if (true) {

double lat = gps.location.lat();
double lng = gps.location.lng();
if (gps.location.isValid() == true) {                                                                                // if gps has a lock... display heading and distance
    float heading = compass.getNavigationAngle();
    int north = static_cast<int>(heading);
    double courseTo = gps.courseTo(lat, lng, TARGETS[dest].lat, TARGETS[dest].lng);
    double distanceTo = gps.distanceBetween(lat, lng, TARGETS[dest].lat, TARGETS[dest].lng);

    int adjustedCourse=map(abs(north-int(courseTo)),0,360,NUM_PIXELS,0);
    int spread = map(distanceTo, MAX_DISTANCE,0,1,NUM_PIXELS);
    pixels.fill(0, 0, 0);
    drawLine(TARGETS[dest].col,adjustedCourse,spread);
    pixels.show();   
  } else {                                                                                                            // or else, display 
    updatePixels(MODE_SEARCHING_FOR_GPS);
    Serial.print("reading from gps... sent: ");
    Serial.print(gps.sentencesWithFix());
    Serial.print(" chars: ");
    Serial.println(gps.charsProcessed());
  }
}