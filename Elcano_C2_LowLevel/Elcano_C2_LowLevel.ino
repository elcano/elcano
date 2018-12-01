#include <Settings.h>
#include <PID_v1.h>
#include <SPI.h>
#include <ElcanoSerial.h>
#include <Servo.h>
#include <SD.h>
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

#define DEBUG 1 // prints to Serial, will change code execution times

static struct hist
{
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
// 100 milliseconds -- adjust to accomodate the fastest needed response or
// sensor data capture or PID updates.
#define LOOP_TIME_MS 100
#define ERROR_HISTORY 20 //number of errors to accumulate
#define ULONG_MAX 0x7FFFFFFF

static long distance_mm = 0;  // odometer

// The DAC select pins are the same on all trikes.
const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)

// This is a value that the RC controller can't produce.
#define INVALID_DATA 0L
long speed_errors[ERROR_HISTORY];
long old_turn_degx1000;

unsigned long stoppedTime_ms;
unsigned long straightTime_ms;
const int WHEEL_MAX_LEFT_US = 1000;  // arbitrary number 5/31/18; what should these be?
const int WHEEL_STRAIGHT_US = 2000;
const int WHEEL_MAX_RIGHT_US = 3000;
int  currentThrottlePWM = MAX_ACC_OUT; // ex. throttle_control
int  currentSteeringUS = WHEEL_STRAIGHT_US; // ex. steer_control
float Odometer_m = 0;
float HubSpeed_kmPh;
const unsigned long HubAtZero = 1159448;

void DAC_Write(int address, int value);
void testBrakes();
void setupWheelRev();

// Time at which this loop pass should end in order to maintain a
// loop period of LOOP_TIME_MS.
unsigned long nextTime;
// Time at which we reach the end of loop(), which should be before
// nextTime if we have set the loop period long enough.
unsigned long endTime;
// How much time we need to wait to finish out this loop pass.
unsigned long delayTime;
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
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
unsigned long MinTickTime_ms;
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 3000
// A speed of less than 0.3 KPH is zero.
unsigned long MaxTickTime_ms;
// MinTickTime_ms = 9239 ms = 9 sec

// Speed in revolutions per second is independent of wheel size.
float SpeedCyclometer_revPs = 0.0;//revolutions per sec

#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_SECOND 2
#define IRQ_RUNNING 3
#define NO_DATA 0xffffffff
volatile byte InterruptState = IRQ_NONE;  // Tells us if we have initialized.
volatile byte ClickNumber = 0;         // Used to distinguish old data from new.
volatile unsigned long TickTime = 0;  // Time from one wheel rotation to the next gives speed.
volatile unsigned long OldTick = 0;
int oldClickNumber;

/**
 * Throttle PID implementation
 */
//PID update frequency in milliseconds
#define PID_CALCULATE_TIME 50
/* double on Arduino AVR is the same as a float. */
double speedCyclometerInput_mmPs = 0;
double PIDThrottleOutput_pwm; //used to tell Throttle and Brake what to do as far as acceleration
double desiredSpeed_mmPs = 0;
double proportional_throttle = .0175;
double integral_throttle = .2;
double derivative_throttle = .00001;
double steerAngleUS = WHEEL_STRAIGHT_US; //Steering angle in microseconds used by writeMicroseconds function. Note: doubles on Arduinos are the same thing as floats, 4bytes, single precision
double PIDSteeringOutput_us; //Output from steerPID.Compute() in microseconds (used by Servo.writeMicroseconds())
double desiredTurn_us = WHEEL_STRAIGHT_US;
double proportional_steering = .0175;
double integral_steering = .5; 
double derivative_steering = .00001;

// PID setup block
PID speedPID(&speedCyclometerInput_mmPs, &PIDThrottleOutput_pwm, &desiredSpeed_mmPs, proportional_throttle, integral_throttle, derivative_throttle, DIRECT);
PID steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT);

int calibratedWheelMaxLeft_us; // CALIBRATED_WHEEL_MAX_LEFT_US
int calibratedWheelStraight_us; // CALIBRATED_WHEEL_STRAIGHT_US
int calibratedWheelMaxRight_us; // CALIBRATED_WHEEL_MAX_RIGHT_US

int calibratedWheelSensorMaxLeft; // CALIBRATED_WHEEL_MAX_LEFT_US
int calibratedWheelSensorStraight;
int calibratedWheelSensorMaxRight; // CALIBRATED_WHEEL_MAX_RIGHT_US

