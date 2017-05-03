#include <ArduinoUnit.h>

#include <Common.h>
#include <IO.h>
//#include "Elcano_C4_Planner.h"

/*  
Elcano Module C4: Path Planner.

Output to C6: Desired route and speed curves and whether it exits a road or changes lane.
Input: RNDF, MDF and initial position files from memory stick.
Input from C5: Speed and position of obstacles.
Input from C6: Position, orientation, velocity and acceleration.


Files:
RNDF (Route Network Definition File). Format is defined on 
http://archive.darpa.mil/grandchallenge/docs/RNDF_MDF_Formats_031407.pdf. 
This is a digital map of all roads in the area where the vehicle will be operating. 
The camera will locate road edges or lane markers and the vehicle will follow them. 
Thus location can be determined primarily from odometry.

MDF (Mission Definition File). These are latitudes and longitudes that the vehicle 
is required to visit.

Initial position. Specifies the starting location and orientation. Velocity is zero. 
If this is a file, it is read by C4 (Path Planner) and passed to C6 (Navigator). 
If it is user input, it is read by C6 (Navigator).

The present C4 module is the only micro-controller that has a file system.

USB: All Arduinos contain a USB port that lets them download code from a PC.
Since a USB connection is not a network, the Arduinos cannot talk to each other over USB.
To enable USB, one of the Arduinos must include a USB server. If there is a USB server, it might be
C4, which may have an OS, or
C6, which needs to talk to lots of instruments, or
C7, which may have a USB link to a camera, or
we could have another processor whose sole function is communication.
*/

/*---------------------------------------------------------------------------------------*/ 


#define PI ((float) 3.1415925)
#ifndef NULL
#define NULL 0 
#endif
#define START  -1
#define EMPTY  -2

void DataReady();
extern bool DataAvailable;

/*---------------------------------------------------------------------------------------*/ 
// EDIT for route
// CONES includes start and stop
#define CONES 5
long goal_lat[CONES] = {  47621881,   47621825,   47623144,   47620616,   47621881};
long goal_lon[CONES] = {-122349894, -122352120, -122351987, -122351087, -122349894};
/*  mph   mm/s
     3    1341
     5    2235
     8    3576
     10   4470
     15   6706
     20   8941
     25  11176 
     30  13411
     45  20117
     60  26822
*/
#define DESIRED_SPEED_mmPs 2235
/*---------------------------------------------------------------------------------------*/ 



#define MAX_WAYPOINTS 20
/*   There are two coordinate systems.
     MDF and RNDF use latitude and longitude.
     C3 and C6 assume that the earth is flat at the scale that they deal with
*/

/*  Nodes and Links define the road network.
    The path for the robot is an array of pointers to Nodes.
    LATITUDE_ORIGIN and LONGITUDE_ORIGIN define  (0,0) on the mm coordinate system. 
    Start is the position of the robot as it begins a new leg of the journey.  
    Destination is the next goal position in mission.
    Path joins Start and Destination. Finding it is the major task 
    of this Path Planner module.
    The Distance numbers are initially multipliers giving path roughness.
    They are replaced by their product with the actual distance.
    Route is a finer scale list of waypoints from present or recent position.
    Exit is the route from Path[last]-> location to Destination.
*/
#define MAP_POINTS 16
struct curve Links[20];
struct junction Nodes[MAP_POINTS] = {
  -140828,  221434, 3 ,   1 ,  END,  END,  1, 1, 1, 1,  // 0
  -140986,   88800, 0 ,   2 ,   5 ,  END,  1, 1, 1, 1,  // 1
  -144313,  -42065, 1 ,   6 ,  END,  END,  1, 1, 1, 1,  // 2
  -78568,   222090, 0 ,   4 ,  END,  END,  1, 1, 1, 1,  // 3
  -38276,   222290, 3 ,   5 ,  END,  END,  1, 1, 1, 1,  // 4
  -39558,    87844, 1 ,   4 ,   8 ,   6 ,  1, 1, 1, 1,  // 5
  -46528,   -41631, 2 ,   5 ,   7 ,   9 ,  1, 1, 1, 1,  // 6
  -45764,  -135413, 6 ,   10,  END,  END,  1, 1, 1, 1,  // 7
  51834,     87232, 5 ,   9 ,   14,  END,  1, 1, 1, 1,  // 8
  53041,    -41220, 6 ,   8 ,   10,  13 ,  1, 1, 1, 1,  // 9
  53438,   -133901, 7 ,   9 ,   11,  END,  1, 1, 1, 1,  // 10
  108750,  -134590, 10 , 12 ,  END,  END,  1, 1, 1, 1,  // 11
  130021,  -143108, 11 , END,  END,  END,  1, 1, 1, 1,  // 12
  182559,   -41031, 9 ,  END,  END,  END,  1, 1, 1, 1,  // 13
  177598,    86098, 8 ,   15,  END,  END,  1, 1, 1, 1,  // 14
  170313,    69008, 14 , END,  END,  END,  1, 1, 1, 1   // 15
};
struct AStar
{
    int ParentID;
    long CostFromStart;
    long CostToGoal;
    long TotalCost;
} Open[MAP_POINTS];

class waypoint Origin, Start;
waypoint Path[MAX_WAYPOINTS];  // course route to goal
waypoint FinePath[MAX_WAYPOINTS];  // a low level part of path that smoothes the corners.
  
waypoint mission[CONES];  // aka MDF
int waypoints = CONES;

