#include "GPS_Coordinates.h"
#include "NeoPixelAnimationManager.h" // Include the header for color definitions

// Define coordinates with color references
GPS_Coordinate BACKYARD(-33.91955388338126, 151.10610149289386, RED);
GPS_Coordinate LITTLE_CAFE(-33.91912615210476, 151.11033004286273, GREEN);
GPS_Coordinate CNR_CANTERBURY(-33.918216637305314, 151.1045469768015, BLUE);
GPS_Coordinate CNR_CROSS(-33.92124868722235, 151.10657241841565, YELLOW);
GPS_Coordinate FRANK_AND_C(-33.91971675633819, 151.1189733679412, YELLOW);
GPS_Coordinate DUMMY(-33.91885048631868, 151.10756407548317, WHITE);

// Constructor Implementation
GPS_Coordinate::GPS_Coordinate(double lat, double lon, uint32_t col) : latitude(lat), longitude(lon), color(col)
{
    // Constructor body (if needed)
}
