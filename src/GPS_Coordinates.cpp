#include "GPS_Coordinates.h"
#include "constants.h" // Include the header for color definitions

// Constructor Implementation
GPS_Coordinate::GPS_Coordinate(double lat, double lon, uint32_t col)
    : latitude(lat), longitude(lon), color(col) {
}

// External array of GPS coordinates
GPS_Coordinate locations[] = {
    // GPS_Coordinate(-33.91955388338126, 151.10610149289386, RED),    // BACKYARD
    // GPS_Coordinate(-33.91955388338126, 151.10610149289386, GREEN),   // CAFE
    // GPS_Coordinate(-33.918216637305314, 151.1045469768015, BLUE),   // CNR CANTERBURY
    // GPS_Coordinate(-33.92124868722235, 151.10657241841565, YELLOW) // CNR CROSS
    GPS_Coordinate(-33.90522019838692, 151.1677241191594, ORANGE), // MOONSHINER GIN DISTILLERY
    GPS_Coordinate(-33.89905736459211, 151.17225177400934, YELLOW), // Enmore country club
    GPS_Coordinate(-33.89878155540173, 151.17456325802118, CYAN), // Baby Dragon
    GPS_Coordinate(-33.89965923091362, 151.17787447020095, PINK), // Buddy’s bar
    GPS_Coordinate(-33.89910145526546, 151.18257136934403, GREEN), // The Imperial Erskineville  
    GPS_Coordinate(-33.897017407611656, 151.17972652780048, MAGENTA) // Pleasure Club
};

const uint8_t MAX_TARGETS=sizeof(locations) / sizeof(locations[0]);