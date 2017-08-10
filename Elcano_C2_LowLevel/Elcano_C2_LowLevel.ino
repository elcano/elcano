#include <Settings.h>

#include <PID_v1.h>
#include <SPI.h>
#include <ElcanoSerial.h>
#include <Servo.h>
#include <SD.h>
using namespace elcano;

long startTime;

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

// TCF 6/27/17  Why is steering angle in milliseconds? Degrees would be more appropriate.
double SteerAngle_wms = STRAIGHT_TURN_MS; //Steering angle in microseconds used by writeMicroseconds function. Note: doubles on Arduinos are the same thing as floats, 4bytes, single precision
double PIDSteeringOutput; //Output from steerPID.Compute() in microseconds (used by Servo.writeMicroseconds())
double desiredAngle = STRAIGHT_TURN_MS;
double steeringP = 1.5;
double steeringI = 1.35; 
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
//const float  HubSpeed2kmPh = 13000000;
const unsigned long HubAtZero = 1159448;

// Time at which this loop pass should end in order to maintain a
// loop period of LOOP_TIME_MS.
unsigned long nextTime;
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
double desiredSpeed = 0;

//PID update frequency in milliseconds
#define PID_CALCULATE_TIME 50

double proportionalConstant = .0175;
double integralConstant = .0141;
double derivativeConstant = .00001;

// PID setup block
PID speedPID(&SpeedCyclometer_mmPs, &PIDThrottleOutput, &desiredSpeed, proportionalConstant, integralConstant, derivativeConstant, DIRECT);
PID steerPID(&SteerAngle_wms, &PIDSteeringOutput, &desiredAngle, steeringP, steeringI, steeringD, DIRECT);

/*-----------------------------------setup-----------------------------------------------*/
void setup()
{
  startTime = millis();
  
  //Set up pins
  STEER_SERVO.attach(STEER_OUT_PIN);
  BRAKE_SERVO.attach(BRAKE_OUT_PIN);

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

  speedPID.SetOutputLimits(MIN_ACC_OUT, MAX_ACC_OUT); //useful if we want to change the limits on what values the output can be set to
  speedPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
  steerPID.SetOutputLimits(RIGHT_TURN_OUT, LEFT_TURN_OUT); //useful if we want to change the limits on what values the output can be set to
  steerPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
  
  for (int channel = 0; channel < 4; channel++){
    DAC_Write(channel, 0); // reset did not clear previous states
  }
  // put vehicle in initial state
  brake(MAX_BRAKE_OUT);
  moveVehicle(MIN_ACC_OUT);
  
  delay(500);   // let vehicle stabilize
  
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
  calibrateSensors();
  calibrationTime_ms = millis();
  steer(STRAIGHT_TURN_OUT);
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN),  ISR_TURN_rise,  RISING);//turn right stick l/r turn
  attachInterrupt(digitalPinToInterrupt(IRPT_GO),    ISR_GO_rise,    RISING);//left stick l/r
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);//ebrake
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);//left stick u/d mode select
  attachInterrupt(digitalPinToInterrupt(IRPT_MOTOR_FEEDBACK), ISR_MOTOR_FEEDBACK_rise, RISING);
  
  Serial.begin(9600);
  
  parseState.dt = &Results;
  parseState.input = &Serial3;
  parseState.output = &Serial3;
  Serial3.begin(baudrate);
  parseState.capture = MsgType::drive;
  // msgType::drive uses `speed_cmPs` and `angle_deg`
  Results.clear();
  SpeedCyclometer_mmPs = 0;
  brake(false);

  // THIS MUST BE THE LAST LINE IN setup().
  nextTime = millis()
}

