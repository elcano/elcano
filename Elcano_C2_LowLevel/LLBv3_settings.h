#pragma once

#define baud 115200   // baudrate for debugging with a host PC over USB serial

/* Pin declarations */
#define WATCHDOG            5   // keep-alive watchdog is routed to Power-on, and may be unused.
#define IRPT_ESTOP          A9  // physical E-Stop port
#define IRPT_CAN			A10 // Interrupt for can msg
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
#define RELAYInversion      false

//Wheel click interrupt (digitally high or low, referred to as an "odometer") and motor hall phases.
#define IRPT_WHEEL 			A8
#define IRPT_PHASE_A        A15
#define IRPT_PHASE_B        A14
#define IRPT_PHASE_C        A12
