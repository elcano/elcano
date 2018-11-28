#pragma once

class Vehicle{
private:
	Brakes brake;
	ThrottleController throttle;

public:
	double distance_mm;
	Vehicle();
	~Vehicle();
	void eStop();
	void move(double, double);
	void update();
	double getSpeed() { return throttle.getSpeedInput_mmPs(); };
	};

