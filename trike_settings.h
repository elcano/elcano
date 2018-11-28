/*
Trike_settings.h is a set of trike specific parameters
*/



//PWM acceptable signal ranges

//motor
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 227

//brakes
#define MIN_BRAKE_OUT 128
#define MAX_BRAKE_OUT 254


//turning values
#define RIGHT_TURN_OUT 128
#define LEFT_TURN_OUT 254
#define STRAIGHT_TURN_OUT 192
#define WHEEL_MAX_RIGHT_US RIGHT_TURN_OUT
#define WHEEL_STRAIGHT_US STRAIGHT_TURN_OUT
#define WHEEL_MAX_LEFT_US LEFT_TURN_OUT

//Not sure what these are
//turn sensors are believed if they are in this range while wheels are straight
#define RIGHT_MIN_COUNT 725
#define RIGHT_MAX_COUNT 785
#define LEFT_MIN_COUNT 880
#define LEFT_MAX_COUNT 940

// Trike specific physical parameters
#define WHEEL_DIAMETER_MM 500
// Wheel Circumference
#define WHEEL_CIRCUM_MM WHEEL_DIAMETER_MM * PI
//Turning radius in cm.
#define TURN_RADIUS 214
#define MOTOR_POLE_PAIRS 23

// @@@ add 
#define MAX_SPEED_KPH 15
#define MEG 1000000
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
#define MIN_SPEED_mPh 3000
#define MIN_SPEED_mmPs ((MIN_SPEED_mPh * 1000.0) / 3600.0)

#define TURN_MAX_DEG 30

// Currently different interrupts are used for the motor phase feedback.
// These are *temporary*.
#define IRPT_MOTOR_FEEDBACK IRPT_RDR
