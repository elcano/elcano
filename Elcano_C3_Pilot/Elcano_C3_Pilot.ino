
#include <Common.h>
//#include <IO_C3.h>
//#include <IO_Mega.h>
#include <Matrix.h>
#include <Elcano_Serial.h>
#include <SPI.h>

/*
// Elcano Contol Module C3: Pilot.

The Pilot program reads a serial line that specifies the desired path and speed 
of the vehicle. It computes the analog signals to control the throttle, brakes 
and steering and sends these to C2.

Input will be recieved and sent using the functions writeSerial and readSerial in 
Elcano_Serial. Inputs will be received from C5(sensors) and C4(planner). Output is sent to 
C2(Low Level).

In USARSIM simulation, these could be written on the serial line as
wheel spin speeds and steering angles needed to
follow the planned path. This would take the form of a DRIVE command to the C1 module over a 
serial line. The format of the DRIVE command is documented in the C1 code.

[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C4 Path planner, which passes on
data from C6 Navigator
*/
/*
#define MAX_PATH 10
#define SMALL_TRACK_ERROR_mm   1200
#define MEDIUM_TRACK_ERROR_mm  2700
#define SMALL_STEER_ERROR_deg     2
#define MEDIUM_STEER_ERROR_deg    6
#define STEER_FACTOR              3

waypoint current_position;  // best estimate of where the robot is.
waypoint mission[MAX_MISSION]; // list of all goal locations
waypoint path[MAX_PATH];       // suggested path for reaching goal; may be a circular buffer
int firstPathTargetLocation=0;   // first index of path[]
int lastPathTargetLocation=0;    // last index of path[]
int activePathTargetLocation=0;  // path[activePathTargetLocation] is closest to current_position.
int trackError_mm=0;  // perpendicular distance to intended path TargetLocation
//int steerError_deg=0; // departure from intended bearing; positive = right
int desiredSpeed_mmPs=0;  // Speed suggested by the path planner
int LeftRange_mm =  7500;
int Range_mm =      7500;
int RightRange_mm = 7500;
char Navigation[BUFFSIZ];  // Holds a serial message giving the current location
char Plan[BUFFSIZ];    // Holds a serial message giving a TargetLocation of the path plan  
void DataReady();    // called by an interrupt when there is serial data to read
extern bool DataAvailable;

// unsigned long millis() is time since program started running
// offset_ms is value of millis() at start_time
// unsigned long offset_ms;
/*
2: Tx: Estimated state; 
      $ESTIM,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
      
   Rx: Desired course
      $EXPECT,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
      // at leat 18 characters
*/
/*
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
/*
void initialize()
{
  int i;
  //  Read waypoints of mission from C4 on serial line
     for (i = 0; i < MAX_MISSION; i++)
      {
        while (!mission[i].readPointString(1000, 0) );
        if (mission[i].index & END)
          break;
      }
  //  Read waypoints of first TargetLocation from C4 on serial line
     firstPathTargetLocation = 0;
     activePathTargetLocation = 0;
     lastPathTargetLocation = MAX_PATH - 1;
     for (i = 0; i < MAX_PATH; i++)
      {
        while (!path[i].readPointString(1000, 0) );
        if (path[i].index & END)
        {
          lastPathTargetLocation = i;
          break;
        }
      }
  //  Read waypoint of intitial position from C6 on serial line
      while (!current_position.readPointString(1000, 0) );  
  //  Synchonize time.
  //     offset_ms = current_position.time_ms;

}
/*---------------------------------------------------------------------------------------*/ 
 /*
// return value is trackError_mm from this TargetLocation
// trackError is positive if left of centerline and negative if right.
int distance(int i,   // index into path[]
    int* pathCompletion)  // per cent of TargetLocation that has been completed
{
  float startX, startY, endX, endY;
  float meX, meY, m;
  float x, y;  // intersection of path TargetLocation and perpendicular to path thru robot position.
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
    *pathCompletion = 100 * abs(current_position.north_mm - startY) / (endY-startY);
    dist_mm = current_position.north_mm - startY >= 0? 
        current_position.east_mm - startX : startX - current_position.east_mm;
  }
  else if (10 * abs(startX-endX) > abs(startY-endY))
  {  // path is East-West
    *pathCompletion = 100 * abs(current_position.east_mm - startX) / (endX-startX);
    dist_mm = current_position.east_mm - startX >= 0?
        current_position.north_mm - startY : startX - current_position.north_mm;
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
    // unit vector of perpendicular is (path[i].Nvector, path[i].Evector)
    // dot product of normal vector with (meX-startX, meY-startY) determines sign of track error.
    if (path[i].Nvector_x1000 * (meX-startX) + path[i].Evector_x1000 * (meY-startY) < 0)
        dist_mm = -dist_mm;
  }
  // Check if we are going the wrong way
  // Dot product is 1 if perfect; -1 if going backwards
  if (current_position.Evector_x1000 * path[i].Evector_x1000 +
      current_position.Nvector_x1000 * path[i].Nvector_x1000 < 0)
    {  // going backwards;  would need to turn around
      dist_mm += dist_mm >= 0 ? TURNING_RADIUS_mm * 2 * PI: -TURNING_RADIUS_mm * 2 * PI;
    }
}
/*---------------------------------------------------------------------------------------*/ 
// Given the current_position and path, find the nearest TargetLocation of the path and the 
// relative position in that TargetLocation. Compute the desired vector and the actual vector.
/*
void WhereAmI()
{
  int dist_mm = MAX_DISTANCE;
  int closest_mm = MAX_DISTANCE;
  int position_mm = 0;
  int i;
  int PerCentDone, done;
  activePathTargetLocation = firstPathTargetLocation;
  done = 0;
  if (firstPathTargetLocation <= lastPathTargetLocation)
  {
    for( i = firstPathTargetLocation; i <= lastPathTargetLocation; i++)
    {
      dist_mm = distance(i,&PerCentDone);
      if (abs(dist_mm) < abs(closest_mm))
      {
        closest_mm = dist_mm;
        activePathTargetLocation = i;
        done = PerCentDone;
      }
    }
  }
  else
  {
    for (i = firstPathTargetLocation; i < MAX_PATH; i++)
    {
      dist_mm = distance(i,&PerCentDone);
      if (abs(dist_mm) < abs(closest_mm))
      {
        closest_mm = dist_mm;
        activePathTargetLocation = i;
        done = PerCentDone;
      }
    }
    for (i = 0; i < lastPathTargetLocation; i++)
    {
      dist_mm = distance(i,&PerCentDone);
      if (abs(dist_mm) < abs(closest_mm))
      {
        closest_mm = dist_mm;
        activePathTargetLocation = i;
        done = PerCentDone;
      }
    }
  }
  if (done >= 100 && activePathTargetLocation != lastPathTargetLocation)
  {  // have passed end of TargetLocation; use the next one
    if (++activePathTargetLocation>= MAX_PATH)
        activePathTargetLocation = 0;
    closest_mm = distance(activePathTargetLocation, &done);
  }
  // compute errors in track, steer and speed
   trackError_mm = closest_mm;
   if (activePathTargetLocation == lastPathTargetLocation)
   {
      desiredSpeed_mmPs = path[activePathTargetLocation].speed_mmPs;
   }
   else
   {
     int pathPerCent = min(100, done);
     pathPerCent = max(0,   done);
     desiredSpeed_mmPs = path[activePathTargetLocation].speed_mmPs + pathPerCent * 
     (path[(activePathTargetLocation+1)%MAX_PATH].speed_mmPs - path[activePathTargetLocation].speed_mmPs);
   }
//   float dotProductM = current_position.Evector_x1000 * path[activePathTargetLocation].Evector_x1000 +
//    current_position.Nvector_x1000 * path[activePathTargetLocation].Nvector_x1000;
//   steerError_deg = acos(dotProductM/MEG);
}

/*---------------------------------------------------------------------------------------*/ 
/*
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

    /*    Slow down depending how close to obstacle. */
    /*
    if (Range_mm < 1250)
    {
         CommandedThrottle = 0;
         CommandedBrake =  FULL_BRAKE;  
    }
    else if (Range_mm < 2500)
    {
         CommandedThrottle = 0;
         CommandedBrake =  HALF_BRAKE;  
    }
    else if (desiredSpeed_mmPs - Speed > 250 && Range_mm > 4000)
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
/*---------------------------------------------------------------------------------------*/ /*
int SetSteering()
{
    static int CommandedSteer = 0;
    int Steer_error_deg = 0;
    int Steer = analogRead(DIRECTION_IN) / 4;  // Units are circle/256
    float DesiredSteer;
    if (path[activePathTargetLocation].Nvector_x1000 >= 0)
    {
      DesiredSteer = acos((float)path[activePathTargetLocation].Evector_x1000 / 1000.) *128. / PI;
    }
    else
    {
      DesiredSteer = 256. - acos((float)path[activePathTargetLocation].Evector_x1000 / 1000.) *128. / PI;
    }
    Steer_error_deg = ((Steer - DesiredSteer) * 256) /360;
    /*
    TO DO;
    If obstacle ahead, but side is clear, move to side.
    Prefer to pass with object on left, since cone toucher is on that side.
    Make a more sophisticated choice of steering angle.
    */
    /*
    if (Range_mm < 5000 &&
       (LeftRange_mm - Range_mm > 500 || RightRange_mm - Range_mm > 500))
    {  // Decide whether to go left or right
       if (LeftRange_mm - RightRange_mm > 1500)
       {// go left
           CommandedSteer -= MEDIUM_STEER_ERROR_deg / STEER_FACTOR;
       }
       else
       { // go right
           CommandedSteer += MEDIUM_STEER_ERROR_deg / STEER_FACTOR;
      }
    }  
    else if ((abs(Steer_error_deg) < SMALL_STEER_ERROR_deg && abs(trackError_mm) < SMALL_TRACK_ERROR_mm) ||
        (abs(trackError_mm) < MEDIUM_TRACK_ERROR_mm && Steer_error_deg * trackError_mm < 0))
        ; // maintain course
    else 
   {  // make correction
       CommandedSteer -= Steer_error_deg / STEER_FACTOR;
   } 
/*   else
    if (DesiredSteer - Steer > 2)
    {  // turn right
      CommandedSteer = 223;
    }
    else if (Steer - DesiredSteer > 2)
    {  // turn left
      CommandedSteer = 159;
    }
  */  
  /*
    analogWrite(STEER_CMD, CommandedSteer);
    return CommandedSteer;  // a value to output to the steer control

}

/*---------------------------------------------------------------------------------------*/ 
/*
void setup() 
{ 
        //pinMode(Rx0, INPUT);
        //pinMode(Tx0, OUTPUT);
        //pinMode(C3_LED, OUTPUT); 
        Serial1.begin(9600); 
        //pinMode(DATA_READY, INPUT);
        //DataAvailable = false;
        //attachInterrupt(0, DataReady, FALLING);
        //initialize();   
} 

*/
/*---------------------------------------------------------------------------------------*/
/*
void loop() 
{
  /*
  int i, k;
  waypoint location;
  static waypoint new_path[MAX_PATH];
  
 if (DataAvailable)
 {
   DataAvailable = false;
   location.readPointString(50, 0);
   if (location.index == POSITION)
   {  //   Read waypoint of current position from C6 on serial line
     current_position = location;
   }
   else
   {  // update to path
       i = location.index & ~END;
       new_path[i] = location; 
       if (location.index & END)
       {  //  Read waypoints of next TargetLocation from C4 on serial line
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
 */
 /*
    // get newest map data from C4 planner
    // Using Elcano_Serial.h Using the SerialData struct in the .h file.
    // Receive a TargetLocation from C4. C4 will only ever send TargetLocations to C3.
    // 
    SerialData instructions;
    readSerial(&Serial1, &instructions);
    Serial.println("test");
    Serial.println(instructions.kind);

    //Test Data for instructions. This is an example of a semgment
    /*instructions.kind = 4;
    instructions.number = 1;
    instructions.speed_cmPs = 100;
    instructions.bearing_deg = 35;
    instructions.posE_cm = 400;
    instructions.posN_cm = 400;

    
    */
