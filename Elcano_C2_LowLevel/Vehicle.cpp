#include "Settings.h"
#include <Arduino.h>
#include "Vehicle.h"


Vehicle::Vehicle(){
	
	
}


Vehicle::~Vehicle(){
}

//Calls the initialization of all 3 objects
void Vehicle::initialize() {
	throttle.initialize();
	brake.initialize();
	steer.initialize(computeAngleLeft());
}

void Vehicle::eStop() {
	if (DEBUG)
		Serial.println("Brake");
	brake.Stop();
	throttle.stop();
}

void Vehicle::stop(int32_t strength) {
	throttle.stop(strength);
}

/*
Side note: if the brakes are more emergency brakes
should we be calling the brakes here?
*/
void Vehicle::move(int32_t dAngle, int32_t dSpeed) {
//If we want to be going slower than we currently are by 10* current speed + 10  then stop
	if (dSpeed < (throttle.getSpeedInput_mmPs() + 10))
		brake.Stop();
	else {
		brake.Release();
		throttle.setDesiredSpeed__mmPs(dSpeed);
	}
	steer.setDesiredTurn(dAngle);
}

/*
This is part of our demo code that 
does not use the pids
*/
void Vehicle::noPID(int32_t dAngle,int32_t dSpeed){
  steer.engageSteering(dAngle);
  brake.Release();
  throttle.engageThrottle(dSpeed);
}

/*
Checks if the brakes have been on too long
Computes the speed and angle
*/
void Vehicle::update() {
	brake.Check();
	throttle.updateSpeed();
	steer.updateAngle(computeAngleLeft());
	}

int32_t Vehicle::computeAngleLeft() {
	int32_t val = analogRead(AngleSensorLeft);
	val = map(val, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
	return val;
}

int32_t Vehicle::computeAngleRight() {
	int32_t val = analogRead(AngleSensorRight);
	val = map(val, MIN_Right_Sensor, MAX_Right_Sensor, MIN_TURN, MAX_TURN);
	return val;
}