/*---------------------------------------------------------------------------------------*/ 
// Fill in the rest of Nodes
void ConstructNetwork(junction *Map, int MapPoints)
{
  double deltaX, deltaY;
  int jj;
  for (int i = 0; i < MapPoints; i++)
  {
      if( Map[i].east_mm == INVALID)  continue;
      for (int j = 0;  j< 4; j++)
      {
        jj = Map[i].destination[j];
        if (jj == END) continue;
        deltaX = Map[i].east_mm;
        deltaX -= Map[jj].east_mm;
        deltaY = Map[i].north_mm;
        deltaY -= Map[jj].north_mm;
        // Distance[] is initially a roughness scale.
        Map[i].Distance[j] *= sqrt(deltaX*deltaX + deltaY*deltaY);
      }
  }
}
/*---------------------------------------------------------------------------------------*/ 
// Set up mission structure from cone latitude and longitude list.
void GetGoals(waypoint *Waypoint, int Goals)
{
  double deltaX, deltaY, Distance;
  for (int i = 0; i < CONES; i++)
  {
    mission[i].latitude  = goal_lat[i];
    mission[i].longitude = goal_lon[i];
    mission[i].Compute_mm();
 /*   Serial.print(mission[i].latitude);   Serial.print(",");
    Serial.print(mission[i].longitude);   Serial.print(",");
    Serial.print(mission[i].east_mm);   Serial.print(",");
    Serial.println(mission[i].north_mm);   */
    mission[i].speed_mmPs = DESIRED_SPEED_mmPs;
    mission[i].index = 1 | GOAL;
    mission[i].sigma_mm = 1000;
    mission[i].time_ms = 0;
    if (i == 0)
    {    // only needed if CONES == 1
      mission[i].Evector_x1000 = 1000;
      mission[i].Nvector_x1000 = 0;
    }
    else
    {
      deltaX = mission[i].east_mm - mission[i-1].east_mm;
      deltaY = mission[i].north_mm - mission[i-1].north_mm;
      Distance = sqrt(deltaX*deltaX + deltaY*deltaY);
      mission[i-1].Evector_x1000 = (deltaX * 1000.) / Distance;
      mission[i-1].Nvector_x1000 = (deltaY * 1000.) / Distance;
    }
    if (i == CONES-1)
    {
      mission[i].Evector_x1000 = mission[i-1].Evector_x1000;
      mission[i].Nvector_x1000 = mission[i-1].Nvector_x1000;
      mission[i].index |= END;
    }
  }
}
/*---------------------------------------------------------------------------------------*/
// Find the distance from (east_mm, north_mm) to a road segment Nodes[i].distance[j]
// return distance in mm, and per cent of completion from i to j.
// distance is negative if (east_mm, north_mm) lies to the left of the road
// when road direction is from i to j

// Compare this routine to distance() in C3 Pilot
long distance(int i, int *k,  // index into Nodes[]
     long east_mm, long north_mm,
    int* perCent)  // per cent of segment that has been completed
{
  float deltaX, deltaY, dist_mm;
  int j, jj;
  long Eunit_x1000, Nunit_x1000;
  long closest_mm = MAX_DISTANCE; 
  long Road_distance, RoadDX_mm, RoadDY_mm;
//  int pathCompletion;
  long pc;
  
  *perCent = 0;
  *k = 0;
  closest_mm = MAX_DISTANCE;
  for (j=0; j < 4; j++)
  {   // Don't make computations twice.
      jj = Nodes[i].destination[j];
      if (jj == END || jj < i) continue;    
      // compute road unit vectors from i to j
      RoadDX_mm = -Nodes[i].east_mm + Nodes[jj].east_mm;
      int Eunit_x1000 = RoadDX_mm  * 1000 / Nodes[i].Distance[j];
      RoadDY_mm = -Nodes[i].north_mm + Nodes[jj].north_mm;
      int Nunit_x1000 = RoadDY_mm * 1000 / Nodes[i].Distance[j];
//      if (abs(RoadDX_mm) >= abs(RoadDY_mm))
//      {
//          pathCompletion = 100 * (east_mm - Nodes[i].east_mm) / RoadDX_mm;
//      }
//      else
//      {
//          pathCompletion = 100 * (north_mm - Nodes[i].north_mm) / RoadDY_mm;
//      }
      // normal vector is (Nunit, -Eunit)
      deltaX = -Nodes[i].east_mm + east_mm ;
      deltaY = -Nodes[i].north_mm + north_mm;
      // sign of return value gives which side of road it is on.
      Road_distance = (-deltaY * Eunit_x1000 + deltaX * Nunit_x1000) / 1000;
      pc =             (deltaX * Eunit_x1000 + deltaY * Nunit_x1000) / (Nodes[i].Distance[j] * 10);
  /*    
      Serial.print("(");   Serial.print(east_mm);  Serial.print(", ");
      Serial.print(north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(Nodes[i].east_mm);  Serial.print(", ");
      Serial.print(Nodes[i].north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(Nodes[jj].east_mm);  Serial.print(", ");
      Serial.print(Nodes[jj].north_mm);  Serial.print(") ");
      Serial.print(Nodes[i].Distance[j]); Serial.print("(");   Serial.print(Eunit_x1000);  Serial.print(", ");
      Serial.print(Nunit_x1000);  Serial.print(") ");
      Serial.print("(");   Serial.print(Road_distance);  Serial.print(", ");
//      Serial.print(pathCompletion);   Serial.print(", "); 
      Serial.print(pc); Serial.println(") ");
 */
      if (abs(Road_distance) < abs(closest_mm) &&
          pc >= 0 && pc <= 100)
      {
          closest_mm = Road_distance;
          *k = jj;
          *perCent = pc;
      }
  }
  return closest_mm;
}
/*---------------------------------------------------------------------------------------*/

