#pragma once

#include "Settings.h"

class C6_Navigator {
private:
	#define mySerial Serial3
	#define GPSRATE 9600
	void setup_GPS();
	bool AcquireGPS(waypoint &gps_position);
	void C6_communication_with_C2();
	long getHeading();
	void findPosition(bool got_GPS, waypoint &op);
	void initial_position(waypoint &op);
	
public:
	C6_Navigator(waypoint &ep, waypoint &op);
	~C6_Navigator();
	void update(waypoint &ep, waypoint &newPos);
};
