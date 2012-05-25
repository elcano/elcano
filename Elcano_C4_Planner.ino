/*  
Elcano Module C4: Path Planner.

Copy the following software files to an Elcano_C6_Navigator directory:
  Elcano_C4_Planner.ino; add new tabs with the following names and copy contents
  Common.cpp
  Common.h
  IO.h

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
#include "Common.h"
#include "IO.h"
#define PI ((float) 3.1415925)
#ifndef NULL
#define NULL 0 
#endif
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
    Nodes and Links come from the RNDF (Route Network Definition File).
    MDF (Mission Definition File) is given.
    Origin is the initial position of the robot and (0,0) on the mm coordinate system. 
    Start is the position of the robot as it begins a new leg of the journey.  
    Destination is next position in MDF.
    Path joins Start and Destination. Finding it is the major task 
    of this Path Planner module.
    Route is a finer scale list of waypoints from present or recent position.
    Exit is the route from Path[last]-> location to Destination.
*/
#define MAP_POINTS 16
struct curve Links[20];
struct junction Nodes[MAP_POINTS] = {
  -140828,  221434, 3 ,   1 ,  END,  END,  0, 0, 0, 0,  // 0
  -140986,   88800, 0 ,   2 ,   5 ,  END,  0, 0, 0, 0,  // 1
  -144313,  -42065, 1 ,   6 ,  END,  END,  0, 0, 0, 0,  // 2
  -78568,   222090, 0 ,   4 ,  END,  END,  0, 0, 0, 0,  // 3
  -38276,   222290, 3 ,   5 ,  END,  END,  0, 0, 0, 0,  // 4
  -39558,    87844, 1 ,   4 ,   8 ,   6 ,  0, 0, 0, 0,  // 5
  -46528,   -41631, 2 ,   5 ,   7 ,   9 ,  0, 0, 0, 0,  // 6
  -45764,  -135413, 6 ,   10,  END,  END,  0, 0, 0, 0,  // 7
  51834,     87232, 5 ,   9 ,   14,  END,  0, 0, 0, 0,  // 8
  53041,    -41220, 6 ,   8 ,   10,  13 ,  0, 0, 0, 0,  // 9
  53438,   -133901, 7 ,   9 ,   11,  END,  0, 0, 0, 0,  // 10
  108750,  -134590, 10 , 12 ,  END,  END,  0, 0, 0, 0,  // 11
  130021,  -143108, 11 , END,  END,  END,  0, 0, 0, 0,  // 12
  182559,   -41031, 9 ,  END,  END,  END,  0, 0, 0, 0,  // 13
  177598,    86098, 8 ,   15,  END,  END,  0, 0, 0, 0,  // 14
  170313,    69008, 14 , END,  END,  END,  0, 0, 0, 0   // 15
};
class waypoint Origin, Start;
struct curve Route, Exit;
waypoint Path[MAX_WAYPOINTS];
//struct junction *Path[20];
  
waypoint mission[CONES];  // aka MDF
int waypoints = CONES;

