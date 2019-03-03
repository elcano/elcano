#pragma once
#include "Settings.h"
#ifndef TESTING
#include <SPI.h>
#include "PID_v1.h" 
#endif

class ThrottleController{
private:
	int32_t currentThrottlePWM;
	double speedCyclometerInput_mmPs = 0;
	double PIDThrottleOutput_pwm;
	double desiredSpeed_mmPs = 0;

	PID speedPID;

	static const uint32_t MIN_TICK_TIME_ms = (WHEEL_CIRCUM_MM * 1000) / MAX_SPEED_mmPs;
	static const uint32_t MAX_TICK_TIME_ms = (WHEEL_CIRCUM_MM * 1000) / MIN_SPEED_mmPs;
	
	
	
	static volatile uint32_t tickTime_ms[2];
	uint32_t calcTime_ms[2];
	int32_t prevSpeed_mmPs;
	
	void write(int32_t address, int32_t value);
	void ThrottlePID(int32_t desiredValue);
	int32_t extrapolateSpeed();

public:
	
	ThrottleController();
	~ThrottleController();
    void initialize();
	void stop();
	void stop(int32_t strength);
    void engageThrottle(int32_t input);
	int32_t getSpeedInput_mmPs() { return speedCyclometerInput_mmPs; };
	int32_t setSpeedInput_mmPs(int32_t speedIn) {speedCyclometerInput_mmPs = speedIn;};
	void setDesiredSpeed__mmPs(int32_t input) { ThrottlePID(input); };
	void updateSpeed();
	static void tick();
};
