/*
Elcano Module C6: Navigator.

The Navigator has the job of making the best estimate of current vehicle
position, attitude, velocity and acceleration.

It uses a variety of sensors, some of which may not be present or reliable.

S1: Hall Odometer.  This unit may be the same odometer used by C1 for
wheel spin speed feedback. If so, it may be received from C1.

Visual Odometry from S4 (Smart camera) as passed though C7 (Visual Data
Management).

S3: Digital Compass

S9: Inertial Navigation Unit.

Lateral deviation from lane from C7.

Intended path from C4.

Commanded course from C3.

Dead reckoning based on prior state estimate.

Distance and bearing to landmarks whose latitude and longitude are recorded
on the RNDF didgital map.

S7: GPS.  GPS is not the primary navigation sensor and the robot should be 
able to operate indoors, in tunnels or other areas where GPS is not available
or accurate.

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
This is the method used to perform sensor fusion.

*/

/*---------------------------------------------------------------------------------------*/ 
#define MAX_WAYPOINTS 10
/*   There are two coordinate systems.
     MDF and RNDF use latitude and longitude.
     C3 and C6 assume that the earth is flat at the scale that they deal with.
     All calculations with C6 are performed in the (east_mm, north_mm)
     coordinate system. A raw GPS reading is in latitude and longitude, but it
     is transformed to the flat earth system using the distance to the nearest
     waypoint.
     A third coordinate system has its origin at the vehicle center and is
     aligned with the vehicle. This is usually transformed into the flat
     world coordinate system.
*/
const int RxD = 0;
const int TxD = 1;

struct waypoint
{
    double latitude;
    double longitude;
    long east_mm;   // max is 2147 km
    long north_mm;
} mission[MAX_WAYPOINTS];
int waypoints;

void setup() 
{ 
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    initialize();
}
/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
  /* Wait until initial position is known. */
  mission[0].latitude = 100; // beyond the north pole.
  while (mission[0].latitude > 90)
  {
    /*
    if (GPS_available)
      GetGPS();  // put latitude and longitude in mission[0]
    if (InitialPositionProvidedFromC4)
      ReadInitialPosition(C4); // put latitude and longitude in mission[0]
    */
  }
  // Set mission[0] x_mm and y_mm to 0.
  // Set Odometer to 0.
  // Set lateral deviation to 0.
  // Read compass.
  // ReadINU.
  // Set attitude.
  // Set velocity and acceleration to zero.
  // SendState(C4);
    mission[1].latitude = 100; // beyond the north pole.
    // Wait to get path from C4
    while (mission[1].latitude > 90)
    {
    /* If (message from C4)
    {
      ReadState(C4);  // get initial route and speed
    }
     Read GPS, compass and IMU and update their estimates.
   */
    }
    // ready to roll
    // Fuse all position estimates.
    // Send vehicle state to C3 and C4.
    
}
/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
  /* Perform dead reckoning from clock and previous state
    Read compass.
    ReadINU.
    Set attitude.
    if (GPS_available)
      GetGPS();
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
    // Fuse all position estimates.
 
    // Send vehicle state to C3 and C4.
    
  */
}