int oldSpeedValue = MIN_ACC_OUT;
int oldTurnValue = WHEEL_STRAIGHT_US;

int countz = 0;

// Sweep communication pin
const int SWEEP_PIN = 17;

ParseState TxStateHiLevel, RxStateHiLevel, RC_State;
SerialData TxDataHiLevel,  RxDataHiLevel,  RC_Data;
/*---------------------------------------------------------------------------------------*/ 

/* Solenoid controlled Brakes.
 *  
 *  Solenoid (Johnson Electic model 150 174432-024) 
 *  can be kept at lower voltage (12V) indefinitely. 
 *  It has a holding force of 14.5 lb.
 *  At the higher voltage, data sheet expects it to be high (24V) for
 *  25% of the time and low for 75%. However, it can stay high for 100 sec.
 *  The solenoid typically reacts in less than a second.
 *  For 0.25 inch throw and 24V it can pull 7 lb.
 *  We are using the part with 0.3 inch throw, 12V in low state and 24V in high
 *  state, but keeping voltage high for only a second or two.
 *  
 *  The solenoid brake on Elcano replaces the earlier linear actuator.
 *  Solenoid advantages:
 *      Faster: Typically 0.2 seconds vs 2 in/sec for no load. 
 *      Slightly Lighter: 482 grams each vs. 1.16 kg
 *      Available; Linear actuator is no longer available for 2 or 4 inch throw.
 *      Less expensive: 2 * $70 vs. $250
 *      More durable; 5 of the linear actutors have failed.
 *  Linear actuator advantages:
 *      Single unit can pull both brakes (25 lb. thrust); solenoid requires a unit for each brake.
 *      No restriction on throw length; solenoid will not work with > 0.3 inch throw.
 *      Maintains last position without power.   
 *      Do not need to worry about heat dissipation.
 *      
 *  The linear actuator was controlled by pulse width on one digital line.    
 *  The solenoids are controlled by relays on two digital lines.
 *      
 *    Tyler Folsom   April 2018  
 *    
     *  Expected behavior
     *  When Green LED is on, NO is connected to COM; NC is not
     *  Writing HIGH to a relay will turn LED on, and connect NO to COM
     *  When green LED is off, NC is connected to COM; NO is not.
     *  Writing LOW to a relay will turn LED off, and connect NC to COM.
     *  You shoud hear a click when the relay operates.
     *  If there is a change in LED, but no click, the relay does not have enough power.
     */
   
class Brakes
{
 public:
  Brakes();
  void Stop();
  void Release();
  void Check();
 private:
  enum brake_state {BR_OFF, BR_HI_VOLTS, BR_LO_VOLTS} state;
  unsigned long clock_hi_ms;
  const int LeftBrakeOnPin = 10;
  const int RightBrakeOnPin = 2;
  const int LeftBrakeVoltPin = 8;
  const int RightBrakeVoltPin = 7;
  const unsigned long MaxHi_ms = 800;
 } ;

// For normal operation
const long int loop_time_ms = 100;  // Limits time in the loop.

Brakes::Brakes()
{
  pinMode( LeftBrakeOnPin, OUTPUT);
  pinMode( RightBrakeOnPin, OUTPUT);
  pinMode( LeftBrakeVoltPin, OUTPUT);
  pinMode( RightBrakeVoltPin, OUTPUT);
  clock_hi_ms = millis();
  state = BR_OFF;
}
void Brakes::Release()
{
  /*  Expected behavior:
   * LEDs go off for relays 2 and 3;
   * Relay 2 has NO (connected to solenoids) open, and there is no power to solenoids.
   * Relay 3 connects COM (other end of solenoid) to NO (12V) 
   */
  digitalWrite(LeftBrakeOnPin, LOW);
  digitalWrite(LeftBrakeVoltPin, LOW);
  digitalWrite(RightBrakeOnPin, LOW);
  digitalWrite(RightBrakeVoltPin, LOW);
  state = BR_OFF;
  if (DEBUG) {
    Serial.println("Brakes off!");
  }
}
void Brakes::Stop()
{
  /*  Expected behavior:
   *  Both LEDs come on for Relays 2 and 3
   *  Relay 2 connects NO (solenoids) to COM (ground)
   *  Relay 3 connects COM (other end of solenoids) to NC (36V)
   */
  digitalWrite(LeftBrakeVoltPin, HIGH);  // Need the higher voltage to activate the solenoid.
  digitalWrite(RightBrakeVoltPin, HIGH); 
  if (state != BR_HI_VOLTS)
  {
    clock_hi_ms = millis();  // keep track of when the higher voltage was applied.
  }
  digitalWrite(LeftBrakeOnPin, HIGH); // Activate solenoid to apply brakes.
  digitalWrite(RightBrakeOnPin, HIGH);
  state = BR_HI_VOLTS;
  if (DEBUG) {
    Serial.println("Brakes on!");
  }
}
void Brakes::Check()
{
  /* Expected behavior
   *  If 36V has been on too long, relay 3 changes LED on to off, switching from 24 to 12V
   *  If the switch is high, brakes will be released, with both LEDs off.
   */
 
  unsigned long tick = millis();
  if (state == BR_HI_VOLTS && tick - clock_hi_ms > MaxHi_ms)
  {  // Have reached maximum time to keep voltage high
    digitalWrite(LeftBrakeVoltPin, LOW); // Set to lower voltage, which will keep brakes applied
    digitalWrite(RightBrakeVoltPin, LOW);
    state = BR_LO_VOLTS;
  }
}

 Brakes brake = Brakes();
 