/*-----------------------------------loop------------------------------------------------*/
void loop() {

  // Get the next loop start time. Note this (and the millis() counter) will
  // roll over back to zero after they exceed the 32-bit size of unsigned long,
  // which happens after about 1.5 months of operation (should check this).
  // But leave the overflow computation in place, in case we need to go back to
  // using the micros() counter.
  // If the new nextTime value is <= LOOP_TIME_MS, we've rolled over.
  nextTime = nextTime + LOOP_TIME_MS;

  // DO NOT INSERT ANY LOOP CODE ABOVE THIS LINE.
 
  // send data to C6
  Results.clear();
  Results.kind = MsgType::drive;
  Results.speed_cmPs = SpeedCyclometer_mmPs/10;
  // temporary
//  Results.speed_cmPs = 0;
  // end temporary
  Results.angle_mDeg = 0;
  Results.write(&Serial3);
  computeSpeed(&history);
  computeAngle();
  ThrottlePID();
  SteeringPID();

  byte automate = processRC();
  // END TEMPORARY
  if (automate == 0x01)
  {
    ParseStateError r = parseState.update();
    if(r == ParseStateError::success)
    {
      processHighLevel(&Results);
    }
  }
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

  // DATA LOGGING: OUTPUT TO SD CARD
  logData();

  // DO NOT INSERT ANY LOOP CODE BELOW THIS POINT.

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

/*------------------------------------ThrottlePID--------------------------------*/
// Compute PID 
// Precondition: None
// Postcondition: None
void ThrottlePID(){
  speedPID.Compute();
  int throttleControl = (int)PIDThrottleOutput;
  //apply control value to vehicle
  moveVehicle(throttleControl);
  if(PIDThrottleOutput == MIN_ACC_OUT){
    //apply brakes
    brake_feather(true);
  }
  else{
    brake_feather(false);
  }
  return;
}

/*---------------------------SteeringPID--------------------------------------*/
// Computer Steering PID
// Precondition: None
// Postcondition: None
void SteeringPID(){
  if(steerPID.Compute()){
    
    int steeringControl = (int)PIDSteeringOutput;
    //apply control value to vehicle
    STEER_SERVO.writeMicroseconds(steeringControl);
  }
  return;
}

void computeAngle(){
  int left = analogRead(A2);
  int right = analogRead(A3);

  int left_wms = map(left, leftsenseleft, leftsenseright, LEFT_TURN_OUT, RIGHT_TURN_OUT); // Left sensor spikes outside of calibrated range between setup() and loop(); temporarily disregard data
  int right_wms = map(right, rightsenseleft, rightsenseright, LEFT_TURN_OUT, RIGHT_TURN_OUT);
}

/*---------------------------------CalibrateSensors----------------------------------*/
// Calibrate sensors on the trike
// Precondition: None
// Postcondition: None
void calibrateSensors(){
  STEER_SERVO.writeMicroseconds(LEFT_TURN_MS); //Calibrate angle sensors for left turn
  delay(4000);
  leftsenseleft = analogRead(A2);
  rightsenseleft = analogRead(A3);
  Serial.print("Left turn sensor readings: ");
  Serial.print(leftsenseleft);
  Serial.println(rightsenseleft);
  
  STEER_SERVO.writeMicroseconds(RIGHT_TURN_MS); //Calibrate angle sensors for right turn
  delay(4000);
  leftsenseright = analogRead(A2);
  rightsenseright = analogRead(A3);
  Serial.print("Right turn sensor readings: ");
  Serial.print(leftsenseright);
  Serial.println(rightsenseright);
  steerPID.SetMode(AUTOMATIC);
}

/*------------------------------------processHighLevel------------------------------------*/
// Process information received form high level
// Precondition: SerialData exists
// Postcondition: desiredAngle and desiredSpeed are both set based on high level information
void processHighLevel(SerialData * results)
{
  Serial3.end();
  Serial3.begin(baudrate);  // clear the buffer
  
  double turn_signal = convertDeg(results->angle_mDeg / 1000.);
  Serial.println(results->angle_mDeg);
  desiredSpeed = results->speed_cmPs * 10;
  desiredAngle = turn_signal;
}

/*-----------------------------------checkEbrake-----------------------------------------*/
// Check if ebrake is on
// Ebrake is on if PWM of the Ebrake channel on the remote is > MIDDLE else Ebrake is off
// Precondition: None
// Postcondition: return true if Ebrake is on, false otherwise
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
      else{
        brake(false);
      }
  }
  return false;
}

