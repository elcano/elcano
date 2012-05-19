/*
// Elcano Contol Module C3: Pilot.

Copy the following software files to an Elcano_C6_Navigator directory:
  Elcano_C3_Pilot.ino; add new tabs with the following names and copy contents
  Common.cpp
  Common.h
  IO.h

The Pilot program reads a serial line that specifies the desired path and speed 
of the vehicle. It computes the analog signals to control the throttle, brakes 
and steering and sends these to C2.

In USARSIM simulation, these could be written on the serial line as
wheel spin speeds and steering angles needed to
follow the planned path. This would take the form of a DRIVE command to the C1 module over a 
serial line. The format of the DRIVE command is documented in the C1 code.

[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C4 Path planner, which passes on
data from C6 Navigator
*/

#include "IO.h"
#include "Common.h"

#define MAX_PATH 10
waypoint current_position;  // best estimate of where the robot is.
waypoint mission[MAX_MISSION]; // list of all goal locations
waypoint path[MAX_PATH];       // suggested path for reaching goal; may be a circular buffer
int firstPathSegment=0;   // first index of path[]
int lastPathSegment=0;    // last index of path[]
int activePathSegment=0;  // path[activePathSegment] is closest to current_position.
int trackError_mm=0;  // perpendicular distance to intended path segment
//int steerError_deg=0; // departure from intended bearing; positive = right
int desiredSpeed_mmPs=0;  // Speed suggested by the path planner
int LeftRange_mm =  7500;
int Range_mm =      7500;
int RightRange_mm = 7500;
char Navigation[BUFFSIZ];  // Holds a serial message giving the current location
char Plan[BUFFSIZ];    // Holds a serial message giving a segment of the path plan  
void DataReady();    // called by an interrupt when there is serial data to read
extern bool DataAvailable;

// unsigned long millis() is time since program started running
// offset_ms is value of millis() at start_time
unsigned long offset_ms;
/*
2: Tx: Estimated state; 
      $ESTIM,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
   Rx: Desired course
      $EXPECT,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
      // at leat 18 characters
*/

