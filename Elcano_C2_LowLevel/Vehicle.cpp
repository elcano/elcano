#include "trike_settings.h"
#include "pin_settings.h"
#include <Arduino.h>
#include "Vehicle.h"

#define PID_CALCULATE_TIME 50


Vehicle::Vehicle():
	throttle(MIN_ACC_OUT,MAX_ACC_OUT,PID_CALCULATE_TIME,SelectAB,SelectCD,DAC_CHANNEL)
{
	
	
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
	//steer.setDesiredTurn(dAngle);
}

void Vehicle::update() {
	brake.Check();
}
