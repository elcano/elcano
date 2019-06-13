#pragma once
#include <due_can.h>
#include "Can_Protocol.h"

namespace elcano{

//set to 'false' to stop debug Serial.print/ln printing
#define DEBUG false  //general debugging entering methods, passing tests etc
#define DEBUG2 false //CAN BUS debugging, checking sent and receieved data to/from CANBUS
#define DEBUG3 false //Navigation and Pilot debuggging, which direction told to travel
#define DEBUG4 true //for testing propper heading of first path in Pilot.cpp


#define CONES 6 //number of mission points. Update this for each use in new area
#define MAX_WAYPOINTS 50 //change if you map with more than 50 points
  

#define MAX_CAN_FRAME_DATA_LEN_16 16
#define TURN_RADIUS_MM 1000
#define MIN_TURNING_RADIUS_MM 1000

#define TURN_SPEED 1050
//#define DESIRED_SPEED_mmPs 1600 
 
const long turn_speed = 835;

//from common.h


#define DESIRED_SPEED_mmPs 1390 //3.1 mph //used in getGoals to set speed to goals
#define SLOW_SPEED_mmPs 833 //1.9 mph

#define EARTH_RADIUS_MM 6371000000.
#define PIf ((float) 3.1415926)
#define TO_RADIANS (PIf/180.)
#define HEADING_PRECISION 1000000.0
#define DR_ERROR_mm 1000.0

// The buffer size that will hold a GPS sentence. They tend to be 80 characters long.
// Got weird results with 90; OK with 120.
#define BUFFSIZ 120
//#define MAX_MISSION 6 //The maximum number of target goal to hit
#define MEG 1000000
#define MAX_MAPS 10    //The maximum number of map files stored to SD card used to allocate space in map array
#define MAX_WaypointS 40   // The maximum number of Waypoints in each map file.

// setting of index to indicate a navigation fix
#define POSITION -1
// A bit flag for index that indicates this Waypoint is the last in a sequence.
#define END  0x04000
// index bit map showing that Waypoint is a goal
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




}//end namespace elcano