//---------------------------------------------------------------------------
char ObstacleString[BUFFSIZ];
char* obstacleDetect()
{
// Calibration shows that readings are 5 cm low.
#define OFFSET 5
    int LeftRange =  analogRead(LEFT) + OFFSET;
    int Range =      analogRead(FRONT) + OFFSET;
    int RightRange = analogRead(RIGHT) + OFFSET;

  sprintf(ObstacleString, 
  "%d.%0.2d,%d.%0.2d,%d.%0.2d,",
  LeftRange/100, LeftRange%100, Range/100, Range%100, RightRange/100, RightRange%100);
 
  return ObstacleString;
}

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
  int i;
  //  Read waypoints of mission from C4 on serial line
     for (i = 0; i < MAX_MISSION; i++)
      {
        while (!mission[i].readPointString(Plan, 1000, 0) );
        if (mission[i].index & END)
          break;
      }
  //  Read waypoints of first segment from C4 on serial line
     firstPathSegment = 0;
     activePathSegment = 0;
     lastPathSegment = MAX_PATH - 1;
     for (i = 0; i < MAX_PATH; i++)
      {
        while (!path[i].readPointString(Plan, 1000, 0) );
        if (path[i].index & END)
        {
          lastPathSegment = i;
          break;
        }
      }
  //  Read waypoint of intitial position from C6 on serial line
      while (!current_position.readPointString(Navigation, 1000, 0) );  
  //  Synchonize time.
      offset_ms = current_position.time_ms;

}
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
// return value is trackError_mm from this segment
int distance(int i,   // index into path[]
    int* pathCompletion)  // per cent of segment that has been completed
{
  float startX, startY, endX, endY;
  float meX, meY, m;
  float x, y;  // intersection of path segment and perpendicular to path thru robot position.
  int dist_mm;   // track error
  float Dist_mm;
  startX = path[i].east_mm;
  startY = path[i].north_mm;
  if (path[i].index & END)
  {  // extrapolate 30 meters
    endX = startX + path[i].Evector_x1000 * 30;
    endY = startY + path[i].Nvector_x1000 * 30;
  }
  else
  {
    endX =path[(i+1)%MAX_PATH].east_mm;
    endY =path[(i+1)%MAX_PATH].north_mm;    
  }
  
  if (abs(startX-endX) < 10 * abs(startY-endY))
  {  // path is North-South
    *pathCompletion = 100 * (current_position.north_mm - startY) / (endY-startY);
    dist_mm = current_position.east_mm;
  }
  else if (10 * abs(startX-endX) > abs(startY-endY))
  {  // path is East-West
    *pathCompletion = 100 * (current_position.east_mm - startX) / (endX-startX);
    dist_mm = current_position.north_mm;
  }
  else
  {
    meX = current_position.east_mm;
    meY = current_position.north_mm;
    m = (endY - startY) / (endX - startX);  // slope of path
    // path is y = m*(x-startX)+startY
    // perpendicular to path is y = (-1/m)*(x-meX)+meY
    // intersection is x = (meX + m*m*startX+m*(meY-startY))/(m*m-1)
    //                 y = meX*(m*m+2)+m*m*startX+m*(meY-startY)/(m*m-1) + meY
    x = (meX + m*m*startX+m*(meY-startY))/(m*m-1);
    y = m*(x-startX)+startY; 
    Dist_mm = sqrt((meX-x)*(meX-x) + (meY-y)*(meY-y));
    dist_mm = Dist_mm;
    if (abs(m) <=1)
    {  // more change in east-west
        *pathCompletion = 100 * (x - startX) / (endX-startX);
    }
    else
    {  // more change north-south
        *pathCompletion = 100 * (y - startY) / (endY-startY);
    }  
  }
  // Check if we are going the wrong way
  // Dot product is 1 if perfect; -1 if going backwards
  if (current_position.Evector_x1000 * path[i].Evector_x1000 +
      current_position.Nvector_x1000 * path[i].Nvector_x1000 < 0)
    {  // going backwards;  would need to turn around
      dist_mm += TURNING_RADIUS_mm * 2 * PI;
    }
}
/*---------------------------------------------------------------------------------------*/ 
// Given the current_position and path, find the nearest segment of the path and the 
// relative position in that segment. Compute the desired vector and the actual vector.
void WhereAmI()
{
  int dist_mm = MAX_DISTANCE;
  int closest_mm = MAX_DISTANCE;
  int position_mm = 0;
  int i;
  int PerCentDone, done;
  activePathSegment = firstPathSegment;
  done = 0;
  if (firstPathSegment <= lastPathSegment)
  {
    for( i = firstPathSegment; i <= lastPathSegment; i++)
    {
      dist_mm = distance(i,&PerCentDone);
      if (dist_mm < closest_mm)
      {
        closest_mm = dist_mm;
        activePathSegment = i;
        done = PerCentDone;
      }
    }
  }
  else
  {
    for (i = firstPathSegment; i < MAX_PATH; i++)
    {
      dist_mm = distance(i,&PerCentDone);
      if (dist_mm < closest_mm)
      {
        closest_mm = dist_mm;
        activePathSegment = i;
        done = PerCentDone;
      }
    }
    for (i = 0; i < lastPathSegment; i++)
    {
      dist_mm = distance(i,&PerCentDone);
      if (dist_mm < closest_mm)
      {
        closest_mm = dist_mm;
        activePathSegment = i;
        done = PerCentDone;
      }
    }
  }
  // compute errors in track, steer and speed
   trackError_mm = closest_mm;
   if (activePathSegment == lastPathSegment)
   {
      desiredSpeed_mmPs = path[activePathSegment].speed_mmPs;
   }
   else
   {
     int pathPerCent = min(100, PerCentDone);
     pathPerCent = max(0,   PerCentDone);
     desiredSpeed_mmPs = path[activePathSegment].speed_mmPs + pathPerCent * 
     (path[(activePathSegment+1)%MAX_PATH].speed_mmPs - path[activePathSegment].speed_mmPs);
   }
//   float dotProductM = current_position.Evector_x1000 * path[activePathSegment].Evector_x1000 +
//    current_position.Nvector_x1000 * path[activePathSegment].Nvector_x1000;
//   steerError_deg = acos(dotProductM/MEG);
}

