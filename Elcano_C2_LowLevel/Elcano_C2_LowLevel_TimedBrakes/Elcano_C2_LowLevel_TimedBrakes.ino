#include <Settings.h>

#include <ElcanoTimedBrake.h>
#include <PID_v1.h>
#include <SPI.h>
#include <Elcano_Serial.h>
#include <Servo.h>
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

static double distance = 0;
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

//==========================================================================================
void ISR_TURN_rise(){
  noInterrupts();
  ProcessRiseOfINT(RC_TURN);
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
// RDR (rudder) is currently not used.
void ISR_RDR_rise() {
  // RDR (rudder) is not used. Instead, use this interrupt for the motor phase feedback, which gives speed.
  noInterrupts();
  ProcessRiseOfINT(RC_RDR);
  attachInterrupt(digitalPinToInterrupt(IRPT_RDR), ISR_RDR_fall, FALLING);
  // The phase frequency is proportional to wheel rotation.
  // An e-bike hub is powered by giving it 3 phased 36 V lines
  // The e-bike controller needs feeback from the hub.
  // The hub supplies 3 Hall Phase sensors; each is a 5V square wave and tells how fast the wheel rotates.
  // The square wave feedback has sone noise, which is cleaned up by an RC low pass filter
  //  with R = 1K, C = 100 nF
  interrupts();
}

void ISR_RDR_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_RDR);
  RC_Done[RC_TURN] = 1;
  //Serial.println("TURN");
  attachInterrupt(digitalPinToInterrupt(IRPT_RDR), ISR_RDR_rise, RISING);
  interrupts();
}

/*---------------------------------------------------------------------------------------*/
//Now used for Brakes 
void ISR_BRAKE_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_BRAKE);
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
//Should be bound to the red switch
void ISR_ESTOP_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_ESTP);
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_RVS_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_RVS);
  attachInterrupt(digitalPinToInterrupt(IRPT_RVS), ISR_RVS_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_TURN_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_TURN);
  RC_Done[RC_TURN] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_BRAKE_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_BRAKE);
  RC_Done[RC_BRAKE] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_ESTOP_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_ESTP);
  RC_Done[RC_ESTP] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_RVS_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_RVS);
  RC_Done[RC_RVS] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_RVS), ISR_RVS_rise, RISING);
  interrupts();
}

void ISR_GO_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_GO);
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_fall, FALLING);
  interrupts();
}

void ISR_GO_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_GO);
  RC_Done[RC_RDR] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
// An e-bike hub motor is powered by giving it 3 phase power. This is supplied
// by the motor controller. The controller needs feeback from the hub.  It
// receives three feedback signals from the motor, one for each phase.  These
// are the *actual* phase rotation of the motor, not the phases supplied by the
// controller, which differ from what the motor is currently doing whenever the
// controller is attempting to speed up or slow down or change direction of the
// motor. The motor phase feedback pulse rate is an integer multiple of wheel
// rotation rate. The factor is the number of pole pairs in the motor. Note
// that the controller has no idea how many pole pairs the motor has, so it
// only controls the phase rate, not the actual rotation rate.  It is "someone
// else's problem" to limit the actual rotation rate or speed. The feedback
// signals are 5V square waves.  They have some noise -- spikes of several
// volts, which exceeds the maximum voltage allowed for Arduino inputs, which
// is 5.5V.  The signal is cleaned up by an RC low pass filter with R = 1K,
// C = 100 nF.
void ISR_MOTOR_FEEDBACK_rise() {
  noInterrupts();
  // This differs from the other interrupt routines since we need the *cycle*
  // duration, not the width of the high pulse.  So here, we get the time from
  // the previous rising edge to the current rising edge.  Q: Are we properly
  // ignoring the first value from all of these interval computations?
  unsigned long old_phase_rise = RC_rise[RC_MOTOR_FEEDBACK];
  ProcessRiseOfINT(RC_MOTOR_FEEDBACK);
  RC_elapsed[RC_MOTOR_FEEDBACK] = RC_rise[RC_MOTOR_FEEDBACK] - old_phase_rise;
  interrupts();
}

/*---------------------------------------------------------------------------------------*/
void setup()
{ //Set up pins
  STEER_SERVO.attach(STEER_OUT_PIN);

  // SPI: set the slaveSelectPin as an output:
  pinMode (SelectAB, OUTPUT);
  pinMode (SelectCD, OUTPUT);
  pinMode (10, OUTPUT);
  SPI.setDataMode( SPI_MODE0);
  SPI.setBitOrder( MSBFIRST);
  // initialize SPI:
  // The following line should not be neccessary. It uses a system library.
  PRR0 &= ~4; // turn off PRR0.PRSPI bit so power isn't off
  SPI.begin();
  for (int channel = 0; channel < 4; channel++){
    DAC_Write(channel, 0); // reset did not clear previous states
  }
  // put vehicle in initial state
  steer(STRAIGHT_TURN_OUT);
  setupBrake();
  moveVehicle(MIN_ACC_OUT);
  delay(500);   // let vehicle stabilize
  Serial.begin(9600);
  rc_index = 0;
  for (int i = 0; i < RC_NUM_SIGNALS; i++)
  {
    RC_rise[i] = INVALID_DATA;
    RC_elapsed[i] = INVALID_DATA;
    RC_Done[i] = 0;
  }
  for (int i = 0; i < ERROR_HISTORY; i++)
  {
    speed_errors[i] = 0;
  }

  setupWheelRev(); // WheelRev4 addition
  CalibrateTurnAngle(32, 20);
  calibrationTime_ms = millis();
  
        attachInterrupt(digitalPinToInterrupt(IRPT_TURN),  ISR_TURN_rise,  RISING);//turn right stick l/r turn
        attachInterrupt(digitalPinToInterrupt(IRPT_GO),    ISR_GO_rise,    RISING);//left stick l/r
        attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);//ebrake
        attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);//left stick u/d mode select