void FindClosestRoad(waypoint *start, 
                    waypoint *road)  // information returned
{
  long closest_mm = MAX_DISTANCE;
  long dist;
  int close_index;
  int perCent;
  long done = 2000;
  int i, j;
  // find closest road.
  for (i = 0; i < MAP_POINTS; i++)
  {
    dist = distance(i, &j, start->east_mm, start->north_mm, &perCent);
    if (abs(dist) < abs(closest_mm))
    {
      close_index = j;
      closest_mm = dist;
      done = perCent;
      road->index = i;
      road->sigma_mm = j;
    }
  }
  if (closest_mm < MAX_DISTANCE)
  {
//    Serial.println(done);
    i = road->index;
    j = close_index;
    road->east_mm =  Nodes[i].east_mm  + 
      done *(Nodes[j].east_mm  - Nodes[i].east_mm) / 100;
    road->north_mm = Nodes[i].north_mm + 
      done *(Nodes[j].north_mm - Nodes[i].north_mm) / 100;
 /*   Serial.print("(");   Serial.print(Nodes[i].east_mm);  Serial.print(", ");
      Serial.print(Nodes[i].north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(Nodes[j].east_mm);  Serial.print(", ");
      Serial.print(Nodes[j].north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(road->east_mm);  Serial.print(", ");
      Serial.print(road->north_mm);  Serial.print(") "); Serial.println(j); */
 }
  else
  { // find closest node
     for (i = 0; i < MAP_POINTS; i++)
    {
      dist = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
      if (dist < closest_mm)
      {
        close_index = i;
        closest_mm = dist;
      }
    }
    road->index = road->sigma_mm = close_index;
    road->east_mm =  Nodes[close_index].east_mm;
    road->north_mm = Nodes[close_index].north_mm;
  }
  road->Evector_x1000 = 1000;
  road->Nvector_x1000 = 0;
  road->time_ms = 0;
  road->speed_mmPs = DESIRED_SPEED_mmPs;
}
/*---------------------------------------------------------------------------------------*/
// start and destination are on the road network given in Nodes.
// start is in Path[1].
// Place other junction waypoints into Path.
// Returned value is next index into Path.
// start->index identifies the closest node.
// sigma_mm holds the index to the other node.
// A* is traditionally done with pushing and popping node from an Open and Closed list.
// Since we have a small number of nodes, we instead reserve a slot on Open and Closed
// for each node. 

int BuildPath (int j, waypoint* start, waypoint* destination)
{
   // Done; we have reached the node that leads to goal turn-off
   // Construct path backward to start.
  int last = 1;
  int route[MAP_POINTS];
  int i, k, node;
  long dist_mm;

   k = MAP_POINTS-1;
   route[k] = j;
//   Serial.println(k);
   while(Open[j].ParentID != START)
   {
      j = route[--k] = Open[j].ParentID;
 //     Serial.println(k);
   }
   Path[last] = start;
   for ( ; k < MAP_POINTS; k++)
   {
     node = route[k];
     Path[++last].east_mm = Nodes[node].east_mm;
     Path[last].north_mm  = Nodes[node].north_mm;
 //    Serial.print(last); Serial.print(", "); 
 //    Serial.print(Path[last].east_mm); Serial.print(", ");
 //    Serial.println(Path[last].north_mm); 
  }
  Path[++last] = destination;
  for (k = 0; k <= last; k++)
  {
     if(k > 0) Path[k].sigma_mm = 10;  // map should be good to a cm.
     Path[k].index = k;
     Path[k].speed_mmPs = DESIRED_SPEED_mmPs;
     Path[k].Compute_LatLon();  // this is never used
  }
  last++;
  for (j = 0; j < last-1; j++)
  {
      Path[j].vectors(&Path[j+1]);
  }

  return last;
}  // end of BuildPath
/*---------------------------------------------------------------------------------------*/

int FindPath(waypoint *start, waypoint *destination)
{

  long ClosedCost[MAP_POINTS];
//  int OpenIndex = 0;
//  int ClosedIndex = 1;
  int  i, j, k;
  long NewCost, NewStartCost, NewCostToGoal;
  long NewIndex;
  long BestCost, BestID;
  bool Processed = false;
  
  for (i = 0; i < MAP_POINTS; i++)
  { // mark all nodes as empty
    Open[i].TotalCost = MAX_DISTANCE;
    ClosedCost[i] = MAX_DISTANCE;
  }

   // get successor nodes of start
   i = start->index;
   Open[i].CostFromStart = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].CostToGoal = destination->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;
   Open[i].ParentID = START;
//   SendPath(start, 1);
//   SendPath(destination, 1);
//   Serial.println(i);
   i = start->sigma_mm;
   Open[i].CostFromStart = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].CostToGoal = destination->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;
   Open[i].ParentID = START;  
