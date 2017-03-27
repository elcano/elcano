#include <Common.h>
#include <Matrix.h>
#include <ElcanoSerial.h>
#include <SPI.h>

using namespace elcano;
/*
// Elcano Contol Module C3: Pilot.

The Pilot program reads a serial line that specifies the desired path and speed 
of the vehicle. It computes the analog signals to control the throttle, brakes 
and steering and sends these to C2.

Input will be recieved and sent using Elcano_Serial. Inputs will be received from
C5(sensors) and C4(planner). Output is sent to C2(Low Level).

In USARSIM simulation, these could be written on the serial line as
wheel spin speeds and steering angles needed to
follow the planned path. This would take the form of a DRIVE command to the C1 module over a 
serial line. The format of the DRIVE command is documented in the C1 code.

[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C4 Path planner, which passes on
data from C6 Navigator
*/

SerialData serialData;
ParseState parseState;


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
  double x;
  double y;
};

struct Turn
{
  float distance_mm;
  float steeringAngle_deg;
};

class Cubic2D
{
public:  
  /* Prefered constructor: 
   * takes start point (start position of the trike) and end point (final position)
   * start bearing and end bearing (compass directions
   * and arc adjust (this value makes the turn wider, a "less straight" line
   */
  Cubic2D(Point start, Point end, float startBearing, float endBearing, float arcAdjust)
  {
    Point startTanPoint = CalculateStartTangentPoint(startBearing);
    Point endTanPoint   = CalculateStartTangentPoint(endBearing);

    endTanPoint = TangentArcAdjustment(endTanPoint, arcAdjust);
    
    CalculateCubic(x, start.x, end.x, startTanPoint.x, endTanPoint.x);
    CalculateCubic(y, start.y, end.y, startTanPoint.y, endTanPoint.y);
  }

  // returns an array of turns of length 100
  void getTurns(Turn turns[100])
  {
    for(int i = 0; i < 100; i++)
    {
      float bearing1 = bearingValueAtT(i/100.);
      float bearing2 = bearingValueAtT((i+1)/100.);
      turns[i].steeringAngle_deg = bearing2 - bearing1;

      while(turns[i].steeringAngle_deg < -60) 
      {
        turns[i].steeringAngle_deg += 360;
      }

      while(turns[i].steeringAngle_deg > 60) 
      {
        turns[i].steeringAngle_deg -= 360;
      }
      
      turns[i].distance_mm = sqrt(sq(valueAtTime((i+1)/100.).x - valueAtTime(i/100.).x) + sq(valueAtTime((i+1)/100.).y - valueAtTime(i/100.).y));
    }
    turns[99].steeringAngle_deg = 0;
  }

  /*
   * Calculation of the current x or y value at the time value passes in.
   */
  Point valueAtTime(float t)
  {
    Point retVal;
    retVal.x = ValueAtTime(x, t);
    retVal.y = ValueAtTime(y, t);
    return retVal;
  }

  /*
   * This is a basic 3rd degree to 2nd degree derivative function. This
   * will be used with the speed calculation.
   */
  float bearingValueAtT(float t)
  {
        Point a = valueAtTime(t);
    Point b = valueAtTime(t+.001);
    Point change;
    change.x = b.x - a.x;
    change.y = b.y - a.y;
    if(change.x == 0)
    {
      if(change.y > 0) return 0;
      else return 180;
    }
    if(change.x >= 0)
    {
      return 90 - (180/PI)*atan(change.y/change.x);
    }
    else return 270 - (180/PI)*atan(change.y/change.x);
  }
  /*
   * Calculation of the speed the trike will need to be traveling at
   * time t.
   */
  float speedAtT(float t)
  {
     float yPrime = DerivativeValueAtT(y,t);
     float xPrime = DerivativeValueAtT(x,t);
     return sqrt(sq(xPrime) + sq(yPrime));
  }

  /*
   * Calculation of the arc length at time t. This allows us to know
   * Where we are by how far we have traveled.
   */
  float ArcLength(float t,float deltaT, float current)
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

  
private:  
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
  

