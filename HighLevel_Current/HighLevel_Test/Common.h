// Common.h - header included for all Elcano modules
#pragma once
#include "Settings_HighLevel.h"
namespace common {bool checksum(char* msg);}

namespace elcano{




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

class  Waypoint// best estimate of position and state
// used either for a Waypoint or a measured navigational fix
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
    // millis() + offset_ms = Waypoint.time_ms
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
  int index;       // used for passing a sequence of Waypoints over serial line.
   
    void Compute_mm(Origin &origin);
    void Compute_LatLon(Origin &origin);
    bool AcquireGPRMC(unsigned long max_wait_ms);
    bool AcquireGPGGA(unsigned long max_wait_ms);
    void fuse(Waypoint reading, int deltaT_ms, Origin &origin);
    void SetTime(char *pTime, char * pDate);
    char* GetLatLon(char* parseptr);
    char* formPointString();
    bool readPointString(unsigned long max_wait_ms, int channel);
    void   operator=(Waypoint& other);
    void   operator=(Waypoint* other);
    long  distance_mm(Waypoint *other);
    void  vectors(Waypoint *other);
    long  distance_mm(long east_mm, long north_mm);
		void Compute_EandN_Vectors(long heading);
    double distance_points_mm(double lat, double lon);  //Haversine distance formula
																//#ifdef MEGA
// The following Waypoint methods exist only on the C6 Navigator module.    
//    void fuse(Waypoint GPS_reading, int deltaT_ms);
//    char* GetLatLon(char* parseptr);
//    bool AcquireGPRMC(unsigned long max_wait_ms);
//    bool AcquireGPGGA(unsigned long max_wait_ms);
//    void SetTime(char *pTime, char * pDate);
       
                                                                //#endif
};
	void ComputePositionWithDR(Waypoint &oldData, Waypoint &newData);
	void FindFuzzyCrossPointXY(Waypoint &gps, Waypoint &dr, Waypoint &estimated_position);
	double CrossPointX(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	//Converts provided Longitude and Latitude to MM
	//bool convertLatLonToMM(long latitude, long longitude);

struct Curve
{
  Curve    *previous;
  Waypoint *present;
  bool     Junction;
  Curve    *next;
};

struct Location_mm
{
  long x, y;
};

struct Junction
{
  long east_mm, north_mm;
  int destination[4];  
  // Where there are < 4 destinations, some pointers are NULL
  // If there are more than 4 destinations, one of the destinations has same location and
  // zero Distance.
  long Distance[4];  // mm
//  int Speed[4];     // mm / s
//  curve *route[4];  // intermediate Waypoints between location and Destination.
  // location != route[n]->present
  // The vehicle does not pass though location if it is turning.  
  // The vehicle travels from (the last Waypoint of the curve leading into the junction)
  // to route[n]->present.
  // When a curve reached a juntion, Curve.next == NULL
};


}// namespace elcano
