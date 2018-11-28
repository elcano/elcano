#include "PID_v1.h"  
#include <Servo.h>
#include "SteeringController.h"
#include <Arduino.h>


SteeringController::SteeringController(double min_turn_out, double max_turn_out, int sampleTime, int servoPin) :
	steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT)
{
	steerPID.SetOutputLimits(min_turn_out, max_turn_out);
	steerPID.SetSampleTime(sampleTime);
	steerPID.SetMode(AUTOMATIC);
}

SteeringController::~SteeringController()
{
}

void SteeringController::SteeringPID(int input) {
	desiredTurn_us = input;
	steerPID.Compute();
	if (PIDSteeringOutput_us != currentSteeringUS) {
		Steer_Servo.writeMicroseconds(PIDSteeringOutput_us);
		currentSteeringUS = PIDSteeringOutput_us;
	}
}