/*
}
*/



// TargetLocation struct is used to store the data of each TargetLocation of the path given by the planner.
// We create a new struct becuase the SerialData should only be used to send data.
struct TargetLocation
{
   long int targetSpeed;
   long int bearing;
   long int northPos;
   long int eastPos;
};

// Process segement assures that we received a valid TargetLocation and not noise.
// it then stores the data in another struct that holds similar data. This is 
// done for loose coupling. If we need to change the point data stored locally
// we don't need to try to change the elcano serial file.
bool ProcessTargetLocation(TargetLocation *currentTargetLocation, SerialData instructions)
{
  // Each statement checks that the data received is not int_max.
  if(instructions.speed_cmPs == 2147483648)
  {
    return false;
  }
  else if(instructions.posE_cm == 2147483648)
  {
    return false;
  }
  else if(instructions.posN_cm == 2147483648)
  {
    return false;
  }
  else if(instructions.bearing_deg == 2147483648)
  {
    return false;
  }
  // If none of the data was corrupted then we can store it in the TargetLocation struct provided.
  currentTargetLocation->targetSpeed = instructions.speed_cmPs;
  currentTargetLocation->bearing = instructions.bearing_deg;
  currentTargetLocation->eastPos = instructions.posE_cm;
  currentTargetLocation->northPos = instructions.posN_cm;
  return true;
}