//        attachInterrupt(digitalPinToInterrupt(IRPT_MOTOR_FEEDBACK), ISR_MOTOR_FEEDBACK_rise, RISING);
  long unsigned state = 0;
  brake(false);
}



/*---------------------------------------------------------------------------------------*/

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

void loop() {
  brake(false);
  computeSpeed(&history);
  // Get the next loop start time. Note this (and the millis() counter) will
  // roll over back to zero after they exceed the 32-bit size of unsigned long,
  // which happens after about 1.5 months of operation (should check this).
  // But leave the overflow computation in place, in case we need to go back to
  // using the micros() counter.
  // If the new nextTime value is <= LOOP_TIME_MS, we've rolled over.
  Throttle_PID(25);
  nextTime = nextTime + LOOP_TIME_MS;
  //Throttle_PID(14 - history.currentSpeed_kmPh);
  byte automate = processRC();
  // @ToDo: Verify that this should be conditional. May be moot if it is
  // replaced in the conversion to the new Elcano Serial protocol.
  if (automate == 0x01)
  {
    processHighLevel(&Results);
  }

  // @ToDo: What is this doing?
  Results.kind = MsgType::sensor;
  Results.angle_mDeg = TurnAngle_degx10() / 10;
  // @ToDo: Is this working and should it be uncommented?
  
  calibrationTime_ms += LOOP_TIME_MS;
  straightTime_ms = (steer_control == STRAIGHT_TURN_OUT) ? straightTime_ms + LOOP_TIME_MS : 0;
  stoppedTime_ms = (throttle_control == MIN_ACC_OUT) ? stoppedTime_ms + LOOP_TIME_MS : 0;
  if (calibrationTime_ms > 40000 && straightTime_ms > 3000 && stoppedTime_ms > 3000)
  {
    CalibrateTurnAngle(16, 10);  // WARNING: No response to controls while calibrating
    calibrationTime_ms = 0;
  }

  // @ToDo: What information do we need to send to C6? Is that communication already
  // hiding in here somewhere, or does it need to be added?

  // Figure out how long we need to wait to reach the desired end time
  // for this loop pass. First, get the actual end time. Note: Beyond this
  // point, there should be *no* more controller activity -- we want
  // minimal time between now, when we capture the actual loop end time,
  // and when we pause til the desired loop end time.
  endTime = millis();
  delayTime = 0L;

  // Did the millis() counter or nextTime overflow and roll over during
  // this loop pass? Did the loop's processing time overrun the desired
  // loop period? We have different computations for the delay time in
  // various cases:
  if ((nextTime >= endTime) &&
      (((endTime < LOOP_TIME_MS) && (nextTime < LOOP_TIME_MS)) ||
       ((endTime >= LOOP_TIME_MS) && (nextTime >= LOOP_TIME_MS)))) {
    // 1) Neither millis() nor nextTime rolled over --or-- millis() rolled
    // over *and* nextTime rolled over when we incremented it. For this case,
    // endTime and nextTime will be in their usual relationship, with
    // nextTime >= endTime, and both nextTime and endTime are either greater
    // than the desired loop period, or both are smaller than that.
    // In this case, we want a delayTime of nextTime - endTime here.
    delayTime = nextTime - endTime;
  } else {
    // (We get here if:
    // nextTime < endTime -or- exactly one of nextTime or endTime rolled over.
    // Negate the first if condition and use DeMorgan's laws...
    // Now pick out the "nextTime rolled over" case. We don't need to test both
    // nextTime and endTime as we know only one rolled over.)
    if (nextTime < LOOP_TIME_MS) {
      // 2) nextTime rolled over when we incremented it, but the millis() timer
      // hasn't yet rolled over.
      // In this case, we know we didn't exhaust the loop time, and the time we
      // need to wait is the remaining time til millis() will roll over, i.e.
      // from endTime until the max long value, plus the time from zero to
      // nextTime.
      delayTime = ULONG_MAX - endTime + nextTime;
    } else {
      // (We get here if:
      // nextTime < endTime -or-
      // nextTime >= endTime -and- nextTime did not roll over but endTime did.)
      // What remains are these two cases:
      // 3) nextTime hasn't rolled over, but millis() has.
      // In this case, we overran the loop time. Since millis() has rolled over,
      // we can just use the normal overrun fixup. So combine this with...
      // 4) Neither nextTime nor millis rolled over, but we overran the desired
      // loop period.
      // In this case, we have no delay, but instead extend the allowed time for
      // this loop pass to the actual time it took.
      nextTime = endTime;
      delayTime = 0L;
    }
  }
  
  // Did we spend long enough in the loop that we should immediately start
  // the next pass?
  if (delayTime > 0L) {
    // No, pause til the next loop start time.
    delay(delayTime);
  }
}


