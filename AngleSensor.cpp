#include "pin_settings.h"
#include "trike_settings.h"
#include "AngleSensor.h"
#include <Arduino.h>



AngleSensor::AngleSensor(){
	
}


AngleSensor::~AngleSensor()
{
}

double AngleSensor::getAngle() {
	int left = analogRead(LEFTTURNSENSOR);
	int right = analogRead(RIGHTTURNSENSOR);
	double angle = map(right, calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight, calibratedWheelMaxLeft_us, calibratedWheelMaxRight_us);
	return angle;
	
}

