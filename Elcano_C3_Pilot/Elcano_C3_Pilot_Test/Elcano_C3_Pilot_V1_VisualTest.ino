#include <TimerOne.h>
#include <Common.h>
#include <Matrix.h>
#include <Elcano_Serial.h>
#define STARTSEG 0
#define ENDSEG 1000 
#define STARTSPLINE 5

//#include <SPI.h>

/*
// Elcano Contol Module C3: Pilot.

The Pilot program reads a serial line that specifies the desired path 
and speed of the vehicle. It computes the analog signals to control 
the throttle, brakes and steering and sends these to C2.

Input will be recieved and sent using the functions writeSerial and 
readSerial in Elcano_Serial. Inputs will be received from C5(sensors)
and C4(planner). Output is sent to C2(Low Level).

In USARSIM simulation, these could be written on the serial line as
wheel spin speeds and steering angles needed to
follow the planned path. This would take the form of a DRIVE command to the 
C1 module over a serial line. The format of the DRIVE command is documented 
in the C1 code.

[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C4 Path planner, 
which passes on data from C6 Navigator
*/

/* TargetLocation struct is used to store the data of each TargetLocation of 
 * the path given by the planner. We create a new struct becuase the 
 * SerialData should only be used to send data.
 */
struct TargetLocation
{
   long int targetSpeedMmps; // millimeters per second
   long int bearing;         // degrees
   long int northPosMm;      // millimeters
   long int eastPosMm;       // millimeters
};

struct Cubic
{
  float a;
  float b;
  float c;
  float d;
};

struct Point
{
  int x;
  int y;
};

struct Instruction
{
  float targetSteeringAngle;
  float targetSpeed;
  float timeMs; //milli seconds
};
////////////////////////////////////////////////////////////////////////////////

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

/* This function converts any angle we are dealing with to be from 0 to 180 and 
 * anything greater than 180 and less than 0 to be represented as a negative 
 * angle. Our circle starts with 0 at the top as true north.
 *             0
 *       -90         90
 *            180
 *            
 */
float UniformAngle(float angle)
{
     while(angle > 180)
     {
        angle -= 360; 
     }
     while(angle < -180)
     {
        angle += 360; 
     }
     return angle;
}

/* Float comparison allows comparison of floats not using the = operator
 * this will return a boolean of the comparison of a and b to the number
 * of decimal places defined by places. 
 */
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


/* This function calculates the angle from the current point to the target 
 * pointin relation to true north.Quadrants are relative to the current 
 * position with y lines pointing true north. four reference quadrants are:
 * 2 1
 * 3 4
 */
