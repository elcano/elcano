#include "IO.h"
#include "Arduino.h"
#include "Common.h"

// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
char dataString[BUFFSIZ];
volatile bool DataAvailable;

/*---------------------------------------------------------------------------------------*/ 
bool checksum(char* msg)
// Assume that message starts with $ and ends with *
{
    int sum = 0;
    if(msg[0] != '$')
      return false;
    for (int i = 1; i < BUFFSIZ-4; i++)
    {
      if(msg[i] == '*')
      {
         int msb = (sum>>4);
         msg[i+1] = msb>9? 'A'+msb-10 : '0'+msb;
         int lsb = (sum&0x0F);
         msg[i+2] = lsb>9? 'A'+lsb-10 : '0'+lsb;
 //      msg[i+3] = '\n';  // rely on Serial.println()
         return true;
      }
      sum ^= msg[i];
    }
    return false;
}
/*---------------------------------------------------------------------------------------*/ 
//---------------------------------------------------------
long int parsedecimal(char *str) 
{
  long int d = 0;
  
  while (str[0] != 0)
  {
   if ((str[0] > '9') || (str[0] < '0'))
     return d;
   d *= 10;
   d += str[0] - '0';
   str++;
  }
  return d;
}
//---------------------------------------------------------
//  read a number of form  123.456,
// On entry, str points to the 1 or -
// On return, it points to the next charater beyond the comma.
// whole number can have any number of digits.
// fraction must be three digits.
// If there is no decimal point, returned value is an integer.
// If there is a decimal point, returned value is scaled.
long int ReadDecimal(char *str)
{
  long whole;
  bool negative = false;
  if (str[0] == ',')
  {
    str++;   // a blank field means no data
    return INVALID;
  }
  if (str[0] == '-')
  {
    negative = true;
    str++;
  }
  whole = parsedecimal(str);
  if (str[0] == '.')
  {
    str++;  // skip decimal point  
    whole *= 1000;
    whole += parsedecimal(str);
  }
  if (str[0] = ',')
    str++;  
  return negative ? -whole : whole;
  
}


void DataReady()  // called from an interrupt
{
  DataAvailable = true;
}
//---------------------------------------------------------
void writeline(int channel)
{
      switch(channel)
      {
      case 1:
         Serial1.println(buffer);
         break;
       case 2:
         Serial2.println(buffer);
         break;
      case 3:
         Serial3.println(buffer);
         break;
      default:
         Serial.println(buffer);
         break;
      }

}
//---------------------------------------------------------
// return true if a line was read; false if not
bool readline(int channel) 
{
  // buffer can hold 128 bytes; if not enough there yet, try later.
  const int MinimumMessage = 14;
  char c;
  char buffidx;                // an indexer into the buffer
  int Available;
  /* DataAvailable is a flag set in response to an interrpt.
  After the data has been sent, the sending processor toggles the DATA_READY line.
  The receiving computer gets interrupted by this signal and sets DataAvailable.
  */
  if (!DataAvailable)
    return false;
  
  buffidx = 0; // start at begining
//  if (Serial3.available() < MinimumMessage)
//    return false;
  while (1) 
  {
      switch(channel)
      {
      case 1:
         Available = Serial1.available();     
         c=Serial1.read();
         break;
       case 2:
         Available = Serial2.available();     
         c=Serial2.read();
         break;
      case 3:
         Available = Serial3.available();     
         c=Serial3.read();
         break;
      default:
         Available = Serial.available();     
         c=Serial.read();
         break;
      }
      if (buffidx == 0 && Available < MinimumMessage)
        return false;
      if (c == -1)
        continue;
      if (c == '\n')
        continue;
      if ((buffidx == BUFFSIZ-1) || (c == '\r')) 
      {
        buffer[buffidx] = 0;
        DataAvailable = false;
        return true;
      }
      buffer[buffidx++]= c;
  }
}

