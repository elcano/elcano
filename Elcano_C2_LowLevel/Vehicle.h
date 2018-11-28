#pragma once
#include "AngleSensor.h"
#include "Brakes.h"
#include "SteeringController.h"
#include "ThrottleController.h"
class Vehicle
{
public:
	//Sensors
	AngleSensor aSensor;
	Brakes brake = Brakes();
	volatile byte InterruptState;
	volatile unsigned long TickTime = 0;
	volatile unsigned long OldTick = 0;
	unsigned long MinTickTime_ms;
	unsigned long MaxTickTime_ms;
	float SpeedCyclometer_revPs = 0.0;
	hist history;

	int calibratedWheelMaxLeft_us; // CALIBRATED_WHEEL_MAX_LEFT_US
	int calibratedWheelStraight_us; // CALIBRATED_WHEEL_STRAIGHT_US
	int calibratedWheelMaxRight_us; // CALIBRATED_WHEEL_MAX_RIGHT_US

	int calibratedWheelSensorMaxLeft; // CALIBRATED_WHEEL_MAX_LEFT_US
	int calibratedWheelSensorStraight;
	int calibratedWheelSensorMaxRight; // CALIBRATED_WHEEL_MAX_RIGHT_US

public:
	Vehicle();
	~Vehicle();
	void update();
	double getAngle();
	double getSpeed();
	void initialize();
	void move(double Angle, double speed, double distance);
	void stop();
	int SteeringPID(int desiredValue);
	void compRevolution();
	long distance_mm = 0;
	};