//   Serial.println(i);
//   Serial.println("Start A*");
//  Serial.flush();
  BestCost = 0;
  while (BestCost < MAX_DISTANCE)
  { 
     BestCost = MAX_DISTANCE;
     BestID = -1;
//     Serial.println(BestCost);
     // pop lowest cost node from Open; i.e. find index of lowest cost item
     for (i = 0; i < MAP_POINTS; i++)
     {
//       Serial.print(i); Serial.print(", "); Serial.print(Open[i].TotalCost); 
//       Serial.print(", "); Serial.println(ClosedCost[i]);
        if (Open[i].TotalCost < BestCost)
        {
          BestID = i;
          BestCost = Open[i].TotalCost;
//     Serial.print("Best: "); Serial.print(i); Serial.print(", "); Serial.println(BestCost);
       }
     }
     if (BestID < 0)
     {
//       Serial.println("No path found.");
//       Serial.flush();
       return INVALID;
     }
     Open[BestID].TotalCost = MAX_DISTANCE;  // Remove node from "stack".    
     if (BestID == destination->index || BestID == destination->sigma_mm)
        {  // Done; we have reached the node that leads to goal turn-off
           // Construct path backward to start.
//          Serial.println("Found a path.");
//          Serial.flush();
          return BuildPath(BestID, start, destination);
        }
    // get successor nodes from map
    i = BestID;
//    Serial.print(i); Serial.print(", "); Serial.println(BestCost);
    for (j = 0; j < 4; j++)
    {
        NewIndex = Nodes[i].destination[j];  
        if (NewIndex == END)
            continue;  // no successor in this slot
        NewStartCost =  Open[i].CostFromStart + Nodes[i].Distance[j]; 
        NewCostToGoal = destination->distance_mm
         (Nodes[NewIndex].east_mm, Nodes[NewIndex].north_mm);
        NewCost = NewStartCost + NewCostToGoal;
        // check if this node is already on Open or Closed.
        if (NewCost >= ClosedCost[NewIndex]) 
            continue;  // Have already looked at this node
        else if (ClosedCost[NewIndex] != MAX_DISTANCE)
        {  // looked at this node before, but at a higher cost
            ClosedCost[NewIndex] = MAX_DISTANCE;  // remove node from Closed
        }
        if (NewCost >= Open[NewIndex].TotalCost)
            continue;   // This node is a less efficient way of getting to a node on the list.
        // Push successor node onto stack.
        Open[NewIndex].CostFromStart = NewStartCost;
        Open[NewIndex].CostToGoal = NewCostToGoal;
        Open[NewIndex].TotalCost = NewCost;
        Open[NewIndex].ParentID = i;
    }  // end of successor nodes
    // Push node onto Closed
    ClosedCost[BestID] =  BestCost; 
  }   // end of while Open is not empty
  
  return 0;  // failure
}
/*---------------------------------------------------------------------------------------*/
// Low level path is a straight line from start to detination.
// PathPlan makes an intermediate level path that uses as many roads as possible.
 int PlanPath (waypoint *start, waypoint *destination)
 {
   waypoint roadOrigin, roadDestination;
   int last = 0;
   Path[0] = start;
   Path[0].index = 0;

//   SendPath( start, 1);
//   SendPath( destination, 1);

   FindClosestRoad( start, &roadOrigin );
   FindClosestRoad( destination, &roadDestination ); 
//   SendPath( start, 1);
//   SendPath( destination, 1);
//   SendPath( &roadOrigin, 1);
//   SendPath( &roadDestination, 1);
   if  (abs(start->east_mm  - roadOrigin.east_mm)
      + abs(start->north_mm - roadOrigin.north_mm)
      + abs(destination->east_mm  - roadDestination.east_mm)
      + abs(destination->north_mm - roadDestination.north_mm) >=
        abs(start->east_mm  - destination->east_mm)
      + abs(start->north_mm - destination->north_mm))
      {  // don't use roads; go direct
        last = 1;
//        Serial.println("Direct route");
      }
      else
      {  // use A* with the road network
        Path[1] = roadOrigin;
        Path[1].index = 1;
//        Serial.println("Use A*");
//        SendPath(Path, 2);
        last = FindPath(&roadOrigin, &roadDestination);
//        Serial.print("A* finds last =  "); Serial.println(last);
      }
    Path[last] = destination;
    Path[last-1].vectors(&Path[last]);
    Path[last].Evector_x1000 = Path[last-1].Evector_x1000;
    Path[last].Nvector_x1000 = Path[last-1].Nvector_x1000;
    Path[last].index = last | END;
    return last;
 }
