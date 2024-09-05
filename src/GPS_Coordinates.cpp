#include "GPS_Coordinates.h"
#include "constants.h" // Include the header for color definitions

// Constructor Implementation
GPS_Coordinate::GPS_Coordinate(double lat, double lon, uint32_t col, const bool p[8])
    : latitude(lat), longitude(lon), color(col) {
    for (int i = 0; i < 8; i++) {
        pass[i] = p[i];  // Assign each element of the pass array individually
    }
}

// Implementation of checkPassword function
bool GPS_Coordinate::checkPassword(const bool inputPass[8]) {
    for (int i = 0; i < 8; i++) {
        if (pass[i] != inputPass[i]) {
            return false;  // Return false if any element doesn't match
        }
    }
    return true;  // Return true if all elements match
}

// Pre-defined pass arrays for each GPS coordinate
const bool pass1[8] = {R,B,R,B,R,B,R,B};
const bool pass2[8] = {B,B,B,B,R,R,R,R};
const bool pass3[8] = {R,R,R,R,B,B,B,B};
const bool pass4[8] = {R,B,B,B,B,B,B,B};
const bool pass5[8] = {R,R,B,B,B,B,B,B};
const bool pass6[8] = {R,R,R,B,B,B,B,B};

// External array of GPS coordinates with unique passes
GPS_Coordinate locations[] = {
    GPS_Coordinate(-33.90522019838692, 151.1677241191594, ORANGE, pass1), // MOONSHINER GIN DISTILLERY
    GPS_Coordinate(-33.89905736459211, 151.17225177400934, YELLOW, pass2), // Enmore country club
    GPS_Coordinate(-33.89878155540173, 151.17456325802118, CYAN, pass3), // Baby Dragon
    GPS_Coordinate(-33.89965923091362, 151.17787447020095, PINK, pass4), // Buddy’s bar
    GPS_Coordinate(-33.89910145526546, 151.18257136934403, GREEN, pass5), // The Imperial Erskineville  
    GPS_Coordinate(-33.897017407611656, 151.17972652780048, MAGENTA, pass6) // Pleasure Club
};

const uint8_t MAX_TARGETS=sizeof(locations) / sizeof(locations[0]);