float NorthOffset(int currentX, int currentY, int targetX, int targetY)
{
  return (atan2(currentX+targetX,currentY+targetY) * 
           57.2957795);
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

//-----------------------Start Hermite Cubic Functions------------------------//

/*
 * FirstCoefficient calculates the first coefficient of the Hermite cubic 
 * function. This requires input of the Tanget values adjusted for the arc 
 * length. the start pointand end point are the map locations we want the bike 
 * to sit at or start at. This functioncan solve for both the x and y equations.
 */
float FirstCoeffiecent(float endTangent, float endValue, float startValue, 
      float startTangent)
{
  float retVal = (endTangent - (2 * endValue) +(2 * startValue) + startTangent);
  return retVal;
}

/*
 * SecondCoefficient calculates the first coefficient of the Hermite cubic 
 * function This requires input of the Tanget values adjusted for the arc 
 * lenght. the start point and end point are the map locations we want the 
 * bike to sit at or start at. This function can solve for both 
 * the x and y equations.
 */
float SecondCoeffiecent(float firstCoeffiecent, float tangentStartValue, 
      float endValue, float startValue)
{
  float retVal = (-firstCoeffiecent-tangentStartValue-startValue+endValue);
  return retVal;
}

/*
 * CalculateCubic will do all the work needed to calculate the cubic function 
 * it takes in a Cubic by reference and stores the values of a b c d in the 
 * struct. this requres input of The start point, end point, point of the 
 * direction of the end tangent adusted by the arc and direction of the start 
 * point adjusted by the arc.
 */
void CalculateCubic(Cubic& function, float startValue, float endValue,
     float startTangent, float endTangent)
{
  function.a = FirstCoeffiecent(endTangent, endValue, startValue,
      startTangent);
  function.b = SecondCoeffiecent(function.a, startTangent,
      endValue, startValue);
  function.c = startTangent;
  function.d = startValue;
}


/*
 *  Calculate start tangent translates angle of the trike to a corresponding 
 *  point on a line that passes through the origin with the slope and compass
 *  direction representing the same angle. Example
 *  angle 0 degress = 0,1 or angle 90 degrees equals 0,1.
 */
Point CalculateStartTangentPoint(float angleDegrees)
{
  Point retVal;
  retVal.x= sin((angleDegrees * 0.0174533));
  retVal.y = cos((angleDegrees * 0.0174533));
  return retVal;
}

/*
 * This function takes 2 points and translates the slope between the two
 * to a point that is on the equivalent slope line that passes through
 * the origin this allows us to calculate the tangent for the next point.
 */
Point pointSlope(Point a, Point b)
{
    float slope = (a.y - b.y) / (a.x - b.x);
    Point wrtOrigin;
    wrtOrigin.x = 1;
    wrtOrigin.y = slope;
    return wrtOrigin;
}

/*
 * This point translates the tangent point value based on the arc length
 * passed in by the user to allow for different curve profiles which
 * allows for tighter or wider turns which will also change corner speed.
 */
void TangentArcAdjustment(Point &target, float arcLength)
{

    target.x = (target.x/sqrt(sq(target.x) + sq(target.y))) * arcLength;
    target.y = (target.y/sqrt(sq(target.x) + sq(target.y))) * arcLength;
    return;
}

/*
 * This is a basic 3rd degree to 2nd degree derivative function. This
 * will be used with the speed calculation.
 */
float DerivativeValueAtT(Cubic x, float t)
{
  Cubic xPrime;
  float retVal;
  xPrime.a = (x.a*(3*sq(t)));
  xPrime.b = (x.b*(2*t));
  xPrime.c = (x.c);

  retVal = (xPrime.a + xPrime.b + xPrime.c);
  return retVal;
}

/*
 * Calculation of the speed the trike will need to be traveling at
 * time t.
 */
float SpeedAtT(Cubic x,Cubic y, float t)
{
   float yPrime = DerivativeValueAtT(y,t);
   float xPrime = DerivativeValueAtT(x,t);
   return sqrt(sq(xPrime) + sq(yPrime));
}

/*
 * Calculation of the arc length at time t. This allows us to know
 * Where we are by how far we have traveled.
 */
float ArcLength(Cubic x, Cubic y, float t,float deltaT, float current)
{
  if(FloatComparison(t,0.00,2))
  {
    return 0;
  }
  else
  {
    float currentX = ValueAtTime(x,t);
    float currentY = ValueAtTime(y,t);
    float previousX = ValueAtTime(x,(t-deltaT));
    float previousY = ValueAtTime(y,(t-deltaT));
    
    return(current + (sqrt(sq(currentX - previousX)+sq(currentY - previousY))));
  }
}

/*
 * Calculation of the current x or y value at the time value passes in.
 */
 float ValueAtTime(Cubic x, float t)
{
  Cubic cubicAtT;
  float retVal;
  cubicAtT.a = (x.a*(pow(t,3)));
  cubicAtT.b = (x.b*(sq(t)));
  cubicAtT.c = (x.c * t);
  cubicAtT.d = x.d;

  retVal = (cubicAtT.a + cubicAtT.b + cubicAtT.c + cubicAtT.d);
  return retVal;
}

//----------------------End Hermite Cubic Functions---------------------------//

/* Instruction will divide the curve between each waypoint into peices the size 
 * passed in
 */
Instruction* InstructionsForWaypoints(int slices, TargetLocation *allTargetLocations, int numWaypoints)
{
  Serial.println(numWaypoints);
  Instruction lowLevelData [numWaypoints*slices];
  float t = ((float)1 / (float)slices);
  Serial.println(t);
  float currentSpeed;
  float currentTurningAngle = 0;
  float angleDelta;
  int count = 0;
  
  Point startTan;
  Point startPoint;
  Point endPoint;
  Point endTan;
  Point nextPoint;
  Point previousPoint;

  
  for( int i = 0; i <= (numWaypoints-1); i++)
  {
    // Calculate cubic
    if (i <= 0) // case for first point
      {
         startTan = CalculateStartTangentPoint(allTargetLocations[i].bearing);
         startPoint.y = allTargetLocations[i].northPosMm;
         startPoint.x = allTargetLocations[i].eastPosMm;
         endPoint.y = allTargetLocations[i+1].northPosMm;
         endPoint.x = allTargetLocations[i+1].eastPosMm;
         nextPoint.y = allTargetLocations[i+2].northPosMm;
         nextPoint.x = allTargetLocations[i+2].eastPosMm;
         endTan = pointSlope(endPoint,nextPoint);
      }
      else if(i >= numWaypoints-1) //case for last point
      {
         previousPoint.y = allTargetLocations[i-1].northPosMm;
         previousPoint.x = allTargetLocations[i-1].eastPosMm;
         startPoint.y = allTargetLocations[i].northPosMm;
         startPoint.x = allTargetLocations[i].eastPosMm;
         startTan = pointSlope(previousPoint, startPoint);
         endPoint.y = allTargetLocations[i+1].northPosMm;
         endPoint.x = allTargetLocations[i+1].eastPosMm;
         endTan = pointSlope(startPoint,endPoint);
      }
      else // all other points
      {
         previousPoint.y = allTargetLocations[i-1].northPosMm;
         previousPoint.x = allTargetLocations[i-1].eastPosMm;
         startPoint.y = allTargetLocations[i].northPosMm;
         startPoint.x = allTargetLocations[i].eastPosMm;
         startTan = pointSlope(previousPoint, startPoint);
         endPoint.y = allTargetLocations[i+1].northPosMm;
         endPoint.x = allTargetLocations[i+1].eastPosMm;
         nextPoint.y = allTargetLocations[i+2].northPosMm;
         nextPoint.x = allTargetLocations[i+2].eastPosMm;
         endTan = pointSlope(endPoint,nextPoint);
      }
      float arclength = 600;
      TangentArcAdjustment(startTan,arclength);
      TangentArcAdjustment(endTan,arclength);
      
      Cubic segmentYCubic;
      Cubic segmentXCubic;
      CalculateCubic(segmentYCubic, startPoint.y, endPoint.y,
      startTan.y, endTan.y);
      CalculateCubic(segmentXCubic, startPoint.x, endPoint.x,
      startTan.x, endTan.x);
      Serial.print(segmentYCubic.a);
      Serial.print(segmentYCubic.b);
      Serial.print(segmentYCubic.c);
      Serial.print(segmentYCubic.d);
      Serial.println("");
      
    // Slice cubic
    // If we get an angle we cannot achieve we need to throw an error and re calcuate the cubic with
    // a larger arc length
    // we must track what our current steering angle is. also speed limits
    // must be sliced into a time that can be feed out to the the writer
    
    for( float j = 0; j <= 1; j+=t)
    {
       Serial.println(j);
       Instruction temp;
       temp.targetSpeed = SpeedAtT(segmentXCubic, segmentYCubic, j);
       
       //check if valid speed if not throw error go slower.
       currentSpeed = temp.targetSpeed;
       //lowLevelData[count] = temp;

       //calculate angle
       if(j >0){
         //Serial.println(ValueAtTime(segmentXCubic,j-t));
         //Serial.println(ValueAtTime(segmentYCubic,j-t));
         //Serial.println(ValueAtTime(segmentXCubic,j));
         //Serial.println(ValueAtTime(segmentYCubic,j));
         //}
         //Serial.println(NorthOffset(ValueAtTime(segmentXCubic,j-t),ValueAtTime(segmentYCubic,j-t),
         //ValueAtTime(segmentXCubic,j), ValueAtTime(segmentYCubic,j)));
         //Serial.println(lowLevelData[count-1].targetSteeringAngle);
         //}
         //Serial.println(ShortestAngle(40.70,41.26));
         //Serial.println(lowLevelData[count-1].targetSteeringAngle);
         //Serial.println(NorthOffset(ValueAtTime(segmentXCubic,j-t),ValueAtTime(segmentYCubic,j-t),
         //            ValueAtTime(segmentXCubic,j), ValueAtTime(segmentYCubic,j)));
         if(count == 0){
            temp.targetSteeringAngle = ShortestAngle(0,NorthOffset(ValueAtTime(segmentXCubic,j-t),ValueAtTime(segmentYCubic,j-t),
            ValueAtTime(segmentXCubic,j), ValueAtTime(segmentYCubic,j)));
         }
         else{
            temp.targetSteeringAngle = ShortestAngle(lowLevelData[count-1].targetSteeringAngle,NorthOffset(ValueAtTime(segmentXCubic,j-t),ValueAtTime(segmentYCubic,j-t),
            ValueAtTime(segmentXCubic,j), ValueAtTime(segmentYCubic,j)));
         }
       }
       /*
       angleDelta = ShortestAngle(currentTurningAngle,NorthOffset(startPoint.x,startPoint.y,
       endPoint.x, endPoint.y));
       currentTurningAngle += angleDelta;
       temp.targetSteeringAngle = angleDelta;
       */
       lowLevelData[count] = temp;
       count++;
       Serial.println(temp.targetSpeed);
       Serial.println(temp.targetSteeringAngle);
    }
  }
  return lowLevelData;
}

////////////////////////////////////////////////////////////////////////////////

//elcano::
ParseState ps;
//elcano::
SerialData dt;
//elcano::
SerialData sd;
int32_t currentSegment = 0;
int tail = 0;
int head = 0;
int totalSegments = 0;
bool complete;
bool insrtructionsReady = false;
Instruction instructions;
TargetLocation currentLocations[MAX_WAYPOINTS];
TargetLocation previousLocations[MAX_WAYPOINTS];
Instruction dataForLowLevel[MAX_WAYPOINTS * 10];



void SendData()
{
    dt.clear();
    dt.kind = //elcano::MsgType::drive;
    MSG_DRIVE;
    dt.speed_cmPs = dataForLowLevel[head].targetSpeed;
    dt.angle_deg = dataForLowLevel[head].targetSteeringAngle;
    head++;
    dt.write(&Serial2);
}

void setup()
{  
        Serial.begin(9600);
        Serial1.begin(9600); 
        dt.clear();
        ps.dt = &dt;
        ps.dev = &Serial1; 
        Timer1.initialize(100000);
        complete = false;

        /*tests
         */
        TargetLocation temp;
        temp.bearing = 45;
        temp.eastPosMm = 0; 
        temp.northPosMm = 0;
        currentLocations[0] = temp;
        temp.eastPosMm = 100; 
        temp.northPosMm = 100;
        currentLocations[1] = temp;
        temp.eastPosMm = 150; 
        temp.northPosMm = 150;
        currentLocations[2] = temp;
        temp.eastPosMm = 300; 
        temp.northPosMm = 300;
        currentLocations[3] = temp;
        temp.eastPosMm = 400; 
        temp.northPosMm = 400;
        currentLocations[4] = temp;
        totalSegments = 5;
        }

void loop() 
{
    /* get newest map data from C4 planner
     * Using Elcano_Serial.h Using the SerialData struct in the .h file.
     * Receive a TargetLocation from C4. C4 will only ever send TargetLocations 
     * to C3.
     */
   

    //-----------------------C4 input--------------------------//
    TargetLocation target;
    noInterrupts();
    int r = ps.update();
    interrupts();
    if(r == //elcano::ParseStateError::success)
     PSE_SUCCESS)
    {
      if(dt.kind ==  MSG_SEG )
      //elcano::MsgType::seg)
      {
        //we're getting a new map
        if(dt.number == STARTSEG)
        {
           target.bearing = 0; //change this to get data from navigator
           currentSegment = 0;
           totalSegments++;
           complete = false;
        }
        // we dont need to receive any more segments
        else if(dt.number == ENDSEG)
        {
          complete = true;
        }
        // append the segments to the
        else
        {
          target.eastPosMm = 0; 
          target.northPosMm = 0; 
          currentLocations[totalSegments] = target;
          totalSegments++;
        }
      }
      else
      {
        dt.write(&Serial2);
      }
    }
    //Add the data to the array of target points.
    /*
    target.northPosMm = dt.posN_cm;
    target.eastPosMm = dt.posE_cm;
    target.targetSpeedMmps;
    currentLocations[currentSegment] = target;
    */

    //We have enough data to start a new spline so we arent running off old data
    if(totalSegments >= STARTSPLINE && currentSegment <= totalSegments)
    {
      //dataForLowLevel = 
      InstructionsForWaypoints(10, currentLocations, totalSegments);
      tail++;
      insrtructionsReady = true;
    }
    if(insrtructionsReady)
    {
      Timer1.start();
      Timer1.attachInterrupt(SendData,100000);
    }
    else
    {
      Timer1.stop();
    }
}


