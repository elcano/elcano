#include "Settings.h"
#include "SteeringController.h"
#ifndef TESTING
#include <Arduino.h>
#include "PID_v1.h"
#endif

SteeringController::SteeringController():
	steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT)
{
	usePids = false;
}

SteeringController::~SteeringController()
{
}

void SteeringController::initialize(){
  //Hacky fix for not burning servo circuit
  pinMode(STEER_ON, OUTPUT);
  digitalWrite(STEER_ON, RELAYInversion ? HIGH : LOW);

  steerPID.SetOutputLimits(MIN_TURN, MAX_TURN);
  steerPID.SetSampleTime(PID_CALCULATE_TIME);
  steerPID.SetMode(AUTOMATIC);

  Steer_Servo.attach(STEER_OUT_PIN);

  //part 2 of hacky fix
  
  //sets the current angle
  steerAngleUS = computeAngleLeft();
  //maps to turn signal
  int input = map(input, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
  //sends the current signal to the servo
  engageSteering(input);
	
	//enable power
	digitalWrite(STEER_ON, RELAYInversion ? LOW : HIGH);
	if(DEBUG)
		Serial.println("Steering On");
}

int32_t SteeringController::update(int32_t desiredAngle) {
	steerAngleUS = computeAngleLeft();
	if (usePids)
		SteeringPID(desiredAngle);
	else
		engageSteering(desiredAngle);
	return steerAngleUS;
}

//Private
void SteeringController::SteeringPID(int32_t input) {
	desiredTurn_us = input;
	steerPID.Compute();
	if (PIDSteeringOutput_us != currentSteeringUS) {
		Steer_Servo.writeMicroseconds(PIDSteeringOutput_us);
		currentSteeringUS = PIDSteeringOutput_us;
	}
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

int32_t SteeringController::computeAngleLeft() {
	int32_t val = analogRead(AngleSensorLeft);
	val = map(val, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
	return val;
}

int32_t SteeringController::computeAngleRight() {
	int32_t val = analogRead(AngleSensorRight);
	val = map(val, MIN_Right_Sensor, MAX_Right_Sensor, MIN_TURN, MAX_TURN);
	return val;
}
