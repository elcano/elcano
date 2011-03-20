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
#include "Serial.cpp"

  
#define MAX_WAYPOINTS 10
/*   There are two coordinate systems.
     MDF and RNDF use latitude and longitude.
     C3 and C6 assume that the earth is flat at the scale that they deal with
*/
struct waypoint
{
    double latitude;
    double longitude;
    long x_mm;  // x is east; max is 2147 km
    long y_mm;  // y is true north
} mission[MAX_WAYPOINTS];
int waypoints;

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
  /* Choose the MDF that defines the current mission 
     The MDF consists of a sequence of points given in latitude and longitude. 
     The mission is to visit each of these points in order. 
     waypoints = ReadMDF(mission); */

     
  /* Select all RNDF files that are appropriate to this mission 
    ReadRNDF(mission); */
     
  /* If (initial position is provided)  // used when GPS is unavailable or inaccurate
     {
        ReadPosition();  // latitude and longitude
        // The origin in the (x_mm, y_mm) coordinate system is defined to be the
        // latitude and longitude at which the robot starts. 
        SendInitialPosition(C6);  // latitude and longitude
     }
     */
     
  /* Convert latitude and longitude positions to flat earth coordinates.
     ConvertWaypoint(mission);
  */

  /* The RNDF defines lane segments of a path.
     At many of these segments, ther is no choice to make; it just leads to the next segment.
     Find those segments that represent intersections or other choice points.
     Then construct a graph whose nodes are intersections and whose arcs are labeled with the
     expected time to move between nodes.
     ConstructNetwork() */
     
     /* Read vehicle position, attitude and velocity.
     ReceiveState(C6); 
     mission[0] = InitialPosition; */
     
     /* Plan a path (using RNDF) between each waypoint. 
     Use the A* algorithm as given in
     Robin Murphy, Introduction to AI Robotics, MIT Press.
     Steve Rabin (ed) AI Game Programming Wisdom, Charles River Media.
     
     for (i = 0; i < waypoints; i++)
     {
       PlanPath (waypoint[i]);
     }
     PlanPath must also receive obstacle information from C5.
     We now have a sequence of RNDF segments that defines the mission from
     the origin to the destination.
     */
     
     /* Construct a series of Hermite curves that link the origin and destination.
        Foley et al., Introduction to Computer Graphics.
        ConstructCurves();     */
     /* Construct the speed profiles over these curves.
        Consider how much time we want to come up to speed, time to decelerate
        and reduced speed around turns. 
        ConstructSpeeds();  */
        
     /* Send the first two Hermite curve segments and initial speed profile to C6.
        C6 will add current vehicle position, attitude, velocity and acceleration 
        to create the vehicle state.  The state information is sent on to C3 and 
        fed back to C4.
        SendState(C6); */
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
    ConstructCurves();
    ConstructSpeeds();
    SendState(C6);
 
    About once a second, write the current position and remaining mission, so that if we
    get a reset, we can start from where we left off.   
    */
}

/* Entry point for the simulator.
   This could be done with namespace, but the Arduino IDE does not handle preprocessor statements
   wrapping a namespace in the standard way.
*/
void C4_Planner_setup() { setup(); }

void C4_Planner_loop() { loop(); }


