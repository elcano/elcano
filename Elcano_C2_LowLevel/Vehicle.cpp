#include "Settings.h"
#include "Vehicle.h"
#ifndef TESTING
#include <Arduino.h>
#include <PinChangeInterrupt/src/PinChangeInterrupt.h>
#endif

volatile int32_t Vehicle::desired_speed_cmPs;
volatile int32_t Vehicle::desired_angle;


Vehicle::Vehicle(){
	
	
}


Vehicle::~Vehicle(){
}

//Calls the initialization of all 3 objects
void Vehicle::initialize() {
	throttle.initialize();
	brake.initialize();
	steer.initialize();
	desired_angle = 0;
	desired_speed_cmPs = 0;
	if (IRPT_CAN)
		attachPCINT(digitalPinToPCINT(IRPT_CAN), recieveCan, RISING);
}

void Vehicle::eStop() {
	if (DEBUG)
		Serial.println("Brake");
	brake.Stop();
	throttle.stop();
}
void Vehicle::update(int32_t tempDspeed, int32_t tempDangle) {
	brake.Update();
	int currentSpeed = throttle.update(tempDspeed);
	int currentAngle = steer.update(tempDangle);
	if (tempDspeed < (currentSpeed * 10))
		brake.Stop();
	else
		brake.Release();
}


/*
Checks if the brakes have been on too long
Computes the speed and angle
*/
void Vehicle::update() {
	int32_t tempDspeed;
	int32_t tempDangle;
	noInterrupts();
	tempDspeed = desired_speed_cmPs;
	interrupts();
	noInterrupts();
	tempDangle = desired_angle;
	interrupts();
	brake.Update();
	int currentSpeed = throttle.update(tempDspeed);
	int currentAngle = steer.update(tempDangle);
	if (tempDspeed < (currentSpeed*10))
		brake.Stop();
	else 
		brake.Release();

	//send msg to can
}




void Vehicle::recieveCan() {
	noInterrupts();
	//recieve message

	interrupts();
}