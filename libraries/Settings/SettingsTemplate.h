#ifndef _SITE_TRICYCLE_SETTINGS_HEADER_
#define _SITE_TRICYCLE_SETTINGS_HEADER_

/*
Settings.h is a site-specific set of trike parameters.
The file SettingsTemplate.h provides examples of how to define parameters,
and shows the necessary parameters.
Only Settings.h should be included from sketches.
Only SettingsTemplate.h should be saved in the git repository.

After downloading the elcano repository for the first time, copy
libraries/Settings/SettingsTemplate.h
to
libraries/Settings/Settings.h
Add your own trike definitions with a new VEHICLE_NUMBER, and set
VEHICLE_NUMBER to your own new number.
*/

// Dr. Folsom's orange trike.
//#define VEHICLE_NUMBER 1
// Yellow Catrike at the University of Washington at Bothell.
#define VEHICLE_NUMBER 2

#if (VEHICLE_NUMBER == 1)

// Parameters used by Elcano_C2_LowLevel
#define RC_SPEKTRUM 
#undef  RC_HITEC
//OUTPUT values -  0 to 255

// MIN and MAX ACC set the minimum signal to get the motor going, and maximum allowable acceleration for the motor
// Values (0-255) represent digital values that get convered into analog pwm signals sent to the motor
#define MIN_ACC_OUT 40
#define MAX_ACC_OUT 227
<<<<<<< HEAD

// MIN and MAX BRAKE_OUT set values to be sent to the brake actuator that controls the brakes on the front wheels
// Values (0-255) represent digital values that get convered into analog pwm signals
#define MIN_BRAKE_OUT 180
#define MAX_BRAKE_OUT 250

// RIGHT, STRAIGHT, and LEFT TURN_OUT set values to be sent to the steer actuator that changes the direction of the front wheels
// Values (0-255) represent digital values that get convered into analog pwm signals
#define RIGHT_TURN_OUT 146
#define LEFT_TURN_OUT 230
#define STRAIGHT_TURN_OUT 180

=======
#define MIN_BRAKE_OUT 167
#define MAX_BRAKE_OUT 207
#define RIGHT_TURN_OUT 146
#define LEFT_TURN_OUT 230
#define STRAIGHT_TURN_OUT 188
>>>>>>> 223c88c1c7eda80690319c5b6e952afe6e00f1ee
// Turn sensors are believed if they are in this range while wheels are straight
// MAX values here are for the safety of the actuator so as not to break/overload it
#define RIGHT_MIN_COUNT 80
#define RIGHT_MAX_COUNT 284
#define LEFT_MIN_COUNT  80
#define LEFT_MAX_COUNT  284

// Trike specific Physical perameters and pins/channels
#define DAC_CHANNEL 0    // output to motor actuator
#define STEER_OUT_PIN 7 // Output to steer actuator
#define BRAKE_OUT_PIN 6  // output to brake actuator
#define WHEEL_DIAMETER_MM 397
#define MOTOR_POLE_PAIRS 23

// Parameters used by MoveActuator
// external PWM output
#define DISK_BRAKE 6
// Use DAC A
#define THROTTLE_CHANNEL 0

// Parameters used by RC_Control_interrupts
// Indices for information stored in array RC_results
// Channel order differs for different vehicles
#define RC_TURN 1
#define RC_AUTO 2
#define RC_GO   3
#define RC_ESTP 4
#define RC_RDR  5
#define RC_RVS  6
#define NUMBER_CHANNELS 6

#endif

#if (VEHICLE_NUMBER == 2)

// Parameters used by Elcano_C2_LowLevel
#undef RC_SPEKTRUM 
#define  RC_HITEC
//OUTPUT values -  0 to 255
<<<<<<< HEAD

// MIN and MAX ACC set the minimum signal to get the motor going, and maximum allowable acceleration for the motor
// Values (0-255) represent digital values that get convered into analog pwm signals sent to the motor
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 227

// MIN and MAX BRAKE_OUT set values to be sent to the brake actuator that controls the brakes on the front wheels
// Values (0-255) represent digital values that get convered into analog pwm signals
#define MIN_BRAKE_OUT 128
#define MAX_BRAKE_OUT 254

// RIGHT, STRAIGHT, and LEFT TURN_OUT set values to be sent to the steer actuator that changes the direction of the front wheels
// Values (0-255) represent digital values that get convered into analog pwm signals
#define RIGHT_TURN_OUT 128
#define LEFT_TURN_OUT 254 
#define STRAIGHT_TURN_OUT 192

=======
#define MIN_ACC_OUT 40
#define MAX_ACC_OUT 227
#define MIN_BRAKE_OUT 167
#define MAX_BRAKE_OUT 207
#define RIGHT_TURN_OUT 146
#define LEFT_TURN_OUT 230
#define STRAIGHT_TURN_OUT 187
>>>>>>> 223c88c1c7eda80690319c5b6e952afe6e00f1ee
// Turn sensors are believed if they are in this range while wheels are straight
// MAX values here are for the safety of the actuator so as not to break/overload it
#define RIGHT_MIN_COUNT 725
#define RIGHT_MAX_COUNT 785
#define LEFT_MIN_COUNT  880
#define LEFT_MAX_COUNT  940

// Trike specific Physical perameters and pins/channels
#define DAC_CHANNEL 0
#define STEER_OUT_PIN 7 // Output to steer actuator
#define BRAKE_OUT_PIN 9  // output to brake actuator
#define WHEEL_DIAMETER_MM 500
#define MOTOR_POLE_PAIRS 23

// Parameters used by MoveActuator
// external PWM output
#define DISK_BRAKE 9
// Use DAC D
#define THROTTLE_CHANNEL 0


// Parameters used by RC_Control_interrupts
#define RC_AUTO 1 //
#define RC_ESTP 2
#define RC_RDR  3 //rudder, side to side on left stick (not used)
#define RC_GO   4 //
#define RC_TURN 5
#define NUMBER_CHANNELS 5
// Controller has no channel for RC_AUTO
<<<<<<< HEAD
#define RC_RVS  6 //reverse, not used on vehicle 2
//OUTPUT values -  0 to 255
#define DAC_CHANNEL 0
=======
#define RC_RVS  6
>>>>>>> 223c88c1c7eda80690319c5b6e952afe6e00f1ee

#endif

#endif