void setup()
{ 
  //Set up pins
  STEER_SERVO.attach(STEER_OUT_PIN);

  // SPI: set the slaveSelectPin as an output:
  pinMode (SelectAB, OUTPUT);
  pinMode (SelectCD, OUTPUT);
  pinMode (10, OUTPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  // initialize SPI:
  // The following line should not be neccessary. It uses a system library.
  PRR0 &= ~4; // turn off PRR0.PRSPI bit so power isn't off
  SPI.begin();

  // ******** Initialized but unused since PID has some issues ************
  speedPID.SetOutputLimits(MIN_ACC_OUT, MAX_ACC_OUT); //useful if we want to change the limits on what values the output can be set to
  speedPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
  speedPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute
  steerPID.SetOutputLimits(WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US); //useful if we want to change the limits on what values the output can be set to
  steerPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
  steerPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute
 
  // **********************************************************

  for (int channel = 0; channel < 4; channel++)
  {
    DAC_Write(channel, 0); // reset did not clear previous states
  }
  // ******* START: System Test and Calibration Cycle ******** \\
  delay(100);
  testBrakes();
  delay(1000);
  // ******* END: System Test and Calibration Cycle ******** \\
  
  for (int i = 0; i < ERROR_HISTORY; i++)
  {
    speed_errors[i] = 0;
  }

  setupWheelRev(); // WheelRev4 addition
    // Setting up data for sending to high level
    Serial.begin(9600); 
    Serial3.begin(baudrate);
    Serial2.begin(baudrate);
    Serial1.begin(baudrate);
    TxDataHiLevel.clear();
    TxStateHiLevel.dt  = &TxDataHiLevel;
    TxStateHiLevel.input = &Serial2;  // not used
    TxStateHiLevel.output = &Serial3;
    TxStateHiLevel.capture = MsgType::sensor;
  
    //setup for receiving data from High level
    RxDataHiLevel.clear();
    RxStateHiLevel.dt  = &RxDataHiLevel;
    RxStateHiLevel.input = &Serial3;
    RxStateHiLevel.output = &Serial2; // not used
    RxStateHiLevel.capture = MsgType::drive;

    // receive data indirectly from RC unit.
    RC_Data.clear();
    RC_State.dt  = &RC_Data;
    RC_State.input = &Serial1;
    RC_State.output = &Serial1;  // not used
    RC_State.capture = MsgType::drive; // match to RadioControl.INO

  speedCyclometerInput_mmPs = 0;

  // Sweep
  pinMode(SWEEP_PIN, INPUT);
  
  // THIS MUST BE THE LAST LINE IN setup().
  nextTime = millis();
}

void loop()
{  
  // Get the next loop start time. Note this (and the millis() counter) will
  // roll over back to zero after they exceed the 32-bit size of unsigned long,
  // which happens after about 1.5 months of operation (should check this).
  // But leave the overflow computation in place, in case we need to go back to
  // using the micros() counter.
  // If the new nextTime value is <= LOOP_TIME_MS, we've rolled over.
  nextTime = nextTime + LOOP_TIME_MS;

  // DO NOT INSERT ANY LOOP CODE ABOVE THIS LINE.

  static long int desired_speed_cmPs, desired_angle;
  static bool e_stop = 0, auto_mode = 0;
  brake.Check();

  // get data from serial
  // get desired steering and speed
  if (auto_mode)
  {
    // Receiving data from High Level
    ParseStateError r = RxStateHiLevel.update();
    if (r == ParseStateError::success) {
      desired_speed_cmPs = RxDataHiLevel.speed_cmPs;
      desired_angle = RxDataHiLevel.angle_mDeg;
    }
  }
  computeSpeed(&history);
  computeAngle(); // TO DO: Convert angle to right units for PID and for sending to High Level.
  
  // Write data to High Level
  TxDataHiLevel.speed_cmPs = (speedCyclometerInput_mmPs + 5) / 10;
  TxDataHiLevel.write(TxStateHiLevel.output);

  // Get data from RC unit
  ParseStateError r = RC_State.update();
  if (r == ParseStateError::success) {
    e_stop = RC_Data.number & 0x01;
    auto_mode = RC_Data.number & 0x02;
    if (!auto_mode)
    {
      desired_speed_cmPs = RC_Data.speed_cmPs; 
      desired_angle = RC_Data.angle_mDeg; 
    }
  } 
  if (e_stop)
  {
    brake.Stop();
    engageWheel(0); // Turn off wheel
  }
  else
  { // Control trike to desired speed and angle
    if (auto_mode) {
      // convert HL values
      SteeringPID(convertHLToTurn(desired_angle));
      ThrottlePID(desired_speed_cmPs);
    }
    else {
      // using RC
      if (desired_speed_cmPs < -10) {
        // throttle control is negative/reverse
        brake.Stop();
        engageWheel(0);
      }
      else {
        brake.Release();
        engageWheel( convertRCToThrottle(desired_speed_cmPs) );
      }
      engageSteering( convertRCToTurn(desired_angle) );
    }
  }
  
  // DO NOT INSERT ANY LOOP CODE BELOW THIS POINT.

  // Figure out how long we need to wait to reach the desired end time
  // for this loop pass. First, get the actual end time. Note: Beyond this
  // point, there should be *no* more controller activity -- we want
  // minimal time between now, when we capture the actual loop end time,
  // and when we pause til the desired loop end time.
  endTime = millis();
  delayTime = 0UL;

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
      delayTime = 0UL;
    }
  }

  // Did we spend long enough in the loop that we should immediately start
  // the next pass?
  if (delayTime > 0UL) {
    // No, pause til the next loop start time.
    delay(delayTime);
  }
}

