#include "Settings.h"
#include "SteeringController.h"
#ifndef TESTING
#include <Arduino.h>
#include "PID_v1.h"
#endif

SteeringController::SteeringController():
	steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT)
{
	//Hacky fix for not burning servo circuit
  pinMode(STEER_ON, OUTPUT);
  digitalWrite(STEER_ON, RELAYInversion ? HIGH : LOW);

  steerPID.SetOutputLimits(MIN_TURN_MS, MAX_TURN_MS);
  steerPID.SetSampleTime(PID_CALCULATE_TIME);
  steerPID.SetMode(AUTOMATIC);

  Steer_Servo.attach(STEER_OUT_PIN);
  //part 2 of hacky fix
  
  //sets the current angle
  steerAngleUS = computeAngleRight();
  //maps to turn signal

  //sends the current signal to the servo
  engageSteering(steerAngleUS);
	
	//enable power
	digitalWrite(STEER_ON, RELAYInversion ? LOW : HIGH);
	if(DEBUG)
		Serial.println("Steering On");
	
	
}

SteeringController::~SteeringController()
{
}


int32_t SteeringController::update(int32_t desiredAngle) {
  desiredAngle=map(desiredAngle,MIN_TURN_Mdegrees,MAX_TURN_Mdegrees,MIN_TURN_MS,MAX_TURN_MS);
	if (USE_PIDS)
		SteeringPID(desiredAngle);
	else
		engageSteering(desiredAngle);
  steerAngleUS = computeAngleRight();
	return map(steerAngleUS, MIN_TURN_MS,MAX_TURN_MS,MIN_TURN_Mdegrees,MAX_TURN_Mdegrees);
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
	if (input > MAX_TURN_MS)
		input = MAX_TURN_MS;
	else if (input < MIN_TURN_MS)
		input = MIN_TURN_MS;
	if (currentSteeringUS != input) {
		if (DEBUG) {
			Serial.print("Steering: ");
			Serial.println(input);
		}
		Steer_Servo.writeMicroseconds(input);
    Serial.println("done");
		currentSteeringUS = input;
	}
}

int32_t SteeringController::computeAngleLeft() {
	int32_t val = analogRead(AngleSensorLeft);
	val = map(val, Left_Read_at_MIN_TURN, Left_Read_at_MAX_TURN, MIN_TURN_MS, MAX_TURN_MS);
	return val;
}

int32_t SteeringController::computeAngleRight() {
	int32_t val = analogRead(AngleSensorRight);
	val = map(val, Right_Read_at_MIN_TURN, Right_Read_at_MAX_TURN, MIN_TURN_MS, MAX_TURN_MS);
 //if(DEBUG){
 //Serial.print("Right sensor: ");
 //Serial.println(val);
 //}
	return val;
}



/*
This function is not connected to any part of the system
	ideally we want to use the two sensors readings and come to 
	a single 'trike turning angle' from the combination of the data
Before this can work at all, we need to measure the vehicle length and distance between the front wheels
	those values are stored in settings.h
We also need to do some kind of mapping from the voltage to a milli degree, and have some understanding
	of when the readings are meaningful and when they are out of range. 
	Right now we are just mapping the values given from the tape on the sensors to the mechanically limited steering range
	so that also needs to be adjusted
*/
int32_t computeTrikeAngle() {
	//read angle
	int32_t lAngle = analogRead(AngleSensorLeft);
	//convert from voltage to milli degrees
	//need to measure two data points from sensor to actual angle
	lAngle = map(lAngle, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
	int32_t r_mm = VEHICLE_LENGTH_MM / tan(lAngle * PI / 180);
	//How do we know which way the vehicle is turning? will one of these be reasonable and the other not?
	int32_t complAngle_goingLeft = atan(VEHICLE_LENGTH_MM / (r_mm + DISTANCE_BETWEEN_WHEELS_MM / 2));
	int32_t complAngle_goingRight = atan(VEHICLE_LENGTH_MM / (r_mm - DISTANCE_BETWEEN_WHEELS_MM / 2));
	//then do same thing with right sensor
	//how should we compare the two?
	//to do check units
		
}
