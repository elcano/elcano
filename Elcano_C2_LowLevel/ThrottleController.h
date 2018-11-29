#pragma once
#include "Settings.h"
#include <SPI.h>
#include "PID_v1.h" 


class ThrottleController{
private:
	int  currentThrottlePWM;
	double speedCyclometerInput_mmPs = 0;
	double PIDThrottleOutput_pwm;
	double desiredSpeed_mmPs = 0;
	PID speedPID;
	const unsigned long MIN_TICK_TIME_ms = (WHEEL_CIRCUM_MM * 1000) / MAX_SPEED_mmPs;
	const unsigned long MAX_TICK_TIME_ms = (WHEEL_CIRCUM_MM * 1000) / MIN_SPEED_mmPs;
	void engageThrottle(int input);
	void write(int address, int value);
	void ThrottlePID(int desiredValue);

	volatile unsigned long tickTime_ms[2];
	volatile unsigned long calcTime_ms[2];
	double prevSpeed_mmPs;
	double extrapolateSpeed();

public:
	ThrottleController();
	~ThrottleController();
	void stop();
	double getSpeedInput_mmPs() { return speedCyclometerInput_mmPs; };
	double setSpeedInput_mmPs(double speedIn) {speedCyclometerInput_mmPs = speedIn;};
	void setDesiredSpeed__mmPs(int input) { ThrottlePID(input); };
	void updateSpeed();
	void tick(unsigned long tick){
		if ((tick - tickTime_ms[0]) > MIN_TICK_TIME_ms) {
			tickTime_ms[1] = tickTime_ms[0];
			tickTime_ms[0] = tick;
		}
	};
};