/*-------------------------------------testBrakes-------------------------------------------*/
/*
 * Purely for testing during setup if brake actuator is responding. Delay is to allow enough
 * time for actuator to move
*/
void testBrakes()
{
  brake.Stop();
  delay(1000);
  brake.Release();
  if (DEBUG) {
    Serial.println(F("Brake test Complete!"));
  }
}

/*-------------------------------------testWheel-------------------------------------------*/
/*
 * Purely for testing during setup if wheel motor is responding. Delay is to allow enough
 * time for motor to run at low speed
*/
void testWheel()
{
  engageWheel(MIN_ACC_OUT); // Min speed (~85 PWM)
  delay(2000);
  engageWheel(0); // Turn off wheel
  if (DEBUG) {
    Serial.println(F("Wheel test Complete!"));
  }
}

/*-------------------------------------testSteering-------------------------------------------*/
/*
 * Purely for testing during setup if wheels actuator is responding. Delay is to allow enough
 * time for actuator to move
*/
void testSteering()
{
  engageSteering(WHEEL_MAX_LEFT_US);
  delay(3000);
  engageSteering(WHEEL_MAX_RIGHT_US);
  delay(6000);
  engageSteering(WHEEL_STRAIGHT_US);
  delay(1000);
  if (DEBUG) {
    Serial.println(F("Steering test Complete!"));
  }
}
/*-------------------------------------engageWheel-------------------------------------------*/
/*
 * Used as a more friendly way to engage the back wheel by passing the PWM value to DAC_Write
 * (TCF June 1, 2018) None of this is really PWM. It is a digital value from 0 to 255 that is 
 * converted to a true analog voltage.
 * Input: PWM value corresponding to desired speed (0-255 : min-max)
 * Output: None
*/
void engageWheel(int inputPWM)
{
    /* !UPDATE THIS OBSERVED INFO! (LAST UPDATE: May 10, 2013, TCF)
    0.831 V at rest 52 counts
    1.20 V: nothing 75
    1.27 V: just starting 79
    1.40 V: slow, steady 87
    1.50 V: brisker 94
    3.63 V: max 227 counts
    255 counts = 4.08 V
  */
  if(inputPWM != currentThrottlePWM)
  {
    DAC_Write(DAC_CHANNEL, inputPWM); // Pass PWM value and correct channel to DAC_Write 
    currentThrottlePWM = inputPWM;  // Remember most recent throttle PWM value.
    if (DEBUG) {
      Serial.print("Wheel engaged = ");
      Serial.println(currentThrottlePWM);
    }
  }
}