bool ReadWaypoints(TargetLocation* TargetLocationArray)
{
  //set up variables
  int count = 0;
  SerialData dataRead;
  TargetLocation currentTargetLocation;
  // loop until we hit the end of the data
   while(true)
   {
    //check if we're done receiveing
    readSerial(&Serial1,&dataRead);
    if(dataRead.number == 789 || count == MAX_WAYPOINTS) // bad number there is no more data or end of data
    {
      if(count == 0) // nothing was read
      {
        return false;
      }
      break;
    }
    else
    //process and store if valid. 
    {
      if(ProcessTargetLocation(&currentTargetLocation, dataRead))
      {
        TargetLocationArray[count] = currentTargetLocation;
        count++;
      }
      // send back acknowledgement
    }
   }
   return true;
}

//this will be the square test of the first autonomous baby step.
void Drive(int myAngle, int myX, int myY, int targetAngle, int targetX, int targetY)
{
  
}

// This function will rotate the bike to the desired angle. 
// This includes calculation of the difference in its current heading and the target 
// angle. Low level commands will be sent to C2 low level controller. 
void RotateToAngle(int targetAngle, int currentHeading)
{
  //We must know full turing angle and lowest speed
  //calculate angle distance and decide which direction to turn

  //Have we reached our target?
  if(targetAngle == currentHeading)
  {
    return;
  }

  //if not we set the steering angle and conitune turning.
  if(shortestAngle(targetAngle, currentHeading))
  {
  }

  //was is our max steering angle?
  
  //test with turn around twice.   
}

