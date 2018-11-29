#pragma once
#include "PID_v1.h" 
#include <Arduino.h>
#include <SPI.h>
#define WHEEL_CIRCUM_MM 50
#define MAX_SPEED_mmPs 20
#define MIN_SPEED_mmPs 10


class ThrottleController{
private:
	int  currentThrottlePWM;
	double speedCyclometerInput_mmPs = 0;
	double PIDThrottleOutput_pwm;
	double desiredSpeed_mmPs = 0;
	PID speedPID;
	unsigned long MIN_TICK_TIME_ms = (WHEEL_CIRCUM_MM * 1000) / MAX_SPEED_mmPs;
	unsigned long MAX_TICK_TIME_ms = (WHEEL_CIRCUM_MM * 1000) / MIN_SPEED_mmPs;
	const double proportional_throttle = .0175;
	const double integral_throttle = .2;
	const double derivative_throttle = .00001;
	void engageThrottle(int input);
	void write(int address, int value);
	int dacAddress;
	int SelectAB;
	int SelectCD;
	void ThrottlePID(int desiredValue);

	volatile unsigned long tickTime_ms[2];
	volatile unsigned long calcTime_ms[2];
	double prevSpeed_mmPs;


public:
	ThrottleController(double, double, int, int, int, int);
	~ThrottleController();
	void stop();
	double getSpeedInput_mmPs() { return speedCyclometerInput_mmPs; };
	double setSpeedInput_mmPs(double speedIn) {speedCyclometerInput_mmPs = speedIn;};
	void setDesiredSpeed__mmPs(int input) { ThrottlePID(input); };
	void updateSpeed();
};

