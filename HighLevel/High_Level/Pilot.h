#pragma once
#include "Common.h"
#include "Planner.h"

//using namespace elcano;
namespace elcano {

class Pilot {
private:
	#define DESIRED_SPEED2 1200
	enum States { STOP, STRAIGHT, ENTER_TURN, LEAVE_TURN, APPROACH_GOAL, LEAVE_GOAL};
	States state;
  Planner *myPlanner;
  int next = 1; //index to pathz in a list

  long speed_mmPs;
  long turn_direction;
  long pre_desired_speed;
  long pre_turn_angle;
  CAN_FRAME output; //CAN frame to carry message to C2

  //Waypoint pathz[MAX_WAYPOINTS];  // course route to goal/mission //not sure why this was here
  //Waypoint pathz[3]; //3 is hardcoded //if testing hard coded
  //Waypoint pathz0, pathz1, pathz2;
  String wrdState[6] = { "STOP", "STRAIGHT", "ENTER_TURN", 
    "LEAVE_TURN", "APPROACH_GOAL", "LEAVE_GOAL"}; //used for clearer debugging

  //last is the the last index of the Path/goal
  int last_index_of_pathz = 2; //hardcode path of the last index/dest to 3 [cur,loc1,goal]
  int q = 0; //for testing to vary speed in C3 find_state
  bool first = true;
  //******************** hard coded alternating speeds for testing ******************
  int speeds[6] = {2000, 2500, 1500, 2500, 1000, 2000};
  int angg[6] = {100, 125, 100, 120, 110, 122};
  int speedIndex = 0;
  int insaneCounter = 0;
  //*********************************************************************************
	
	long turning_radius_mm(long speed_mmPs);
	bool test_past_destination(int n, Waypoint &estPos);
	bool test_approach_intersection(long turn_radius_mm, int n, Waypoint &estPos);
	bool test_leave_intersection(long turning_radius_mm, int n, Waypoint &estPos);
	int get_turn_direction_angle(int n, Waypoint &estPos);
	void find_state(long turn_radius_mm, int n, Waypoint & estimated_pos);
	void hardCoded_Pilot_Test();
	void Pilot_communicate_LowLevel();
  void initializePosition(Waypoint &estPos, Waypoint &oldPos);
  void populate_path();
	
public:
	Pilot(Origin &org, Waypoint &estimated_pos, Waypoint &old_pos);
	~Pilot(){} //destructor
	void update(Waypoint &estimated_pos, Waypoint &old_pos);
};

} // namespace elcano
