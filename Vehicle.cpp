#include "trike_settings.h"
#include "Brakes.h"
#include "ThrottleController.h"
#include <Arduino.h>
#include "Vehicle.h"

#define DAC_CHANNEL 0
#define PID_CALCULATE_TIME 50
#define SelectAB 53
#define SelectCD 53

Vehicle::Vehicle():
  throttle(MIN_ACC_OUT,MAX_ACC_OUT,PID_CALCULATE_TIME,SelectAB,SelectCD,DAC_CHANNEL)
{
  iState = 0;
  
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

void Vehicle::updateSpeed(){
	if(iState <=1)
    throttle.setSpeedInput_mmPs(0);
  else if(iState == 2){
    throttle.setSpeedInput_mmPs(WHEEL_CIRCUM_MM *(1000 / msPwheelRev);
    
  }
}

void Vehicle::WheelRev(){
	unsigned long timeOfTick = millis();
	if(iState != 3)
		iState ++;
	if((timeOfTick - TickTime)>MIN_TICK_TIME_ms){
		msPwheelRev = TickTime - timeOfTick;
		TickTime = timeOfTick;
	}
}
