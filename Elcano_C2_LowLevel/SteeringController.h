#pragma once
#include <Servo.h>
#include "PID_v1.h"


class SteeringController{
	Servo Steer_Servo;
	PID steerPID;
	double steerAngleUS;
	double PIDSteeringOutput_us;
	double desiredTurn_us;
	int currentSteeringUS;


	void SteeringPID(int input);
public:
	SteeringController();
	~SteeringController();
	void updateAngle(double input) { steerAngleUS = input; };
	void setDesiredTurn(int input) { SteeringPID(input); };
};

