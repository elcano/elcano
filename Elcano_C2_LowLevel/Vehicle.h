#pragma once

#include "Brakes.h"
#include "ThrottleController.h"
#include "SteeringController.h"

class Vehicle{
private:
	static Brakes brake;
	static ThrottleController throttle;
	SteeringController steer;
	static volatile int32_t desired_speed_cmPs;
	static volatile int32_t desired_angle;
	int32_t currentSpeed;
	int32_t currentAngle;
	static void recieveCan();

public:
	Vehicle();
	~Vehicle();
	void update();
	static void eStop();
  void test(){
    throttle.test();
    };
	
};
