#include <PID_v1.h>
#include <Servo.h>
#include <ElcanoSerial.h>
using namespace elcano;
/*
 * C2 is the low-level controller that sends control signals to the hub motor,
 * brake servo, and steering servo.  It is (or will be) a PID controller, but
 * may also impose limits on control values for the motor and servos as a safety
 * measure to protect against incorrect PID settings.
 *
 * It receives desired speed and heading from either of two sources, an RC
 * controller operated by a person, or the C3 pilot module.  These are mutually
 * exclusive.
 *
 * RC commands are received directly as interrupts on a bank of pins.  C3 commands
 * are received over a serial line using the Elcano Serial protocol.  Heading and
 * speed commands do not need to be passed through to other modules, but because
 * the Elcano Serial protocol uses a unidirectional ring structure, C2 may need to
 * pass through *other* commands that come from C3 but are intended for modules
 * past C2 on the ring.
 */

static struct hist {
  long olderSpeed_mmPs;  // older data
  unsigned long olderTime_ms;   // time stamp of older speed
  float currentSpeed_kmPh;
  long oldSpeed_mmPs;  // last data from the interrupt
  byte oldClickNumber;
  unsigned long oldTime_ms;  // time stamp of old speed
  long tickMillis;
  long oldTickMillis;
  byte nowClickNumber;  // situation when we want to display the speed
  unsigned long nowTime_ms;
  unsigned long TickTime_ms;  // Tick times are used to compute speeds
  unsigned long OldTick_ms;   // Tick times may not match time stamps if we don't process
  // results of every interrupt
} history;
//
// @ToDo: Are these specific to some particular setup or trike? If so,
// they should be moved to Settings.h.
// @ToDo: Constants do not need to be stored in memory. They can be #define symbols.
// It is not clear that the Arduino compiler will optimize away unchanging values
// even if not marked volatile.
// On Mega, TX must use d10-15, d50-53, or a8-a15 (62-69)
const int softwareTx = 10;  // to 7 segment LED display
const int softwareRx = 7;   // not used
//SoftwareSerial s7s(softwareRx, softwareTx);
// @ToDo: This has changed. Is it specific to some particular setup or trike?
// If so, it should be moved to Settings.h.
#define s7s Serial2
Servo STEER_SERVO;
Servo BRAKE_SERVO;
// 10 milliseconds -- adjust to accomodate the fastest needed response or
// sensor data capture.
#define LOOP_TIME_MS 10
#define ERROR_HISTORY 20 //number of errors to accumulate
//#define TEN_SECONDS_IN_MICROS 10000000
#define ULONG_MAX 4294967295

/*================ReadTurnAngle ================*/
// @ToDo: Are these specific to a particular trike? If so, move them to Settings.h.
// Value measured at analog input A2 from right steering column when wheels pointed straight ahead.
// An analog voltage can be 0 to 5V, which correspond to angles from 0 to 360 degrees.
// Analog input reads this as a value form 0 to 1024.
// We may want to mount the sensors so that straight is close to 500.
// This number is not critical, since actual wheel turn range is much less than 360
int RightStraight_A2 = 750; // Original value 181
// Value measured at analog input A3 from left steering column when wheels pointed straight ahead.
int LeftStraight_A3  = 914; // Original value 181
// Calibration constants
// Aangle (degrees) = (Value - RightStraight_A2) * RIGHT_DEGx1000pCOUNT / 1000
#define RIGHT_DEGx1000pCOUNT  333
#define LEFT_DEGx1000pCOUNT  333
// A correct sensor will produce a reading between these values
int Left_Min_Count = 850; // Real values are in setup().
int Left_Max_Count = 980;
int Right_Min_Count = 698;
int Right_Max_Count = 808;

