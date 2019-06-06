#pragma once
#include "Common.h"
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20.h>

namespace elcano {
  

class Localization {
private:  
 // Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to DEBUG and listen to the raw GPS sentences.
#define GPSECHO  false

//  SoftwareSerial mySerial(15, 14); //rx tx
  #define GPSRATE 9600
  //index of hardcoded gps coordinates for testing
  double gpsTest[8] = {47.760850, -122.190044, 47.9, -122, 51, -123, 50.5, -120};
  int gpsIndex = 0; //helper with array above
  bool passedInitial = false; //to hardcode GPS only first time

 // elcano::Origin *originSt;

  long extractSpeed = 0; //alternative to checksum since it's not implemented ie check for bad incoming data through serial

  elcano::Waypoint newPos, GPS_reading;
 
  
  /* Assign a unique ID to this sensor at the same time */
  Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

  /* Assign a unique ID to this sensor at the same time */
  Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

  bool got_GPS = false;
  CAN_FRAME incoming;

	
	void setup_GPS();
	bool AcquireGPS(Waypoint &gps_position);
	void Local_communication_with_LowLevel();
	long getHeading();
  double getAccelX();
  double getAccelY();
  double getAccelZ();
  double getGyroRoll();
  double getGyroPitch();
  double getGyroYaw();
	void findPosition(Waypoint &estimPos, bool got_GPS, Waypoint &op);
	void initial_position(Origin &ogn, Waypoint &estimPos, Waypoint &old_pos);
public:
	Localization(Origin &org, Waypoint &estimated_pos, Waypoint &old_pos); 
	~Localization(){} //destructor
  void update(Origin &ogn, Waypoint &ep, Waypoint &newPos);
};
} // namespace elcano
