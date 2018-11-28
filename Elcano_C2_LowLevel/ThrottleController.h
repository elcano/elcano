#pragma once
 

class ThrottleController{
private:
	int  currentThrottlePWM;
	double speedCyclometerInput_mmPs = 0;
	double PIDThrottleOutput_pwm;
	double desiredSpeed_mmPs = 0;
	PID speedPID;

	const double proportional_throttle = .0175;
	const double integral_throttle = .2;
	const double derivative_throttle = .00001;
	void engageThrottle(int input);
	void write(int address, int value);
	int dacAddress;
	int SelectAB;
	int SelectCD;
	void ThrottlePID(int desiredValue);
public:
	ThrottleController(double, double, int, int, int, int);
	~ThrottleController();
	void stop();
	double getSpeedInput_mmPs() { return speedCyclometerInput_mmPs; };
	double setSpeedInput_mmPs(double speedIn) {speedCyclometerInput_mmPs = speedIn;};
	void setDesiredSpeed__mmPs(int input) { ThrottlePID(input); };
	
};

