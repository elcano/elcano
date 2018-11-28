#pragma once
#ifndef pin_settings
#define pin_settings

// Trike specific pins/channels
#define DAC_CHANNEL 0
// Output to steer actuator
#define STEER_OUT_PIN 7
// Output to brake actuator
#define BRAKE_OUT_PIN 8
// Parameters used by MoveActuator
// external PWM output
// DISK_BRAKE is deprecated, use BRAKE_OUT_PIN
// Use DAC D
#define THROTTLE_CHANNEL 0
// The assignment of interrupts / pins used for inputs from the RC controller
// might differ depending on the trike.
// D2  = Int 0
#define IRPT_RVS 0
// D21 = Int 2
#define IRPT_TURN 2
// D20 = Int 3
#define IRPT_GO 3
// RDR (rudder) is not used. Instead, use this interrupt for the motor
// phase feedback, which gives speed.
// D18 = Int 5
#define IRPT_RDR 5
// D19 = Int 4
#define IRPT_ESTOP 4

#define IRPT_SWITCH 19

#define IRPT_WHEEL 3

// The DAC select pins are the same on all trikes.
const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)

#define LEFTTURNSENSOR A2 
#define RIGHTTURNSENSOR A3

//brakes
 const int LeftBrakeOnPin = 10;
 const int RightBrakeOnPin = 2;
 const int LeftBrakeVoltPin = 8;
 const int RightBrakeVoltPin = 7;
#endif