/* The Float Comparison function allows you to compare floats to any X number 
 * of decimal places. This allows us to compare two floats without errors
 * that = comparison will give.
 */

float ShortestAngle(float currentAngle, float targetAngle)
{
     // handle cases of positve past 180 and negative past -180
     // This simplifies our calulations of smallest angle
     currentAngle = UniformAngle(currentAngle);
     targetAngle = UniformAngle(targetAngle);

     //find the lowest angle

     // case of positive positve
     if( currentAngle >= 0 && targetAngle >= 0)
     {
        if(currentAngle > targetAngle)
        {
          return (currentAngle - targetAngle) * -1;
        }
        else
        {
          return (targetAngle - currentAngle);
        }
     }
     
     // case of negative negative
     else if( currentAngle <= 0 && targetAngle <= 0)
     {
        if(currentAngle > targetAngle)
        {
          return (targetAngle - currentAngle);
        }
        else
        {
          return (currentAngle - targetAngle) * -1;
        }
     }
     
     // case of positve negative
     else if( currentAngle >= 0 && targetAngle <= 0)
     {
        float retVal = (-currentAngle + targetAngle);
        if(abs((180 - currentAngle) - (-180 - targetAngle)) < abs(retVal))
        {
          retVal = ((180 - currentAngle) - (-180 - targetAngle));
        }
        return retVal;
     }
     //case of negative positve
     else if( currentAngle <= 0 && targetAngle >= 0)
     {
        float retVal = (-currentAngle + targetAngle);
        if(abs(-(180 + currentAngle) - (180 - targetAngle)) < abs(retVal))
        {
          retVal = (-(180 + currentAngle) - (180 - targetAngle));
        }
        return retVal;
     }
}

// This function converts any angle we are dealing with to be from 0 to 180 and anything
// greater than 180 and less than 0 to be represented as a negative angle. Our circle
// starts with 0 at the top as true north
//             0
//       -90         90
//            180
float UniformAngle(float angle)
{
    if(angle > 180)
     {
        angle = -(360 - angle); 
     }
     if(angle < -180)
     {
        angle = 360 + angle; 
     }
     return angle;
}

// Float comparison allows comparison of floats not using the = operator
// this will return a boolean of the comparison of a and b to the number
// of decimal places defined by places. 
bool FloatComparison(float a, float b, int places)
{
  // values are cast to an integer for = comparison of
  // values.
  int aVal;
  int bVal;
  // each case represents the number of decimal places compared.
  switch(places)
  {
    case 1:
        aVal = a*10;
        bVal = b*10;
        break;
    case 2:
        aVal = a*100;
        bVal = b*100;
        break;
    case 3:
        aVal = a*1000;
        bVal = b*1000;
        break;
    case 4:
        aVal = a*10000;
        bVal = b*10000;
        break;

    default:
        aVal = a;
        bVal = b;
        break;
  }
  // return cases.
  if(aVal == bVal)
  {
    return true;
  }
  else 
  {
    return false;
  }
}


/* This function calculates the angle from the current point to the target point
 * in relation to true north.
 * Quadrants are relative to the current position with y lines pointing true north.
 * for reference quadrants are:
 * 2 1
 * 3 4
 */
