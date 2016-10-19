#pragma once
// Common.h - header included for all Elcano modules

// latitude and longitude are multiplied by 1,000,000.
// (47.621300, -122.350900) is Seattle Center House.
#define LATITUDE_ORIGIN   47621300
#define LONGITUDE_ORIGIN -122350900
#define EARTH_RADIUS_MM 6371000000.
#define PIf ((float) 3.1415926)
#define PId 3.14159265358979
#define PI ((float) 3.1415925)
#define TO_RADIANS (PId/180.)
#define COS_LAT (cos(((double) LATITUDE_ORIGIN)/1000000. * TO_RADIANS))
// The buffer size that will hold a GPS sentence. They tend to be 80 characters long.
// Got weird results with 90; OK with 120.
#define BUFFSIZ 120
#define MAX_MISSION 6
#define MEG 1000000
#define MAX_WAYPOINTS 40   // The maximum number of waypoints in each map file.

// setting of index to indicate a navigation fix
#define POSITION -1
// A bit flag for index that indicates this waypoint is the last in a sequence.
#define END  0x04000
// index bit map showing that waypoint is a goal
#define GOAL 0x02000 
#define MAX_DISTANCE 0x3ffffff
// value if latitude, longitude or bearing is missing.
#define INVALID MAX_DISTANCE

bool checksum(char* msg);

class waypoint // best estimate of position and state
// used either for a waypoint or a measured navigational fix
{
  public:
    // Since the Arduino supports only 6 digits of precision in float/double,
    // all latitudes and longitudes are recorded as integers.
    // The (east_mm and north_mm) position carries more precision.
    long latitude;   //  DDFFFFFF; 6 digits after virtual decimal point
    long longitude;  // DDDFFFFFF
    long east_mm;  // x is east; max is 2147 km
    long north_mm;  // y is true north
    long sigma_mm; // standard deviation of position.
    unsigned long time_ms;   // time of reading since start_time, start_date  
    // millis() + offset_ms = waypoint.time_ms
  /*
  We represent points as locations in mm. A 32 bit signed long is good for +/- 2000 km.
  Latitudes and longitudes are given as a double with up to 9 digits,
  such as 47.622876, -122.352307
  On the Arduino, a float is the same as a double, giving 6 to 7 decimal digits.
  This means that mm resolution only applies within 1 km of the origin.
  */
  int Evector_x1000;   // 1000 * unit vector pointing east
  int Nvector_x1000;   // 1000 * unit vector pointing north
//    int bearing;  // degrees. 0 = North; 90 = East.
    long speed_mmPs; // vehicle speed in mm per second.
    int index;       // used for passing a sequence of waypoints over serial line.
    
    void Compute_mm();
    void Compute_LatLon();
    char* formPointString();
    bool readPointString(unsigned long max_wait_ms, int channel);
    void   operator=(waypoint& other);
    void   operator=(waypoint* other);
    long  distance_mm(waypoint *other);
    void  vectors(waypoint *other);
    long  distance_mm(long east_mm, long north_mm);   
    void fuse(waypoint GPS_reading, int deltaT_ms);
    void SetTime(char *pTime, char * pDate);
};

struct curve
{
  curve    *previous;
  waypoint *present;
  bool     Junction;
  curve    *next;
};

struct Location_mm
{
  long x, y;
};
struct junction
{
  long east_mm, north_mm;
  int destination[4];  
  // Where there are < 4 destinations, some pointers are NULL
  // If there are more than 4 destinations, one of the destinations has same location and
  // zero Distance.
  long Distance[4];  // mm
};