  /*
 * FirstCoefficient calculates the first coefficient of the Hermite cubic 
 * function. This requires input of the Tanget values adjusted for the arc 
 * length. the start point and end point are the map locations we want the bike 
 * to sit at or start at. This functioncan solve for both the x and y equations.
 */
  float FirstCoeffiecent(float endTangent, float endValue, float startValue, 
        float startTangent)
{
  float retVal = (endTangent - (2 * endValue) +(2 * startValue) + startTangent);
  return retVal;
}

  /*
 * SecondCoefficient calculates the second coefficient of the Hermite cubic 
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
 *  angle 0 degress = 1,0 or angle 90 degrees equals 0,1.
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
  
  Cubic x;
  Cubic y;
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
//    readSerial(&Serial1,&dataRead);
    // bad number there is no more data or end of data
    ParseStateError r = parseState.update();
    if(r == ParseStateError::success)
    {
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
   }
   return true;
}


void noCompasTurn(int degrees)
{
  // Turning diameter = 2 * (Wb / sine(A)) 
  // Wb = 876 mm
  // Distance for n degree turn = (n * diameter * pi) / (360)
  
  int speed_cmPs = 500;
  int wheelAngle_deg = 30;  // use settings.h
  int wb_mm = 876;          // use settings.h
  if(degrees < 0)
  {
    wheelAngle_deg *= -1;
  }

  float turnDiameter_mm = 2*(wb_mm/abs(sin(wheelAngle_deg)));
  float distance_mm = (degrees * turnDiameter_mm * PI) / 360;
  
  int time_ms = ((speed_cmPs * 10) / distance_mm) * 1000;
  SerialData command;
  command.kind = MsgType::drive;
//    command.speed_cmPs = speed_cmPs;
  command.speed_cmPs = 500;
  command.angle_mDeg = wheelAngle_deg * 1000;
  
  command.write(&Serial1);
  delay(time_ms);
  command.speed_cmPs = 0;
  command.angle_mDeg = 0;
  command.write(&Serial1);
    
}

// turn a number of degrees. Positive number for left, negative for right
void turn(int turnAmount)
{
    SerialData command;
    command.kind = MsgType::drive;
    while(true) // wait until information on the bearing is recieved by C6
    {
      ParseStateError r = parseState.update();
      if(r == ParseStateError::success)
      {
        Serial1.end();
        Serial1.begin(baudrate);  // clear the buffer
        break;
      }
      Serial.println("waiting for comms");
    }

    int initialBearing = serialData.bearing_deg;
 
    // send a slow speed to C2 and either a left or a right turn
    command.speed_cmPs = 500;
   
    if(turnAmount < 0) 
    {
      command.angle_mDeg = -30 * 1000;
    }
    else
    {
      command.angle_mDeg = 30 * 1000;
    }
    command.write(&Serial1); // send command



    int currentBearing = 0;
    int oldBearing = 0;
    
    // while direction not met
    while(currentBearing < turnAmount)
    {
      command.write(&Serial1); // send command
      ParseStateError r = parseState.update();
      if(r == ParseStateError::success)
      {
        currentBearing = abs((serialData.bearing_deg - initialBearing));
        if(currentBearing > (360-turnAmount) 
        && oldBearing < turnAmount 
        && (initialBearing <= turnAmount 
        || initialBearing >= (360 - turnAmount)))
        {
          currentBearing %= 180; 
        }
      }
      oldBearing = currentBearing;
      Serial.println("keep turning");
    }
    Serial.println("done");
    // send command to stop
    command.speed_cmPs = 0;
    command.angle_mDeg = 0;
    command.write(&Serial1);

}

void squareRoutine(){
  long length_mm = 2000;     // default value
  long speed_mms = 5000;      // default value
  for(int i = 0; i < 4; i++){
    moveFixedDistance(length_mm, speed_mms);
    turn(90);
  }
}

/*-----------------------------------moveFixedDistance------------------------------------*/
void moveFixedDistance(long length_mm, long speed_mms)
{ 
  while(true)
  {
    ParseStateError r = parseState.update();
    if(r == ParseStateError::success)
    {
      serialData.posE_cm /= 10000; // avoid overflow
      serialData.posN_cm /= 10000;
      break;
    }
    Serial.println("waiting for initial location: " + String(static_cast<int8_t>(r)));
  }
  double initialDistance_cm = sqrt(abs(abs(serialData.posE_cm) * abs(serialData.posE_cm) + abs(serialData.posN_cm) * abs(serialData.posN_cm)));
  Serial.println(initialDistance_cm);
  serialData.clear();
  serialData.kind = MsgType::drive;
  serialData.speed_cmPs = speed_mms/10; // The initial speed to send.
  serialData.angle_mDeg = 0; // what should this actually be
  serialData.write(&Serial1);

  double currentDistance_cm = 0;
  double length_cm = length_mm / 10;
  while(currentDistance_cm < length_cm + initialDistance_cm){
    ParseStateError r = parseState.update();
    if(r == ParseStateError::success){
      serialData.posE_cm /= 10000;
      serialData.posN_cm /= 10000;
      double currentLocation_cm = sqrt(abs(abs(serialData.posE_cm) * abs(serialData.posE_cm) + abs(serialData.posN_cm) * abs(serialData.posN_cm)));
      double delta_cm = abs(currentLocation_cm - initialDistance_cm);
      currentDistance_cm += delta_cm;
      Serial.println("Current: " + String(currentDistance_cm));
      serialData.write(&Serial1);

    }    
  }
  Serial.println("Done moving fixed distance");
  serialData.kind = MsgType::drive;
  serialData.speed_cmPs = 0;
  serialData.angle_mDeg = 0;
  serialData.write(&Serial1);
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


////////////////////////////////////////////////////////////////////////////////
void setup() 
{  

  Serial.begin(9600);
  Serial1.begin(baudrate);
  /* 
   *  this for loop is so that there is time between uploading
   *  the program and the program moving the trike (for debugging purposes)
   */
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  for(int i = 5; i >= 0; i--)
  {
    Serial.println(i);
    digitalWrite(13,HIGH); 
    delay(200);
    digitalWrite(13,LOW); 
    delay(800);
  }
  
  parseState.dt       = &serialData;
  parseState.input    = &Serial1;
  parseState.output   = &Serial1;
  parseState.capture = MsgType::sensor | MsgType::seg;
  serialData.clear();
  pinMode(8,OUTPUT);
  
  long length_mm = 2000;     // default value
  long speed_mms = 5000;
//  moveFixedDistance(length_mm, speed_mms);
//  noCompasTurn(90);
  go20mNorth();
}

void go20mNorth()
{
  Point start, end;
  int startBearing;
//  while(true)
//  {
//    ParseStateError r = parseState.update();
//    if(r == ParseStateError::success)
//    {
//      start.x = serialData.posE_cm/100.;
//      start.y = serialData.posN_cm/100.;
//      startBearing = serialData.bearing_deg;
//      break;
//    }
//    Serial.println("waiting");
//  }
  start.x = 0;
  start.y = 0;
  
  end.x = start.x + 2;
  end.y = start.y + 2;
  //start to end, starting bearing, end pointing souch, 10 for arc length modifier (arbitrarily chosen)
  Cubic2D path(start, end, 0, 90, 10); 
  Turn turns[100];
  path.getTurns(turns);
  for(int i = 0; i < 100; i++)
  {
    SerialData command;
    command.kind = MsgType::drive;
    command.speed_cmPs = 0; // The initial speed to send.
    command.angle_mDeg = turns[i].steeringAngle_deg * -1 * 5 * 1000; // the 5 is to exadurate it for more visual results
    Serial.println(command.angle_mDeg);
    command.write(&Serial1);
//    Serial.println(String(turns[i].distance_mm) + ", " + String(turns[i].steeringAngle_deg));
    delay(500);
    
  }

//  for(int i = 0; i < 100; i++)
//  {
//    Serial.println(String(path.valueAtTime(i/100.).x) + ", " + String(path.valueAtTime(i/100.).y));
//  }
}

void loop() 
{

  //-----------------------Input from C6--------------------//
  ParseStateError r = parseState.update();
  if(r == ParseStateError::success)
  {
//    Serial.print(serialData.posN_cm);
//    Serial.print("\t");
//    Serial.println(serialData.posE_cm);
  }

  //-----------------------Output to C2-----------------------//
  serialData.kind = MsgType::drive;
  serialData.angle_mDeg = 0;
  serialData.speed_cmPs = 0;
  serialData.write(&Serial1);



}
