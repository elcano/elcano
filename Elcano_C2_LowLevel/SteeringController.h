#pragma once
#include <Servo.h>
#include "PID_v1.h"


class SteeringController{
	Servo Steer_Servo;
	PID steerPID;
	double steerAngleUS;
	double PIDSteeringOutput_us;
	double desiredTurn_us;
	int currentSteeringUS =0;
	void turnOn(double input);
	void SteeringPID(int input);
public:
	SteeringController();
	~SteeringController();
	void initialize(double input);
	void engageSteering(double input);
	void updateAngle(double input) { steerAngleUS = input; };
	void setDesiredTurn(int input) { SteeringPID(input); };
};
