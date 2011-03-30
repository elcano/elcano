// Common.h - header included for all Elcano modules

// value if latitude, longitude or bearing is missing.
#define INVALID 1000

struct waypoint
{
    double latitude;
    double longitude;
    long east_mm;  // x is east; max is 2147 km
    long north_mm;  // y is true north
    int bearing;  // degrees. 0 = North; 90 = East.
};

struct curve
{
  curve    *previous;
  waypoint *present;
  bool     Junction;
  curve    *next;
};

struct junction
{
  waypoint *location;  // bearing is set to INVALID
  junction *destination[4];  
  // Where there are < 4 destinations, some pointers are NULL
  // If there are more than 4 destinations, one of the destinations has same location and
  // zero Distance.
  int Distance[4];  // mm
  int Speed[4];     // mm / s
  curve *route[4];  // intermediate waypoints between location and Destination.
  // location != route[n]->present
  // The vehicle does not pass though location if it is turning.  
  // The vehicle travels from (the last waypoint of the curve leading into the junction)
  // to route[n]->present.
  // When a curve reached a juntion, Curve.next == NULL
};

#define PI ((float) 3.1415925)