/*---------------------------------------------------------------------------------------*/
void Print7headers (bool processed)
{
  processed ? Serial.print("processed data \t") : Serial.print("received data \t");
#ifdef RC_SPEKTRUM
  Serial.print("Time\t");
  Serial.print("TURN\t");
  Serial.print("AUTO\t");
  Serial.print("GO\t");
  Serial.print("Rudder\t");
  Serial.print("E-Stop\t");
  Serial.println("Reverse\t");
#endif

#ifdef RC_HITEC
  Serial.print("Time\t");
  Serial.print("TURN\t");
  Serial.print("AUTO\t");
  Serial.print("GO\t");
  Serial.print("E-Stop\t");
  Serial.print("Rudder\t");
  Serial.println("Reverse\t");
#endif
}
/*---------------------------------------------------------------------------------------*/
void Print7 (bool processed, unsigned long results[7])
{

  processed ? Serial.print("processed data \t") : Serial.print("received data \t");
  Serial.print(results[0]); Serial.print("\t");
  Serial.print(results[1]); Serial.print("\t");
  Serial.print(results[2]); Serial.print("\t");
  Serial.print(results[3]); Serial.print("\t");
  Serial.print(results[4]); Serial.print("\t");
  Serial.print(results[5]); Serial.print("\t");
  Serial.println(results[6]);
}
/*---------------------------------------------------------------------------------------*/
void LogData(unsigned long commands[7], SerialData *sensors)  // data for spreadsheet
{
  show7seg(HubSpeed_kmPh);
  Serial.print(millis()); Serial.print("\t");                        //(ms) Time
  Serial.print(sensors->speed_cmPs); Serial.print("\t");             //(cm/s) Speed
  Serial.print(sensors->speed_cmPs * 36.0 / 1000.); Serial.print("\t"); //(km/h) Speed
  Serial.print(HubSpeed_kmPh); Serial.print("\t");                   //(km/h) Hub Speed
  Serial.print(sensors->angle_mDeg); Serial.print("\t");              //(deg) Angle
  int right = analogRead(A3);
  int left = analogRead(A2);
  Serial.print(right); Serial.print("\t");                           //Right turn sensor
  Serial.print(left); Serial.print("\t");                            //Left turn sensor
  Serial.print(throttle_control); Serial.print("\t");                //Throttle
  Serial.print(brake_control); Serial.print("\t");                   //Brake
  Serial.print(steer_control); Serial.print("\t");                   //Steer
  Serial.println(Odometer_m);                                        //(m) Distance
}
/*---------------------------------------------------------------------------------------*/
void PrintHeaders()
{
  Serial.print("(ms) Time\t");
  Serial.print("(cm/s) Speed\t");
  Serial.print("(km/h) Speed\t");
  Serial.print("(km/h) Hub Speed\t");
  Serial.print("(deg) Angle\t");
  Serial.print("Right\t");
  Serial.print("Left\t");
  Serial.print("Throttle\t");
  Serial.print("Brake\t");
  Serial.print("Steer\t");
  Serial.println("(m) Distance");
}



// Moves the vehicle a fixed distance at 14 km/h
// currently overshoots by about 48 meters
bool moveFixedDistance(double length_m, double desiredSpeed){
  if(length_m < 0) length_m = 0;        // ensures a negative value isn't given, as this will cause an infinite loop
  double start = distance;
  while(distance < length_m  + start){  // go until the total distance travaled has increased by the desired distance 
    computeSpeed(&history);
    Throttle_PID(desiredSpeed - history.currentSpeed_kmPh);
    if(checkEbrake()) return false;
    Serial.println(distance);
  }
  brake(true);
  moveVehicle(0);
  delay(1000);
  brake(false);
  return true;
}


//circleRoutine
void circleRoutine() {
  steer(LEFT_TURN_OUT);
  delay(1000);
  double desiredSpeed = 14;
  moveFixedDistance(TURN_CIRCUMFERENCE_CM, desiredSpeed);
  steer(STRAIGHT_TURN_OUT);
}

void figure8Routine(){
  Serial.println("RUNNING FIGURE 8");
  double desiredSpeed = 14;
  for(int i = 0; i < 2; i++)
  {
    // Make a left circleRoutine for 2/3 the circumference
    steer(LEFT_TURN_OUT);
    delay(1000);
    if(!moveFixedDistance((2/3.0) * TURN_CIRCUMFERENCE_CM, desiredSpeed)) break;
  
    // Move straight for 5 m
    steer(STRAIGHT_TURN_OUT);
    delay(1000);
    if(!moveFixedDistance(50, desiredSpeed)) break;
  
    // Make a right circleRoutine for 2/3 the circumference
    steer(RIGHT_TURN_OUT);
    delay(1000);
    if(!moveFixedDistance((2/3.0) * TURN_CIRCUMFERENCE_CM, desiredSpeed)) break;
  
    // Move straight for 5 m
    steer(STRAIGHT_TURN_OUT);
    delay(1000);
    if(!moveFixedDistance(50, desiredSpeed)) break;
  }
}

/*---------------------------------------------------------------------------------------*/
//squareRoutine
void squareRoutine(unsigned long sides, unsigned long &rcAuto) {
  Serial.println("Starting square routine...");
  rcAuto = HIGH;
  long straightSpeed = 2750;        //mmPs
  long turnSpeed = 1800;            //mmPs
  sides = sides * 1000;             //convert side length to mm
  unsigned long sideSec = sides / straightSpeed;  //calculate seconds per side at set speed
  sideSec = sideSec * 1000;         //convert to ms
  unsigned long loopTime;           //start time of while loops for throttle
  float turnDist = TURN_RADIUS_CM * PI / 2 * 10; //turnDist == 1/4 turn circumference in mm
  unsigned long turnSec = (long)turnDist / turnSpeed * 1000; //turnSec == time for 90-degree turn at 1250 mmPs
  for(int i = 0; i < 4; i++){
    steer(STRAIGHT_TURN_OUT);
    brake(MAX_BRAKE_OUT);
    delay(1000);
    brake(MIN_BRAKE_OUT);
    delay(100);
    loopTime = millis();
    while (millis() < (loopTime + sideSec)) {
      moveVehicle(112);
    }
    
    steer(LEFT_TURN_OUT);
    brake(MAX_BRAKE_OUT);
    delay(1000);

    brake(MIN_BRAKE_OUT);
    delay(100);
    loopTime = millis();
    while (millis() < (loopTime + turnSec)) {
      moveVehicle(96);
    }
  }
  brake(MAX_BRAKE_OUT);
  delay(1000);
  brake(MIN_BRAKE_OUT);
  rcAuto = LOW;
}