/*---------------------------------------------------------------------------------------*/
// Transmit the path to C3 Pilot over a serial line.
void SendPath(waypoint *course, int count)
{
  char *dataString;
  for( int i = 0; i < count; i++)
  {
    dataString = course[i].formPointString();
    checksum(dataString);
    Serial.println(dataString);
    if (course[i].index & END)
        break;
  }
}

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
  int last; 
  Origin.Evector_x1000 = INVALID; 
  Origin.Nvector_x1000 = INVALID; 
  Origin.east_mm = 0;
  Origin.north_mm = 0;  
  Origin.latitude = INVALID;
  Origin.longitude = INVALID;  
  
     ConstructNetwork(Nodes, MAP_POINTS);
     
  /* If (initial position is provided)  // used when GPS is unavailable or inaccurate
     {
        ReadPosition();  // latitude and longitude
        // The origin in the (east_mm, north_mm) coordinate system is defined to be the
        // latitude and longitude at which the robot starts. 
        // (Evector, Nvector) is initial robot attitude.
        SendInitialPosition(C6);  // latitude and longitude
     }
     */
     
  /* Convert latitude and longitude positions to flat earth coordinates.
     Fill in waypoint structure  */
       GetGoals(mission, CONES);
       
       // Send mission to C3.
       SendPath(mission, CONES);
      Serial.println();     
     /* Plan a Path (using map) between each Node. 
     Use the A* algorithm as given in
     Bryan Stout "The Basics of A* for Path Planning" in
     Mark DeLoura (ed) Game Programming Gems, Charles River Media, 2000. */
     
      last = PlanPath (&mission[0], &mission[1]);
      Path[last].index |= GOAL;
     
     /*
     PlanPath does not look at obstacles; The Pilot (C3) does that.  As the vehicle 
     position diverges from expectations due to obstacles, C4 keeps updating the plan.
     The output from PlanPath is a sequence of waypoints Path[] where each waypoint
     is associated with a junction.  We must then supplement the junction
     points with all the intermediate non-junction points in the RNDF so that
     the vehicle can follow curves in the road. The first section of this is Route.
     
     We now have a sequence of segments that defines the mission from
     the origin to the destination.
     
     We will need finer grain that the Route waypoints from the map. */
     
     SendPath(Path, MAX_WAYPOINTS);
      Serial.println();     
    
     /* Read vehicle position, attitude and velocity.
     ReceiveState(C6); 
     fill in Origin.latitude, Origin.longitude, Origin.bearing 
     set Start to Origin; 
     
     GetPosition supplements each pair of waypoints with an Hermite curve that 
     links them.
        Foley et al., Introduction to Computer Graphics.
           */
       
     /* Construct the speed profiles over the Route.
        Consider how much time we want to come up to speed, time to decelerate
        and reduced speed around turns. 
        ConstructSpeeds();  */
        
     /* Send the Route and initial speed profile to C6.
        C6 will add current vehicle position, attitude, velocity and acceleration 
        to create the vehicle state.  The state information is sent on to C3 and 
        fed back to C4.
        SendState(C6); */
}
/*---------------------------------------------------------------------------------------*/ 
/*void setup() 
{ 
        pinMode(Rx0, INPUT);
        pinMode(Tx0, OUTPUT);
        pinMode(LED, OUTPUT); 
     	Serial.begin(9600); 
        Serial.flush();
        Serial.println();
        Serial.println();
        pinMode(DATA_READY, INPUT);
        DataAvailable = false;
        attachInterrupt(0, DataReady, FALLING);

   initialize();
}
/*---------------------------------------------------------------------------------------*/ 
/*void loop() 
{
  static int Goal = 1;
  int last;
  /*
    Maintain a list of waypoints from current position to destination.
    Eliminate any waypoints that have already been visited.
    
    ReceiveState(C6);  // This may be at the interrupt level
    PlanPath();  // only from current position to next waypoint
    UpdateRoute();
    ConstructSpeeds();
    SendState(C6);
 
    About once a second, write the current position and remaining mission, so that if we
    get a reset, we can start from where we left off.   
    */
 //   Serial.println("Loop");
/*    if (DataAvailable)
    {
        // read vehicle position from C6
        readline(0);
        // send data to C3
        writeline(0);
        digitalWrite(C4_DATA_SENT, HIGH);  // transition interrupts the processor
        delay(1);
        digitalWrite(C4_DATA_SENT, LOW);
        Start.readPointString(1000, 0);  
        last = PlanPath (&Start, &mission[Goal]);
        Path[last].index |= GOAL;
        // TO DO: Make a fine path, providing proper curve path
        if (last < MAX_WAYPOINTS/2 && Goal < CONES)
        {
          last = PlanPath (&Start, &mission[++Goal]);
          Path[last].index |= GOAL;
        }
        SendPath(Path, MAX_WAYPOINTS);
        Serial.println();     
    }
    else
    {  // delay for 1 sec, waiting for data
        long end_time = millis() + 1000;
        while (millis() < end_time)
        {
            if (DataAvailable) break;
        }
    }
}*/

/*-----------------------------------------------------------------------------------------------*/
/* ConstructNetwork tests

  Inputs: junction *Map, int MapPoints
  Expected values for *Map: array of junctions
  Expected values for MapPoints: MAP_POINTS
  
  junction values:
  long east_mm, long north_mm, int destination[4], long Distance[4]
  
  destination is the index of the graph nodes this junction has an edge to
  Distance is initially the distance multiplier for each destination node.
  After this function is called, Distance is changed to actual distance calculation (in mm).

*/

// Happy path test with small map
// Verifies that the distances are correctly updated in the map passed to
// the function.
test(ConstructNetwork_small) 
{
  int m = 4;
  
  // input junction
  junction actual[4] = {
    0,  0, 1 ,  END,  END,  END,  1, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    0,  0, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 2
    0,  0, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 3
    
  };

  // expected output
  junction expected[4] = {
    0,  0, 1 ,  END,  END,  END,  5, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  5, 1, 1, 1,  // 1
    0,  0, 0 ,  END,  END,  END,  0, 1, 1, 1,  // 2
    0,  0, 0 ,  END,  END,  END,  0, 1, 1, 1,  // 3
    
  };
  
  // Call the function being tested
  ConstructNetwork(actual, m);

  // Verify the data in the array passed to the function is updated
  // Because we don't have a print overload for junction, we need to
  // make assertations on individual pieces of data within the struct.
  // The same applies to arrays.
  for(int i = 0; i < m; i++)
  {
    assertEqual(actual[i].east_mm, expected[i].east_mm);
    assertEqual(actual[i].north_mm, expected[i].north_mm);
    for(int j = 0; j < 4; j++){
      assertEqual(actual[i].destination[j], expected[i].destination[j]);
      assertEqual(actual[i].Distance[j], expected[i].Distance[j]);
    }
  }
}

