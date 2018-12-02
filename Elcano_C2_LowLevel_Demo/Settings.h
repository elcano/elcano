#pragma once
#include <Arduino.h>


#ifndef ArduinoSettings
#define ArduinoSettings

//BAUD
const int baud  = 9600;

//PINS
#define STEER_OUT_PIN		8
#define DAC_CHANNEL 		0
#define SelectAB			53
#define SelectCD			49
#define AngleSensorRight	A2
#define AngleSensorLeft		A3

#define BrakeOnPin 			10
#define BrakeVoltPin 		8

//Wheel click interrupt
#define IRPT_WHEEL 			3


#endif

#ifndef MotorSettings
#define MotorSettings

//min/max acceleration
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 227

#define MAX_SPEED_KmPh 20

// max speed 
const double KmPh_mmPs = 1000000/3600;
const double MAX_SPEED_mmPs = MAX_SPEED_KmPh*KmPh_mmPs;

// min speed -> slower is interpreted as stopped
//set as 1% of the maximum speed -> 0.4 KmPh
const double MIN_SPEED_mmPs = 0.01*MAX_SPEED_mmPs;


#endif

#ifndef BrakeSettings
#define BrakeSettings

//not currently in use??
#define MIN_BRAKE_OUT 128
#define MAX_BRAKE_OUT 254
//time brakes are allowed to be high
const unsigned long MaxHi_ms = 800;

#endif

#ifndef TurnSettings
#define TurnSettings

#define MIN_TURN 70
#define MAX_TURN 130
#define CENTER_TURN 94



//Derived settings
#define WHEEL_MAX_RIGHT_US RIGHT_TURN_OUT
#define WHEEL_STRAIGHT_US STRAIGHT_TURN_OUT
#define WHEEL_MAX_LEFT_US LEFT_TURN_OUT

#endif

#ifndef VehicleSettings
#define VehicleSettings

#define WHEEL_DIAMETER_MM 500
//derived settings
const double WHEEL_CIRCUM_MM = (WHEEL_DIAMETER_MM * PI);

#endif

#ifndef PIDSettings
#define PIDSettings

#define PID_CALCULATE_TIME 50

// Motor PID
const double proportional_throttle = .0175;
const double integral_throttle = .2;
const double derivative_throttle = .00001;

// Steering PID
const double proportional_steering = .0175;
const double integral_steering = .5;
const double derivative_steering = .00001;

#endif

#ifndef AngleSensorRightSettings
#define AngleSenseorRightSettings

#define MIN_Right_Sensor 725
#define MAX_Right_Sensor 785

#endif

#ifndef AngleSensorLeftSettings
#define AngleSensorLeftSettings

#define MIN_Left_Sensor 880
#define MAX_Left_Sensor 940

#endif
