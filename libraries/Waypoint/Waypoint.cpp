#include <IO.h>
#include <Arduino.h>
#include <Waypoint.h>

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

//========================= Items only for C6 Navigator =================================
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
    speedX = ((REAL)(speed_mmPs) * Evector_x1000) / MEG;  // m/sec
    speedY = ((REAL)(speed_mmPs) * Nvector_x1000) / MEG;
    measurements[0] = GPS_reading.east_mm / 1000. + speedX * deltaT_s;
    measurements[1] = GPS_reading.north_mm / 1000. + speedY *deltaT_s;
    
    REAL GPS_sigma = ((REAL) GPS_reading.sigma_mm) / 1000.;
    variance[0] = variance[3] = GPS_sigma * GPS_sigma;
    
    Filter(State, uncertainty, measurements, deltaT_s, variance);
    
    east_mm = State[0] * 1000;
    north_mm = State[1] * 1000;
    speedX = State[2] * 1000;
    speedY = State[3] * 1000;
    sigma_mm = sqrt(uncertainty[0]) * 1000;
    Compute_LatLon();
    speed_mmPs = 1000 * sqrt(speedX*speedX + speedY*speedY);
    if (speed_mmPs > 100 || speed_mmPs < -100) 
    {
      Evector_x1000 = (MEG * speedX/speed_mmPs);
      Nvector_x1000 = (MEG * speedY/speed_mmPs);
    }
}
