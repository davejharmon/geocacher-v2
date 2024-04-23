#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NEO_PIN 6
#define NEO_PIXELS 35
#define NEO_MAX_PIXEL NEO_PIXELS-1

static const double Loc_Lat[] = {-33.91952370633243, -33.91822409906599, -33.91912393552625, -33.924046113553075};  // hard code the GPS coordinates
static const double Loc_Lon[] = {151.1061196360865, 151.10455351069186, 151.11037782032437,  151.10803934131863};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

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
    delay(wait);
  }
}
void rainbow(uint8_t wait) {                                                                                          // fil the dots with a rainbow
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
void rainbowCycle(uint8_t wait) {                                                                                     // slightly different, this makes the rainbow equally distributed throughout
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
void theaterChase(uint32_t c, uint8_t wait) {                                                                         // theatre style crawling lights
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
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
      index = NEO_PIXELS + i;                                     // Wrap around for negative indices
    } else if (i >= NEO_PIXELS) {
      index = i - NEO_PIXELS;                                     // Wrap around for indices beyond NUM_LEDS
    }
    pixels.setPixelColor(index, c);                               // Set the color of the pixel at index
  }
  pixels.show();                                                  // Display the updated pixel colors
}

void setup(void) {
  Serial.begin(9600);

  pixels.begin();                                                                                                   // initialise neopixel pixels
  pixels.setBrightness(50);
  pixels.show();

  Serial.println("Hello world!");
  Serial.println("");
  colorWipe(pixels.Color(255, 0, 0), 50);
}

void loop(void) {
  rainbowCycle(5);
}