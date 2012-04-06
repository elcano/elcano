#include "Arduino.h"
#include "GPS.h"
#define REAL double
// Use pin 4 to control power to the GPS
#define powerpin 4
// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
// but some are 38400 or other. Check the datasheet!
#define GPSRATE 4800

// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
String dataString = String("                                                  ");
void Filter(REAL* x, REAL* P, REAL* measure, REAL deltaT);

// sGPS_file contains UTC of first valid GPS
// first valid GPS acquisition typically takes 42 seconds.


// unsigned long millis() is time since program started running
// offset_ms is value of millis() at start_time
unsigned long offset_ms = 0;

/* There are more accurate ways to compute distance between two latitude and longitude points.
   We use a simple approximation, since we are interesed in a flat projection over a small area.
   Curvature of the earth is not significant.
*/
void waypoint::Compute_mm()
{
    double temp;
    temp = (longitude - LONGITUDE_ORIGIN) * TO_RADIANS * cos(LATITUDE_ORIGIN*TO_RADIANS);
    temp *= EARTH_RADIUS_MM;
    east_mm = temp;
    north_mm = (latitude - LATITUDE_ORIGIN) * TO_RADIANS * EARTH_RADIUS_MM;
    north_mm = temp;
}
void waypoint::Compute_LatLon()
{
    longitude = east_mm / (EARTH_RADIUS_MM * TO_RADIANS * cos(LATITUDE_ORIGIN*TO_RADIANS)) + LONGITUDE_ORIGIN;
    latitude = (north_mm / (EARTH_RADIUS_MM * TO_RADIANS)) + LATITUDE_ORIGIN; 
}
//---------------------------------------------------------
uint32_t parsedecimal(char *str) 
{
  uint32_t d = 0;
  
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
void readline(void) 
{
  char c;
  char buffidx;                // an indexer into the buffer
  
  buffidx = 0; // start at begninning
  while (1) 
  {
      c=Serial3.read();
      if (c == -1)
        continue;
      if (c == '\n')
        continue;
      if ((buffidx == BUFFSIZ-1) || (c == '\r')) 
      {
        buffer[buffidx] = 0;
        return;
      }
      buffer[buffidx++]= c;
  }
}
char* waypoint::formDataString()
{
  // now log the information
  // make a string for assembling the data to log:
  char dataString[BUFFSIZ];
  sprintf(dataString, "%.6f,%.6f,%d,%d,%d,%d,%d\n", 
  latitude, longitude, east_mm, north_mm, sigmaE_mm, sigmaN_mm, time_ms);  

  return dataString;
}
/*---------------------------------------------------------------------------------------*/ 
void waypoint::fuse(waypoint GPS_reading, int deltaT_ms)
{
    // Assume uncertainty standard deviation is 10 meters.
    // The numbers below are variance in mm.
    REAL uncertainty[] = {1.0E10, 0, 0, 0,
                          0, 1.0E10, 0, 0,
                          0, 0, 1.0E10, 0,
                          0, 0, 0, 1.0E10};
    REAL State[4];

    REAL deltaT_s = deltaT_ms / (1000.0);
    REAL measurements[2];
    REAL speedX, speedY;
    REAL angle = (90.-bearing)*PI/180.;
    speedX = speed_mmPs * cos(angle);
    speedY = speed_mmPs * sin(angle);
    measurements[0] = GPS_reading.east_mm + speedX * deltaT_s;
    measurements[1] = GPS_reading.north_mm + speedY *deltaT_s;
    State[0] = east_mm;
    State[1] = north_mm;
    State[2] = speedX;
    State[3] = speedY;
    uncertainty[0] = 
    uncertainty[10] = sigmaE_mm * sigmaE_mm;
    uncertainty[5] = 
    uncertainty[15] = sigmaN_mm * sigmaN_mm;
//  Following statement produces unspecified "Error compiling" on Arduino.
    Filter(State, uncertainty, measurements, deltaT_s);

}
//---------------------------------------------------------- 
void waypoint::GetLatLon(char* parseptr)
{
    uint32_t latitd, longitd;
    char latdir, longdir;
    uint32_t degree, fraction;
    latitd = parsedecimal(parseptr);
    if (latitd != 0) 
    {
        latitd *= 10000;
        parseptr = strchr(parseptr, '.')+1;
        latitd += parsedecimal(parseptr);
    }
    parseptr = strchr(parseptr, ',') + 1;
    // read latitude N/S data
    if (parseptr[0] != ',') {
      latdir = parseptr[0];
    }
    // longitude
    parseptr = strchr(parseptr, ',')+1;
    longitd = parsedecimal(parseptr);
    if (longitd != 0) {
      longitd *= 10000;
      parseptr = strchr(parseptr, '.')+1;
      longitd += parsedecimal(parseptr);
    }
    parseptr = strchr(parseptr, ',')+1;
    // read longitude E/W data
    if (parseptr[0] != ',') {
      longdir = parseptr[0];
    }       
// latitude is ddmmmmmm 
    degree = latitd/1000000;
    fraction = (latitd%1000000)*100/60;
    latitude = ((REAL)fraction)/1000000.;
    latitude += degree;
    if (latdir == 'S')
       latitude = -latitude;
   
    degree = longitd/1000000;
    fraction = (longitd%1000000)*100/60;
    longitude = ((REAL)fraction)/1000000.;
    longitude += degree;
    if (longdir == 'W')
       longitude = -longitude;
    Compute_mm();
  
}
//---------------------------------------------------------- 
// Aquire a GPS signal and fill in the waypoint data.
// return 1 if valid, zero if not.
// wait up to max_wait milliseconds to get a valid signal.
bool waypoint::AcquireGPS(long max_wait_ms)
{
  uint8_t groundspeed, trackangle;
  char status;
  char *parseptr;              // a character pointer for parsing
  // value of millis() for GPS data
  unsigned long AcquisitionTime_ms;

  char* pTime;
  char* pDate;
//  long tmp;
  status = 'V';
  sigmaE_mm = 10000;
  sigmaN_mm = 10000;
  while (status != 'A')
  {
    readline();
    // http://users.erols.com/dlwilson/gpshdop.htm uses the model
    // error = sqrt( (3.04*HDOP)^2 + (3.57)^2)
    // see http://en.wikipedia.org/wiki/Error_analysis_for_the_Global_Positioning_System
    // get Horizontal Dillution of Precision (HDOP).
    if (strncmp(buffer, "$GPGGA",6) == 0) 
    {
      AcquisitionTime_ms = millis();
      parseptr = buffer+7;
    }
    // check if $GPRMC (global positioning fixed data)
    if (strncmp(buffer, "$GPRMC",6) == 0) 
    {  
      AcquisitionTime_ms = millis();
      // hhmmss time data
      pTime =
      parseptr = buffer+7;
      parseptr = strchr(parseptr, ',') + 1;
      status = parseptr[0];  // A = data valid; V = data not valid
      if (status != 'A')
          continue;
      parseptr += 2;      
      // grab latitude & long data
      GetLatLon(parseptr);
      // groundspeed
      parseptr = strchr(parseptr, ',')+1;
      groundspeed = parsedecimal(parseptr);  
      // track angle
      parseptr = strchr(parseptr, ',')+1;
      trackangle = parsedecimal(parseptr); 
      // date
      parseptr = strchr(parseptr, ',')+1;
      pDate = parseptr;
      if (offset_ms == 0)
      {
          SetTime(pTime, pDate);
          offset_ms = AcquisitionTime_ms;
      }      
      time_ms = AcquisitionTime_ms;
    }
    return true;
  }
  return false;
}

