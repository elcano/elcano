#ifndef _SITE_TRICYCLE_SETTINGS_HEADER_
#define _SITE_TRICYCLE_SETTINGS_HEADER_

/*
Settings.h is a site-specific set of trike parameters.
The file SettingsTemplate.h provides examples of how to define parameters,
and shows the necessary parameters.
Only Settings.h should be included from sketches, but not saved in the git repository.
Only SettingsTemplate.h should be saved in the git repository.

After downloading the elcano repository for the first time, copy
libraries/Settings/SettingsTemplate.h
to
libraries/Settings/Settings.h
Add your own trike definitions with a new VEHICLE_NUMBER, and set
VEHICLE_NUMBER to your own new number.
*/

// Dr. Folsom's orange trike.
#define VEHICLE_NUMBER 1
// Yellow Catrike at the University of Washington at Bothell.
//#define VEHICLE_NUMBER 2

#if (VEHICLE_NUMBER == 1)

// Parameters used by Elcano_C2_LowLevel
#define RC_SPEKTRUM 
#undef  RC_HITEC

// Values (0-255) represent digital values that are PWM signals used by analogWrite.

// MIN and MAX ACC set the minimum signal to get the motor going, and maximum allowable acceleration for the motor
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 235

// MIN and MAX BRAKE_OUT set values to be sent to the brake actuator that controls the brakes on the front wheels
#define MIN_BRAKE_OUT 202
#define MAX_BRAKE_OUT 250

// RIGHT, STRAIGHT, and LEFT TURN_OUT set values to be sent to the steer actuator that changes the direction of the front wheels
#define RIGHT_TURN_OUT 229
#define LEFT_TURN_OUT 127
#define STRAIGHT_TURN_OUT 178

// RIGHT, STRAIGHT, and LEFT TURN_MS are pulse widths in msec received from the RC controller
#define RIGHT_TURN_MS 1000
#define LEFT_TURN_MS 2000
#define STRAIGHT_TURN_MS 1500

// Turn sensors are believed if they are in this range while wheels are straight
// These numbers vary considerably, depending on which sensor angle is set to straight.
#define RIGHT_MIN_COUNT 80
#define RIGHT_MAX_COUNT 284
#define LEFT_MIN_COUNT  80
#define LEFT_MAX_COUNT  284

// Trike specific pins/channels
// Output to motor actuator
#define DAC_CHANNEL 0
// Output to steer actuator
#define STEER_OUT_PIN 9
// Output to brake actuator
#define BRAKE_OUT_PIN 6
// DISK_BRAKE is deprecated, use BRAKE_OUT_PIN

// Trike-specific physical parameters
#define WHEEL_DIAMETER_MM 482
// Wheel Circumference
#define WHEEL_CIRCUM_MM (long) (WHEEL_DIAMETER_MM * PI)
//		Turning radius in cm.
#define TURN_RADIUS_CM 209
//		Turning speed in degrees per ms.
#define TURN_SPEED_DPMS 29700
//		Smallest change in turning angle, in millidegrees
#define TURN_RESOLUTION_MDEG 60
//		Maximum turning angle, in degrees
#define TURN_MAX_DEG 30
//	Motor
#define MOTOR_POLE_PAIRS 23
// maximum allowed speed
#define MAX_SPEED_KPH 15

// Use DAC A
#define THROTTLE_CHANNEL 0

// Parameters used by RC_Control_interrupts
// Indices for information stored in array RC_results
// Channel order differs for different vehicles
#define RC_TURN 1
#define RC_AUTO 2
#define RC_GO   5
#define RC_ESTP 4
#define RC_RDR  5
#define RC_RVS  6
#define RC_BRAKE 3
#define NUMBER_CHANNELS 6

// The assignment of pins used for inputs from the RC controller
// might differ depending on the trike.
#define IRPT_RVS 2
#define IRPT_TURN 21 
#define IRPT_GO 19
//EStop is bound to Channel 5, the red switch on the left side of the controller
//Designed to be toggled on when we want to brake and 
#define IRPT_ESTOP 20
// RDR is used for use this interrupt for the motor
// phase feedback, which gives speed.
#define IRPT_RDR 3
//Switch is actually used for Channel 4(Aka rudder) on the controller
//takes in input from Channel 4 and uses it to control throttle and simple routines
#define IRPT_SWITCH 20

