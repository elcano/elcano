
#include <ArduinoUnit.h>
#include <Common.h>
#include <Matrix.h>
#include <Elcano_Serial.h>

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
}

void DrivePath(int &northDist, int &eastDist)
{
  
}

// Unit Test of ProcessTargetLocation that tests if the it can throw out bad data
// and return false and can accurately load the values into the new struct.
test(ProcessTargetLocation)
{
  SerialData TargetLocationTest;
  TargetLocation newTargetLocation;
  //failing test
  assertFalse(ProcessTargetLocation(&newTargetLocation,TargetLocationTest));
  //passing test
  TargetLocationTest.posE_cm = 50;
  TargetLocationTest.posN_cm = 40;
  TargetLocationTest.bearing_deg = 35;
  TargetLocationTest.speed_cmPs = 100;
  ProcessTargetLocation(&newTargetLocation,TargetLocationTest);
  assertEqual(newTargetLocation.bearing, 35);
  assertEqual(newTargetLocation.targetSpeed,100);
  assertEqual(newTargetLocation.northPos,40);
  assertEqual(newTargetLocation.eastPos,50);
}

//slightly modified for testing
bool ReadWaypoints(TargetLocation* TargetLocationArray, SerialData* testData)
{
  //set up variables
  int count = 0;
  SerialData dataRead;
  TargetLocation currentTargetLocation;
  // loop until we hit the end of the data
   while(true)
   {
    //check if we're done receiveing
    dataRead = testData[count];
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

int PothagarianDistance(float currentX, float currentY, float targetX, float targetY)
{
    return sqrt(sq(abs(currentX - targetX)) + sq(abs(currentY - targetY)));
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
  return (atan2(currentX+targetX,currentY+targetY) * 
           57.2957795);
}

/* The Float Comparison function allows you to compare floats to any X number 
 * of decimal places. This allows us to compare two floats without errors
 * that = comparison will give.
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
//******************************************************************************
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
//******************************************************************************

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
//******************************************************************************


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

//******************************************************************************

float FirstCoeffiecent(float endTangent, float endValue, float startValue, float startTangent)
{
  float retVal = (endTangent - (2 * endValue) +(2 * startValue) + startTangent);
  return retVal;
}

//******************************************************************************

float SecondCoeffiecent(float firstCoeffiecent, float tangentStartValue, float endValue, float startValue)
{
  float retVal = (-firstCoeffiecent-tangentStartValue-startValue+endValue);
  return retVal;
}

//******************************************************************************

void CalculateCubic(Cubic& function, float startValue, float endValue, float startTangent, float endTangent)
{
  function.a = FirstCoeffiecent(endTangent, endValue, startValue, startTangent);
  function.b = SecondCoeffiecent(function.a, startTangent, endValue, startValue);
  function.c = startTangent;
  function.d = startValue;
}

//******************************************************************************

Point CalculateStartTangentPoint(float angleDegrees)
{
  Point retVal;
  retVal.x= sin((angleDegrees * 0.0174533));
  retVal.y = cos((angleDegrees * 0.0174533));
  return retVal;
}

//******************************************************************************

Point pointSlope(Point a, Point b)
{
    float slope = (a.y - b.y) / (a.x - b.x);
    Point wrtOrigin;
    wrtOrigin.x = 1;
    wrtOrigin.y = slope;
    return wrtOrigin;
}
//******************************************************************************

Point TangentArcAdjustment(Point target, float arcLength)
{
    Point retVal;
    retVal.x = (target.x/sqrt(sq(target.x) + sq(target.y))) * arcLength;
    retVal.y = (target.y/sqrt(sq(target.x) + sq(target.y))) * arcLength;
    return retVal;
}

//******************************************************************************

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

//******************************************************************************

float SpeedAtT(Cubic x,Cubic y, float t)
{
   float yPrime = DerivativeValueAtT(y,t);
   float xPrime = DerivativeValueAtT(x,t);
   return sqrt(sq(xPrime) + sq(yPrime));
}

//******************************************************************************

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
//******************************************************************************
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

//******************************************************************************

// This test creates an array of data to try instead of a serial line so the code is slightly
// modified we create 3 signals to send and test if valid signals will be received and terminated
// When apporopriate. We also test if the first element is end data we will stop at that element.
test(ReadTargetLocations)
{
  TargetLocation allTargetLocations[MAX_WAYPOINTS];
  SerialData TargetLocationTest;
  TargetLocationTest.number = 1;
  TargetLocationTest.posE_cm = 50;
  TargetLocationTest.posN_cm = 40;
  TargetLocationTest.bearing_deg = 35;
  TargetLocationTest.speed_cmPs = 100;
  SerialData TargetLocationTest2;
  TargetLocationTest2.number = 1;
  TargetLocationTest2.posE_cm = 50;
  TargetLocationTest2.posN_cm = 40;
  TargetLocationTest2.bearing_deg = 35;
  TargetLocationTest2.speed_cmPs = 100;
  SerialData TargetLocationTest3;
  TargetLocationTest3.posE_cm = 50;
  TargetLocationTest3.posN_cm = 40;
  TargetLocationTest3.bearing_deg = 35;
  TargetLocationTest3.speed_cmPs = 100;
  TargetLocationTest3.number = 789;
  SerialData testData [3];
  testData [0] = TargetLocationTest;
  testData [1] = TargetLocationTest2;
  testData [2] = TargetLocationTest3;
  assertTrue(ReadWaypoints(allTargetLocations,testData));
  testData [0] = TargetLocationTest3;
  assertFalse(ReadWaypoints(allTargetLocations,testData));
}

test(Hypotenuse)
{
  assertEqual(PothagarianDistance(0,0,3,4),5);
  assertEqual(PothagarianDistance(3,4,0,0),5);
}

test(NorthOffset)
{
  assertEqual(FloatComparison(NorthOffset(0,0,3,4),atan(.75)* 57.2957795,1),true);
  assertEqual(FloatComparison(NorthOffset(0,0,-3,4),atan(-.75)* 57.2957795,1),true);
  assertEqual(FloatComparison(NorthOffset(0,0,3,-4),(180 + atan(-.75)* 57.2957795),1),true);
  assertEqual(FloatComparison(NorthOffset(0,0,-3,-4),(-180 - atan(-.75)* 57.2957795),1),true);
}

test(FloatComparison)
{
  assertEqual(FloatComparison(3.41,3.46,1),true);
  assertNotEqual(FloatComparison(3.41,3.46,2),true);
  assertEqual(FloatComparison(-143.13,-143.13,1),true);
}

test(AngleUniformity)
{
  assertEqual(UniformAngle(-182.0),178.0);
  assertNotEqual(UniformAngle(-182.0),177.0);
  assertEqual(UniformAngle(340.0),-20.0);
}

test(CurrentSmallestAngle)
{
  assertEqual(ShortestAngle(1.0,15.0),14.0);
  assertEqual(ShortestAngle(15.0,1.0),-14.0);
  assertEqual(ShortestAngle(0.0,10.0),10.0);
  assertEqual(ShortestAngle(10.0,0.0),-10.0);
  assertEqual(ShortestAngle(-35.0,-8.0),27.0);
  assertEqual(ShortestAngle(-8.0,-35.0),-27.0);
  assertEqual(ShortestAngle(0.0,-7.0),-7.0);
  assertEqual(ShortestAngle(-7.0,0.0),7.0);
  assertEqual(ShortestAngle(179.0,-175.0),6.0)
  assertEqual(ShortestAngle(4.0,-4.0),-8.0)
  assertEqual(ShortestAngle(170.0,-170.0),20.0)
  assertEqual(ShortestAngle(-179.0,175.0),-6.0)
  assertEqual(ShortestAngle(-170.0,170.0),-20.0)
  assertEqual(ShortestAngle(-1.0,5.0),6.0)
  assertEqual(ShortestAngle(-90.0,90.0),180.0)
  assertEqual(ShortestAngle(90.0,-90.0),-180.0)
}

test(BasicRangeTest)
{
  assertEqual(ValidRange(5,5,3,3,10),true);
  assertEqual(ValidRange(5,5,3,3,1),false);
}

test(CurrentFirstCoeffiecentTest)
{
  assertTrue(FloatComparison (FirstCoeffiecent(466.69, 80,550,0),1406.6,1));
  assertTrue(FloatComparison (FirstCoeffiecent(-466.69, 200,25,660),-156.69,2));
}

test(CurrentSecondCoeffiecentTest)
{
  assertTrue(FloatComparison (SecondCoeffiecent(1406.69,0,550,80),-1876.69,2));
  assertTrue(FloatComparison (SecondCoeffiecent(-156.69,660,25,200),-328.31,2));
}

test(CalculateCubic)
{
  Cubic testCubic;
  CalculateCubic(testCubic,550,80, 0, 466.69);
  assertTrue(FloatComparison(testCubic.a,1406.69,1));
  assertTrue(FloatComparison(testCubic.b,-1876.6,1));
  assertTrue(FloatComparison(testCubic.c,0,1));
  assertTrue(FloatComparison(testCubic.d,550,1));
}

test(CurrentStartPoint)
{
  Point startPoint = CalculateStartTangentPoint(135);
  assertTrue(FloatComparison(startPoint.x,.707,3));
  assertTrue(FloatComparison(startPoint.y,-.707,3));
  startPoint = CalculateStartTangentPoint(0);
  assertTrue(FloatComparison(startPoint.x,0,3));
  assertTrue(FloatComparison(startPoint.y,1,3));
}

test(CurrentArcAdjustment)
{
  Point testPoint;
  testPoint.x = 1;
  testPoint.y = -1;
  Point returnPoint;
  returnPoint = TangentArcAdjustment(testPoint,660);
  assertTrue(FloatComparison(returnPoint.x, 466.6,1));
  assertTrue(FloatComparison(returnPoint.y, -466.6,1));
  testPoint.x = 0;
  testPoint.y = 1;
  returnPoint = TangentArcAdjustment(testPoint,660);
  assertTrue(FloatComparison(returnPoint.x, 0,1));
  assertTrue(FloatComparison(returnPoint.y, 660,1));
  
}

test(CurrentPointSlope)
{
    Point P_1; 
    P_1.x = 10;
    P_1.y = 10;
    Point P_2;
    P_2.x = 11;
    P_2.y = 9;
    Point R_1 = pointSlope(P_1, P_2);
    assertEqual((R_1.y/R_1.x),-1);
}

test(CurrentDerivative)
{
  Cubic test;
  test.a = 1406.69;
  test.b = -1876.69;
  test.c = 0;
  assertTrue(FloatComparison(DerivativeValueAtT(test,.02),-73.38,2));
  assertTrue(FloatComparison(DerivativeValueAtT(test,.04),-143.38,2));
  test.a = 156.69;
  test.b = -328.31;
  test.c = 660;
  assertTrue(FloatComparison(DerivativeValueAtT(test,.02),646.68,2));
}

test(CurrentSpeed)
{
  Cubic testX;
  testX.a = 1406.69;
  testX.b = -1876.69;
  testX.c = 0;
  Cubic testY;
  testY.a = 156.69;
  testY.b = -328.31;
  testY.c = 660;
  assertTrue(FloatComparison(SpeedAtT(testX, testY, .02),650.83,2));
}

test(CurrentValueAtT)
{
  Cubic testX;
  testX.a = 1406.69;
  testX.b = -1876.69;
  testX.c = 0;
  testX.d = 550;
  Cubic testY;
  testY.a = 156.69;
  testY.b = -328.31;
  testY.c = 660;
  testY.d = 25;
  assertTrue(FloatComparison(ValueAtTime(testX,.02),549.26,1))
  assertTrue(FloatComparison(ValueAtTime(testY,.02),38.067,1))
}

test(CurrentArcLength)
{
  Cubic testX;
  testX.a = 1406.69;
  testX.b = -1876.69;
  testX.c = 0;
  testX.d = 550;
  Cubic testY;
  testY.a = 156.69;
  testY.b = -328.31;
  testY.c = 660;
  testY.d = 25;
  assertTrue(FloatComparison(ArcLength(testX,testY,.02,.02,0),13.0,1));
}

////////////////////////////////////////////////////////////////////////////////
void setup() 
{  
   Serial.begin(9600);
   //Serial1.begin(9600);     
}

void loop() 
{
      //Test of process TargetLocation
      Test::run();



  /*
    int steeringAngle = 35;
    int speedSetting = 300;
    TargetLocation allTargetLocations[100];
    // get newest map data from C4 planner
    // Using Elcano_Serial.h Using the SerialData struct in the .h file.
    // Receive a TargetLocation from C4. C4 will only ever send TargetLocations to C3.


    //-----------------------C4 input--------------------------//
    SerialData instructions;
    readSerial(&Serial1, &instructions);
    TargetLocation currentTargetLocation;
    ProcessTargetLocation(&currentTargetLocation,instructions);

    //Test of input from C4.
    //Serial.println("test");
    //Serial.println(instructions.kind);
  
    //-----------------------C5 input-------------------------//
    SerialData sensorData;
    readSerial(&Serial2, &sensorData);
    

    //---------------------C2 output-------------------------------//

    //Send data to low level.
    SerialData toLowLevel;
    toLowLevel.kind = MSG_DRIVE;
    toLowLevel.angle_mDeg = steeringAngle;
    toLowLevel.speed_cmPs = speedSetting;
    toLowLevel.write(&Serial1);

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
     sensorData.angle_mDeg = 12;
     sensorData.posE_cm = 50;
     sensorData.posN_cm = 50;
     sensorData.bearing_deg = 15;
    */

    //Test Data for C2 drive output. Example drive commands.
    /*
    toLowLevel.kind = 1;
    toLowLevel.speed_cmPs = 400;
    toLowLevel.angle_mDeg = 35;
     */
    
    

}
