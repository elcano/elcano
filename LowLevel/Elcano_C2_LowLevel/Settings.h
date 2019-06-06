#pragma once


#define DEBUG true
#define USE_PIDS false
//Currently can be 3 or 1
#define LLBversion 3

#ifndef TESTING
#include <Arduino.h>
#endif

#if LLBversion == 3
#include "LLBv3_settings.h"
#else
#include "LLBv1_settings.h"
#endif



/*
Minimum and maximum values to send to the motor
Minimum and maximum speed allowed
(currently minimum is set to 1% of the maximum for no real reason)
*/

//min/max acceleration
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 227
#define MAX_SPEED_KmPh 20

// max speed 
const int32_t KmPh_mmPs = 1000000/3600;
const int32_t MAX_SPEED_mmPs = MAX_SPEED_KmPh*KmPh_mmPs;  //at 20KMPH this is roughly 5,555

// min speed -> slower is interpreted as stopped
//set as 1% of the maximum speed -> 0.2 KmPh
const int32_t MIN_SPEED_mmPs = 0.01*MAX_SPEED_mmPs;



/*
Time that the brakes can be high
*/

//not currently in use??
#define MIN_BRAKE_OUT 128
#define MAX_BRAKE_OUT 254
//time brakes are allowed to be high
const uint32_t MaxHi_ms = 800;



/*
Settings for the Steering 
Minimum/Maximum and center turning signals
*/
#define MIN_TURN_Mdegrees -24000
#define MAX_TURN_Mdegrees 25000
#define MIN_Right_Sensor 725
#define MAX_Right_Sensor 785
#define MIN_Left_Sensor 485
#define MAX_Left_Sensor 313
#define Left_Read_at_MIN_TURN  485
#define Right_Read_at_MIN_TURN 725
#define Left_Read_at_MAX_TURN 313
#define Right_Read_at_MAX_TURN 785
#define MIN_TURN_MS 1000
#define MAX_TURN_MS 1850




/*
Vehicle Data
Wheel Diameter, Turn Radius
*/

#define WHEEL_DIAMETER_MM 495.3
//derived settings
const int32_t WHEEL_CIRCUM_MM = (WHEEL_DIAMETER_MM * PI);

//needs to be measured, I just picked a number it makes no sense

//should be measured from the midpoint between the front wheel axles and the rear axle
#define VEHICLE_LENGTH_MM 5

#define DISTANCE_BETWEEN_WHEELS_MM 3

/*
PID tunning for steering and throttle
*/

#define PID_CALCULATE_TIME 50

// Motor PID
const double proportional_throttle = .0175;
const double integral_throttle = .2;
const double derivative_throttle = .00001;

// Steering PID
const double proportional_steering = .0175;
const double integral_steering = .5;
const double derivative_steering = .00001;



/*
Min is all the way turned to the left
*/

#define MIN_Right_Sensor 725
#define MAX_Right_Sensor 785
#define MIN_Left_Sensor 485
#define MAX_Left_Sensor 313
