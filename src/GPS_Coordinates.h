#ifndef GPS_COORDINATES_H
#define GPS_COORDINATES_H

#include <stdint.h> // For uint32_t

// Define the GPS_Coordinate class
class GPS_Coordinate {
public:
    double latitude;
    double longitude;
    uint32_t color;
    bool pass[8];  // Array to hold 8 boolean values
    bool checkPassword(const bool pass[8]);

    // Constructor
    GPS_Coordinate(double lat, double lon, uint32_t col, const bool p[8]);
};

// External array of GPS coordinates
extern GPS_Coordinate locations[];

extern const uint8_t MAX_TARGETS;

#endif // GPS_COORDINATES_H
