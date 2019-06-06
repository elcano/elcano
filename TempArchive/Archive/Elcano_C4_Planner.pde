/*  
Elcano Module C4: Path Planner.

Since this module reads files from a memory stick, it may use a local operating system.

Another option is to have the MDF and RNDF on a PC, which compiles them into the Arduino
code for C6. Before the robot starts, the PC is plugged into the Arduino and the program 
is downloaded. The PC is removed before the robot starts.

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
#define PI ((float) 3.1415925)

#ifdef SIMULATOR
#include "..\\Elcano\\Elcano\\Elcano_simulator.h"
#include <math.h>

#else
#include "Serial.cpp"
#endif

namespace C4_Planner {
    void setup();
    void loop();
}
using namespace C4_Planner;

#ifndef NULL
#define NULL 0 
#endif

#define MAX_WAYPOINTS 10
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
struct curve Links[20];
struct junction Nodes[20];
struct waypoint Origin, Start;
struct curve Route, Exit;
struct junction *Path[20];
struct waypoint Destination;
struct waypoint MDF[6];

/*---------------------------------------------------------------------------------------*/ 
#ifdef SIMULATOR
namespace C4_Planner {
#endif
  
waypoint mission[MAX_WAYPOINTS];
int waypoints;

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
  Origin.bearing = INVALID; 
  Origin.east_mm = 0;
  Origin.north_mm = 0;  
  Origin.latitude = INVALID;
  Origin.longitude = INVALID;  
  
  /* Choose the MDF that defines the current mission 
     The MDF consists of a sequence of points given in latitude and longitude. 
     The mission is to visit each of these points in order. 
     MDF = ReadMDF(mission); */

     
  /* Select all RNDF files that are appropriate to this mission. 
     Initially we will assume that there is a single RNDF file.
     ReadRNDF(mission); */
  /* The RNDF defines lane segments of a path.
     Use them to fill in Links and Nodes.
     At many of the RNDF segments, ther is no choice to make; it just leads to the next waypoint.
     Find those segments that represent intersections or other choice points.
     Then construct a graph whose nodes are intersections and whose arcs are labeled with the
     expected time and distance to move between nodes.
     ConstructNetwork() */
     
  /* If (initial position is provided)  // used when GPS is unavailable or inaccurate
     {
        ReadPosition();  // latitude and longitude
        // The origin in the (east_mm, north_mm) coordinate system is defined to be the
        // latitude and longitude at which the robot starts. 
        // Bearing is initial robot attitude.
        SendInitialPosition(C6);  // latitude and longitude
     }
     */
     
  /* Convert latitude and longitude positions to flat earth coordinates.
     for each waypoint in Origin, Start, Destination, MDF, Nodes, Links
       LatatitudeLongitude(waypoint *Waypoint);
  */
 
     /* Read vehicle position, attitude and velocity.
     ReceiveState(C6); 
     fill in Origin.latitude, Origin.longitude, Origin.bearing 
     set Start to Origin; */
     
     
     /* Plan a Path (using RNDF) between each Node. 
     Use the A* algorithm as given in
     Robin Murphy, Introduction to AI Robotics, MIT Press.
     Steve Rabin (ed) AI Game Programming Wisdom, Charles River Media. */
     
     
/*   for (i = 0; i < waypoints; i++)
     {
       PlanPath (waypoint[i]);
     }
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
     use the function GetPosition.
    
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
*/
void MakeCurve(waypoint *start, waypoint *End, float t, waypoint *newp)
{
	double p0, p1, p2, p3;
	double q0, q1, q2, q3;
	double angle;
	double dx, dy, cos_end, sin_end;
	p0 = (double) start->east_mm;
	q0 = (double) start->north_mm;
	angle = (90-start->bearing) * PI / 180;
	p1 = cos(angle);
	q1 = sin(angle);
	dx = (double) End->east_mm - p0;
	dy = (double) End->north_mm - q0;
	angle = (90-End->bearing) * PI / 180;
	cos_end = cos(angle);
	sin_end = sin(angle);
	p2 = 3*dx - (2*p1 + cos_end);
	q2 = 3*dy - (2*q1 + sin_end);
	p3 = -2*dx + p1 + cos_end;
	q3 = -2*dy + q1 + sin_end;
	newp->east_mm  = (long) (p0 + t*(p1 + t*(p2 + t*p3)));
	newp->north_mm = (long) (q0 + t*(q1 + t*(q2 + t*q3)));
}
/*---------------------------------------------------------------------------------------*/ 
/*  GetPosition is a more general interface to MakeCurve.
    Here t can be any value. The integral part counts the segments that make up the curve.
*/
void GetPosition(curve *path, float t, waypoint *newp)
{
	curve *start = path;
	curve *End;
	newp->latitude = INVALID;
	newp->longitude = INVALID;
	newp->east_mm = 0;
	newp->north_mm = 0;
	newp->bearing = INVALID;
	if (path->present == NULL) return;
	if (t == 0 || (t > 0 && path->next == NULL) ||
		(t < 0 && path->previous == NULL))
	{
		newp->latitude =  path->present->latitude;
		newp->longitude = path->present->longitude;
		newp->east_mm =   path->present->east_mm;
		newp->north_mm =  path->present->north_mm;
		newp->bearing =   path->present->bearing;
		return;
	}
	while (t++ < 1)
	{
		start = start->previous;
		End =   start->previous;
		if (End == NULL)
		{
			newp->latitude =  start->present->latitude;
			newp->longitude = start->present->longitude;
			newp->east_mm =   start->present->east_mm;
			newp->north_mm =  start->present->north_mm;
			newp->bearing =   start->present->bearing;
			return;
		}
	}
		while (t-- > 1)
	{
		start = start->next;
		End =   start->next;
		if (End == NULL)
		{
			newp->latitude =  start->present->latitude;
			newp->longitude = start->present->longitude;
			newp->east_mm =   start->present->east_mm;
			newp->north_mm =  start->present->north_mm;
			newp->bearing =   start->present->bearing;
			return;
		}
	}
	if (t < 0)
		t = -t;
	if (t == 1)
	{
		newp->latitude =  End->present->latitude;
		newp->longitude = End->present->longitude;
		newp->east_mm =   End->present->east_mm;
		newp->north_mm =  End->present->north_mm;
		newp->bearing =   End->present->bearing;
		return;
	}
	MakeCurve( start->present, End->present, t, newp);
}
/*---------------------------------------------------------------------------------------*/ 
void setup() 
{ 
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
#ifdef SIMULATOR
} // end namespace
#endif
/* Entry point for the simulator.
   This could be done with namespace, but the Arduino IDE does not handle preprocessor statements
   wrapping a namespace in the standard way.
*/
void C4_Planner_setup() { C4_Planner::setup(); }

void C4_Planner_loop() { C4_Planner::loop(); }


