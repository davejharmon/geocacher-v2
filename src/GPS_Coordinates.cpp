#include "GPS_Coordinates.h"
#include "NeoPixelAnimationManager.h" // Include the header for color definitions

// Constructor Implementation
GPS_Coordinate::GPS_Coordinate(double lat, double lon, uint32_t col)
    : latitude(lat), longitude(lon), color(col) {
}

// External array of GPS coordinates
GPS_Coordinate locations[] = {
    GPS_Coordinate(-33.91955388338126, 151.10610149289386, RED),    // BACKYARD
    GPS_Coordinate(-33.91955388338126, 151.10610149289386, GREEN),   // CAFE
    GPS_Coordinate(-33.918216637305314, 151.1045469768015, BLUE),   // CNR CANTERBURY
    GPS_Coordinate(-33.92124868722235, 151.10657241841565, YELLOW) // CNR CROSS
};

const uint8_t MAX_TARGETS=sizeof(locations) / sizeof(locations[0]);