/*-------------------------------------engageSteering-------------------------------------------*/
/*
 * Engages the steering system by extending or retracting the actuator. Input value is in
 * microseconds (~1000-2000 : min-max), where the min value positions the brake into standby
 * (or near disk grab) position
 * Input: Int for microseconds to move the actuator
 * Output: None
*/
void engageSteering(int inputMicroseconds)
{
  // 1 sensor tick = 1.7 us of servo
  if(inputMicroseconds != currentSteeringUS)
  {
    STEER_SERVO.writeMicroseconds(inputMicroseconds);
//    Results.angle_mDeg = inputMicroseconds; // Need to do some kind of backwards conversion to C6
    currentSteeringUS = inputMicroseconds;
    if (DEBUG) {
      Serial.print("Steering engaged = ");
      Serial.println(inputMicroseconds);
    }
  }
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
      {
        byte1 |= 0x80; // second channnel
      }
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
    if(address <= 3)
    {
      if(address == 3)
      {
        byte1 |= 0x80; // second channnel
      }
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
    digitalWrite(SelectCD, HIGH);
  }
}
/*----------------------------setupWheelRev----------------------------------------------*/
void setupWheelRev()
{
  float MinTick = WHEEL_CIRCUM_MM;
  MinTick *= 1000.0;
  MinTick /= MAX_SPEED_mmPs;
  MinTickTime_ms = MinTick;
  float MIN_SPEED_mmPs = ((MIN_SPEED_mPh * 1000.0) / 3600.0);
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

/*----------------------------WheelRev---------------------------------------------------*/
// WheelRev is called by an interrupt.
void WheelRev()
{
  countz++;
  oldClickNumber = ClickNumber;
  //static int flip = 0;
  unsigned long tick;
  noInterrupts();
  tick = millis();
  if(InterruptState != IRQ_RUNNING)
  {
    // Need to process 1st two interrupts before results are meaningful.
    InterruptState++;
  }
  
  if((tick - TickTime) > MinTickTime_ms)
  {
    OldTick = TickTime;
    TickTime = tick;
    ++ClickNumber;
  }
  interrupts();
}

/*----------------------------computeSpeed-----------------------------------------------*/
void computeSpeed(struct hist *data)
{
  unsigned long WheelRev_ms = TickTime - OldTick;
  float SpeedCyclometer_revPs = 0.0; //revolutions per sec
  if((InterruptState == IRQ_NONE) || (InterruptState == IRQ_FIRST))
  { // No data
    speedCyclometerInput_mmPs = 0;
    SpeedCyclometer_revPs = 0;
  }
  else if(InterruptState == IRQ_SECOND)
  { //  first computed speed
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    speedCyclometerInput_mmPs = data->oldSpeed_mmPs = data->olderSpeed_mmPs = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    data->oldTime_ms = OldTick;
    data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber = ClickNumber;
  }
  else if(InterruptState == IRQ_RUNNING)
  { //  new data for second and following computed speeds
    if(TickTime == data->nowTime_ms)
    {//no new data
      //check to see if stopped first
      unsigned long timeStamp = millis();
      if((timeStamp - data->nowTime_ms) > MaxTickTime_ms)
      { // too long without getting a tick
        speedCyclometerInput_mmPs = 0;
        SpeedCyclometer_revPs = 0;
        if((timeStamp - data->nowTime_ms) > (2 * MaxTickTime_ms))
        {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
        }
        return;
      }
      if(data->oldSpeed_mmPs > speedCyclometerInput_mmPs)
      { // decelerrating, extrapolate new speed using a linear model
//       Serial.println("data->oldSpeed_mmPs > speedCyclometerInput_mmPs");
       float deceleration = ((float)(data->oldSpeed_mmPs - speedCyclometerInput_mmPs) / (float)(timeStamp - data->nowTime_ms));
       speedCyclometerInput_mmPs = (data->oldSpeed_mmPs - deceleration * (timeStamp - data->nowTime_ms));
       if(speedCyclometerInput_mmPs < 0)
       {
         speedCyclometerInput_mmPs = 0;
       }
       SpeedCyclometer_revPs = (speedCyclometerInput_mmPs / WHEEL_CIRCUM_MM);
      }
    }
    else // data is different from last
    {
      //update time block
      data->olderTime_ms = data->oldTime_ms;
      data->oldTime_ms = data->nowTime_ms;
      data->nowTime_ms = TickTime;
      data->oldClickNumber = data->nowClickNumber;
      data->nowClickNumber = ClickNumber;
  
      //update speed block
      data->olderSpeed_mmPs = data->oldSpeed_mmPs;
      data->oldSpeed_mmPs = speedCyclometerInput_mmPs;
      SpeedCyclometer_revPs = (1000.0 / WheelRev_ms);
      speedCyclometerInput_mmPs  = (WHEEL_CIRCUM_MM * SpeedCyclometer_revPs);
  
      data->oldTickMillis = data->tickMillis;
      data->tickMillis = millis();
      
      data->currentSpeed_kmPh = speedCyclometerInput_mmPs/260.0;
      distance_mm += ((data->oldTime_ms - data->olderTime_ms)/1000.0) * (data->oldSpeed_mmPs);
  
      if(data->TickTime_ms-data->OldTick_ms > 1000)
      {
        data->currentSpeed_kmPh = 0;
      }
    }
  }
}

int convertRCToTurn(int RCturn) {
  // we convert byte values (0-255) to microseconds (1000-2000)
  int turn = (int)(RCturn * 3.9370) + 1000; // 3.9370 = 500/127, i.e. us/byte = slope 
  if (DEBUG) {
    Serial.print("RC turn input: ");
    Serial.println(RCturn);
    Serial.print("RC turn us: ");
    Serial.println(turn);
  }
  return turn;
}

int convertRCToThrottle(int RCthrottle) {
  // we convert byte values (0-255, with center = 127) to throttle (~50-255)
  // remember this will return negative throttle values
  int throttle = (RCthrottle - 127) * 2;
  if (DEBUG) {
    Serial.print("RC throttle input: ");
    Serial.println(RCthrottle);
    Serial.print("RC throttle: ");
    Serial.println(throttle);
  }
  return throttle;
}

/*************************** START HIGH LEVEL PROCESSING SECTION ********************************/

/*------------------------------------convertHLToTurn------------------------------------------*/

int convertHLToTurn(int turnValue)
{
  // TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
  return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US);
  //return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight);
}

