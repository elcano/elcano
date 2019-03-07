#pragma once

#include "Brakes.h"
#include "ThrottleController.h"
#include "SteeringController.h"

class Vehicle{
private:
	Brakes brake;
	ThrottleController throttle;
	SteeringController steer;
	static volatile int32_t desired_speed_cmPs;
	static volatile int32_t desired_angle;
	int32_t current_speed;
	int32_t current_angle;
public:
	Vehicle();
	~Vehicle();
	void initialize();
	void update();
	void update(int32_t tempDspeed, int32_t tempDangle);
	void eStop();
	void recieveCan();
};