float NorthOffset(int currentX, int currentY, int targetX, int targetY)
{
  // quadrant 4
  if ((currentX > targetX) && (currentY > targetY))
  {
    return (-180 + (atan(float(currentX+targetX)/float(currentY+targetY)) * 57.2957795));
  }
  // quadrant 3
  else if((currentX < targetX) && (currentY > targetY))
  {
    return (180 + (atan(float(currentX+targetX)/float(currentY+targetY)) * 57.2957795));
  }
  // quadarant 1 or 2
  else
  {
     return (atan(float(currentX+targetX)/float(currentY+targetY)) * 57.2957795);
  }
}

// Calculate the hypotenuse side of the triangle passed in.
int PothagarianDistance(int currentX, int currentY, int targetX, int targetY)
{
    return sqrt(sq(abs(currentX - targetX)) + sq(abs(currentY - targetY)));
}

// Returns if the two points are with in the target range of eachother.
// Units passed in should be the same for both x, y, and range.
bool ValidRange(float x1,float y1, float x2,float y2, float range)
{
  bool retVal = false;
  float temp = PothagarianDistance(x1,y1,x2,y2);
  if(temp < range)
  {
    retVal = true; 
  }
  return retVal;
}

////////////////////////////////////////////////////////////////////////////
void setup() 
{  
        Serial1.begin(9600); 
        //Serial2.begin(9600);
        //Serial3.begin(9600); 
        pinMode(8,OUTPUT);
}

void loop() 
{
    int steeringAngle = 35;
    int speedSetting = 300;
    // get newest map data from C4 planner
    // Using Elcano_Serial.h Using the SerialData struct in the .h file.
    // Receive a TargetLocation from C4. C4 will only ever send TargetLocations to C3.


    //-----------------------C4 input--------------------------//
    SerialData instructions;
    readSerial(&Serial1, &instructions);
    TargetLocation currentTargetLocation;
    ProcessTargetLocation(&currentTargetLocation,instructions);
    TargetLocation allTargetLocations[MAX_WAYPOINTS];
    ReadWaypoints(allTargetLocations);
    

    //Test of input from C4.
    //Serial.println("test");
    //Serial.println(instructions.kind);
  
    //-----------------------C5 input-------------------------//
    //SerialData sensorData;
    //readSerial(&Serial2, &sensorData);
    

    //---------------------C2 output-------------------------------//

    //Send data to low level.
    //SerialData toLowLevel;
    //toLowLevel.kind = MSG_DRIVE;
    //toLowLevel.angle_deg = steeringAngle;
    //toLowLevel.speed_cmPs = speedSetting;
    //writeSerial(&Serial3, &toLowLevel);

    //Test of output to C2.
    // Outputting to C2 uses the Elcano Serial kind 1 to send a "drive signal to C2"
    // The drive signal includes angle and speed.
    
    //Test Data for instructions C4. This is an example of a semgment
    /*instructions.kind = 4;
    instructions.number = 1;
    instructions.speed_cmPs = 100;
    instructions.bearing_deg = 35;
    instructions.posE_cm = 400;
    instructions.posN_cm = 400;

    //Test Data for C5 sensor input. This is an example of a sensor signal.
    /*
     sensorData.kind = 2;
     sensorData.speedcmPs = 100;
     sensorData.angle_deg = 12;
     sensorData.posE_cm = 50;
     sensorData.posN_cm = 50;
     sensorData.bearing_deg = 15;
    */

    //Test Data for C2 drive output. Example drive commands.
    /*
    toLowLevel.kind = 1;
    toLowLevel.speed_cmPs = 400;
    toLowLevel.angle_deg = 35;
     */
    
    //turning test
    Drive(

}



/*---------------------------------------------------------------------------------------*/ 
/* The format of the command received over the serial line from C6 Navigator is 
  $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<Vx>,<Vy>,POSITION*CKSUM
The Pilot is provided with a few straight TargetLocations and speed profiles that define how
the vehicle travels over the next few meters.

TargetLocations in the path planner include a few parameters:
Four points specifying a cubic spline as an Hermite curve [Foley et al, Introduction to Computer Graphics]. 
  - Start point.
  - Start velocity.
  - End point.
  - End velocity.
These points by themselves specify a family of curves. Specifying the parameter t 
for the speed at which the curve is traversed makes the curve unique. The Pilot 
deals with staight TargetLocations, which are sent by the path planner over the serial line. 
These TargetLocations include the desired speed at the start of each TargetLocation.
The pilot receives the current vehicle position and velocity.

The points are defined on a coordinate system in meters with the X axis east and the 
Y axis true north.


Characters are ASCII. Wide characters are not used.

*/


