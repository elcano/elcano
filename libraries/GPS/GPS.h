// Common.h - header included for all Elcano modules

// value if latitude, longitude or bearing is missing.
#define INVALID 1000
// latitude and longitude are multiplied by 1,000,000.
// (47.621300, -122.350900) is Seattle Center House.
#define LATITUDE_ORIGIN   47621300
#define LONGITUDE_ORIGIN -122350900
#define EARTH_RADIUS_MM 6371000000.
#define PIf ((float) 3.1415926)
#define PId 3.14159265358979
#define TO_RADIANS (PId/180.)
// The buffer size that will hold a GPS sentence. They tend to be 80 characters long.
// Got weird results with 90; OK with 120.
#define BUFFSIZ 120
//#define GPSRATE 4800
#define MEG 1000000


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
    long sigmaE_mm; // standard deviation of estimate of east_mm.
    long sigmaN_mm; // standard deviation of estimate of north_mm
    unsigned long time_ms;   // time of reading since start_time, start_date  
// millis() + offset_ms = waypoint.time_ms
  /*
  We represent points as locations in mm. A 32 bit signed long is good for +/- 2000 km.
  Latitudes and longitudes are given as a double with up to 9 digits,
  such as 47.622876, -122.352307
  On the Arduino, a float is the same as a double, giving 6 to 7 decimal digits.
  This means that mm resolution only applies within 1 km of the origin.
  */
    int bearing;  // degrees. 0 = North; 90 = East.
    long speed_mmPs; // vehicle speed in mm per second.
    
    void Compute_mm();
    void Compute_LatLon();
    bool AcquireGPRMC(unsigned long max_wait_ms);
    bool AcquireGPGGA(unsigned long max_wait_ms);
    void fuse(waypoint reading, int deltaT_ms);
    void SetTime(char *pTime, char * pDate);
    char* GetLatLon(char* parseptr);
    char* formDataString();
    void   operator=(waypoint& other);
};

class instrument // measurements
{
  public:
    // Since the Arduino supports only 6 digits of precision in float/double,
    // all latitudes and longitudes are recorded as integers.
    // The (east_mm and north_mm) position carries more precision.
    long latitude;   //  DDFFFFFF; 6 digits after virtual decimal point
    long longitude;  // DDDFFFFFF
    long sigmaGPS_mm; // standard deviation of estimate of world position from GPS
    float angle; // radians. Defines rotation of world coordinate system into vehicle coordinates.
    // angle is a function of both bearing and yaw.
    float speedForward_mPs;
    float speedRight_mPs;
    float accelerationForward_mPs2;
    float accelerationRight_mPs2;
    float yaw_deg;
    float roll_deg;
    float pitch_deg;
    
    void Read(waypoint GPS);
};
class controls // Vehicle commands
{
  int steer_servo; // value of analog output from C2
  int motor_servo; // 0 to 4095
  int brake_servo;
  int stick_steer; // command input from joystick or pilot computer
  int stick_go;    // acceleration or deceleration. 0 to 4095
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

bool readline(int channel);