// Happy path test with large map
// Verifies that the distances are correctly updated in the map passed to
// the function.
// 113 was the largest number of nodes we could test without the Mega 2560 throwing an error


/*
 * BAD TEST
test(ConstructNetwork_large) 
{
  int m = 110;
  // input junction
  junction actual[110] = {
    0,  0, 1 ,   2 ,   3 ,   4 ,  1, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 2
    0,  0, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 3
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 4
  };

  // expected output
  junction expected[110] = {
    0,  0, 1 ,   2 ,   3 ,   4 ,  5, 5, 0, 5,  // 0
    3,  4, 0 ,  END,  END,  END,  5, 1, 1, 1,  // 1
    3,  4, 0 ,  END,  END,  END,  5, 1, 1, 1,  // 2
    0,  0, 0 ,  END,  END,  END,  0, 1, 1, 1,  // 3
    3,  4, 0 ,  END,  END,  END,  5, 1, 1, 1,  // 4
  };
  
  // Call the function being tested
  ConstructNetwork(actual, m);

  // Verify the data in the array passed to the function is updated
  // Because we don't have a print overload for junction, we need to
  // make assertations on individual pieces of data within the struct.
  // The same applies to arrays.
  for(int i = 0; i < m; i++)
  {
    assertEqual(actual[i].east_mm, expected[i].east_mm);
    assertEqual(actual[i].north_mm, expected[i].north_mm);
    for(int j = 0; j < 4; j++){
      assertEqual(actual[i].destination[j], expected[i].destination[j]);
      assertEqual(actual[i].Distance[j], expected[i].Distance[j]);
    }
  }
}
*/

// Test for empty Map
test(ConstructNetwork_empty_map) 
{
  int m = 0;
  
  // input junction
  junction actual[0] = {};
  
  // Call the function being tested
  ConstructNetwork(actual, m);

  // This test just verifies that it doesn't crash; the assertion is to show
  // that it did not crash and the test passed.
  assertTrue(true);
}

// Test for MapPoints being smaller than the size of the array
// Verifies that the distances are correctly updated in the map passed to
// the function.

test(ConstructNetwork_less_mappoints) 
{
  int m = 1;
  
  // input junction
  junction actual[2] = {
    0,  0, 1 ,  END,  END,  END,  1, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    
  };

  // expected output
  junction expected[2] = {
    0,  0, 1 ,  END,  END,  END,  5, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    
  };
  
  // Call the function being tested
  ConstructNetwork(actual, m);

  // Verify the data in the array passed to the function is updated
  // Because we don't have a print overload for junction, we need to
  // make assertations on individual pieces of data within the struct.
  // The same applies to arrays.
  for(int i = 0; i < m; i++)
  {
    assertEqual(actual[i].east_mm, expected[i].east_mm);
    assertEqual(actual[i].north_mm, expected[i].north_mm);
    for(int j = 0; j < 4; j++){
      assertEqual(actual[i].destination[j], expected[i].destination[j]);
      assertEqual(actual[i].Distance[j], expected[i].Distance[j]);
    }
  }
}

/*
 BAD TEST
// Test for MapPoints being larger than the size of the array
// Verifies that the distances are correctly updated in the map passed to
// the function.
test(ConstructNetwork_more_mappoints) 
{
  int m = 3;
  
  // input junction
  junction actual[2] = {
    0,  0, 1 ,  END,  END,  END,  1, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    
  };

  // expected output
  junction expected[2] = {
    0,  0, 1 ,  END,  END,  END,  5, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  5, 1, 1, 1,  // 1
    
  };
  
  // Call the function being tested
  ConstructNetwork(actual, m);

  // Verify the data in the array passed to the function is updated
  // Because we don't have a print overload for junction, we need to
  // make assertations on individual pieces of data within the struct.
  // The same applies to arrays.
  for(int i = 0; i < 2; i++)
  {
    assertEqual(actual[i].east_mm, expected[i].east_mm);
    assertEqual(actual[i].north_mm, expected[i].north_mm);
    for(int j = 0; j < 4; j++){
      assertEqual(actual[i].destination[j], expected[i].destination[j]);
      assertEqual(actual[i].Distance[j], expected[i].Distance[j]);
    }
  }
}
*/
// Test for MapPoints being negative
// Verifies that the distances are correctly updated in the map passed to
// the function.
test(ConstructNetwork_negative_mappoints) 
{
  int m = -1;
  
  // input junction
  junction actual[2] = {
    0,  0, 1 ,  END,  END,  END,  1, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    
  };

  // expected output
  junction expected[2] = {
    0,  0, 1 ,  END,  END,  END,  1, 1, 1, 1,  // 0
    3,  4, 0 ,  END,  END,  END,  1, 1, 1, 1,  // 1
    
  };
  
  // Call the function being tested
  ConstructNetwork(actual, m);

  // Verify the data in the array passed to the function is updated
  // Because we don't have a print overload for junction, we need to
  // make assertations on individual pieces of data within the struct.
  // The same applies to arrays.
  for(int i = 0; i < 2; i++)
  {
    assertEqual(actual[i].east_mm, expected[i].east_mm);
    assertEqual(actual[i].north_mm, expected[i].north_mm);
    for(int j = 0; j < 4; j++){
      assertEqual(actual[i].destination[j], expected[i].destination[j]);
      assertEqual(actual[i].Distance[j], expected[i].Distance[j]);
    }
  }
}

