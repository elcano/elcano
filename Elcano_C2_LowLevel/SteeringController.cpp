#include "Settings.h"
#include "SteeringController.h"
#ifndef TESTING
#include <Arduino.h>
#include "PID_v1.h"
#endif

SteeringController::SteeringController():
	steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT)
{

}

SteeringController::~SteeringController()
{
}

void SteeringController::initialize(int32_t input){
  
  //Hacky fix for not burning servo circuit
  pinMode( STEER_ON, OUTPUT);
  digitalWrite(STEER_ON, RELAYInversion ? HIGH : LOW);

  
  steerPID.SetOutputLimits(MIN_TURN, MAX_TURN);
  steerPID.SetSampleTime(PID_CALCULATE_TIME);
  steerPID.SetMode(AUTOMATIC);
  Steer_Servo.attach(STEER_OUT_PIN);


  turnOn(input);
}

void SteeringController::engageSteering(int32_t input) {
	if (input > MAX_TURN)
		input = MAX_TURN;
	else if (input < MIN_TURN)
		input = MIN_TURN;
	if (currentSteeringUS != input) {
		if (DEBUG) {
			Serial.print("Steering: ");
			Serial.println(input);
		}
		Steer_Servo.write(input);
		currentSteeringUS = input;
	}
}

//Private
void SteeringController::turnOn(int32_t input) {
	//sets the current angle
	updateAngle(input);
	//maps to turn signal
	input = map(input, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
	//sends the current signal to the servo
	engageSteering(input);
	
	//enable power
	digitalWrite(STEER_ON, RELAYInversion ? LOW : HIGH);
	if(DEBUG)
		Serial.println("Steering On");
	

}

void SteeringController::SteeringPID(int32_t input) {
	desiredTurn_us = input;
	steerPID.Compute();
	if (PIDSteeringOutput_us != currentSteeringUS) {
		Steer_Servo.writeMicroseconds(PIDSteeringOutput_us);
		currentSteeringUS = PIDSteeringOutput_us;
	}
}
