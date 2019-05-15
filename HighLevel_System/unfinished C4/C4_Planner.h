#pragma once
#include "Settings.h"

class C4_Planner {
private:
	void ConstructNetwork(Junction *Map, int MapPoints);
	void GetGoals(Junction *nodes , int Goals);
	long distance(int, int*, long, long, int*);
	void FindClosestRoad(Waypoint *start, Waypoint *road);
	void test_closestRoad();
	int BuildPath (int j, Waypoint* start, Waypoint* destination);
	void test_buildPath();
	int PlanPath (Waypoint *start, Waypoint *destination);
	void SelectMap(Waypoint currentLocation, char* fileName, char* nearestMap);
	void initialize_C4();
	void test_mission();
	void test_path();
	void test_distance();
	
public:
	C4_Planner();
	~C4_Planner();

};
