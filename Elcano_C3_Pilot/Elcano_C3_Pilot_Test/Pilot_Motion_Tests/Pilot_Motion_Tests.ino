
#include <Common.h>
#include <Matrix.h>
#include <Elcano_Serial.h>
#include <SPI.h>

/*
   Elcano Contol Module C3: Pilot Motion Tests.
   This is verson of the Pilot that has the loop replaced with varisous
   test cases. This is to be used for testing of communication to the low level.
   data from C5 and C4 are simulated.
*/

struct TargetLocation
{
   long int targetSpeed;
   long int bearing;
   long int northPos;
   long int eastPos;
};

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
        pinMode(8,OUTPUT);
}

void loop() 
{
    //Most basic test of output to the low level
    SerialData lowLevelData;
    lowLevelData.angle_deg = 15;
    lowLevelData.speed_cmPs = 120;
    writeSerial(&Serial1 ,&lowLevelData);

    //Basic test to see if the bike can rotate to an angle
    

}