double SteerAngle_wms = STRAIGHT_TURN_OUT; //Steering angle in microseconds used by writeMicroseconds function. Note: doubles on Arduinos are the same thing as floats, 4bytes, single precision
double PIDSteeringOutput; //Output from steerPID.Compute() in microseconds (used by Servo.writeMicroseconds())
double desiredAngle = STRAIGHT_TURN_OUT;
double steeringP = 1.5;
double steeringI = 1; 
double steeringD = .005;
int leftsenseleft;
int rightsenseleft;
int leftsenseright;
int rightsenseright;

static long distance_mm = 0;
// RC_rise contains the time value collected in the rising edge interrupts.
// RC_elapsed contains the width of the pulse. The rise and fall interrupts
// should alternate.
#define ProcessFallOfINT(Index)  RC_elapsed[Index]=(micros()-RC_rise[Index])
#define ProcessRiseOfINT(Index) RC_rise[Index]=micros()

// @ToDo: Do these differ per trike? If so, move to Settings.h.
const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)

volatile int rc_index = 0;
// This is a value that the RC controller can't produce.
#define INVALID_DATA 0L
// How many RC signals we receive
#define RC_NUM_SIGNALS 7
volatile unsigned long RC_rise[RC_NUM_SIGNALS];
volatile unsigned long RC_elapsed[RC_NUM_SIGNALS];
// This tells us when we have started receiving RC data. Until then, we
// ignore RC_rise and RC_elapsed.
volatile bool RC_Done[RC_NUM_SIGNALS];
volatile boolean synced = false;
volatile bool flipping;

long speed_errors[ERROR_HISTORY];
long old_turn_degx1000;
unsigned long calibrationTime_ms;

unsigned long stoppedTime_ms;
unsigned long straightTime_ms;
int  throttle_control = MIN_ACC_OUT;
int  brake_control = MIN_BRAKE_OUT;
int  steer_control = STRAIGHT_TURN_OUT;
float Odometer_m = 0;
float HubSpeed_kmPh;
const float  HubSpeed2kmPh = 13000000;
const unsigned long HubAtZero = 1159448;

int max_rc = MAX_RC;
int mid = MIDDLE;
int min_rc = MIN_RC;

// Time at which this loop pass should end in order to maintain a
// loop period of LOOP_TIME_MS.
unsigned long nextTime = millis();
// Time at which we reach the end of loop(), which should be before
// nextTime if we have set the loop period long enough.
unsigned long endTime;
// How much time we need to wait to finish out this loop pass.
unsigned long delayTime;

// Inter-module communications data.
SerialData Results;

ParseState parseState;

/*========================================================================/
  ============================WheelRev4 code==============================/
  =======================================================================*/

/* Wheel Revolution Interrupt routine
   Ben Spencer 10/21/13
   Modified by Tyler Folsom 3/16/14; 3/3/16
   A cyclometer gives a click once per revolution.
   This routine computes the speed.
*/

#define SerialOdoOut  Serial3
#define SerialMonitor Serial

#define MEG 1000000
#define MAX_SPEED_KPH 50
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
unsigned long MinTickTime_ms;
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 3000
// A speed of less than 0.3 KPH is zero.
unsigned long MaxTickTime_ms;
// MinTickTime_ms = 9239 ms = 9 sec

double SpeedCyclometer_mmPs = 0;
// Speed in revolutions per second is independent of wheel size.
float SpeedCyclometer_revPs = 0.0;//revolutions per sec

#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_SECOND 2
#define IRQ_RUNNING 3
#define NO_DATA 0x7FFFFFFF
volatile byte InterruptState = IRQ_NONE;  // Tells us if we have initialized.
volatile byte ClickNumber = 0;         // Used to distinguish old data from new.
volatile unsigned long TickTime = 0;  // Time from one wheel rotation to the next gives speed.
volatile unsigned long OldTick = 0;
int oldClickNumber;


/**
 * Throttle PID implementation
 */
double PIDThrottleOutput; //used to tell Throttle and Brake what to do as far as acceleration
double desiredSpeed = 2000.0; //aprox 10kph

//PID update frequency in milliseconds
#define PID_CALCULATE_TIME 50

double proportionalConstant = .0175;
double integralConstant = .0141;
double derivativeConstant = .00001;



