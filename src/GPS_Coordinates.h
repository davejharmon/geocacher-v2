//GPS_Coordinates.h
#ifndef GPS_COORDINATES_H
#define GPS_COORDINATES_H

#include <Arduino.h>
#include "neopixelhandler.h"

class GPS_Coordinate {
  public:
    double lat;   // GPS latitude
    double lng;   // GPS longitude
    uint32_t col; // NeoPixel color identity

    // Constructor
    GPS_Coordinate(double latitude, double longitude, uint32_t c) : lat(latitude), lng(longitude), col(c) {}
};

// Declare GPS coordinates
extern GPS_Coordinate BACKYARD;
extern GPS_Coordinate LITTLE_CAFE;
extern GPS_Coordinate CNR_CANTERBURY;
extern GPS_Coordinate CNR_CROSS;
extern GPS_Coordinate FRANK_AND_C;
extern GPS_Coordinate DUMMY;
extern const GPS_Coordinate TARGETS[];
extern const int TARGET_COUNT;

extern GPS_Coordinate HEDGEMAZE;
extern GPS_Coordinate TREEHOUSE;
extern GPS_Coordinate LONE_TREE;
extern GPS_Coordinate MOSSVALE;

#endif
