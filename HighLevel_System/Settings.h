#pragma once
#include <Common.h>
using namespace elcano;

#define DEBUG false  //general debugging entering methods, passing tests etc
#define DEBUG2 true //CAN BUS debugging, checking sent and receieved data to/from CANBUS
#define DEBUG3 true //Navigation and Pilot debuggging, which direction told to travel

#define MAX_CAN_FRAME_DATA_LEN_16 16
#define MAX_WAYPOINTS 3
#define TURN_RADIUS_MM 2000
#define MIN_TURNING_RADIUS_MM 1000

#define TURN_SPEED 1050
#define DESIRED_SPEED_mmPs 1600
 
const long turn_speed = 835;
