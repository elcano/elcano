#pragma once
class AngleSensor
{
private:
	static const int MAX_RIGHT = RIGHT_MAX_COUNT;
	static const int MIN_RIGHT = RIGHT_MIN_COUNT;
	static const int MAX_LEFT = LEFT_MAX_COUNT;
	static const int MIN_LEFT = LEFT_MIN_COUNT;
	int calibratedWheelMaxLeft_us; // CALIBRATED_WHEEL_MAX_LEFT_US
	int calibratedWheelStraight_us; // CALIBRATED_WHEEL_STRAIGHT_US
	int calibratedWheelMaxRight_us; // CALIBRATED_WHEEL_MAX_RIGHT_US

	int calibratedWheelSensorMaxLeft; // CALIBRATED_WHEEL_MAX_LEFT_US
	int calibratedWheelSensorStraight;
	int calibratedWheelSensorMaxRight; // CALIBRATED_WHEEL_MAX_RIGHT_US


public:
	AngleSensor();
	~AngleSensor();
	//@@Todo: getAngle() returns degree that the trike is turning
	double getAngle();
};