/*-----------------------------------------------------------------------------------------------*/
/* distance tests

  Inputs: int i, int *k, long east_mm, long north_mm, int *perCent
  Expected values for i: a value between 0 and the last index in Nodes[]
  Expected values for *k: an address
  Expected values for east_mm: any value (long)
  Expected values for north_mm: any value (long)
  Expected values for *perCent: an address
  
  Finds the distance from the east, north coordinates to the node at index i   

  junction values:
  long east_mm, long north_mm, int destination[4], long Distance[4]

*/

// Happy path test for distance()
/*test(distance_standard_skip)
{
/*    Nodes[0].east_mm = 3;
    Nodes[0].north_mm = 4;
    for(int j = 0; j < 4; j++)
    {
      Nodes[0].destination[j] = (j+1)%16;
      Nodes[0].Distance[j] = 1;
    }
  for(int i = 1; i < 16; i++)
  {
    Nodes[i].east_mm = 0;
    Nodes[i].north_mm = 0;
    for(int j = 0; j < 4; j++)
    {
      Nodes[i].destination[j] = j;
      Nodes[i].Distance[j] = 1;
    }
  }*/
/*  ConstructNetwork(Nodes, MAP_POINTS);  // This function does not work until after distances are calculated
  GetGoals(mission, CONES);
  for(int i = 0; i < 16; i++)
  {
    Serial.print(Nodes[i].east_mm);
    Serial.print(',');
    Serial.print(Nodes[i].north_mm);
    Serial.print(',');
    for(int j = 0; j < 4; j++)
    {
      Serial.print(Nodes[i].destination[j]);
      Serial.print(',');
    }
    for(int j = 0; j < 4; j++)
    {
      Serial.print(Nodes[i].Distance[j]);
      Serial.print(',');
    }
    Serial.println();
  }
  int i = 0;
  int k;
  long east_mm = 0;
  long north_mm = 0;
  int perCent;
  
  for (i; i < 16; i++)
  {
    long dist = distance(i, &k, east_mm, north_mm, &perCent);
    Serial.print(dist);
    Serial.print(',');
    Serial.print(k);
    Serial.print(',');
    Serial.println(perCent);
  }

}

/*-----------------------------------------------------------------------------------------------*/
/* FindClosestRoad tests

  Inputs: waypoint *start, waypoint *road
  Expected values for *start: an address to a waypoint
  Expected values for *road: an address to a waypoint
  
  Finds the closest road to a waypoint   

  waypoint data: long latitude, long longitude, long east_mm, long_north_mm, long sigma_mm, unsigned long time_ms,
                 int Evector_x1000, int Nvector_x1000, long speed_mmPs, int index
                 
  waypoint functions:
    void Compute_mm()
    void Compute)LatLon()
    char* formPointString()
    bool readPointString(unsigned long max_wait_ms, int channel)
    void operator=(waypoint& other)
    void operator=(waypoint* other)
    long distance_mm(waypoint* other)
    void vectors(waypoint *other)
    long distance_mm(long east_mm, long north_mm)
*/

// Happy Path test for FindClosest Road
// Verifies that the node closest to the start location is found
test(FindClosestRoad_standard)
{
  waypoint start;
  waypoint road;
  
  start.east_mm = Nodes[4].east_mm + 1;
  start.north_mm = Nodes[4].north_mm + 1;
  
  FindClosestRoad(&start, &road);
  
  assertEqual(road.east_mm, Nodes[4].east_mm);
  assertEqual(road.north_mm, Nodes[4].north_mm);
  
}

// FindClosestRoad test for bottom-left
test(FindClosestRoad_bottom_left)
{
  waypoint start;
  waypoint road;
  
  start.east_mm = -2147483647;
  start.north_mm = -2147483647;
  FindClosestRoad(&start, &road);
    
  assertNotEqual(road.east_mm, 2147483647);    // We don't expect the bottom-leftmost point 
  assertNotEqual(road.north_mm, 2147483647);   // to be closest to the top-rightmost point
}

// FindClosestRoad test for top-left
test(FindClosestRoad_top_left)
{
  waypoint start;
  waypoint road;
  
  start.east_mm = -2147483647;
  start.north_mm = 2147483647;
  FindClosestRoad(&start, &road);
    
  assertNotEqual(road.east_mm, 2147483647);    // We don't expect the top-leftmost point 
  assertNotEqual(road.north_mm, -2147483647);   // to be closest to the bottom-rightmost point
}

// FindClosestRoad test for top-right
test(FindClosestRoad_top_right)
{
  waypoint start;
  waypoint road;
  
  start.east_mm = 2147483647;
  start.north_mm = 2147483647;
  FindClosestRoad(&start, &road);
    
  assertNotEqual(road.east_mm, -2147483647);    // We don't expect the top-rightmost point 
  assertNotEqual(road.north_mm, -2147483647);   // to be closest to the bottom-leftmost point
}

// FindClosestRoad test for bottom-right
test(FindClosestRoad_bottom_right)
{
  waypoint start;
  waypoint road;
  
  start.east_mm = 2147483647;
  start.north_mm = -2147483647;
  FindClosestRoad(&start, &road);
    
  assertNotEqual(road.east_mm, -2147483647);    // We don't expect the bottom-rightmost point 
  assertNotEqual(road.north_mm, 2147483647);   // to be closest to the top-leftmost point
}

