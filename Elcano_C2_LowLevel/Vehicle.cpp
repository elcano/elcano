#include "Settings.h"
#include <Arduino.h>
#include "Vehicle.h"


Vehicle::Vehicle(){
	
	
}


Vehicle::~Vehicle()
{
}

void Vehicle::eStop() {
	brake.Stop();
	throttle.stop();
}

void Vehicle::move(double dAngle, double dSpeed) {
	if (dSpeed < (throttle.getSpeedInput_mmPs() + 10))
		brake.Stop();
	else {
		brake.Release();
		throttle.setDesiredSpeed__mmPs(dSpeed);
	}
	steer.setDesiredTurn(dAngle);
}

void Vehicle::update() {
	brake.Check();
	throttle.updateSpeed();
	steer.updateAngle(computeAngleLeft());
	}

long Vehicle::computeAngleLeft() {
	long val = analogRead(AngleSensorLeft);
	val = map(val, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
	return val;
}

long Vehicle::computeAngleRight() {
	long val = analogRead(AngleSensorRight);
	val = map(val, MIN_Right_Sensor, MAX_Right_Sensor, MIN_TURN, MAX_TURN);
	return val;
}
