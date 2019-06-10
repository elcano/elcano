// Common.h - header included for all Elcano modules
namespace common {bool checksum(char* msg);}

namespace elcano
{
#define EARTH_RADIUS_MM 6371000000.
#define PIf ((float) 3.1415926)
#define TO_RADIANS (PIf/180.)
#define HEADING_PRECISION 1000000.0
#define DR_ERROR_mm 1000.0

// The buffer size that will hold a GPS sentence. They tend to be 80 characters long.
// Got weird results with 90; OK with 120.
#define BUFFSIZ 120
#define MAX_MISSION 6 //The maximum number of target goal to hit
#define MEG 1000000
#define MAX_MAPS 10    //The maximum number of map files stored to SD card
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

#define WHEEL_BASE_mm         800
#define TURNING_RADIUS_mm    4000
#define MAX_ACCEL_mmPs2      2000
#define STANDARD_ACCEL_mmPs2 1000
// but we send a number from 0 to 255
#define STANDARD_ACCEL       55
#define MAX_ACCEL            110
#define WALK_SPEED_mmPs      1000
// 10 mph = 4.44 m/s
#define MAX_SPEED_mmPs       4444

class Origin
{
public:
	double latitude;
	double longitude;
    
    float cos_lat;
    
    Origin(){} //Defualt constructor
    
    //constructor for hardcoding Origin in global scope
    //cos_lat will be initialized depending on latDeg and latMin
    Origin(double lat, double log);
};

class waypoint // best estimate of position and state
// used either for a waypoint or a measured navigational fix
{
  public:
    // Since the Arduino supports only 6 digits of precision in float/double,
    // all latitudes and longitudes are recorded as integers.
    // The (east_mm and north_mm) position carries more precision.
		double latitude;
		double longitude;
    long east_mm;  	// x is east; max is 2147 km
    long north_mm;  // y is true north
    long sigma_mm = 3000; // standard deviation of position for GPS. 
    unsigned long time_ms;   // time of reading since start_time, start_date  
    // millis() + offset_ms = waypoint.time_ms
  /*
  We represent points as locations in mm. A 32 bit signed long is good for +/- 2000 km.
  Latitudes and longitudes are given as a double with up to 9 digits,
  such as 47.622876, -122.352307
  On the Arduino, a float is the same as a double, giving 6 to 7 decimal digits.
  This means that mm resolution only applies within 1 km of the origin.
  */
	long Evector_x1000;   // 1000 * unit vector pointing east
  long Nvector_x1000;   // 1000 * unit vector pointing north
  long bearing_deg;  // degrees. 0 = North; 90 = East.
    long speed_mmPs; // vehicle speed in mm per second.
    int index;       // used for passing a sequence of waypoints over serial line.
   
    void Compute_mm(Origin &origin);
    void Compute_LatLon(Origin &origin);
    bool AcquireGPRMC(unsigned long max_wait_ms);
    bool AcquireGPGGA(unsigned long max_wait_ms);
    void fuse(waypoint reading, int deltaT_ms, Origin &origin);
    void SetTime(char *pTime, char * pDate);
    char* GetLatLon(char* parseptr);
    char* formPointString();
    bool readPointString(unsigned long max_wait_ms, int channel);
    void   operator=(waypoint& other);
    void   operator=(waypoint* other);
    long  distance_mm(waypoint *other);
    void  vectors(waypoint *other);
    long  distance_mm(long east_mm, long north_mm);
		void Compute_EandN_Vectors(long heading);
																//#ifdef MEGA
// The following waypoint methods exist only on the C6 Navigator module.    
//    void fuse(waypoint GPS_reading, int deltaT_ms);
//    char* GetLatLon(char* parseptr);
//    bool AcquireGPRMC(unsigned long max_wait_ms);
//    bool AcquireGPGGA(unsigned long max_wait_ms);
//    void SetTime(char *pTime, char * pDate);
       
                                                                //#endif
};
	void ComputePositionWithDR(waypoint &oldData, waypoint &newData);
	void FindFuzzyCrossPointXY(waypoint &gps, waypoint &dr, waypoint &estimated_position);
	double CrossPointX(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	//Converts provided Longitude and Latitude to MM
	//bool convertLatLonToMM(long latitude, long longitude);

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
//  int Speed[4];     // mm / s
//  curve *route[4];  // intermediate waypoints between location and Destination.
  // location != route[n]->present
  // The vehicle does not pass though location if it is turning.  
  // The vehicle travels from (the last waypoint of the curve leading into the junction)
  // to route[n]->present.
  // When a curve reached a juntion, Curve.next == NULL
};


}
