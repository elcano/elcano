
#include "Arduino.h"
#include "GPS.h"
#define REAL double
// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
// but some are 38400 or other. Check the datasheet!

// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
char dataString[BUFFSIZ];
void Filter(REAL* x, REAL* P, REAL* measure, REAL deltaT, REAL* variance);

// sGPS_file contains UTC of first valid GPS
// first valid GPS acquisition typically takes 42 seconds.


// unsigned long millis() is time since program started running
// offset_ms is value of millis() at start_time
unsigned long offset_ms = 0;
double CosLatitude;

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
    relative = ((double) diff)/1000000. * TO_RADIANS * CosLatitude;
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
    relative = ((double)east_mm) * (1000000 / (EARTH_RADIUS_MM * TO_RADIANS * CosLatitude));  
    diff = relative;
    longitude = diff + LONGITUDE_ORIGIN;
    relative = ((double)north_mm) * (1000000 / (EARTH_RADIUS_MM * TO_RADIANS));  
    diff = relative;
    latitude = diff +  LATITUDE_ORIGIN;
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
// return true if a line was read; false if not
bool readline(int channel) 
{
  // buffer can hold 128 bytes; if not enough there yet, try later.
  const int MinimumMessage = 30;
  char c;
  char buffidx;                // an indexer into the buffer
  
  buffidx = 0; // start at begining
//  if (Serial3.available() < MinimumMessage)
//    return false;
  while (1) 
  {
      switch(channel)
      {
      case 1:
         c=Serial1.read();
         break;
       case 2:
         c=Serial2.read();
         break;
      case 3:
         c=Serial3.read();
         break;
      default:
         c=Serial.read();
         break;
      }
     if (c == -1)
        continue;
 //     Serial.print(c);
      if (c == '\n')
        continue;
      if ((buffidx == BUFFSIZ-1) || (c == '\r')) 
      {
        buffer[buffidx] = 0;
 //       Serial.print("/nReturn");
        return true;
      }
      buffer[buffidx++]= c;
//      Serial.print(buffidx, DEC);
  //    Serial.print("\n");
  }
}
char* waypoint::formDataString()
// function uses a global dataString buffer; thus a second call to formDataString
// may overwrite results from the previous call.
{
  // now log the information
  // make a string for assembling the data to log:
  long latFraction, lonFraction, eastFraction, northFraction;
  latFraction = latitude  >= 0? latitude%MEG  : (-latitude)%MEG;
  lonFraction = longitude >= 0? longitude%MEG : (-longitude)%MEG;
  eastFraction = east_mm  >= 0? east_mm%1000  : (-east_mm)%1000;
  northFraction = north_mm>= 0? north_mm%1000 : (-north_mm)%1000;
  sprintf(dataString, 
  "%ld.%0.6ld,%ld.%0.6ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,",
  latitude/MEG, latFraction, longitude/MEG, lonFraction,
  east_mm/1000, eastFraction, north_mm/1000, northFraction, 
  sigmaE_mm/1000, sigmaE_mm%1000, sigmaN_mm/1000, sigmaN_mm%1000, 
  time_ms/1000, time_ms%1000);  
 
  return dataString;
}
void waypoint::operator=(waypoint& right)
{
  latitude =   right.latitude;
  longitude =  right.longitude;
  east_mm =    right.east_mm;
  north_mm =   right.north_mm;
  sigmaE_mm =  right.sigmaE_mm;
  sigmaN_mm =  right.sigmaN_mm;
  time_ms =    right.time_ms;  
  bearing  =   right.bearing;
  speed_mmPs = right.speed_mmPs;
  return;
}

/*---------------------------------------------------------------------------------------*/ 
void waypoint::fuse(waypoint GPS_reading, int deltaT_ms)
{
    // Assume uncertainty standard deviation is 10 meters.
    // The numbers below are variances in m.
    // speed standard deviation is in m/sec; 
    // assuming no time error it is same as position standard deviation
    static REAL uncertainty[] = {100., 0,   0,   0,
                          0,   100., 0,   0,
                          0,    0,  100., 0,
                          0,    0,   0,  100.};
    static REAL State[4] = {5000000, 0, 0, 0};
    REAL variance[] = {100., 0,
                         0, 100.};

    REAL deltaT_s = ((REAL) deltaT_ms) / 1000.0;
    REAL measurements[2];
    REAL speedX, speedY;
    if (State[0] > 2500000)
    {  // first time
      State[0] = GPS_reading.east_mm / 1000.;
      State[1] = GPS_reading.north_mm /1000.;
    }
    REAL angle = (90.-bearing)*PI/180.;
    speedX = (speed_mmPs * cos(angle)) / 1000.;
    speedY = (speed_mmPs * sin(angle)) / 1000.;
    measurements[0] = GPS_reading.east_mm / 1000. + speedX * deltaT_s;
    measurements[1] = GPS_reading.north_mm / 1000. + speedY *deltaT_s;
    
    REAL GPS_sigma = ((REAL) GPS_reading.sigmaE_mm) / 1000.;
    variance[0] = variance[3] = GPS_sigma * GPS_sigma;
    
    Filter(State, uncertainty, measurements, deltaT_s, variance);
    
    east_mm = State[0] * 1000;
    north_mm = State[1] * 1000;
    speedX = State[2] * 1000;
    speedY = State[3] * 1000;
    sigmaE_mm = sqrt(uncertainty[0]) * 1000;
    sigmaN_mm = sqrt(uncertainty[5]) * 1000;
    Compute_LatLon();
    speed_mmPs = sqrt(speedX*speedX + speedY*speedY);
    if (speed_mmPs > 100 || speed_mmPs < -100) 
    {
      angle = asin(speedY/speed_mmPs);
      bearing = 90. - 180./PI*angle;
    }
}
//---------------------------------------------------------- 
char* waypoint::GetLatLon(char* parseptr)
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
    latitude = fraction + degree * 1000000;
    if (latdir == 'S')
       latitude = -latitude;
   
    degree = longitd/1000000;
    fraction = (longitd%1000000)*100/60;
    longitude = fraction + degree * 1000000;
    if (longdir == 'W')
       longitude = -longitude;
    Compute_mm();