#define IRPT_BRAKE 18

#define IRPT_WHEEL 3

// Currently different interrupts are used for the motor phase feedback.
// These are *temporary*.
#define RC_MOTOR_FEEDBACK RC_RVS
#define IRPT_MOTOR_FEEDBACK IRPT_RVS

#endif

#if (VEHICLE_NUMBER == 2)

// Parameters used by Elcano_C2_LowLevel
#undef RC_SPEKTRUM 
#define  RC_HITEC

// Values (0-255) represent digital values that get converted into PWM signals by analogWrite.

// MIN and MAX ACC set the minimum signal to get the motor going, and maximum allowable acceleration for the motor
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 227

// MIN and MAX BRAKE_OUT set values to be sent to the brake actuator that controls the brakes on the front wheels
#define MIN_BRAKE_OUT 128
#define MAX_BRAKE_OUT 254

// RIGHT, STRAIGHT, and LEFT TURN_OUT set values to be sent to the steer actuator that changes the direction of the front wheels
#define RIGHT_TURN_OUT 128
#define LEFT_TURN_OUT 254 
#define STRAIGHT_TURN_OUT 192

// Turn sensors are believed if they are in this range while wheels are straight
#define RIGHT_MIN_COUNT 725
#define RIGHT_MAX_COUNT 785
#define LEFT_MIN_COUNT  880
#define LEFT_MAX_COUNT  940

// Trike specific pins/channels
#define DAC_CHANNEL 0
// Output to steer actuator
#define STEER_OUT_PIN 7
// Output to brake actuator
#define BRAKE_OUT_PIN 8

// Trike specific physical parameters
#define WHEEL_DIAMETER_MM 500
// Wheel Circumference
#define WHEEL_CIRCUM_MM WHEEL_DIAMETER_MM * PI
//Turning radius in cm.
#define TURN_RADIUS 214
#define MOTOR_POLE_PAIRS 23

// Parameters used by MoveActuator
// external PWM output
// DISK_BRAKE is deprecated, use BRAKE_OUT_PIN
// Use DAC D
#define THROTTLE_CHANNEL 0

// Parameters used by RC_Control_interrupts
#define RC_AUTO 1
#define RC_ESTP 2
#define RC_RDR  3
#define RC_GO   4
#define RC_TURN 5
// Controller has no channel for RC_AUTO
// Reverse, not used on vehicle 2
#define RC_RVS  6
#define NUMBER_CHANNELS 5

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

// Currently different interrupts are used for the motor phase feedback.
// These are *temporary*.
#define RC_MOTOR_FEEDBACK RC_RDR
#define IRPT_MOTOR_FEEDBACK IRPT_RDR

#endif

// These values are specific to the RC controller in use. Only values that are
// independent of the trike should go here.
// Note we observed that the midpoint and range differs for each axis of each stick,
// so we may need to add min, max, middle values for each.
// If we have more controllers, and they are interchangeable, or if assigning names
// becomes unwieldy, or if we have controllers of the same type with different
// configuration settings, then we could consider assigning a controller number
// similar to how we assign the trike number. For instance, if we find that
// Spektrum controllers have sticks that "slip" and where the min, max, and middle
// may change, then we'd want to identify specific controllers to set their current
// values. Note also that ephermal values that change often should be changed
// directly in Settings.h -- those changes do not need to be made in
// SettingsTemplate.h.
// The interrupt or pin numbers aren't here, as those depend on wiring on the trike.

#ifdef RC_SPEKTRUM

// @ToDo: These are not correct. We have observed that the values differ per
// stick and per axis. Also, they vary over time, as though something is slipping
// in the stick mechanism. We may need a calibration procedure to run periodically.
// Since changes in these values are ephemral, changes should not be made here,
// but only in Settings.h.
// RC input values - pulse widths in milliseconds
#define DEAD_ZONE 75
// was 1322; new stable value = 1510
#define MIDDLE 1510
// extremes of RC pulse width in millisec
// was 911:
#define MIN_RC 1090
// was 1730:
#define MAX_RC 1930

#endif

#ifdef RC_HITEC

// RC input values - pulse widths in milliseconds
#define DEAD_ZONE 75
#define MIDDLE 1380
// extremes of RC pulse width in millisec
#define MIN_RC 960
#define MAX_RC 1800

#endif

#endif