// Turns the wheels straight and stops the vehicle
// Will not work correctly until wheel with built in speedometer is installed
void allStop()
{
  steer(STRAIGHT_TURN_OUT);
  while(history.currentSpeed_kmPh > .01) // error of .01 kmph
  {
    computeSpeed(&history);
    Serial.println(history.currentSpeed_kmPh);
    Throttle_PID(0 - history.currentSpeed_kmPh);
  }
}

bool checkEbrake()
{
    if (RC_Done[RC_ESTP]) //RC_Done determines if the signal from the remote controll is done processing
  {
    RC_elapsed[RC_ESTP] = (RC_elapsed[RC_ESTP] > MIDDLE ? HIGH : LOW);
  
    if (RC_elapsed[RC_ESTP] == HIGH)
    {
      E_Stop();  // already done at interrupt level
      return true;
    }
  }
  return false;
}


// @ToDo: Q: What do the expressions "1st pulse", etc. mean? Is this a
// leftover from trying to combine the RC controls into a single stream?
/*---------------------------------------------------------------------------------------*/
byte processRC()
{
  //RC_TURN, RC_ESTOP, RC_BRAKE, RC_AUTO
  boolean autoMode = false; // Once RC_AUTO is implemented, this will default to false
  //ESTOP
  
  checkEbrake();
  
  autoMode = isAutomatic();
  
  if(autoMode){
    doAutoMovement();
  }
  else // not in autonomous mode
  {
    doManualMovement();
  }
  return 0x00;
}

boolean isAutomatic(){
    if(RC_Done[RC_BRAKE]){
      if(RC_elapsed[RC_BRAKE] > MIDDLE + TICK_DEADZONE){
        return true;           // It is manual control and not autonomous control
      }
    }
    return false;
}

void doAutoMovement(){
  if(RC_elapsed[RC_BRAKE] > TICK1 - TICK_DEADZONE && RC_elapsed[RC_BRAKE] < TICK1 + TICK_DEADZONE)
  {
    Serial.println("AT TICK 1");
    delay(1000); // delay and if statement ensure that the remote wasn't simply going past the tick
    if(RC_elapsed[RC_BRAKE] > TICK1 - TICK_DEADZONE && RC_elapsed[RC_BRAKE] < TICK1 + TICK_DEADZONE)
    {
      moveFixedDistance(300, 15);
    }
  }
  else if(RC_elapsed[RC_BRAKE] > TICK2 - TICK_DEADZONE && RC_elapsed[RC_BRAKE] < TICK2 + TICK_DEADZONE){
    long unsigned state = 0;
    Serial.println("AT TICK 2");
    delay(1000); // delay and if statement ensure that the remote wasn't simply going past the tick
    if(RC_elapsed[RC_BRAKE] > TICK2 - TICK_DEADZONE && RC_elapsed[RC_BRAKE] < TICK2 + TICK_DEADZONE)
    {
      circleRoutine();
    }
  }
  else if(RC_elapsed[RC_BRAKE] > TICK3 - TICK_DEADZONE && RC_elapsed[RC_BRAKE] < TICK3 + TICK_DEADZONE){
    //another routine
    Serial.println("AT TICK 3");
    delay(1000); // delay and if statement ensure that the remote wasn't simply going past the tick
    if(RC_elapsed[RC_BRAKE] > TICK3 - TICK_DEADZONE && RC_elapsed[RC_BRAKE] < TICK3 + TICK_DEADZONE)
    {
      figure8Routine();
    }
  }
}

void applySpeed(float speed_kph)
{
//  Throttle_PID
}

void doManualMovement(){
  //THROTTLE
    //TODO: if less than the middle, reverse, otherwise forward
    if(RC_Done[RC_GO])
    {
      
      if(RC_elapsed[RC_GO] < MIDDLE){
        //moveVehicle(convertThrottle(RC_elapsed[RC_GO]));
      }
      else{
        // apply brakes or reverse
      }
      //Serial.println(RC_elapsed[RC_GO]);
      //moveVehicle(convertThrottle(RC_elapsed[RC_GO]));
    }
  //TURN
    if (RC_Done[RC_TURN]) 
    {
      Serial.println(String(convertTurn(RC_elapsed[RC_TURN])));
      steer(convertTurn(RC_elapsed[RC_TURN]));
    }
}

