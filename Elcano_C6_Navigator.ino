/*
Elcano Module C6: Navigator.
Copy the following files to an Elcano_C6_Navigator directory:
  Elcano_C6_Navigator.ino; add new tabs with the following names and copy contents
  C6_IO.h
  GPS.cpp
  GPS.h
  KalmanFilter.cpp
  Matrix.cpp
  Matrix.h

The Navigator has the job of making the best estimate of current vehicle
position, attitude, velocity and acceleration.

It uses a variety of sensors, some of which may not be present or reliable.

Near future: S1: Hall Odometer.  This unit was originally on C1 and gives
wheel spin speed feedback.

Future: Visual Odometry from S4 (Smart camera) as passed though C7 (Visual Data
Management).

Future: S3: Digital Compass

Future: S9: Inertial Navigation Unit.

Future: Lateral deviation from lane from C7.

Future: Intended path from C4.

Future: Commanded course from C3.

Yes, from KF: Dead reckoning based on prior state estimate.

Future: Distance and bearing to landmarks whose latitude and longitude are recorded
on the RNDF didgital map.

Yes: S7: GPS.  GPS should not the primary navigation sensor and the robot should be 
able to operate indoors, in tunnels or other areas where GPS is not available
or accurate.

An alternative to the Kalman Filter (KF) is fuzzy numbers.
The position estimate from most sensors is a pair of fuzzy numbers.
[Milan Mares, Computation Over Fuzzy Quantities, CRC Press.]
A fuzzy number can be thought of as a triangle whose center point
is the crisp position and whose limits are the tolerances to which
the position is known.

The odometry sensor is one dimensional and gives the position along the 
intended path. Lane deviation is also one dimensional and gives position
normal to the intended path. Odometry, lane following and a digital map
should be sufficient for localization.  An odometer will drift. This drift 
can be nulled out by landmark recognition or GPS.

GPS provides a pyramid aligned north-south and east-west, in contrast to
odometry / lane deviation, which is a pyramid oriented in the direction of 
vehicle motion. The intersection of two fuzzy sets is their minimum.
By taking the minima of all position estimate pyramids, we get a surface
describing the possible positions of the vehicle. Crispifying the surface
gives the estimated vehicle position. 
The fuzzy method may be used to perform sensor fusion.

Serial lines:
0: Monitor
1: INU
2: Tx: Estimated state; 
      $ESTIM,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
   Rx: Desired course
      $EXPECT,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
      // at leat 18 characters
3: GPS
  Rx: $GPRMC,...  typically 68 characters
      $GPGSA,...
      $GPGGA,... typically 68 characters
  Tx: $PSRF103,...
*/

/*---------------------------------------------------------------------------------------*/ 
#include "GPS.h"
#include "C6_IO.h"
#include "Matrix.h"  
#include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

#ifdef SIMULATOR
#include "..\\Elcano\\Elcano\\Elcano_simulator.h"
#endif


namespace C6_Navigator {
    void setup();
    void loop();
}
// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
// but some are 38400 or other. Check the datasheet!
#define GPSRATE 4800

#define MAX_WAYPOINTS 10
/*   There are two coordinate systems.
     MDF and RNDF use latitude and longitude.
     C3 and C6 assume that the earth is flat at the scale that they deal with.
     All calculations with C6 are performed in the (east_mm, north_mm)
     coordinate system. A raw GPS reading is in latitude and longitude, but it
     is transformed to the flat earth system using the distance from the origin,
     as set by (LATITUDE_ORIGIN, LONGITUDE_ORIGIN).
     A third coordinate system has its origin at the vehicle center and is
     aligned with the vehicle. This is usually transformed into the flat
     world coordinate system.
*/


#ifdef SIMULATOR
namespace C6_Navigator {
#endif
// limited to 8+3 characters
#define FILE_NAME "GPSLog.csv"
File dataFile;
char GPSfile[BUFFSIZ] = "mmddhhmm.csv"; 
String formDataString();
 
int waypoints;
waypoint mission[MAX_WAYPOINTS];
waypoint GPS_reading;
waypoint estimated_position;
const int LoopPeriod = 100;  // msec

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
         msg[i+3] = '\n';
         return true;
      }
      sum ^= msg[i];
    }
    return false;
}
/*---------------------------------------------------------------------------------------*/ 