/*------------------------------------processRC-------------------------------------------*/
// Determine whether to read from remote control or process from high level
// Process from high level if the brake channel on the remote control is > MIDDLE 
// Precondition: None
// Postcondition: return 0x01 if processing from high level else 0x00 if processing from remote control
byte processRC()
{
  //RC_TURN, RC_ESTOP, RC_BRAKE, RC_AUTO
  boolean autoMode = false; // Once RC_AUTO is implemented, this will default to false
  //ESTOP
  
  checkEbrake();

  autoMode = isAutomatic();
  
  if(autoMode){
    return 0x01;
  }
  else // not in autonomous mode
  {
    doManualMovement();
  }
  return 0x00;
}

/*------------------------------------isAutomatic-----------------------------------------*/
// Check whether to read from the Remote Control or to process from high level
// The vehicle is automatic if the brake channel on the remote control is > MIDDLE 
// Precondition: None
// PostCondition: Return true if autonomous and false otherwise
boolean isAutomatic(){
    if(RC_Done[RC_BRAKE]){
      if(RC_elapsed[RC_BRAKE] > MIDDLE + DEAD_ZONE){
        return true;           // It is manual control and not autonomous control
      }
    }
    return false;
}

/*------------------------------------doManualMovement------------------------------------*/
// Act on information received from the remote control
// Currently only does turning and straight movement
// Precondition: None
// PostCondition: None
void doManualMovement(){
  //THROTTLE
    //TODO: if less than the middle, reverse, otherwise forward
    bool doFeather = false;
    bool on = checkEbrake();
    if(RC_Done[RC_GO] && !on)
    {
      if(RC_elapsed[RC_GO] > MIDDLE +  2 * DEAD_ZONE){
        moveVehicle(convertThrottle(RC_elapsed[RC_GO]));
      }
      else{
        moveVehicle(0);
      }
    }
  //TURN
    if (RC_Done[RC_TURN] && !on) 
    {
      steer(convertTurn(RC_elapsed[RC_TURN]));
    }
}

/*------------------------------------convertTurn----------------------------------------*/
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
  // LEFT_TURN_MS > RIGHT_TURN_MS
  else
  {
    int value = map(input, MIN_RC, MAX_RC, RIGHT_TURN_OUT, LEFT_TURN_OUT);
    return value;
  }
  // @ToDo: Fix this so it is correct in any case.
  // If a controller requires some value to be reversed, then specify that
  // requirement in Settings.h, and use the setting here.
#ifdef RC_HITEC
  input = MAX_RC - (input - MIN_RC);
#endif
}

/*------------------------------------convertDeg------------------------------------------*/
// Convert PWM values from Remote Control to corresponding trike specific angle in degrees
int convertDeg(int deg)
{
  int result = map(deg, -TURN_MAX_DEG, TURN_MAX_DEG, RIGHT_TURN_OUT, LEFT_TURN_OUT);
  if (result > LEFT_TURN_OUT)
    result = LEFT_TURN_OUT;
  return result;
}

/*------------------------------------convertThrottle-------------------------------------*/
// Convert PWM values from Remote Control to corresponding trike specific throttle values 
// Precondition: MIDDLE + 2 * DEAD_ZONE < input < MAX_RCGO
int convertThrottle(int input)
{
  int mapping = map(input, MIDDLE + 2 * DEAD_ZONE, MAX_RC, 80, 140);
  return mapping;
}

/*------------------------------------liveThrottle----------------------------------------*/
//Tests for inputs
// Input not in throttle dead zone
boolean liveThrottle(int acc)
{
  return (acc > MIDDLE + DEAD_ZONE);
}

/*------------------------------------doRoutine-------------------------------------------*/
boolean doRoutine(int acc){
  if(acc < 800) return false;
  return (acc < MIN_RC + DEAD_ZONE);
}

/*-------------------------------------liveBrake------------------------------------------*/
// Input is not in brake dead zone
boolean liveBrake(int b)
{
  if (b < 500) return false;
  return (b > (MIDDLE + DEAD_ZONE));
}

/*-------------------------------------Emergency stop--------------------------------------*/
void E_Stop()
{
  Serial.println("E_Stop");
  brake(true);
  moveVehicle(MIN_ACC_OUT);
  delay (2000);   // inhibit output
  // TO DO: disable 36V power
}

