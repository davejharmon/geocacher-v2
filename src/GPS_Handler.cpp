#include "GPS_Handler.h"

GPSHandler::GPSHandler(uint8_t rxPin, uint8_t txPin)
    : gpsSerial(rxPin, txPin), lastDirection(0.0f) {
}

void GPSHandler::begin() {
    gpsSerial.begin(9600); // GPS module typically operates at 9600 baud rate
}

void GPSHandler::update() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
        
        if (gps.location.isUpdated()) {
            // Example: Calculate direction based on GPS data.
            // Note: TinyGPSPlus does not provide direction directly. You may need to calculate direction based on latitude and longitude.
            // This is a placeholder for where you would implement your direction logic.
            lastDirection = gps.location.lat(); // Example: Use latitude as placeholder
        }
    }
}

float GPSHandler::getDirection() {
    return lastDirection;
}
