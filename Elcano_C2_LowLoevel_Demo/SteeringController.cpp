#include "Settings.h"
#include "SteeringController.h"
#include <Arduino.h>


SteeringController::SteeringController():
	steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT)
{

}
void SteeringController::initialize(){
    steerPID.SetOutputLimits(MIN_TURN, MAX_TURN);
  steerPID.SetSampleTime(PID_CALCULATE_TIME);
  steerPID.SetMode(AUTOMATIC);
  Steer_Servo.attach(STEER_OUT_PIN);
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
