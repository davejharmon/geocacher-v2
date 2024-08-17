#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

class GPSHandler {
public:
    GPSHandler(uint8_t rxPin, uint8_t txPin);
    void begin();
    void update();
    float getDirection();

private:
    SoftwareSerial gpsSerial;
    TinyGPSPlus gps;
    float lastDirection;
};

#endif
