#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <GPS_Coordinates.h>

class GPSHandler {
public:
    GPSHandler(uint8_t rxPin, uint8_t txPin);
    void begin();
    void update();
    double getDirection(GPS_Coordinate target);
    double getDistance(GPS_Coordinate target);
    bool isValid();
    void debug();

private:
    SoftwareSerial gpsSerial;
    TinyGPSPlus gps;
};

#endif