void initialize()
{
  pinMode(GPS_POWER, OUTPUT);
  char* GPSString;
  char* disable = "$PSRF103,i,00,00,01*xxxxxx";
  char* querryGGA = "$PSRF103,00,01,00,01*25";
  bool GPS_available = false;
  /* Wait until initial position is known. */
  mission[0].latitude = 100; // beyond the north pole.
  Serial3.begin(GPSRATE);   
  digitalWrite(GPS_POWER, LOW);         // pull low to turn on!
//  Serial.print("Acquiring GPS RMC...");
  GPS_available = mission[0].AcquireGPRMC(70000);
  if (GPS_available)
  {
    GPSString = mission[0].formDataString();
    Serial.print(GPSString);
    estimated_position = mission[0];
//    Serial.println("OK");
  }
  else
  {
    estimated_position.latitude = 47.62130;  // set by hand here
    estimated_position.longitude = -122.35090;
    estimated_position.Compute_mm();
    estimated_position.sigmaE_mm = 1.0E5; // 10 m standard deviation
    estimated_position.sigmaN_mm = 1.0E5;
    estimated_position.time_ms = millis();
//    Serial.println("Failed");
  }
  estimated_position.speed_mmPs = 0;
  estimated_position.bearing = 0;  // to be taken from path or set by hand
  GPSString = estimated_position.formDataString();  
  Serial.println(GPSString);
/*
    if (InitialPositionProvidedFromC4)
      ReadInitialPosition(C4); // put latitude and longitude in mission[0]
    */
  // Set Odometer to 0.
  // Set lateral deviation to 0.
  // Read compass.
  // ReadINU.
  // Set attitude.
  // Set velocity and acceleration to zero.
  // SendState(C4);
    mission[1].latitude = 100; // beyond the north pole.
    // Wait to get path from C4
//    while (mission[1].latitude > 90)
    {
    /* If (message from C4)
    {
      ReadState(C4);  // get initial route and speed
    }
     Read GPS, compass and IMU and update their estimates.
   */
    }
    /* disable GPS messages;
    for (char i='0'; i < '6'; i++)
    {
      disable[9] = i;
      checksum(disable);
      Serial3.println(disable);
    }
    Serial3.println(querryGGA);
    */
//    GPS_available = GPS_reading.AcquireGPGGA(300);
    // ready to roll
    // Fuse all position estimates.
    // Send vehicle state to C3 and C4.
    
}
/*---------------------------------------------------------------------------------------*/ 

void setup() 
{ 
    char* Header = "Latitude,Longitude,East_mm,North_mm,SigmaE_mm,SigmaN,mm,Time_ms,";
    pinMode(Rx0, INPUT);
    pinMode(Tx0, OUTPUT);
    pinMode(GPS_POWER, OUTPUT);
    Serial3.begin(GPSRATE);    
    Serial.begin(9600);
    // prints title with ending line break 
    Serial.println("GPS parser");  
    digitalWrite(GPS_POWER, LOW);         // pull low to turn on!
    initialize();
//    Serial.print("Initializing GPS SD card...");
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(chipSelect, OUTPUT);
    pinMode(53, OUTPUT);  // Unused CS on Mega
    pinMode(GPS_RED_LED, OUTPUT);
    pinMode(GPS_GREEN_LED, OUTPUT);
    digitalWrite(GPS_GREEN_LED, LOW);
    
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) 
    {
      Serial.println("Card failed, or not present");
      digitalWrite(GPS_RED_LED, HIGH);
      // don't do anything more:
      return;
    }
    Serial.println("card initialized.\n");
    digitalWrite(GPS_RED_LED, LOW);
    dataFile = SD.open(GPSfile, FILE_WRITE);
    // if the file is available, write date and time to it:
    if (dataFile) 
    {
        dataFile.println(GPSfile);
        dataFile.print(Header);
        dataFile.println(Header);
        dataFile.close();
    }  

}
/*---------------------------------------------------------------------------------------*/ 
void waypoint::SetTime(char *pTime, char * pDate)
{
//     was: GPSfile = "GPS_yymmdd_hhmmss.CSV"; 
//     now: GPSfile = "mmddhhmm.CSV";
//   strncpy(GPSfile+4,  pDate+4, 2);  // year   
     strncpy(GPSfile,  pDate+2, 2);  // month   
     strncpy(GPSfile+2,  pDate, 2);    // day    
     strncpy(GPSfile+4, pTime,2);     // GMT hour
     strncpy(GPSfile+6, pTime+2,2);     // minute
//   strncpy(GPSfile+11, pTime+4,2);     // second
     Serial.println(GPSfile);
}

