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
   
//pre-defined goal/destination to get to UWB Soccer Field
  //1) 47.760907, -122.190527
  //2) 47.761150, -122.190190
  //3) 47.761023, -122.189800
  //4) 47.760812, -122.189531
  //5) 47.760565, -122.189858
  //6) 47.760681, -122.190255

  //hardcode cone locations multiplied by 1000000 used 6 decimal places in numbers //put in order of travel
  long goal_lat[CONES] = {47760907, 47761150, 47761023, 47760812, 47760565, 47760681};
  long goal_lon[CONES] = {-122190527, -122190190,-122189800, -122189531, -122189858, -122190255}; 

  Waypoint mission[CONES]; //aka MDF //The target Nodes to hit
  Junction Nodes[MAX_WAYPOINTS]; //Storing the loaded map use map_points to track size of Nodes when loaded
  int map_points; //filled in when loads the map
  int mission_index; //index of mission working on
  #define currentlocation  -1 //currentLocation
  
  Waypoint Start;

  //methods
  void SelectMap(Origin &orgin, Waypoint &startLocation, char* fileName, char* nearestMap);
  bool LoadMap(char* fileName);
  void initialize_Planner(Origin &orign, Waypoint &estimPos);
  void ConstructNetwork(Junction *Map);
  void GetGoals(Origin &ori, Junction *nodes);
  long distance(int& cur_node, int &k, long &cur_east_mm, long &cur_north_mm, int &perCent);
  void FindClosestRoad(Waypoint &start, Waypoint &road);
  int BuildPath(Origin &orgi, long &j, Waypoint &start, Waypoint &destination);
  int FindPath(Origin &borigin, Waypoint &start, Waypoint &destination); //calls build
  int PlanPath(Origin &origin, Waypoint &start, Waypoint &destination); //calls find

  public:
  Planner(Origin &org, Waypoint &estimated_pos);
  ~Planner(){}
  //elcano::Origin *origin;
  Waypoint path[MAX_WAYPOINTS];  // course route to goal/mission //MAX_WAYPOINTS is hardcoded in Settings_Highlevel.h
 // Waypoint path[CONES]; //6 is hardcoded in Settings_Highlevel.h
 //last is the the last index of the Path/goal
  int last_index_of_path; //used by pilot

};

} // namespace elcano