/*-------------------------------------steer-----------------------------------------------*/
void steer(int pos)
{
  STEER_SERVO.writeMicroseconds(pos);
  steer_control = pos;
}

/*-------------------------------------brake_feather---------------------------------------*/
void brake_feather(bool on)
{
  if(on) BRAKE_SERVO.writeMicroseconds((MAX_BRAKE_OUT + MIN_BRAKE_OUT)/2);
  else BRAKE_SERVO.writeMicroseconds(MIN_BRAKE_OUT);
}

/*-------------------------------------brake-----------------------------------------------*/
void brake(bool on)
{
  int microsecs = (on ? MAX_BRAKE_OUT : MIN_BRAKE_OUT);
  BRAKE_SERVO.writeMicroseconds(microsecs);
}

/*-------------------------------------DAC_Write-------------------------------------------*/
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

/*-------------------------------------moveVehicle------------------------------------------*/
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

/*----------------------------WheelRev---------------------------------------------------*/
// WheelRev is called by an interrupt.
void WheelRev()
{
  oldClickNumber = ClickNumber;
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
/*----------------------------setupWheelRev----------------------------------------------*/
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
  
  speedPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute

  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
}


/*----------------------------computeSpeed-----------------------------------------------*/
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
    distance_mm += ((data -> oldTime_ms - data -> olderTime_ms)/1000.0) * (data -> oldSpeed_mmPs);

    if(data->TickTime_ms-data->OldTick_ms > 1000) data->currentSpeed_kmPh = 0;
    return;
  }
}

/*-----------------------------CalibrateTurnAngle----------------------------------------*/
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

/*------------------------------mapThrottle----------------------------------------------*/
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
    return map(value, MIDDLE-DEAD_ZONE, MIN_RC, 0, MAX_SPEED_KPH);
}

/*-----------------------------------ThrottlePID------------------------------------*/
void ThrottlePID(double desired){
  desiredSpeed = desired;
  speedPID.Compute();
  int throttleControl = (int)PIDThrottleOutput;
  //apply control value to vehicle
  moveVehicle(throttleControl);
  if(PIDThrottleOutput == MIN_ACC_OUT){
    //apply brakes
    //brake(MAX_BRAKE_OUT);
  }
  else{
    //brake(MIN_BRAKE_OUT);
  }
  
  return;
}

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

// SD CARD
// logData() outputs throttle, brake, steer,
// speed and distance data to an SD card
// into a file with filename defined below
void logData() {

  // Name of data file on SD card
  char* FILENAME = "datalog.txt";
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(FILENAME, FILE_WRITE);

  // Create a string with data information
  String dataString = "";

  dataString += "(ms) Time\t";
  dataString += "(km/h) Speed\t";
  dataString += "(km/h) HubSpeed\t";
  dataString += "(ms) Angle\t";
  dataString += "Right\t";
  dataString += "Left\t";
  dataString += "Throttle\t";
  dataString += "Brake\t";
  dataString += "Steer\t";
  dataString += "(m) Distance \n";
  
  dataString += millis() + "\t";                            //(ms) Time
  dataString += String(history.currentSpeed_kmPh) + "\t";   //(km/h) Speed
  dataString += String(HubSpeed_kmPh) + "\t";               //(km/h) Hub Speed
  dataString += String(SteerAngle_wms) + "\t";              //(microseconds) Angle

  // TODO replace these pins with something labaled, idk what is going on here
  int right = analogRead(A3);
  int left = analogRead(A2);
  dataString += right + "\t";                               //Right turn sensor
  dataString += left + "\t";                                //Left turn sensor
  dataString += throttle_control + "\t";                    //Throttle
  dataString += brake_control + "\t";                       //Brake
  dataString += steer_control + "\t";                       //Steer
  dataString += String(Odometer_m) + "\n";                  //(m) Distance
      
  // if the file opened okay, write to it
  if (dataFile) {
    dataFile.print(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.print("Data Logger: Logged data to ");
    Serial.print(FILENAME);
    Serial.println(".txt");
  }
  // if the file isn't opening, pop up an error:
  else {
    Serial.print("Data logger: error opening ");
    Serial.print(FILENAME);
    Serial.println(".txt");
  }
}





