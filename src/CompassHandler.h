#ifndef COMPASSHANDLER_H
#define COMPASSHANDLER_H

#include <Arduino.h>
#include <LSM303.h>

class CompassHandler {
public:
    CompassHandler();

    void begin();
    float getNorth(); // Returns the angle in degrees

private:
    LSM303 lsm;
    float alpha = 0.2; // Smoothing factor
    float heading = 0; // Smoothed heading value
    int north;
};

#endif