int convertHLToSpeed(int speedValue)
{
  // TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
 // return map(speedValue, 0, MAX_SPEED_CMS, MIN_ACC_OUT, MAX_ACC_OUT);
}

/*---------------------------SteeringPID--------------------------------------*/
// Computer Steering PID
// Precondition: None
// Postcondition: None
void SteeringPID(int desiredValue)
{
  desiredTurn_us = desiredValue;
  // Input into PID is microseconds and output is US
  steerPID.Compute();
  Serial.print("  PID STEER OUT = ");
  Serial.println(PIDSteeringOutput_us);
  engageSteering((int)PIDSteeringOutput_us);
  if (DEBUG) {
    Serial.print("PID STEER OUT = ");
    Serial.println(PIDSteeringOutput_us);
  }
}
/*------------------------------------ThrottlePID--------------------------------*/
// Compute PID 
// Precondition: None
// Postcondition: None
void ThrottlePID(int desiredValue)
{
  desiredSpeed_mmPs = desiredValue;
  // Input into PID is PWM and output is PWM
  if(desiredSpeed_mmPs < (speedCyclometerInput_mmPs + 10))
  {
     brake.Stop();
  }
  else
  {
    speedPID.Compute();
    currentThrottlePWM = (int)PIDThrottleOutput_pwm;
    brake.Release();
    engageWheel(currentThrottlePWM);
    if (DEBUG) {
      Serial.print("PID THROTTLE OUT = ");
      Serial.println(PIDThrottleOutput_pwm);
    }
  }
  if (DEBUG) {
    Serial.print("SPEED MMPS = ");
    Serial.print(speedCyclometerInput_mmPs);
    Serial.print(" DESIRED SPEED = ");
    Serial.println(desiredSpeed_mmPs);
  }
}

void computeAngle()
{
  int left = analogRead(A2);
  int right = analogRead(A3);

   steerAngleUS = map(analogRead(A3), calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight, calibratedWheelMaxLeft_us, calibratedWheelMaxRight_us);
 }

/*************************** END HIGH LEVEL PROCESSING SECTION ********************************/

