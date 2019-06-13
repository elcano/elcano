#pragma once
#include "Common.h"
#include <due_can.h>
#include "Can_Protocol.h"
using namespace elcano;

//origin is set to the UWB map
//#define ORIGIN_LAT 47.758949 
//#define ORIGIN_LONG -122.190746

//origin set to center of UWB soccer field
#define ORIGIN_LAT 47.760850 
#define ORIGIN_LONG -122.190044


#define MAX_CAN_FRAME_DATA_LEN_16 16
#define MAX_WAYPOINTS 3
#define TURN_RADIUS_MM 1000
#define MIN_TURNING_RADIUS_MM 1000

#define TURN_SPEED 1050
#define DESIRED_SPEED_mmPs 1600
