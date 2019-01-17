#pragma once

#include "Brakes.h"
#include "ThrottleController.h"
#include "SteeringController.h"

class Vehicle{
private:
	Brakes brake;
	ThrottleController throttle;
	SteeringController steer;
	int32_t computeAngleLeft();
	int32_t computeAngleRight();
public:
	Vehicle();
	~Vehicle();
	void initialize();
	void eStop();
	void stop(int32_t strength);
	void move(int32_t, int32_t);
	void update();
	int32_t getSpeed() { return throttle.getSpeedInput_mmPs(); };
	void tick() { throttle.tick(millis()); };
	
	void noPID(int32_t,int32_t);
};
