#include <ArduinoUnit.h>

#include <Common.h>
#include <IO_C3.h>
#include <IO_Mega.h>
#include <Matrix.h>

/*
// Elcano Contol Module C3: Pilot.

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

#define MAX_PATH 10
#define SMALL_TRACK_ERROR_mm   1200
#define MEDIUM_TRACK_ERROR_mm  2700
#define SMALL_STEER_ERROR_deg     2
#define MEDIUM_STEER_ERROR_deg    6
#define STEER_FACTOR              3

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

void setup() 
{
        pinMode(Rx0, INPUT);
        pinMode(Tx0, OUTPUT);
        pinMode(C3_LED, OUTPUT); 
        Serial.begin(9600); 
        pinMode(DATA_READY, INPUT);
        DataAvailable = false;
        attachInterrupt(0, DataReady, FALLING);
        initialize(); 

}

void initialize()
{
  //  Read waypoints of mission from C4 on serial line
     for (int i = 0; i < MAX_MISSION; i++)
      {
        while (!mission[i].readPointString(1000, 0) );
        if (mission[i].index & END)
          break;
      }
  //  Read waypoints of first segment from C4 on serial line
     firstPathSegment = 0;
     activePathSegment = 0;
     lastPathSegment = MAX_PATH - 1;
     for (int i = 0; i < MAX_PATH; i++)
      {
        while (!path[i].readPointString(1000, 0) );
        if (path[i].index & END)
        {
          lastPathSegment = i;
          break;
        }
      }
  //  Read waypoint of intitial position from C6 on serial line
      while (!current_position.readPointString(1000, 0) );  
  //  Synchonize time.
  //     offset_ms = current_position.time_ms;

}

test(initializeBasicData)
{
    initialize();
}



void loop() 
{
  Test::run();
  // put your main code here, to run repeatedly:

}
