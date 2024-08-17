#include "CompassHandler.h"

CompassHandler::CompassHandler() {}

void CompassHandler::begin() {
  lsm.init();  // initialize LSM303
  lsm.enableDefault();
  lsm.m_min = (LSM303::vector<int16_t>){-608, -544, -495};  // get values using this: https://github.com/pololu/lsm303-arduino/tree/master/examples/Calibrate
  lsm.m_max = (LSM303::vector<int16_t>){+369, +478, +418};    
}

int CompassHandler::getNorth() {
    lsm.read();
    float rawHeading = lsm.heading();
    heading = alpha * rawHeading + (1 - alpha) * heading;
    int north = static_cast<int>(heading);
    return north;
}