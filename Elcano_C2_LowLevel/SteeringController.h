#pragma once
#ifndef TESTING
#include <Servo.h>
#include "PID_v1.h"
#endif

class SteeringController{
	Servo Steer_Servo;
	PID steerPID;
	double steerAngleUS;
	double PIDSteeringOutput_us;
	double desiredTurn_us;
	int32_t currentSteeringUS =0;
	void SteeringPID(int32_t input);
	int32_t computeAngleLeft();
	int32_t computeAngleRight();
	bool usePids;
	void engageSteering(int32_t input);
public:
	SteeringController();
	~SteeringController();
	void initialize();
	int32_t update(int32_t desiredAngle);
	void changePIDuse(bool usePids) { usePids = usePids; }
};
