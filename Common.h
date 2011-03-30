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
  curve    *next;
};

#define PI ((float) 3.1415925)