/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
    REAL deltaT_ms;
    unsigned long time = millis();
    unsigned long endTime = time + LoopPeriod;
    char* pData;
    char* pGPS;

    bool GPS_available = GPS_reading.AcquireGPGGA(300);

    /* Perform dead reckoning from clock and previous state
    Read compass.
    ReadINU.
    Set attitude.
    Read Hall Odometer;
    Read Optical Odometer;
    Read lane deviation;  
    If (message from C4)
    {
      ReadState(C4);  // get new route and speed
    }
    If (message from C3)
    {
      ReadDrive(C3);  // get commanded wheel spin and steering
    }
    if (landmarks availabe)
    {  // get the position based on bearing and angle to a known location.
      ReadLandmarks(C4); 
    }
    // Fuse all position estimates with a Kalman Filter */
    deltaT_ms = LoopPeriod + (time - GPS_reading.time_ms);
//    estimated_position.fuse(GPS_reading, deltaT_ms);
 
    // Send vehicle state to C3 and C4.
    

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    dataFile = SD.open(GPSfile, FILE_WRITE);
  
    // if the file is available, write to it:
    if (GPS_available && dataFile) 
    {
        digitalWrite(GPS_GREEN_LED, HIGH);
        digitalWrite(GPS_RED_LED, LOW);
        pGPS = GPS_reading.formDataString();
        dataFile.println(pGPS);
  //    pData = estimated_position.formDataString();
//      dataFile.print(pData);
        dataFile.close();
          // print to the serial port too:
          Serial.println(pGPS);
/*        Serial.print(GPS_reading.latitude, DEC); Serial.print(",");
          Serial.print(GPS_reading.longitude, DEC); Serial.print(",");
          Serial.print(GPS_reading.east_mm, DEC); Serial.print(",");
          Serial.print(GPS_reading.north_mm, DEC); Serial.print(",");
          Serial.print(GPS_reading.sigmaE_mm, DEC); Serial.print(",");
          Serial.print(GPS_reading.sigmaN_mm, DEC); Serial.print(",");
          Serial.println(GPS_reading.time_ms, DEC); */
    }  
    // if the file isn't open, pop up an error:
    else 
    {
      if (!GPS_available)
      {
        digitalWrite(GPS_GREEN_LED, LOW);
        Serial.print("GPS not available");
      }
      else
      {
        digitalWrite(GPS_RED_LED, HIGH);
        Serial.println("error opening file");
      }
    } 
  // delay, but don't count time in loop
  while (time < endTime)
  {
    time = millis();
  }

}

#ifdef SIMULATOR
} // end namespace
#else
void Show(char* x)
{
  Serial.print(x);
}
void Show(REAL x)
{
  Serial.print(x);
  Serial.print(", ");
}
#endif

/* Entry point for the simulator.
   This could be done with namespace, but the Arduino IDE does not handle preprocessor statements
   wrapping a namespace in the standard way.
*/
void C6_Navigator_setup() { C6_Navigator::setup(); }

void C6_Navigator_loop() { C6_Navigator::loop(); }
