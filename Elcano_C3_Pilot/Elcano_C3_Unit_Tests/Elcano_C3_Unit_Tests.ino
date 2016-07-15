
#include <ArduinoUnit.h>
#include <Common.h>
//#include <IO_C3.h>
//#include <IO_Mega.h>
#include <Matrix.h>
#include <Elcano_Serial.h>

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

int PothagarianDistance(int currentX, int currentY, int targetX, int targetY)
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
//**********************************************************************************************
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
//*********************************************************************************************

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
//*************************************************************************************************


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
//**********************************************************************************************

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
  assertEqual(FloatComparison(NorthOffset(0,0,3,4),atan(.75)* 57.2957795,2),true);
  assertEqual(FloatComparison(NorthOffset(0,0,-3,4),atan(-.75)* 57.2957795,2),true);
  assertEqual(FloatComparison(NorthOffset(0,0,3,-4),(180 + atan(-.75)* 57.2957795),2),true);
  assertEqual(FloatComparison(NorthOffset(0,0,-3,-4),(-180 - atan(-.75)* 57.2957795),2),true);
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

////////////////////////////////////////////////////////////////////////////
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
    toLowLevel.angle_deg = steeringAngle;
    toLowLevel.speed_cmPs = speedSetting;
    writeSerial(&Serial1, &toLowLevel);

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
    
    

}