/*---------------------------------------------------------------------------------------*/ 
// Fill in the rest of Nodes
void ConstructNetwork(junction *Map, int MapPoints)
{
  double deltaX, deltaY;
  for (int i = 0; i < MapPoints; i++)
  {
      if( Map[i].east_mm == INVALID)  continue;
      for (int j = 0;  j< 4; j++)
      {
        if (Map[i].destination[j] == END) continue;
        deltaX = Map[i].east_mm;
        deltaX -= Map[Map[i].destination[j]].east_mm;
        deltaY = Map[i].north_mm;
        deltaY -= Map[Map[i].destination[j]].north_mm;
        Map[i].Distance[j] = sqrt(deltaX*deltaX + deltaY*deltaY);
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
void SendMission()
{
  char *dataString;
  for( int i = 0; i < CONES; i++)
  {
    dataString = mission[i].formPointString();
    checksum(dataString);
    Serial.println(dataString);
  }
}
/*---------------------------------------------------------------------------------------*/
int distance(int i, int *j,  // index into Nodes[]
    int* pathCompletion)  // per cent of segment that has been completed
{
  return 0;
}
/*---------------------------------------------------------------------------------------*/

void FindClosestRoad(waypoint *location, 
            waypoint *road )  // information returned
{
  long closest_mm = MAX_DISTANCE;
  long dist;
  int close_index;
  int perCent, done;
  int i, j;
  // find closest road.
  for (i = 0; i < MAP_POINTS; i++)
  {
    dist = distance(i, &j, &perCent);
    if (dist < closest_mm && perCent >= 0 && perCent <= 100)
    {
      close_index = i;
      closest_mm = dist;
      done = perCent;
    }
  }
  if (closest_mm < MAX_DISTANCE)
  {
    i = road->index = close_index;
    road->sigma_mm = Nodes[close_index].destination[j];
    j = road->sigma_mm;
    road->east_mm = Nodes[i].east_mm + done *(Nodes[j].east_mm - Nodes[i].east_mm) / 100;
    road->north_mm = Nodes[i].north_mm + done *(Nodes[j].north_mm - Nodes[i].north_mm) / 100;
  }
  else
  { // find closest node
     for (i = 0; i < MAP_POINTS; i++)
    {
      dist = location->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
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
}
/*---------------------------------------------------------------------------------------*/
// origin and destination are on the road network given in Nodes.
// origin is in Path[1].
// Place other junction waypoints into Path.
// Returned value is next index into Path.
// origin->index identifies the closest node.
// sigma_mm holds the index to the other node.
#define ORIGIN -1
class AStar
{
 public: 
    int id;
    int ParentID;
    int CostFromStart;
    int CostToGoal;
};
int FindPath(waypoint *origin, waypoint *destination)
{

  int last = 2;
  int route[MAP_POINTS];
  AStar Open[MAP_POINTS];
  AStar Closed[MAP_POINTS];
  int OpenIndex = 0;
  int ClosedIndex = 1;
  int i, j, k;
  int NewCost;
  bool Processed = false;
//  Location_mm loc_mm;
  Open[0].CostFromStart = 0;
  Open[0].CostToGoal = origin->distance_mm(destination);
  Open[0].ParentID = 0;
  Open[0].id = ORIGIN;  // the origin
  while (OpenIndex >= 0)
  {  // TO DO; pop lowest cost node from Open
     i = Open[OpenIndex].id;
     if (i == destination->index ||
         i == destination->sigma_mm)
        {  // TO DO: Maybe done; we have reached the node that leads to goal turn-off
           // if really done, construct path backward to origin.
          return last;
        }
    if (i == ORIGIN)
    {  // get successor nodes of origin
       OpenIndex++;
       Open[OpenIndex].id = origin->index;
       Open[OpenIndex].CostFromStart = origin->distance_mm
         (Nodes[Open[OpenIndex].id].east_mm,Nodes[Open[OpenIndex].id].north_mm);
       Open[OpenIndex].CostToGoal = destination->distance_mm
         (Nodes[Open[OpenIndex].id].east_mm,Nodes[Open[OpenIndex].id].north_mm);
       Open[OpenIndex].ParentID = ORIGIN;
       OpenIndex++;
       Open[OpenIndex].id = origin->sigma_mm;
       Open[OpenIndex].CostFromStart = origin->distance_mm
         (Nodes[Open[OpenIndex].id].east_mm,Nodes[Open[OpenIndex].id].north_mm);
       Open[OpenIndex].CostToGoal = destination->distance_mm
         (Nodes[Open[OpenIndex].id].east_mm,Nodes[Open[OpenIndex].id].north_mm);
       Open[OpenIndex].ParentID = ORIGIN;
    }
    else
    {  // get successor nodes from map
      for (j = 0; j < 4; j++)
      {
        NewCost = Open[i].CostFromStart + Nodes[i].Distance[j];
      }
      // check if this node is already on Open or Closed.
      Processed = false;
      for (k = OpenIndex; k > 0; k--)
      {
          if (Open[k].id == Nodes[i].destination[i])
          {
            Processed = true;
            break;
          }                   
     }
     if (Processed && Open[k].CostFromStart <= NewCost) 
         continue;
      for (k = ClosedIndex; k >= 0; k--)
      {
          if (Closed[k].id == Nodes[i].destination[i])
          {
            Processed = true;
            break;
          }      
      }
     if (Processed && Closed[k].CostFromStart <= NewCost) 
         continue;
    // store the new or improved information
       OpenIndex++;
       Open[OpenIndex].id = Nodes[i].destination[j];
       Open[OpenIndex].CostFromStart = origin->distance_mm
         (Nodes[Open[OpenIndex].id].east_mm, Nodes[Open[OpenIndex].id].north_mm);
       Open[OpenIndex].CostToGoal = destination->distance_mm
         (Nodes[Open[OpenIndex].id].east_mm, Nodes[Open[OpenIndex].id].north_mm);
       Open[OpenIndex].ParentID = i;
    // TO DO; if new node is in closed, remove new node from closed
    // if new node is in open, adjust information
    // else, add new node to open.
    // } end Node not a goal
    // if node is not a goal, push it onto Closed.
    }  // end of succeessor from map.
  }   // end of while Open is not empty
  
  return 0;  // failure
}
/*---------------------------------------------------------------------------------------*/
// Low level path is a straight line from origin to detination.
// PathPlan makes an intermediate level path that uses as many roads as possible.
 void PlanPath (waypoint *origin, waypoint *destination)
 {
   waypoint roadOrigin, roadDestination;
   Path[0] = origin;
   Path[0] = 0;

   FindClosestRoad( origin, &roadOrigin );
   FindClosestRoad( destination, &roadDestination ); 
   if  (abs(origin->east_mm  - roadOrigin.east_mm)
      + abs(origin->north_mm - roadOrigin.north_mm)
      + abs(destination->east_mm  - roadDestination.east_mm)
      + abs(destination->north_mm - roadDestination.north_mm) >=
        abs(origin->east_mm  - destination->east_mm)
      + abs(origin->north_mm - destination->north_mm))
      {  // don't use roads; go direct
        Path[1] = destination;
        Path[1].index = 1 | END;
      }
      else
      {  // use A* with the road network
        Path[1] = roadOrigin;
        Path[1].index = 1;
        int last = FindPath(&roadOrigin, &roadDestination);
        Path[last] = destination;
        Path[last].index = last | END;
      }
 }
/*---------------------------------------------------------------------------------------*/ 
/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
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
       SendMission();
     
     /* Plan a Path (using RNDF) between each Node. 
     Use the A* algorithm as given in
     Robin Murphy, Introduction to AI Robotics, MIT Press.
     Steve Rabin (ed) AI Game Programming Wisdom, Charles River Media. */
     
     
      PlanPath (&mission[0], &mission[1]);
     
     /*
     PlanPath must also receive obstacle information from C5.
     The output from PlanPath is a sequence of waypoints Path[] where each waypoint
     is associated with a junction in the RNDF.  We must then supplement the junction
     points with all the intermediate non-junction points in the RNDF so that
     the vehicle can follow curves in the road. The first section of this is Route.
     
     We now have a sequence of RNDF segments that defines the mission from
     the origin to the destination.
     
     We do not need finer grain that the Route waypoints from the RNDF, since each of them is 
     implicitly linked by a cubic curve, which defines a smoother path than
     line segments would. To find the points in between the RNDF waypoints,
     use the function GetPosition. */
    
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
/* Given a pair of waypoints and a parameter t, MakeCurve finds an intermediate waypoint
   on a smooth Hermite curve connecting them.
   0 < t < 1.
   Unfortunately, t is non linear.
   What we would really like is to know the distance from start to End along the curve,
   so that we could find a waypoint corresponding to a fraction of that distance.
   The distance along a Hermite (equivalently Bezier) curve cannot be found in closed form.
   Instead we can use t to find a sequence of points connecting start and End.
   If the straight line distance between any of these intermediate points is too large,
   subdivide with an intermediate value of t.

void MakeCurve(waypoint *start, waypoint *End, float t, waypoint *newp)
{
	double p0, p1, p2, p3;
	double q0, q1, q2, q3;
	double angle;
	double dx, dy, cos_end, sin_end;
	p0 = (double) start->east_mm;
	q0 = (double) start->north_mm;
//	angle = (90-start->bearing) * PI / 180;
	p1 = (double)(start->Evector_x1000) / 1000.;
	q1 = (double)(start->Nvector_x1000) / 1000.;
	dx = (double) End->east_mm - p0;
	dy = (double) End->north_mm - q0;
//	angle = (90-End->bearing) * PI / 180;
	cos_end = (double)(End->Evector_x1000) / 1000.;
	sin_end = (double)(End->Nvector_x1000) / 1000.;
	p2 = 3*dx - (2*p1 + cos_end);
	q2 = 3*dy - (2*q1 + sin_end);
	p3 = -2*dx + p1 + cos_end;
	q3 = -2*dy + q1 + sin_end;
	newp->east_mm  = (long) (p0 + t*(p1 + t*(p2 + t*p3)));
	newp->north_mm = (long) (q0 + t*(q1 + t*(q2 + t*q3)));
}
*/
/*---------------------------------------------------------------------------------------*/ 
/*  GetPosition is a more general interface to MakeCurve.
    Here t can be any value. The integral part counts the segments that make up the curve.

void GetPosition(curve *path, float t, waypoint *newp)
{
	curve *start = path;
	curve *End;
	newp->latitude = INVALID;
	newp->longitude = INVALID;
	newp->east_mm = 0;
	newp->north_mm = 0;
	newp->Evector_x1000 = INVALID;
	newp->Nvector_x1000 = INVALID;
	if (path->present == NULL) return;
	if (t == 0 || (t > 0 && path->next == NULL) ||
		(t < 0 && path->previous == NULL))
	{
              newp = path->present;
	      return;
	}
	while (t++ < 1)
	{
		start = start->previous;
		End =   start->previous;
		if (End == NULL)
		{
			newp =  start->present;
			return;
		}
	}
		while (t-- > 1)
	{
		start = start->next;
		End =   start->next;
		if (End == NULL)
		{
			newp =  start->present;
			return;
		}
	}
	if (t < 0)
		t = -t;
	if (t == 1)
	{
		newp =  End->present;
		return;
	}
	MakeCurve( start->present, End->present, t, newp);
}
*/
/*---------------------------------------------------------------------------------------*/ 
void setup() 
{ 
        pinMode(Rx0, INPUT);
        pinMode(Tx0, OUTPUT);
        pinMode(LED, OUTPUT); 
     	Serial.begin(9600); 
        pinMode(DATA_READY, INPUT);
        DataAvailable = false;
        attachInterrupt(0, DataReady, FALLING);

   initialize();
}
/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
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
}


