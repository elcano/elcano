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
waypoint current_position;
waypoint mission[MAX_MISSION];
waypoint path[MAX_PATH];
char Navigation[BUFFSIZ];
char Plan[BUFFSIZ];
void DataReady();
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
     for (i = 0; i < MAX_PATH; i++)
      {
        while (!path[i].readPointString(Plan, 1000, 0) );
        if (path[i].index & END)
          break;
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
// Given the current_position and path, find the nearest segment of the path and the 
// relative position in that segment. Compute the desired vector and the actual vector.
void WhereAmI()
{
}

/*---------------------------------------------------------------------------------------*/ 
int SetSpeed()
{
    int Speed = analogRead(SPEED_IN);
    int go = digitalRead(MOVE_OK); // Read Stop/Go signal from C2
    if (go == LOW)
    {
      analogWrite(BRAKE_CMD, 255);  // full brake
      analogWrite(THROTTLE_CMD, 0);
      Speed = 0;
      return Speed;
    }

  //  Read sonar obstacle detectors
    int LeftRange =  analogRead(LEFT) + OFFSET;
    int Range =      analogRead(FRONT) + OFFSET;
    int RightRange = analogRead(RIGHT) + OFFSET;
    
    return Speed;
 
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
    
    
    int Speed = SetSpeed();
 //   int Turn  = DesiredHeading();
/* TO DO:
   
// Read path and speed profile for next motions.

  Compute course
  Send joystick signals to C2

*/ 
}
/*---------------------------------------------------------------------------------------*/ 
/* The format of the command received over the serial line from C6 Navigator is 
  $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<bearing>,POSITION*CKSUM
The Pilot is provided with a few curve segments and speed profiles that define how
the vehicle travels over the next few meters.

Segments must include a few parameters:
Four points specifying a cubic spline as an Hermite curve [Foley et al, Introduction to Computer Graphics]. 
  - Start point.
  - Start velocity.
  - End point.
  - End velocity.
These points by themselves specify a family of curves. Specifying the parameter t 
for the speed at which the curve is traversed makes the curve unique. This value
is also provided to the Pilot over the serial line.

The points are defined on a coordinate system in meters with the X axis east and the 
Y axis true north.

The four points and t parameter specify a unique path.  They do not specify the speed at 
which the vehicle moves over the path. Vehicle speed is given by a sequence of ordered
pairs consisting of distance traveled on the path in meters and the requested speed at
that point in meters / second.  The first item in this sequence would be zero position and the
the current speed of the vehicle. This item may or may not be explicitly included. 
The distances travelled on the curve are cummulative and the last one should be equal
to the arc length of the curve.

The pilot must also receive the current vehicle position, attitude, velocity and acceleration.

Characters are ASCII. Wide characters are not used.

TO DO: Define the structure used for curve segment and motion profile.
TO DO: Write GetSerial.
*/
void GetSerial( unsigned long *TimeOfCmd)
{
}

