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

// case code: longshirt 60 | socks 25 | beanie 40
// Pre-defined pass arrays for each GPS coordinate
const bool pass1[8] = {B,R,R,B,R,R,B,B};    // GRF | 0110 1100 42 (Ikea instructions)
const bool pass2[8] = {B,B,B,R,R,B,R,R};    // ECC | 0001 1011 Sudoku
const bool pass3[8] = {R,B,R,B,B,R,R,B};    // BDB | 1010 0110 bracelets
const bool pass4[8] = {B,R,B,B,R,R,R,B};    // UNC | 0100 1110 Playing cards
const bool pass5[8] = {R,B,B,R,B,B,B,R};    // BDB | 1001 0001 (145 beersstyh) 
const bool pass6[8] = {R,R,B,B,B,R,B,R};    // TIE | 1100 0101 Whatever, written on paper

// External array of GPS coordinates with unique passes
GPS_Coordinate locations[] = {
    // GPS_Coordinate(-33.91967783735073, 151.10577383255026,ORANGE, pass1), // home
    // GPS_Coordinate(-33.91910629885007, 151.11030656025792,PINK, pass2) // home
    

    GPS_Coordinate(-33.899100160454374, 151.1722720627143, PINK, pass1), // Enmore country club (Robot 42)
    GPS_Coordinate(-33.89890773439434, 151.17461249964893, CYAN, pass2), // Baby Dragon
    GPS_Coordinate(-33.89847583639927, 151.17796925863925, ORANGE, pass3), // UNCANNY
    GPS_Coordinate(-33.89963476094663, 151.17783206371465, YELLOW, pass4), // Buddy’s bar
    GPS_Coordinate(-33.89910338747424, 151.18257015797354, GREEN, pass5), // The Imperial Erskineville  
    GPS_Coordinate(-33.89696766650776, 151.17969929266317, MAGENTA, pass6) // Pleasure Club
};

const uint8_t MAX_TARGETS=sizeof(locations) / sizeof(locations[0]);