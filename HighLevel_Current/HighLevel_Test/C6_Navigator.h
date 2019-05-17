#pragma once
#include "Settings_HighLevel.h"
#include <Adafruit_GPS.h>

class C6_Navigator {
private:
	#define mySerial Serial3
	#define GPSRATE 9600
	void setup_GPS();
	bool AcquireGPS(Waypoint &gps_position);
	void C6_communication_with_C2();
	long getHeading();
	void findPosition(bool got_GPS, Waypoint &op);
	void initial_position(Waypoint &op);
	
public:
	C6_Navigator(Waypoint &ep, Waypoint &op); 
	~C6_Navigator(){} //destructor
	void update(Waypoint &ep, Waypoint &newPos);
};
