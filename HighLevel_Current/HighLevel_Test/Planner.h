#pragma once
#include "Common.h"
#include "Settings_HighLevel.h"

namespace elcano {



class Planner{
  private:
    struct AStar
    {
      int ParentID;
      long CostFromStart;
      long CostToGoal;
      long TotalCost;
    } Open[MAX_WaypointS];  

  
  elcano::Waypoint mission[CONES]; //aka MDF //The target Nodes to hit
  //Waypoint path[MAX_WaypointS];  // course route to goal/mission
  elcano::Waypoint path[CONES]; //6 is hardcoded
  #define currentlocation  -1 //currentLocation
  

  //pre-defined goal/destination to get to UWB Soccer Field
  //1) 47.760907, -122.190527
  //2) 47.761150, -122.190190
  //3) 47.761023, -122.189800
  //4) 47.760812, -122.189531
  //5) 47.760565, -122.189858
  //6) 47.760681, -122.190255

  //hardcode cone locations multiplied by 1000000 used 6 decimal places in numbers
  long goal_lat[CONES] = {47760907, 47761150, 47761023, 47760812, 47760565, 47760681};
  long goal_lon[CONES] = {-122190527, -122190190,-122189800, -122189531, -122189858, -122190255};
  
  Waypoint Start;

  //last is the the last index of the Path/goal
  int last_index_of_path = CONES -1; //hardcode path of the last index/dest to 3 [cur,loc1,goal]

  Junction Nodes[MAX_WaypointS]; //Storing the loaded map
  int map_points; //filled in when loads the map

  //methods
  void SelectMap(Waypoint startLocation, char* fileName, char* nearestMap);
  bool LoadMap(char* fileName);
  void initialize_Planner(elcano::Waypoint &estimated_pos);
  void ConstructNetwork(Junction *Map, int MapPoints);
  void GetGoals(Junction *nodes, int Goals);
  long distance(int cur_node, int *k, long cur_east_mm, long cur_north_mm, int* perCent);
  void FindClosestRoad(Waypoint *start, Waypoint *road);
  int BuildPath(int j, Waypoint *start, Waypoint *destination);
  int FindPath(Waypoint *start, Waypoint *destination);
  int PlanPath(Waypoint *start, Waypoint *destination);

  public:
  Planner(elcano::Waypoint &estimated_pos);
  ~Planner(){}
  elcano::Origin *origin;
  
};

} // namespace elcano