/*-----------------------------------------------------------------------------------------------*/
/* FindPath tests

  Inputs: waypoint *start, waypoint *destination
  Expected values for *start: an address to a waypoint
  Expected values for *destination: an address to a waypoint
  Output: integer
          0 = failure
          INVALID = No path found
          other = BuildPath return value, which is an index into Path[]
  
          struct AStar Open[].ParentID changed to START
          

  Finds a path from start to destination.   

  waypoint data: long latitude, long longitude, long east_mm, long_north_mm, long sigma_mm, unsigned long time_ms,
                 int Evector_x1000, int Nvector_x1000, long speed_mmPs, int index
                 
  waypoint functions:
    void Compute_mm()
    void Compute)LatLon()
    char* formPointString()
    bool readPointString(unsigned long max_wait_ms, int channel)
    void operator=(waypoint& other)
    void operator=(waypoint* other)
    long distance_mm(waypoint* other)
    void vectors(waypoint *other)
    long distance_mm(long east_mm, long north_mm)
*/

// Happy path test for FindPath.
// Verifies the output is correct.
// Verifies the Open parent ID is set correctly.
test(FindPath_Standard)
{
  Origin.Evector_x1000 = INVALID; 
  Origin.Nvector_x1000 = INVALID; 
  Origin.east_mm = 0;
  Origin.north_mm = 0;  
  Origin.latitude = INVALID;
  Origin.longitude = INVALID;  

  Nodes[0].east_mm = 3;
  Nodes[0].north_mm = 4;
  Nodes[0].destination[0] = 1;
  Nodes[0].destination[1] = 3;
  Nodes[0].destination[2] = 4;
  Nodes[0].destination[3] = END;
  Nodes[0].Distance[0] = 3;
  Nodes[0].Distance[1] = sqrt(6*6 + 4*4);
  Nodes[0].Distance[2] = 4;
  Nodes[0].Distance[3] = 1;
  
  Nodes[1].east_mm = 0;
  Nodes[1].north_mm = 4;
  Nodes[1].destination[0] = 0;
  Nodes[1].destination[1] = 2;
  Nodes[1].destination[2] = END;
  Nodes[1].destination[3] = END;
  Nodes[1].Distance[0] = 3;
  Nodes[1].Distance[1] = 3;
  Nodes[1].Distance[2] = 1;
  Nodes[1].Distance[3] = 1;

  Nodes[2].east_mm = -3;
  Nodes[2].north_mm = 4;
  Nodes[2].destination[0] = 1;
  Nodes[2].destination[1] = 3;
  Nodes[2].destination[2] = END;
  Nodes[2].destination[3] = END;
  Nodes[2].Distance[0] = 3;
  Nodes[2].Distance[1] = 4;
  Nodes[2].Distance[2] = 1;
  Nodes[2].Distance[3] = 1;

  Nodes[3].east_mm = -3;
  Nodes[3].north_mm = 0;
  Nodes[3].destination[0] = 0;
  Nodes[3].destination[1] = 2;
  Nodes[3].destination[2] = 4;
  Nodes[3].destination[3] = END;
  Nodes[3].Distance[0] = sqrt(6*6 + 4*4);
  Nodes[3].Distance[1] = 4;
  Nodes[3].Distance[2] = 6;
  Nodes[3].Distance[3] = 1;

  Nodes[4].east_mm = 3;
  Nodes[4].north_mm = 0;
  Nodes[4].destination[0] = 3;
  Nodes[4].destination[1] = 0;
  Nodes[4].destination[2] = END;
  Nodes[4].destination[3] = END;
  Nodes[4].Distance[0] = 6;
  Nodes[4].Distance[1] = 4;
  Nodes[4].Distance[2] = 1;
  Nodes[4].Distance[3] = 1;
  
  waypoint start;
  waypoint destination;
  
  start.index = 0;
  start.sigma_mm = 0;
  start.east_mm = Nodes[0].east_mm;
  start.north_mm = Nodes[0].north_mm;
  destination.index = 3;
  destination.sigma_mm = 0;
  destination.east_mm = Nodes[3].east_mm;
  destination.north_mm = Nodes[3].north_mm;
  
  int actual = FindPath(&start, &destination);
  Serial.println(actual);
  Serial.print(Path[0].formPointString());
  Serial.print(Nodes[0].east_mm);
  Serial.print(", ");
  Serial.println(Nodes[0].north_mm);
  Serial.print(Path[1].formPointString());
  Serial.print(Nodes[1].east_mm);
  Serial.print(", ");
  Serial.println(Nodes[1].north_mm);
  Serial.print(Path[2].formPointString());
  Serial.print(Nodes[2].east_mm);
  Serial.print(", ");
  Serial.println(Nodes[2].north_mm);
  Serial.print(Path[3].formPointString());
  Serial.print(Nodes[3].east_mm);
  Serial.print(", ");
  Serial.println(Nodes[3].north_mm);
  Serial.print(Path[4].formPointString());
  Serial.print(Nodes[4].east_mm);
  Serial.print(", ");
  Serial.println(Nodes[4].north_mm);

  assertEqual(Open[1].ParentID, start.index);

  
}
  
test(ok) 
{
  int x=3;
  int y=3;
  assertEqual(x,y);
}

test(bad)
{
  int x=3;
  int y=3;
  assertNotEqual(x,y);
}


void setup()
{
  Serial.begin(115200);
  Test::out = &Serial;   // Output test results to the Serial monitor
  //Test::exclude("*_skip");  // Skips all test with "_skip" at the end of the name
  //Test::exclude("*_large");  // Skips all test with "_large" at the end of the name

}

void loop()
{
  Test::run();

}

