#pragma once
#include <Arduino.h>

const int baud  = 115200;   // baudrate for debugging with a host PC over USB serial

/* Pin declarations */
#define WATCHDOG            5   // keep-alive watchdog is routed to Power-on, and may be unused.
#define IRPT_ESTOP          A9  // physical E-Stop port
#define CAN_SS              49  // CANbus slave select

#define DAC_SS              48  // DAC slave select
#define DAC_CHANNEL 		0   // we only use channel A for the throttle. B is routed but unused.
#define SelectAB			53
//#define SelectCD	-- channels C and D are not defined for LLBv3, there is no second DAC.

#define STEER_OUT_PIN		6
#define STEER_ON			8
#define AngleSensorRight	A3
#define AngleSensorLeft		A2
// originally intended as a return ground, these analog pins are free and on the steering header.
#define AngleSensorRight_B  A7
#define AngleSensorLeft_B   A6

// Brakes, have relays for both on/off as well as selecting 12/24v power.
#define BrakeOnPin 			39
#define BrakeVoltPin 		37
#define RELAYInversion      true

//Wheel click interrupt (digitally high or low, referred to as an "odometer") and motor hall phases.
#define IRPT_WHEEL 			A8
#define IRPT_PHASE_A        A15
#define IRPT_PHASE_B        A14
#define IRPT_PHASE_C        A12

/*
Minimum and maximum values to send to the motor
Minimum and maximum speed allowed
(currently minimum is set to 1% of the maximum for no real reason)
*/

// min/max acceleration
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 227

#define MAX_SPEED_KmPh 20

// max speed
const double KmPh_mmPs = 1000000/3600;
const double MAX_SPEED_mmPs = MAX_SPEED_KmPh*KmPh_mmPs;

// min speed -> slower is interpreted as stopped
//set as 1% of the maximum speed -> 0.2 KmPh
const double MIN_SPEED_mmPs = 0.01*MAX_SPEED_mmPs;

/* Time that the brakes can be high */
//not currently in use??
#define MIN_BRAKE_OUT 128
#define MAX_BRAKE_OUT 254
//time brakes are allowed to be high
const unsigned long MaxHi_ms = 800;

/*
Settings for the Steering
Minimum/Maximum and center turning signals
*/
#define MIN_TURN 70
#define MAX_TURN 130
#define CENTER_TURN 94

//Derived settings
#define WHEEL_MAX_RIGHT_US RIGHT_TURN_OUT
#define WHEEL_STRAIGHT_US STRAIGHT_TURN_OUT
#define WHEEL_MAX_LEFT_US LEFT_TURN_OUT

/*
Vehicle Data
Wheel Diameter, Turn Radius
*/
#define WHEEL_DIAMETER_MM 500
//derived settings
const double WHEEL_CIRCUM_MM = (WHEEL_DIAMETER_MM * PI);

/* PID tunning for steering and throttle */
#define PID_CALCULATE_TIME 50

// Motor PID
const double proportional_throttle = .0175;
const double integral_throttle = .2;
const double derivative_throttle = .00001;

// Steering PID
const double proportional_steering = .0175;
const double integral_steering = .5;
const double derivative_steering = .00001;

/* Min is all the way turned to the left */
#define MIN_Right_Sensor 725
#define MAX_Right_Sensor 785
#define MIN_Left_Sensor 485
#define MAX_Left_Sensor 313
