#include "CompassHandler.h"

CompassHandler::CompassHandler() {}

void CompassHandler::begin() {
  lsm.init();  // initialize LSM303
  lsm.enableDefault();
  // lsm.m_min = (LSM303::vector<int16_t>){-608, -544, -495};  // get values using this: https://github.com/pololu/lsm303-arduino/tree/master/examples/Calibrate
  // lsm.m_max = (LSM303::vector<int16_t>){+369, +478, +418};    
}

float CompassHandler::getNorth() {
    lsm.read();
    float rawHeading = lsm.heading();
    float reversedHeading = -rawHeading + 100.0;  // compass constant offset (offset for mounting reversed & position)
    while (reversedHeading >  360.0) reversedHeading -= 360.0;
    while (reversedHeading <    0.0) reversedHeading += 360.0;    // reversedHeading +=180;
    // heading = alpha * rawHeading + (1 - alpha) * heading;
    // int north = static_cast<int>(rawHeading);
    // Serial.print("N:");
    // Serial.print(reversedHeading);
    return reversedHeading;
}