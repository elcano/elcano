#pragma once
#include <Servo.h>
#include "PID_v1.h"


class SteeringController{
	Servo Steer_Servo;
	PID steerPID;
	double steerAngleUS;
	double PIDSteeringOutput_us;
	double desiredTurn_us;
	int32_t currentSteeringUS =0;
	void turnOn(int32_t input);
	void SteeringPID(int32_t input);
public:
	SteeringController();
	~SteeringController();
	void initialize(int32_t input);
	void engageSteering(int32_t input);
	void updateAngle(int32_t input) { steerAngleUS = input; };
	void setDesiredTurn(int32_t input) { SteeringPID(input); };
};
