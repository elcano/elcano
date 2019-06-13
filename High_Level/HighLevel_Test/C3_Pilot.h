#pragma once
#include "Settings_HighLevel.h"

class C3_Pilot {
private:
	#define DESIRED_SPEED2 1200
	enum States { STOP, STRAIGHT, ENTER_TURN, LEAVE_TURN, APPROACH_GOAL, LEAVE_GOAL};
	States state;
	
	long turning_radius_mm(long speed_mmPs);
	bool test_past_destination(int n);
	bool test_approach_intersection(long turn_radius_mm, int n);
	bool test_leave_intersection(long turning_radius_mm, int n);
	int get_turn_direction_angle(int n);
	void find_state(long turn_radius_mm, int n);
	void C3_communicate_C2();
  void initializePosition(Waypoint &oldPos);
  void populate_path();
	
public:
	C3_Pilot();
	~C3_Pilot();
	void update(Waypoint &ep, Waypoint &op);
};