/*---------------------------------------------------------------------------------------*/
void processHighLevel(SerialData * results)
{
  //Steer
  int turn_signal = convertDeg(results->angle_mDeg);
  steer(turn_signal);
  //End Steer
  //Throttle
  long kmPh_to_mms = 277.778;
  long currentSpeed = history.currentSpeed_kmPh * kmPh_to_mms;
  long desiredSpeed = 10*results->speed_cmPs;
  Serial.println("currentSpeed = " + String(currentSpeed) + " desired speed = " + String(desiredSpeed));
  Throttle_PID(desiredSpeed - currentSpeed);
  //End Throttle
}
/*---------------------------------------------------------------------------------------*/
//Converts RC values to corresponding values for the PWM output
int convertTurn(int input)
{
  
  long int steerRange, rcRange;
  long output;
  int trueOut;
  //  Check if Input is in steer dead zone
  if ((input <= MIDDLE + DEAD_ZONE) && (input >= MIDDLE - DEAD_ZONE))
  {
    return STRAIGHT_TURN_OUT;
  }
  // On SPEKTRUM, MIN_RC = 1 msec = stick right; MAX_RC = 2 msec = stick left
  // On HI_TEC, MIN_RC = 1 msec = stick left; MAX_RC = 2 msec = stick right
  // LEFT_TURN_OUT > RIGHT_TURN_OUT
  else
  {
    return map(input, MIN_RC, MAX_RC, RIGHT_TURN_OUT, LEFT_TURN_OUT);
  }
  // @ToDo: Fix this so it is correct in any case.
  // If a controller requires some value to be reversed, then specify that
  // requirement in Settings.h, and use the setting here.
#ifdef RC_HITEC
  input = MAX_RC - (input - MIN_RC);
#endif
}

/*---------------------------------------------------------------------------------------*/
int convertDeg(int deg)
{
  const int actuatorRange = LEFT_TURN_OUT - RIGHT_TURN_OUT;
  const int degRange = TURN_MAX_DEG * 2;
  deg += TURN_MAX_DEG;
  double operand = (double)deg / (double)degRange;
  operand *= actuatorRange;
  operand += RIGHT_TURN_OUT;
  //set max values if out of range
  int result = (int)operand;
  if (result > LEFT_TURN_OUT)
    result = LEFT_TURN_OUT;
  return result;
}
/*---------------------------------------------------------------------------------------*/
int convertThrottle(int input)
{
  return map(input, 1400, 1000, 80, 140);
}

/*---------------------------------------------------------------------------------------*/
//Tests for inputs
// Input not in throttle dead zone
boolean liveThrottle(int acc)
{
  return (acc > MIDDLE + DEAD_ZONE);
}

boolean doRoutine(int acc){
  if(acc < 800) return false;
  return (acc < MIN_RC + DEAD_ZONE);
}

