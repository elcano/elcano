#pragma once

#include "Brakes.h"
#include "ThrottleController.h"
#include "SteeringController.h"

class Vehicle{
private:
	Brakes brake;
	ThrottleController throttle;
	SteeringController steer;
	long computeAngleLeft();
	long computeAngleRight();
public:
	Vehicle();
	~Vehicle();
	void eStop();
	void move(double, double);
	void update();
	double getSpeed() { return throttle.getSpeedInput_mmPs(); };
	void tick() { throttle.tick(millis()); };
	};

