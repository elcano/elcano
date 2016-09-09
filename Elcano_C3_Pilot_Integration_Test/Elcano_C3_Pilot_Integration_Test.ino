
#include <Common.h>
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

// TargetLocation struct is used to store the data of each TargetLocation of the path given by the planner.
// We create a new struct becuase the SerialData should only be used to send data.
struct TargetLocation
{
   long int targetSpeed;
   long int bearing;
   long int northPos;
   long int eastPos;
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
  float x;
  float y;
};
////////////////////////////////////////////////////////////////////////////////
/* Process segement assures that we received a valid TargetLocation and not 
 * noise. It then stores the data in another struct that holds similar data. 
 * This is done for loose coupling. If we need to change the point data stored 
 * locally, we don't need to try to change the elcano serial file.
 */
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
    // bad number there is no more data or end of data
    if(dataRead.number >= 789 || count >= MAX_WAYPOINTS)
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

/* This function will rotate the bike to the desired angle. 
 * This includes calculation of the difference in its current heading and the 
 * target angle. Low level commands will be sent to C2 low level controller. 
 */
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
  if(ShortestAngle(targetAngle, currentHeading))
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
Point TangentArcAdjustment(Point target, float arcLength)
{
    Point retVal;
    retVal.x = (target.x/sqrt(sq(target.x) + sq(target.y))) * arcLength;
    retVal.y = (target.y/sqrt(sq(target.x) + sq(target.y))) * arcLength;
    return retVal;
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

////////////////////////////////////////////////////////////////////////////////
void setup() 
{  
        Serial1.begin(9600); 
        //Serial2.begin(9600);
        //Serial3.begin(9600); 
        pinMode(8,OUTPUT);
}

void loop() 
{
  /*
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
    //Drive()
    //{
      
    //}

   SerialData testData;
   testData.kind = 1;
   testData.angle_deg = 20;
   testData.speed_cmPs = 10;
   writeSerial(&Serial,&testData);

}