/*---------------------------------------------------------------------------------------*/
// Input is not in brake dead zone
boolean liveBrake(int b)
{
  if (b < 500) return false;
  return (b > (MIDDLE + DEAD_ZONE));
}
/*---------------------------------------------------------------------------------------*/
// Emergency stop
void E_Stop()
{
  brake(true);
  moveVehicle(MIN_ACC_OUT);
  delay (2000);   // inhibit output
  // TO DO: disable 36V power
}
/*---------------------------------------------------------------------------------------*/
//Send values to output pin
void steer(int pos)
{
  STEER_SERVO.writeMicroseconds(pos);
  steer_control = pos;
}
/*---------------------------------------------------------------------------------------*/
int convertBrake(unsigned long amount){
  const int brakeRange = MAX_BRAKE_OUT - MIN_BRAKE_OUT;
  const int rcRange = MAX_RC - (MIDDLE + DEAD_ZONE);
  amount -= (MIDDLE + DEAD_ZONE);
  float operand = (float)amount / (float)rcRange;
  operand *= brakeRange;
  operand += MIN_BRAKE_OUT;
  int result = (int)operand;
  if(result > MAX_BRAKE_OUT)
  {
    result = MAX_BRAKE_OUT;
  }
  return result;
}
/*---------------------------------------------------------------------------------------*/
//sets brake to max if true, min otherwise
void brake(bool on)
{
  if(on) applyBrakes();
  else releaseBrakes();
}
/*---------------------------------------------------------------------------------------*/
/* DAC_Write applies value to address, producing an analog voltage.
  // address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
  // value: digital value converted to analog voltage
  // Output goes to mcp 4802 Digital-Analog Converter Chip via SPI
  // There is no input back from the chip.
*/
void DAC_Write(int address, int value)
/*
  REGISTER 5-3: WRITE COMMAND REGISTER FOR MCP4802 (8-BIT DAC)
  A/B — GA SHDN D7 D6 D5 D4 D3 D2 D1 D0 x x x x
  bit 15 bit 0
  bit 15 A/B: DACA or DACB Selection bit
  1 = Write to DACB
  0 = Write to DACA
  bit 14 — Don’t Care
  bit 13 GA: Output Gain Selection bit
  1 = 1x (VOUT = VREF * D/4096)
  0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
  bit 12 SHDN: Output Shutdown Control bit
  1 = Active mode operation. VOUT is available.
  0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
  VOUT pin is connected to 500 k (typical)
  bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.
  With 4.95 V on Vcc, observed output for 255 is 4.08V.
  This is as documented; with gain of 2, maximum output is 2 * Vref
*/
{
  int byte1 = ((value & 0xF0) >> 4) | 0x10; // active mode, bits D7-D4
  int byte2 = (value & 0x0F) << 4; // D3-D0
  if (address < 2)
  {
    // take the SS pin low to select the chip:
    digitalWrite(SelectAB, LOW);
    if (address >= 0)
    {
      if (address == 1)
        byte1 |= 0x80; // second channnel
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
    digitalWrite(SelectAB, HIGH);
  }
  else
  {
    // take the SS pin low to select the chip:
    digitalWrite(SelectCD, LOW);
    if (address <= 3)
    {
      if (address == 3)
        byte1 |= 0x80; // second channnel
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
    digitalWrite(SelectCD, HIGH);
  }
}
/*---------------------------------------------------------------------------------------*/
void moveVehicle(int acc)
{
//  Serial.println(acc);  
  /* Observed behavior on ElCano #1 E-bike no load (May 10, 2013, TCF)
    0.831 V at rest 52 counts
    1.20 V: nothing 75
    1.27 V: just starting 79
    1.40 V: slow, steady 87
    1.50 V: brisker 94
    3.63 V: max 227 counts
    255 counts = 4.08 V
  */
  DAC_Write(DAC_CHANNEL, acc);
  throttle_control = acc;    // post most recent throttle.
}

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

long SpeedCyclometer_mmPs = 0;
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

/*---------------------------------------------------------------------------------------*/
// WheelRev is called by an interrupt.
// This is all WAY TOO LONG for an interrupt
void WheelRev()
{
  //static int flip = 0;
  unsigned long tick;
  noInterrupts();
  tick = millis();
  if (InterruptState != IRQ_RUNNING){
    // Need to process 1st two interrupts before results are meaningful.
    InterruptState++;
  }
  
  if (tick - TickTime > MinTickTime_ms){
    OldTick = TickTime;
    TickTime = tick;
    ++ClickNumber;
  }
  interrupts();
}
/*---------------------------------------------------------------------------------------*/

void setupWheelRev()
{
  float MinTick = WHEEL_CIRCUM_MM;
  MinTick *= 1000.0;
  MinTick /= MAX_SPEED_mmPs;
  MinTickTime_ms = MinTick;
  float MIN_SPEED_mmPs =  ((MIN_SPEED_mPh * 1000.0) / 3600.0);
  float MaxTick = (WHEEL_DIAMETER_MM * PI * 1000.0) / MIN_SPEED_mmPs;
  MaxTickTime_ms = MaxTick;
  TickTime = millis();
  // OldTick will normally be less than TickTime.
  // When it is greater, TickTime - OldTick is a large positive number,
  // indicating that we have not moved.
  // TickTime would overflow after days of continuous operation, causing a glitch of
  // a display of zero speed.  It is unlikely that we have enough battery power to ever see this.
  OldTick = TickTime;
  InterruptState = IRQ_NONE;
  ClickNumber = 0;
  history.oldSpeed_mmPs = history.olderSpeed_mmPs = NO_DATA;

  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
}
/*---------------------------------------------------------------------------------------*/

void computeSpeed(struct hist *data){
  //cyclometer has only done 1 or 2 revolutions
  //normal procedures begin here
  unsigned long WheelRev_ms = TickTime - OldTick;
  float SpeedCyclometer_revPs = 0.0;//revolutions per sec
  if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)
  { // No data
    SpeedCyclometer_mmPs = 0;
    SpeedCyclometer_revPs = 0;
    return;
  }
  
  if (InterruptState == IRQ_SECOND)
  { //  first computed speed
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  =
      data->oldSpeed_mmPs = data->olderSpeed_mmPs = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    data->oldTime_ms = OldTick;
    data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber = ClickNumber;
    return;
  }

  if (InterruptState == IRQ_RUNNING)
  { //  new data for second computed speed
    
    if(TickTime == data->nowTime_ms)
    {//no new data
        //check to see if stopped first
      unsigned long timeStamp = millis();
      if (timeStamp - data->nowTime_ms > MaxTickTime_ms)
      { // too long without getting a tick
         SpeedCyclometer_mmPs = 0;
         SpeedCyclometer_revPs = 0;
         if (timeStamp - data->nowTime_ms > 2 * MaxTickTime_ms)
         {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
         }
         return;
       }
        
       if (data->oldSpeed_mmPs > SpeedCyclometer_mmPs)
       { // decelerrating, extrapolate new speed using a linear model
          float deceleration = (float) (data->oldSpeed_mmPs - SpeedCyclometer_mmPs) / (float) (timeStamp - data->nowTime_ms);

          SpeedCyclometer_mmPs = data->oldSpeed_mmPs - deceleration * (timeStamp - data->nowTime_ms);
          if (SpeedCyclometer_mmPs < 0)
            SpeedCyclometer_mmPs = 0;
          SpeedCyclometer_revPs = SpeedCyclometer_mmPs / WHEEL_CIRCUM_MM;
       }
       else
       { // accelerating; should get new data soon

       }
       return;
    }

    //update time block
    data->olderTime_ms = data->oldTime_ms;
    data->oldTime_ms = data->nowTime_ms;
    data->nowTime_ms = TickTime;
    data->oldClickNumber = data->nowClickNumber;
    data->nowClickNumber = ClickNumber;

    //update speed block
    data->olderSpeed_mmPs = data->oldSpeed_mmPs;
    data->oldSpeed_mmPs = SpeedCyclometer_mmPs;
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;

    data->oldTickMillis = data->tickMillis;
    data->tickMillis = millis();
    
    data->currentSpeed_kmPh = SpeedCyclometer_mmPs/260.0;
    distance += ((data -> oldTime_ms - data -> olderTime_ms)/1000.0) * (data -> oldSpeed_mmPs)/100;
    //distance /= (1000.0 * 1000.0);

    if(data->TickTime_ms-data->OldTick_ms > 1000) data->currentSpeed_kmPh = 0;
    return;
  }
}

/*---------------------------------------------------------------------------------------*/
void PrintSpeed( struct hist *data)
{
  Serial.print(SpeedCyclometer_mmPs); Serial.print("\t");
  Serial.print(data->oldSpeed_mmPs); Serial.print("\t");
  Serial.print(data->olderSpeed_mmPs); Serial.print("\t");
  Serial.print(data->oldClickNumber); Serial.print("\t");
  Serial.print(data->nowClickNumber); Serial.print("\t");
  Serial.print(data->olderTime_ms); Serial.print("\t");
  Serial.print(data->oldTime_ms); Serial.print("\t");
  Serial.print(data->nowTime_ms); Serial.print("\t");
  Serial.print(data->TickTime_ms); Serial.print("\t");
  Serial.println(data->OldTick_ms);
}
/*---------------------------------------------------------------------------------------*/
void show_speed(SerialData *Results)
{
  computeSpeed (&history);
  PrintSpeed(&history);

  Odometer_m += (float)(LOOP_TIME_MS * SpeedCyclometer_mmPs) / 1000.0;
  // Since Results have not been cleared, angle information will also be sent.
}
/*---------------------------------------------------------------------------------------*/
/*========================CalibrateTurnAngle======================*/
/* The Hall angle sensors we are using have been observed to drift,
   and should periodically be zeroed.
   This routine should only be called when
          - Wheels are pointed straight ahead, and have been for a while.
          - Trike is not moving, and is stable.
   Calibration will block any response to controls; there will be
   no turning or movement during calibration. This condition should be
   very brief -- this does not wait nor turn off interrupts -- so should
   be safe to call during loop().
*/
void CalibrateTurnAngle(int count, int pause)
{
  int totalRight = 0;
  int totalLeft = 0;
  int i, left, right;
  for (i = 0; i < count; i++)
  {
    totalRight += analogRead(A2);
    totalLeft += analogRead(A3);
    delay(pause);
  }
  right = totalRight / count;
  left  = totalLeft  / count;
  // Only recalibrate when instruments are reasonable
  // Do not make garbage readings the new normal.
  if (RIGHT_MIN_COUNT <= right && right <= RIGHT_MAX_COUNT)
  {
    RightStraight_A2 = right;
    Right_Min_Count = RightStraight_A2 - 60;   // 60 counts is 20 degrees
    Right_Max_Count = RightStraight_A2 + 60;
  }
  else
  {
    RightStraight_A2 = RIGHT_MIN_COUNT + (RIGHT_MAX_COUNT - RIGHT_MIN_COUNT) / 2 ;
    Right_Min_Count = RIGHT_MIN_COUNT;
    Right_Max_Count = RIGHT_MAX_COUNT;
  }
  if  (LEFT_MIN_COUNT  <= left  && left  <= LEFT_MAX_COUNT)
  {
    LeftStraight_A3  = left;
    Left_Min_Count  = LeftStraight_A3  - 60;
    Left_Max_Count  = LeftStraight_A3  + 60;
  }
  else
  {
    LeftStraight_A3  = LEFT_MIN_COUNT + (LEFT_MAX_COUNT - LEFT_MIN_COUNT) / 2 ;
    Left_Min_Count = LEFT_MIN_COUNT;
    Left_Max_Count = LEFT_MAX_COUNT;
  }
  old_turn_degx1000 = 0; // straight
}
/*---------------------------------------------------------------------------------------*/
/*======================ReadTurnAngle======================*/
int TurnAngle_degx10()
{
  long new_turn_degx1000;
  long expected_turn_degx1000;
  int new_turn_degx10;
  long min_ang, max_ang;
  bool OK_right = false;
  bool OK_left = false;
  int right = analogRead(A2);
  int left = analogRead(A3);
  if ((Right_Min_Count <= right) && (right <= Right_Max_Count))
    OK_right = true;
  if ((Left_Min_Count <= left) && (left <= Left_Max_Count))
    OK_left = true;
  long right_degx1000 = (right - RightStraight_A2) * RIGHT_DEGx1000pCOUNT;
  long left_degx1000  = (left -  LeftStraight_A3) *  LEFT_DEGx1000pCOUNT;
  expected_turn_degx1000 = old_turn_degx1000;
  if (OK_left && OK_right)
  { // use the median
    if (right_degx1000 < left_degx1000)
    {
      min_ang = right_degx1000;
      max_ang = left_degx1000;
    }
    else
    {
      min_ang = left_degx1000;
      max_ang = right_degx1000;
    }
    if (expected_turn_degx1000 < min_ang)
      new_turn_degx1000 = min_ang;
    else if (expected_turn_degx1000 > max_ang)
      new_turn_degx1000 = max_ang;
    else
      new_turn_degx1000 = expected_turn_degx1000;
  }
  else if (OK_left)
  {
    new_turn_degx1000 = (left_degx1000 + expected_turn_degx1000) / 2;
  }
  else if (OK_right)
  {
    new_turn_degx1000 = (right_degx1000 + expected_turn_degx1000) / 2;
  }
  else
  { // No sensors; use last valid measurement
    new_turn_degx1000 = old_turn_degx1000;
  }
  new_turn_degx10 = (int) (new_turn_degx1000 / 100);
  old_turn_degx1000 = new_turn_degx1000;
  return new_turn_degx10;
}
/*---------------------------------------------------------------------------------------*/
/*
 * Maps pulse width to km/h. 
 * Maximum speed = 15 km/h 
 * Minimum speed = 0 km/h
 * Precondition: 
 *              value > 1000 && value < 2000
 * Postcondition:
 *              value < MIDDLE - DEAD_ZONE reverses the trike
 *              otherwise the trike moves forward
 * Reverse capabilities not implemented yet
 */
float mapThrottle(int value){
  Serial.print(String(value) + ", ");
  if(value > MIDDLE - DEAD_ZONE)
  {
    return 0;// in future, add reverse
  }
  else
    return map(value, MIDDLE-DEAD_ZONE, MIN_RC, 0, MAX_SPEED);
}

void set_Speed_kph(float speed_kph){
  
}

void Throttle_PID(long error_speed_mmPs)

/* Use throttle and brakes to keep vehicle at a desired speed.
 * error_speed_mmPs = Desired speed in millimeters per second
   A PID controller uses the error in the set point to increase or decrease the juice.
   P = proportional; change based on present error
   I = integra;  change based on recent sum of errors
   D = derivative: change based on how error is changing.
   The controller needs to avoid problems of being too sluggish or too skittery.
   A sluggish control (overdamped) takes too long to reach the set-point.
   A skitterish control (underdamped) can overshoot, then undershoot, producing
   oscillations and jerky motion.
   Getting the right control is a matter of tuning right parts of P, I, and D,
   which is something of a black art.
   For more information, search for:
   VanDoren Proportional Integral Derivative Control
*/
{
  static int  throttle_control = MIN_ACC_OUT;
  static int  brake_control = MAX_BRAKE_OUT;
  static int error_index = 0;
  int i;
  static long error_sum = 0;
  long mean_speed_error = 0;
  long extrapolated_error = 0;
  long PID_error;
  const long speed_tolerance_mmPs = 75;  // about 0.2 mph
  // setting the max_error affacts control: anything bigger gets maximum response
  const long max_error_mmPs = 2500; // about 5.6 mph

  //lets look through this
  error_sum -= speed_errors[error_index];
  speed_errors[error_index] = error_speed_mmPs;
  error_sum += error_speed_mmPs;
  mean_speed_error = error_sum / ERROR_HISTORY;
  i = (error_index - 1) % ERROR_HISTORY;
  if (++error_index >= ERROR_HISTORY)
    error_index = 0;
  extrapolated_error = 2 * error_speed_mmPs - speed_errors[i];
  PID_error = P_TUNE * error_speed_mmPs
              + I_TUNE * mean_speed_error
              + D_TUNE * extrapolated_error;

//  Serial.println("PID_error = " + String(PID_error) + " speed_tolerance_mmPs = " + String(speed_tolerance_mmPs));
  if (PID_error > speed_tolerance_mmPs)
  { // too fast
    long throttle_decrease = (MAX_ACC_OUT - MIN_ACC_OUT) * PID_error / max_error_mmPs;
    throttle_control -= throttle_decrease;
    if (throttle_control < MIN_ACC_OUT)
      throttle_control = MIN_ACC_OUT;
    moveVehicle(throttle_control);

    long brake_increase = (MAX_BRAKE_OUT - MIN_BRAKE_OUT) * PID_error / max_error_mmPs;
    brake_control -= brake_increase;
    if (brake_control > MAX_BRAKE_OUT)
      brake_control = MAX_BRAKE_OUT;
//    brake(brake_control);
    brake(true);
  }
  else if (PID_error < speed_tolerance_mmPs)
  { // too slow
    long throttle_increase = (MAX_ACC_OUT - MIN_ACC_OUT) * PID_error / max_error_mmPs;
    throttle_control += throttle_increase;
    if (throttle_control > MAX_ACC_OUT)
      throttle_control = MAX_ACC_OUT;
    moveVehicle(throttle_control);

    // release brakes
    long brake_decrease = (MAX_BRAKE_OUT - MIN_BRAKE_OUT) * PID_error / max_error_mmPs;
    brake_control += brake_decrease;
    if (brake_control < MIN_BRAKE_OUT)
      brake_control = MIN_BRAKE_OUT;
//    brake(brake_control);
    brake(false);
  }
  //error_index = (error_index + 1) % ERROR_HISTORY;
  // else maintain current speed
}

/*---------------------------------------------------------------------------------------*/
/* Serial 7-Segment Display Example Code
    Serial Mode Stopwatch
   by: Jim Lindblom
     SparkFun Electronics
   date: November 27, 2012
   license: This code is public domain.
   This example code shows how you could use software serial
   Arduino library to interface with a Serial 7-Segment Display.
   There are example functions for setting the display's
   brightness, decimals and clearing the display.
   The print function is used with the SoftwareSerial library
   to send display data to the S7S.
   Circuit:
   Arduino -------------- Serial 7-Segment
     3.3V   --------------------  VCC
     GND  --------------------  GND
      10   --------------------  RX
*/

void setup7seg()
{
  // Must begin s7s software serial at the correct baud rate.
  //  The default of the s7s is 9600.
  s7s.begin(9600);

  // Clear the display, and then turn on all segments and decimals
  clearDisplay();  // Clears display, resets cursor
  setBrightness(255);  // High brightness
}
/*---------------------------------------------------------------------------------------*/
void show7seg(int speed_mmPs)
{
  char tempString[4];  // Will be used with sprintf to create strings
  // convert mm/s to km/h
  int speed_kmPhx10 = (speed_mmPs * .036);
  // Magical sprintf creates a string for us to send to the s7s.
  //  The %4d option creates a 4-digit integer.
  sprintf(tempString, "%4d", speed_kmPhx10);
  String temp3 = (String)tempString;
  // This will output the tempString to the S7S
  s7s.print(temp3);
  setDecimals(0b00000100);  // Sets digit 3 decimal on
}
/*---------------------------------------------------------------------------------------*/
// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplay()
{
  s7s.write(0x76);  // Clear display command
  s7s.write(0x79); // Send the Move Cursor Command
  s7s.write(0x00); // Move Cursor to left-most digit
}
/*---------------------------------------------------------------------------------------*/
// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightness(byte value)
{
  s7s.write(0x7A);  // Set brightness command byte
  s7s.write(value);  // brightness data byte
}
/*---------------------------------------------------------------------------------------*/
// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimals(byte decimals)
{
  s7s.write(0x77);
  s7s.write(decimals);
}

