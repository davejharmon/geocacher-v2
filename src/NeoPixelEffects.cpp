// NeoPixelEffects.cpp
#include "NeoPixelEffects.h"

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setColor(Adafruit_NeoPixel& pixels, uint32_t c) {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
  }
  pixels.show();
}

void colorWipe(Adafruit_NeoPixel& pixels, uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void rainbow(Adafruit_NeoPixel& pixels, uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void rainbowCycle(Adafruit_NeoPixel& pixels, uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void theaterChase(Adafruit_NeoPixel& pixels, uint32_t c, uint8_t wait, uint8_t rpt) {
  for (int j = 0; j < rpt; j++) {
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) {
        pixels.setPixelColor(i + q, c);
      }
      pixels.show();
      delay(wait);
      for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) {
        pixels.setPixelColor(i + q, 0);
      }
    }
  }
}

void theaterChaseRainbow(Adafruit_NeoPixel& pixels, uint8_t wait) {
  for (int j = 0; j < 256; j++) {
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) {
        pixels.setPixelColor(i + q, Wheel((i + j) % 255));
      }
      pixels.show();
      delay(wait);
      for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) {
        pixels.setPixelColor(i + q, 0);
      }
    }
  }
}

void drawLine(Adafruit_NeoPixel& pixels, uint32_t c, int center, int length) {
  int start = center - (length / 2);
  int end = center + (length / 2);

  pixels.fill(0, 0, 0);

  for (int i = start; i <= end; i++) {
    int index = i;
    if (i < 0) {
      index = pixels.numPixels() + i;
    } else if (i >= pixels.numPixels()) {
      index = i - pixels.numPixels();
    }
    pixels.setPixelColor(index, c);
  }
  pixels.show();
}
