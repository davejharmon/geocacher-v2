#include "GPS_Handler.h"

GPSHandler::GPSHandler(uint8_t rxPin, uint8_t txPin)
    : gpsSerial(rxPin, txPin) {
}

void GPSHandler::begin() {
    gpsSerial.begin(9600); // GPS module typically operates at 9600 baud rate
}

void GPSHandler::update() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());    
    }
}

double GPSHandler::getDirection(GPS_Coordinate target) {
    return gps.courseTo(gps.location.lat(), gps.location.lng(), target.latitude, target.longitude);
}

// distance to target in meters
double GPSHandler::getDistance(GPS_Coordinate target) {
    return gps.distanceBetween(gps.location.lat(), gps.location.lng(), target.latitude, target.longitude);
}


bool GPSHandler::isValid() {
    return gps.location.isValid();
}

void GPSHandler::debug() {
    static int lastSatellites = -1; // Initialize with an impossible value to ensure the first print
    static String lastStatus = "";
    static int lastFailed = -1;

    int satellites = gps.satellites.value();
    String status = gps.location.isValid() ? "GPS Fix acquired" : "No GPS Fix";
    int failed = gps.failedChecksum();

    // Check if any value has changed
    if (satellites != lastSatellites || status != lastStatus || failed != lastFailed) {
        String debugLine =  status + ", Satellites: " + String(satellites) + ", Failed checksums: " + String(failed) + ", Passed: "+String(gps.passedChecksum());
        Serial.println(debugLine);

        // Update the stored values to the current ones
        lastSatellites = satellites;
        lastStatus = status;
        lastFailed = failed;
    }
}