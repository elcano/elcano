#pragma once
#include "Common.h"


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

 
  //Cones 1-6 on soccer map
  double goal_lat[CONES] = {47.760907, 47.761150, 47.761023, 47.760812, 47.760565, 47.760681};
  double goal_lon[CONES] = {-122.190527, -122.190190,-122.189800, -122.189531, -122.189858, -122.190255}; 

  Waypoint mission[CONES]; //aka MDF //The target Nodes to hit
  Junction mapNodes[MAX_WAYPOINTS]; //Storing the loaded map use map_points to track size of mapNodes when loaded
  int map_points; //filled in when loads the map
  int mission_index; //index of mission working on
  #define currentlocation  -1 //currentLocation
  
  Waypoint Start;

  //methods
  void initialize_Planner(Origin &orign, Waypoint &estimPos);
  void SelectMap(Origin &orgin, Waypoint &startLocation, char* fileName, char* nearestMap);
  bool LoadMap(char* fileName);
  void ConstructNetwork(Junction *Map);
  void GetGoals(Origin &ori, Junction *mapNodes);
  long distance(int& cur_node, int &k, long &cur_east_mm, long &cur_north_mm, int &perCent);
  void FindClosestRoad(Waypoint &start, Waypoint &road);
  int PlanPath(Origin &origin, Waypoint &start, Waypoint &destination); //calls find
  int FindPath(Origin &borigin, Waypoint &start, Waypoint &destination); //calls build
  int BuildPath(Origin &orgi, long &j, Waypoint &start, Waypoint &destination);
  
  

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