/*  Serial.print("\tLat: ");
    Serial.print(latitd/1000000, DEC); Serial.print(".");
    Serial.print((latitd%1000000)*100/60, DEC); Serial.print(',');
    Serial.print("\tLong: ");
    Serial.print(longitd/1000000, DEC); Serial.print(".");
    Serial.print((longitd%1000000)*10/6, DEC); Serial.print('\n'); */
 
    return parseptr;
}
//---------------------------------------------------------- 
// Aquire a GPS GPRMC signal and fill in the waypoint data.
// return 1 if valid, zero if not.
// wait up to max_wait milliseconds to get a valid signal.
bool waypoint::AcquireGPRMC(unsigned long max_wait_ms)
{
  uint8_t groundspeed, trackangle;
  char status ='V'; // V = data invalid
  char *parseptr;              // a character pointer for parsing
  // value of millis() for GPS data
  unsigned long AcquisitionTime_ms;
  char* pTime;
  char* pDate;
  unsigned long TimeOut = millis() + max_wait_ms;
// $GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10
  sigmaE_mm = 10000;
  sigmaN_mm = 10000;
//  Serial.println("looking");
  CosLatitude = cos(((double) LATITUDE_ORIGIN)/1000000. * TO_RADIANS);
  while (status != 'A') // A = data valid
  {
    if (!readline(3))
    {  // nothing to read; how long have we waited?
      if (millis() > TimeOut)
      {
         Serial.println("Timed out");
         return false;
      }
    }
//    Serial.println("Read line");
    Serial.println(buffer);
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
      parseptr = GetLatLon(parseptr);
      // groundspeed
      parseptr = strchr(parseptr, ',')+1;
//      Serial.println(parseptr);
      groundspeed = parsedecimal(parseptr);  
      // track angle
      parseptr = strchr(parseptr, ',')+1;
      trackangle = parsedecimal(parseptr); 
      // date
      parseptr = strchr(parseptr, ',')+1;
      pDate = parseptr;
 /*   Serial.print("\tGroundspeed: ");
      Serial.print(groundspeed, DEC); Serial.print(",");
      Serial.print("\tHeading: ");
      Serial.print(trackangle, DEC); Serial.print(",");
      Serial.print("\tDate: ");
      Serial.println(pDate); */
      if (offset_ms == 0)
      {
          SetTime(pTime, pDate);
          offset_ms = AcquisitionTime_ms;
      }      
      time_ms = AcquisitionTime_ms;
    }
  }
  if (status == 'A')
    return true;
  return false;
}
//---------------------------------------------------------- 
// Aquire a GPS $GPGGA signal and fill in the waypoint data.
// return 1 if valid, zero if not.
// wait up to max_wait milliseconds to get a valid signal.
bool waypoint::AcquireGPGGA(unsigned long max_wait_ms)
{
  uint8_t satelites_used, hdop;
  REAL HDOP, error_m, error_mm;
  char FixIndicator = '0';
  char *parseptr;              // a character pointer for parsing
  // value of millis() for GPS data
  unsigned long AcquisitionTime_ms;

  char* pTime;
  unsigned long TimeOut = millis() + max_wait_ms;
  sigmaE_mm = 10000;
  sigmaN_mm = 10000;
  // $GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M,,,,0000*18
  while (FixIndicator == '0')
  {
    if (!readline(3))
    {  // nothing to read; how long have we waited?
      if (millis() > TimeOut)
         return false;
    }    
//    Serial.println(buffer);
    if (strncmp(buffer, "$GPGGA",6) == 0) 
    {
//    Serial.println(buffer);
      AcquisitionTime_ms = millis();
      pTime =
      parseptr = buffer+7;
      parseptr = strchr(parseptr, ',') + 1;
   
      // grab latitude & long data
      parseptr = GetLatLon(parseptr);
      parseptr = strchr(parseptr, ',') + 1;
      FixIndicator = parseptr[0];  // A = data valid; V = data not valid
      if (FixIndicator == '0')
          continue;
      parseptr += 2;      
       // satelites used
      parseptr = strchr(parseptr, ',')+1;
      satelites_used = parsedecimal(parseptr);  
      // HDOP
      parseptr = strchr(parseptr, ',')+1;
      hdop = parsedecimal(parseptr); 
      hdop *= 10;
      parseptr = strchr(parseptr, '.')+1;
      hdop += parsedecimal(parseptr);
      // http://users.erols.com/dlwilson/gpshdop.htm uses the model
      // error = sqrt( (3.04*HDOP)^2 + (3.57)^2)
      // see http://en.wikipedia.org/wiki/Error_analysis_for_the_Global_Positioning_System
      // get Horizontal Dillution of Precision (HDOP).
      HDOP = (REAL) hdop / 10.;
      error_m = sqrt((3.04*HDOP)*(3.04*HDOP) + 3.57*3.57);
      error_mm = 1000 * error_m;
      sigmaE_mm = sigmaN_mm = error_mm;
     
      if (offset_ms == 0)
      {
          SetTime(pTime, "1205xx");
          offset_ms = AcquisitionTime_ms;
      }      
      time_ms = AcquisitionTime_ms;
    }
  }
  if (FixIndicator == '0')
    return false;
  return true;
}
void instrument::Read(waypoint GPS)
{
}