/* There are more accurate ways to compute distance between two latitude and longitude points.
   We use a simple approximation, since we are interesed in a flat projection over a small area.
   Curvature of the earth is not significant.
*/
void waypoint::Compute_mm()
{
    // compute relative to origin, since Arduino double is limited to 6 digits.
    long diff;
    double relative;
    diff = longitude - LONGITUDE_ORIGIN;
    relative = ((double) diff)/1000000. * TO_RADIANS * COS_LAT;
    relative *= EARTH_RADIUS_MM;
    east_mm = relative;
    diff = latitude - LATITUDE_ORIGIN;
    relative = ((double) diff)/1000000. * TO_RADIANS * EARTH_RADIUS_MM;
    north_mm = relative;
}
void waypoint::Compute_LatLon()
{
    long diff;
    double relative;
    relative = ((double)east_mm) * (1000000 / (EARTH_RADIUS_MM * TO_RADIANS * COS_LAT));  
    diff = relative;
    longitude = diff + LONGITUDE_ORIGIN;
    relative = ((double)north_mm) * (1000000 / (EARTH_RADIUS_MM * TO_RADIANS));  
    diff = relative;
    latitude = diff +  LATITUDE_ORIGIN;
}
//---------------------------------------------------------
/*
   Estimated state (index=-1)  or Waypoint, or list of next waypoints on route (index > 0); 
      $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<Evector_x1000>,<Nvector_x1000>,<index>*CKSUM
      // at leat 18 characters
*/

char* waypoint::formPointString()
// function uses a global dataString buffer; thus a second call to formDataString
// may overwrite results from the previous call.
{
  // now log the information
  // make a string for assembling the data to log:
  long eastFraction, northFraction, speedFraction;
  eastFraction =  east_mm  >= 0? east_mm%1000    : (-east_mm)%1000;
  northFraction = north_mm >= 0? north_mm%1000   : (-north_mm)%1000;
  speedFraction = speed_mmPs>=0? speed_mmPs%1000 : (-speed_mmPs)%1000;
  sprintf(dataString, 
  "$POINT,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%d,%d,%d*\r\n",
  east_mm/1000, eastFraction, north_mm/1000, northFraction, 
  sigma_mm/1000, sigma_mm%1000, time_ms/1000, time_ms%1000,
  speed_mmPs/1000, speed_mmPs%1000, Evector_x1000, Nvector_x1000, index);  
 
  return dataString;
}
bool waypoint::readPointString(unsigned long max_wait_ms, int channel)
{
  char* parsePtr;
  unsigned long end_time = millis() + max_wait_ms;
  while (millis() < end_time)
  {
    if (readline(channel) && strncmp(buffer, "$POINT",6) == 0) 
    {
     //  $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<Evector_x1000>,<Nvector_x1000>,<index>*CKSUM
     parsePtr = buffer+7;
     east_mm =  ReadDecimal(parsePtr);
     north_mm = ReadDecimal(parsePtr);
     sigma_mm = ReadDecimal(parsePtr);
     time_ms  = ReadDecimal(parsePtr);
     speed_mmPs=ReadDecimal(parsePtr);
     Evector_x1000 =  ReadDecimal(parsePtr);
     Nvector_x1000 =  ReadDecimal(parsePtr);
     index    = ReadDecimal(parsePtr);
     return true;
    }
  }
  return false;
}
void waypoint::operator=(waypoint& right)
{
  latitude =   right.latitude;
  longitude =  right.longitude;
  east_mm =    right.east_mm;
  north_mm =   right.north_mm;
  sigma_mm =   right.sigma_mm;
  time_ms =    right.time_ms;  
  Evector_x1000  =   right.Evector_x1000;
  Nvector_x1000  =   right.Nvector_x1000;
  speed_mmPs = right.speed_mmPs;
  index    =   right.index;
  return;
}
void waypoint::operator=(waypoint* right)
{
  latitude =   right->latitude;
  longitude =  right->longitude;
  east_mm =    right->east_mm;
  north_mm =   right->north_mm;
  sigma_mm =   right->sigma_mm;
  time_ms =    right->time_ms;  
  Evector_x1000  =   right->Evector_x1000;
  Nvector_x1000  =   right->Nvector_x1000;
  speed_mmPs = right->speed_mmPs;
  index    =   right->index;
  return;
}
long  waypoint::distance_mm(waypoint *other)
{
  double deltaX, deltaY;
  deltaX = east_mm - other->east_mm;
  deltaY = north_mm - other->north_mm;
  return sqrt(deltaX*deltaX + deltaY*deltaY);
}
void  waypoint::vectors(waypoint *other)
{
  double deltaX, deltaY, dist;
  deltaX = -east_mm + other->east_mm;
  deltaY = -north_mm + other->north_mm;
  dist = sqrt(deltaX*deltaX + deltaY*deltaY);
  Evector_x1000 = (deltaX * 1000.) / dist;
  Nvector_x1000 = (deltaY * 1000.) / dist;
}
long  waypoint::distance_mm(long East_mm, long North_mm)
{
  double deltaX, deltaY;
  deltaX = East_mm - east_mm;
  deltaY = North_mm - north_mm;
  return sqrt(deltaX*deltaX + deltaY*deltaY);
}