/*---------------------------------------------------------------------------------------*/ 
int SetSpeed()
{
    int Speed = analogRead(SPEED_IN) * MAX_SPEED_mmPs / 4095;
    static int CommandedThrottle = 0;
    static int CommandedBrake = 0;
    int go = digitalRead(MOVE_OK); // Read Stop/Go signal from C2
    if (go == LOW || desiredSpeed_mmPs == 0)
    {
      CommandedThrottle = 0;
      CommandedBrake = 255;   // full brake
    }

    /*
    TO DO;
    Slow down depending how close to obstacle.
    */
    if (desiredSpeed_mmPs - Speed > 250)
    {
      CommandedThrottle = STANDARD_ACCEL;
      CommandedBrake = 0;  
    }
    else if (Speed - desiredSpeed_mmPs > 250)
    {
      CommandedThrottle = 0;
      CommandedBrake = (Speed - desiredSpeed_mmPs > 1500)? FULL_BRAKE: HALF_BRAKE;  
    }
    
    analogWrite(BRAKE_CMD, CommandedBrake); 
    analogWrite(THROTTLE_CMD, CommandedThrottle);
    return Speed;
 
}
/*---------------------------------------------------------------------------------------*/ 
int SetSteering()
{
    static int CommandedSteer = 0;
    int Steer = analogRead(DIRECTION_IN) / 4;  // Units are circle/256
    float DesiredSteer;
    if (path[activePathSegment].Nvector_x1000 >= 0)
    {
      DesiredSteer = acos((float)path[activePathSegment].Evector_x1000 / 1000.) *128. / PI;
    }
    else
    {
      DesiredSteer = 256. - acos((float)path[activePathSegment].Evector_x1000 / 1000.) *128. / PI;
    }
    /*
    TO DO;
    If obstacle ahead, but side is clear, move to side.
    Prefer to pass with object on left, since cone toucher is on that side.
    Make a more sophisticated choice of steering angle.
    */
    if (DesiredSteer - Steer > 2)
    {  // turn right
      CommandedSteer = 223;
    }
    else if (Steer - DesiredSteer > 2)
    {  // turn left
      CommandedSteer = 159;
    }
    
    analogWrite(STEER_CMD, CommandedSteer);
    return Steer;

}
/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
  int i, k;
  waypoint location;
  static waypoint new_path[MAX_PATH];
  
 if (DataAvailable)
 {
   location.readPointString(Navigation, 50, 0);
   if (location.index == POSITION)
   {  //   Read waypoint of current position from C6 on serial line
     current_position = location;
   }
   else
   {  // update to path
       i = location.index & ~END;
       new_path[i] = location; 
       if (location.index & END)
       {  //  Read waypoints of next segment from C4 on serial line
          for (k = 0; k <= i; k++)
          {
             path[k] = new_path[k];
          }
       } 
   }
 }
     WhereAmI(); 
  //  Read sonar obstacle detectors
    LeftRange_mm =  10 *(analogRead(LEFT) + OFFSET);
    Range_mm =      10 *(analogRead(FRONT) + OFFSET);
    RightRange_mm = 10 *(analogRead(RIGHT) + OFFSET);
// Use fuzzy controller to find proper speed and steering
//  Send joystick signals to C2    
    int Speed = SetSpeed();
    int Turn  = SetSteering();

}
/*---------------------------------------------------------------------------------------*/ 
/* The format of the command received over the serial line from C6 Navigator is 
  $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<Vx>,<Vy>,POSITION*CKSUM
The Pilot is provided with a few straight segments and speed profiles that define how
the vehicle travels over the next few meters.

Segments in the path planner include a few parameters:
Four points specifying a cubic spline as an Hermite curve [Foley et al, Introduction to Computer Graphics]. 
  - Start point.
  - Start velocity.
  - End point.
  - End velocity.
These points by themselves specify a family of curves. Specifying the parameter t 
for the speed at which the curve is traversed makes the curve unique. The Pilot 
deals with staight segments, which are sent by the path planner over the serial line. 
These segments include the desired speed at the start of each segment.
The pilot receives the current vehicle position and velocity.

The points are defined on a coordinate system in meters with the X axis east and the 
Y axis true north.


Characters are ASCII. Wide characters are not used.

*/


