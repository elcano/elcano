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
double SteerAngle_wms = WHEEL_STRAIGHT_US; //Steering angle in microseconds used by writeMicroseconds function. Note: doubles on Arduinos are the same thing as floats, 4bytes, single precision
double PIDSteeringOutput; //Output from steerPID.Compute() in microseconds (used by Servo.writeMicroseconds())
double desiredAngle = WHEEL_STRAIGHT_US;
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
int  currentThrottlePWM = MIN_ACC_OUT; // ex. throttle_control
int  currentBrakeUS = MIN_BRAKE_US; // ex. brake_control
int  currentSteeringUS = WHEEL_STRAIGHT_US; // ex. steer_control
float Odometer_m = 0;
float HubSpeed_kmPh;
//const float  HubSpeed2kmPh = 13000000;
const unsigned long HubAtZero = 1159448;

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
#define MEG 1000000
// #define MAX_SPEED_KPH 50 Already defined in Settings file
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

int calibratedWheelMaxLeftUS; // CALIBRATED_WHEEL_MAX_LEFT_US
int calibratedWheelStraightUS; // CALIBRATED_WHEEL_STRAIGHT_US
int calibratedWheelMaxRightUS; // CALIBRATED_WHEEL_MAX_RIGHT_US

int calibratedWheelSensorMaxLeft; // CALIBRATED_WHEEL_MAX_LEFT_US
int calibratedWheelSensorMaxRight; // CALIBRATED_WHEEL_MAX_RIGHT_US

bool syncedRCValues = false;
int middleRCThrottleValue = -1;
int middleRCTurnValue = -1;
int middleRCBrakeValue = -1;

int oldSpeedValue = MIN_ACC_OUT;
int oldTurnValue = WHEEL_STRAIGHT_US;

bool ebrakeEngaged = false;

void setup()
{
  startTime = millis(); // PURPOSE ??
  
  //Set up pins
  STEER_SERVO.attach(STEER_OUT_PIN);
  BRAKE_SERVO.attach(BRAKE_OUT_PIN);

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
  steerPID.SetOutputLimits(RIGHT_TURN_OUT, LEFT_TURN_OUT); //useful if we want to change the limits on what values the output can be set to
  steerPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
  // **********************************************************

  for (int channel = 0; channel < 4; channel++)
  {
    DAC_Write(channel, 0); // reset did not clear previous states
  }

  Serial.begin(9600);
  
  // ******* START: System Test and Calibration Cycle ******** \\
  delay(1000);
  testBrakes();
  delay(2000);
  //testWheel();
  //testSteering();
  calibrateSteering(); // RECORD THESE VALUES AND USE FOR CONVERSIONS
  delay(500);
  // ******* END: System Test and Calibration Cycle ******** \\
  
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
  //CalibrateTurnAngle(32, 20); // Used in original to recalibrate wheel sensors,
  // potentially not needed if there is no sensor drift

  calibrationTime_ms = millis(); // PURPOSE ??
  
  // steer(STRAIGHT_TURN_OUT); NOT NEEDED ??
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN),  ISR_TURN_rise,  RISING);//turn right stick l/r turn
  attachInterrupt(digitalPinToInterrupt(IRPT_GO),    ISR_GO_rise,    RISING);//left stick l/r
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);//ebrake
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);//left stick u/d mode select
  attachInterrupt(digitalPinToInterrupt(IRPT_MOTOR_FEEDBACK), ISR_MOTOR_FEEDBACK_rise, RISING);
  
  //Serial.begin(9600);
  
  parseState.dt = &Results;
  parseState.input = &Serial3;
  parseState.output = &Serial3;
  Serial3.begin(baudrate);
  parseState.capture = MsgType::drive;
  // msgType::drive uses `speed_cmPs` and `angle_deg`
  Results.clear();
  SpeedCyclometer_mmPs = 0;
  // brake(false); NOT NEEDED ??
}

void loop()
{
//  engageSteering(calibratedWheelStraightUS);
//  delay(3000);
//  engageSteering(calibratedWheelMaxRightUS);
//  delay(10000);
//  engageSteering(calibratedWheelStraightUS);
//  delay(5000);
//  engageSteering(calibratedWheelMaxLeftUS);
//  delay(10000);
  
  // TO ADD:
  // PAT'S TIMER SECTION
  // WAITING TO BE COMPLETED
  // CALCULATION AND PROCESSING DATA WILL BE PLACED INSIDE OF IT
  
  // ******* START: Report Data (Speed...etc) Back to HighLevel System ******** \\
  Results.clear();
  Results.kind = MsgType::drive;
  Results.speed_cmPs = (SpeedCyclometer_mmPs / 10); // SpeedCyclometer_mmPs calculated by computeSpeed()
//       //Results.angle_mDeg = ; NEEDS TO BE SET TO CURRENT ANGLE
  Results.write(&Serial3); // PROBABLY NEEDS TO GO AFTER COMPUTING FUNCTIONS
  
  nextTime = nextTime + LOOP_TIME_MS;
  
  //computeSpeed(&history);
  
  //computeAngle(&history); UNUSED CURRENTLY
  
  //ThrottlePID(); UNUSED CURRENTLY UNTIL
  //SteeringPID(); PID ACCURACY IS SOLVED
  
  // ******* END: Report Data (Speed...etc) Back to HighLevel System ******** \\

  if(!emergencyBrakeEngaged())
  {
//     NOTE TURN CONVERSIONS WILL NEED MIN AND MAX TURN VALUES REPLACED
//     WITH ONES ACQUIRED FROM CALIBRATIONS
    Serial.println("EBrake Disengaged");
    if (isAutonomous())
    {
      Serial.println("isAutonomous = TRUE");
      // Process High Level Input if Anyhing to Parse
      ParseStateError r = parseState.update();
      if(r == ParseStateError::success)
      {
        Serial.println("Something to Process");
        processHighLevelInput(&Results); // Calls moveVehicle()
        //moveVehicle();
      }
    }
    else
    {
      Serial.println("isAutonomous = FALSE");
      processRCInput(); // Calls moveVehicle()
      //moveVehicle();
    }
  }
  else
  {
    Serial.println("EBrake Engaged");
  }
  delay(1);
  //processHighLevelInput(&Results); // Calls moveVehicle()
  // DATA LOGGING: OUTPUT TO SD CARD
  // Will display error if file not found
  //logData(); Currently not needed
}

/*-------------------------------------testBrakes-------------------------------------------*/
/*
 * Purely for testing during setup if brake actuator is responding. Delay is to allow enough
 * time for actuator to move
*/
void testBrakes()
{
  engageBrakes(MAX_BRAKE_US);
  delay(1000);
  engageBrakes(MIN_BRAKE_US);
  Serial.println("**** Brake test Complete! ****");
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
  Serial.println("**** Wheel test Complete! ****");
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
  Serial.println("**** Steering test Complete! ****");
}

/*-------------------------------------calibrateSteering-------------------------------------------*/
/*
 * Used for calibrating the new straight wheels sensor value and servo microseconds. The initial wheels sensor
 * value is read and depending on how far it is from WHEEL_STRAIGHT_SENSOR it increments or decrements the microseconds
 * sent to the servo. Once within range memorize the microseconds and sensor value
 * Input: None
 * Output: None
*/
void calibrateSteering()
{
  Serial.println("**** Steering calibration Begin! ****");
  calibratedWheelMaxLeftUS = 2000;
  engageSteering(calibratedWheelMaxLeftUS);
  delay(3000);
  int startingSensorValue = analogRead(A2);
  int currentSensorValue = analogRead(A2);
  while((startingSensorValue - currentSensorValue) < 3)
  {
    calibratedWheelMaxLeftUS -= 1;
    engageSteering(calibratedWheelMaxLeftUS);
    delay(2);
    currentSensorValue = analogRead(A2);
    Serial.print(analogRead(A2));
    Serial.print(" ");
    Serial.println(calibratedWheelMaxLeftUS);
  }
  calibratedWheelSensorMaxLeft = currentSensorValue;

  calibratedWheelStraightUS = (WHEEL_STRAIGHT_US + 100);
  engageSteering(calibratedWheelStraightUS);
  delay(1500);
  currentSensorValue = analogRead(A2);
  while(!((currentSensorValue > (WHEEL_STRAIGHT_SENSOR - 1)) && (currentSensorValue < (WHEEL_STRAIGHT_SENSOR + 1))))
  {
    calibratedWheelStraightUS -= 1;
    engageSteering(calibratedWheelStraightUS);
    delay(2);
    currentSensorValue = analogRead(A2);
    Serial.print(analogRead(A2));
    Serial.print(" ");
    Serial.println(calibratedWheelStraightUS);
  }

  calibratedWheelMaxRightUS = 950;
  engageSteering(calibratedWheelMaxRightUS);
  delay(1500);
  startingSensorValue = analogRead(A2);
  currentSensorValue = analogRead(A2);
  while((currentSensorValue - startingSensorValue) < 3)
  {
    calibratedWheelMaxRightUS += 1;
    engageSteering(calibratedWheelMaxRightUS);
    delay(2);
    currentSensorValue = analogRead(A2);
    Serial.print(analogRead(A2));
    Serial.print(" ");
    Serial.println(calibratedWheelMaxRightUS);
  }
  calibratedWheelSensorMaxRight = currentSensorValue;
  delay(1000); 
  
  Serial.println("**** Steering calibration Complete! ****");
  Serial.print("calibratedWheelSensorMaxRight = ");
  Serial.print(calibratedWheelSensorMaxRight);
  Serial.println("  ");
  Serial.print("calibratedWheelSensorMaxLeft = ");
  Serial.print(calibratedWheelSensorMaxLeft);
  Serial.println("  ");
  Serial.print("WHEEL_STRAIGHT_SENSOR = ");
  Serial.print(WHEEL_STRAIGHT_SENSOR);
  Serial.println("  ");
  Serial.print("calibratedWheelMaxRightUS = ");
  Serial.print(calibratedWheelMaxRightUS);
  Serial.println("  ");
  Serial.print("calibratedWheelMaxLeftUS = ");
  Serial.print(calibratedWheelMaxLeftUS);
  Serial.println("  ");
  Serial.print("calibratedWheelStraightUS = ");
  Serial.println(calibratedWheelStraightUS);
  Serial.println();
  int minSensorDelta = min((calibratedWheelSensorMaxLeft - WHEEL_STRAIGHT_SENSOR), (WHEEL_STRAIGHT_SENSOR - calibratedWheelSensorMaxRight));
  calibratedWheelSensorMaxLeft = (WHEEL_STRAIGHT_SENSOR + minSensorDelta);
  calibratedWheelSensorMaxRight = (WHEEL_STRAIGHT_SENSOR - minSensorDelta);
  int minWheelUSDelta = min((calibratedWheelMaxLeftUS - calibratedWheelStraightUS), (calibratedWheelStraightUS - calibratedWheelMaxRightUS));
  calibratedWheelMaxLeftUS = (calibratedWheelStraightUS + minWheelUSDelta);
  calibratedWheelMaxRightUS = (calibratedWheelStraightUS - minWheelUSDelta);
  Serial.println("**** NEW CALIBRATIONS ****");
  Serial.print("calibratedWheelSensorMaxRight = ");
  Serial.print(calibratedWheelSensorMaxRight);
  Serial.println("  ");
  Serial.print("calibratedWheelSensorMaxLeft = ");
  Serial.print(calibratedWheelSensorMaxLeft);
  Serial.println("  ");
  Serial.print("WHEEL_STRAIGHT_SENSOR = ");
  Serial.print(WHEEL_STRAIGHT_SENSOR);
  Serial.println("  ");
  Serial.print("calibratedWheelMaxRightUS = ");
  Serial.print(calibratedWheelMaxRightUS);
  Serial.println("  ");
  Serial.print("calibratedWheelMaxLeftUS = ");
  Serial.print(calibratedWheelMaxLeftUS);
  Serial.println("  ");
  Serial.print("calibratedWheelStraightUS = ");
  Serial.println(calibratedWheelStraightUS);
  Serial.println();
}

/*-------------------------------------engageBrakes-------------------------------------------*/
/*
 * Engages the brake system by extending or retracting the actuator. Input value is in
 * microseconds (~1250-1980 : min-max), where the min value positions the brake into standby
 * (or near disk grab) position
 * Input: Boolean whether or not to fully break
 * Output: None
*/
void engageBrakes(int brakeValue)
{
  // Extend actuator and engage brake
  BRAKE_SERVO.writeMicroseconds(brakeValue);
  currentBrakeUS = brakeValue;  // Remember most recent brake microseconds value.
  Serial.print("Brake engaged = ");
  Serial.print(brakeValue);
  Serial.print("  ");
  Serial.print("currentBrakeUS = ");
  Serial.println(currentBrakeUS);
}

/*-------------------------------------engageWheel-------------------------------------------*/
/*
 * Used as a more friendly way to engage the back wheel by passing the PWM value to DAC_Write
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
  DAC_Write(DAC_CHANNEL, inputPWM); // Pass PWM value and correct channel to DAC_Write 
  currentThrottlePWM = inputPWM;  // Remember most recent throttle PWM value.
  Serial.print("Wheel engaged = ");
  Serial.println(currentThrottlePWM);
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
  STEER_SERVO.writeMicroseconds(inputMicroseconds);
  
  //currentSteeringUS = inputMicroseconds;  // Remember most recent steering microseconds value.
  Results.angle_mDeg = inputMicroseconds; // Need to do some kind of backwards conversion to C6
  
  //Serial.print("Steering engaged = ");
  //Serial.println(currentSteeringUS);
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

void syncRCInputValues()
{
  if(RC_Done[RC_GO])
  {
    middleRCThrottleValue = RC_elapsed[RC_GO];
  }
  if(RC_Done[RC_TURN])
  {
    middleRCTurnValue = RC_elapsed[RC_TURN];
  }
  if(RC_Done[RC_BRAKE])
  {
    middleRCBrakeValue = RC_elapsed[RC_BRAKE];
  }
  Serial.println("******* Syncing Complete! *******");
  Serial.print(middleRCThrottleValue);
  Serial.print(" ");
  Serial.print(middleRCTurnValue);
  Serial.print(" ");
  Serial.println(middleRCBrakeValue);
}

void unsyncRCInputValues()
{
  middleRCThrottleValue = -1;
  middleRCTurnValue = -1;
  middleRCBrakeValue = -1;
  Serial.println("******* Unyncing Complete! *******");
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
  
  speedPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute

  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
}

/*----------------------------WheelRev---------------------------------------------------*/
// WheelRev is called by an interrupt.
void WheelRev()
{
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
  //cyclometer has only done 1 or 2 revolutions
  //normal procedures begin here
  unsigned long WheelRev_ms = TickTime - OldTick;
  float SpeedCyclometer_revPs = 0.0; //revolutions per sec
  if((InterruptState == IRQ_NONE) || (InterruptState == IRQ_FIRST))
  { // No data
    SpeedCyclometer_mmPs = 0;
    SpeedCyclometer_revPs = 0;
  }
  else if(InterruptState == IRQ_SECOND)
  { //  first computed speed
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs = data->oldSpeed_mmPs = data->olderSpeed_mmPs = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    data->oldTime_ms = OldTick;
    data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber = ClickNumber;
  }
  else if(InterruptState == IRQ_RUNNING)
  { //  new data for second computed speed
    if(TickTime == data->nowTime_ms)
    {//no new data
      //check to see if stopped first
      unsigned long timeStamp = millis();
      if((timeStamp - data->nowTime_ms) > MaxTickTime_ms)
      { // too long without getting a tick
        SpeedCyclometer_mmPs = 0;
        SpeedCyclometer_revPs = 0;
        if((timeStamp - data->nowTime_ms) > (2 * MaxTickTime_ms))
        {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
        }
        return;
       }
       if(data->oldSpeed_mmPs > SpeedCyclometer_mmPs)
       { // decelerrating, extrapolate new speed using a linear model
         float deceleration = ((float)(data->oldSpeed_mmPs - SpeedCyclometer_mmPs) / (float)(timeStamp - data->nowTime_ms));
         SpeedCyclometer_mmPs = (data->oldSpeed_mmPs - deceleration * (timeStamp - data->nowTime_ms));
         if(SpeedCyclometer_mmPs < 0)
         {
           SpeedCyclometer_mmPs = 0;
         }
         SpeedCyclometer_revPs = (SpeedCyclometer_mmPs / WHEEL_CIRCUM_MM);
       }
       else
       { // accelerating; should get new data soon
         // ????
       }
    }
    else
    {
      //update time block
      data->olderTime_ms = data->oldTime_ms;
      data->oldTime_ms = data->nowTime_ms;
      data->nowTime_ms = TickTime;
      data->oldClickNumber = data->nowClickNumber;
      data->nowClickNumber = ClickNumber;
  
      //update speed block
      data->olderSpeed_mmPs = data->oldSpeed_mmPs;
      data->oldSpeed_mmPs = SpeedCyclometer_mmPs;
      SpeedCyclometer_revPs = (1000.0 / WheelRev_ms);
      SpeedCyclometer_mmPs  = (WHEEL_CIRCUM_MM * SpeedCyclometer_revPs);
  
      data->oldTickMillis = data->tickMillis;
      data->tickMillis = millis();
      
      data->currentSpeed_kmPh = SpeedCyclometer_mmPs/260.0;
      distance_mm += ((data->oldTime_ms - data->olderTime_ms)/1000.0) * (data->oldSpeed_mmPs);
  
      if(data->TickTime_ms-data->OldTick_ms > 1000)
      {
        data->currentSpeed_kmPh = 0;
      }
    }
  }
//  Serial.println("**** Compute Speed Complete! ****");
//  Serial.print("Older Speed mmPs = ");
//  Serial.print(data->olderSpeed_mmPs);
//  Serial.print("  ");
//  Serial.print("Data Current Speed kmPh = ");
//  Serial.print(data->currentSpeed_kmPh);
//  Serial.print("  ");
//  Serial.print("Current Speed mmPs = ");
//  Serial.print(SpeedCyclometer_mmPs);
//  Serial.print("  ");
//  Serial.print("Old Speed mmPs = ");
//  Serial.println(data->oldSpeed_mmPs);
}

/*-----------------------------------emergencyBrakeEngaged-----------------------------------------*/
/*
 * 
 */
// INCCORECT INFO
// Check if ebrake is on
// Ebrake is on if PWM of the Ebrake channel on the remote is > MIDDLE else Ebrake is off
// Precondition: None
// Postcondition: return true if Ebrake is on, false otherwise
bool emergencyBrakeEngaged()
{
  if (RC_Done[RC_ESTP]) //RC_Done determines if the signal from the remote controll is done processing
  {
    //RC_elapsed[RC_ESTP] = ((RC_elapsed[RC_ESTP] > MIDDLE) ? HIGH : LOW);
    //if (RC_elapsed[RC_ESTP] == HIGH)
    if(RC_elapsed[RC_ESTP] > MIDDLE)
    {
      engageEmergencyBrake();  // already done at interrupt level
      ebrakeEngaged = true;
      return true;
    }
    else if(ebrakeEngaged)
    {
      engageBrakes(MIN_BRAKE_US);
      ebrakeEngaged = false;
    }
  }
  return false;
}

/*-------------------------------------engageEmergencyBrake--------------------------------------*/
void engageEmergencyBrake()
{
  Serial.println("****! Emergency Brake Engaged !****");
  engageBrakes(MAX_BRAKE_US);
  moveVehicle(0, WHEEL_STRAIGHT_US);
  delay(500);   // inhibit output
  // TO DO: disable 36V power - USE A RELAY?
}

/*------------------------------------isAutonomous------------------------------------------*/
bool isAutonomous()
{
  // TO DO: FIX THIS BODY BECAUSE IT WOULD ALWAYS PASS EVEN IF
  // RC IS OFF
  if(RC_elapsed[RC_BRAKE] > (MIN_BRAKE_RC + 20))
  {
    if(!syncedRCValues)
    {
      syncRCInputValues();
      syncedRCValues = true;
    }
  }
  else
  {
    if(syncedRCValues)
    {
      unsyncRCInputValues();
      syncedRCValues = false;
    }
  }
//  Serial.print("RC_BRAKE DONE? = ");
//  Serial.print(RC_Done[RC_BRAKE]);
//  Serial.print(" ");
//  Serial.println(RC_elapsed[RC_BRAKE]);
  if(RC_Done[RC_BRAKE] && (RC_elapsed[RC_BRAKE] > (MIN_BRAKE_RC + 20)))
  {
    if(RC_elapsed[RC_BRAKE] > (middleRCBrakeValue + DEAD_ZONE))
    {
      return false;
    }
  }
  return true;
}

/*************************** START HIGH LEVEL PROCESSING SECTION ********************************/

/*------------------------------------processHighLevelInput------------------------------------------*/
void processHighLevelInput(SerialData * results)
{
  // TO DO: FIGURE OUT IF SPEED CONVERSION IS CORRECT
  Serial3.end();
  Serial3.begin(baudrate);  // clear the buffer
  
  //double turn_signal = convertDeg(results->angle_mDeg / 1000.);
  //Serial.println(results->angle_mDeg);
  
  int turnValue = convertHLToTurn(results->angle_mDeg); // IF GUARANTEED TO ALWAYS
  int speedValue = (results->speed_cmPs * 10);          // RECEIVE INPUT IN CERTAIN RANGE
  moveVehicle(convertHLToSpeed(results->speed_cmPs), turnValue);
  
  //desiredSpeed = results->speed_cmPs * 10;
  //desiredAngle = turn_signal;
}

/*------------------------------------convertHLToTurn------------------------------------------*/

int convertHLToTurn(int turnValue)
{
  // TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
  //int result = map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US);
  return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, calibratedWheelMaxLeftUS, calibratedWheelMaxRightUS);
}

int convertHLToSpeed(int speedValue)
{
  // TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
  return map(speedValue, 0, 100 , MIN_ACC_OUT, MAX_ACC_OUT);
}

/*************************** END HIGH LEVEL PROCESSING SECTION ********************************/


/*************************** START RC INPUT PROCESSING SECTION ********************************/

/*------------------------------------processRCInput------------------------------------------*/
void processRCInput()
{
  // ******************** BOARD WIRING ********************
  //              (L)       BR  TR  TH      EM    (R)
  //   Top Board Pins:  *   *   *   *   *   *
  //
  //                            BR  TH  EM  TR
  //              (L)           18  19  20  21    (R)
  // Bottom Board Pins: *   *   *   *   *   *
  // Green (
  int speedValue = 0;
  int turnValue = WHEEL_STRAIGHT_US;
  int brakeValue = MIN_BRAKE_US;
  // TO DO: MAP RC INPUT TO CORRECT SPEED AND STEERING VALUES
  if(RC_Done[RC_GO])
  {
    //if(RC_elapsed[RC_GO] > (MIDDLE +  (2 * DEAD_ZONE)))
    if(RC_elapsed[RC_GO] >= (middleRCThrottleValue + 30))
    {
      speedValue = convertRCToThrottle(RC_elapsed[RC_GO]);
    }
    else if(RC_elapsed[RC_GO] <= (middleRCThrottleValue - 30))
    {
      engageBrakes(convertRCToBrake(RC_elapsed[RC_GO]));
    }
  }
  //TURN
  if (RC_Done[RC_TURN]) 
  {
    if((RC_elapsed[RC_TURN] <= middleRCTurnValue - 30) || (RC_elapsed[RC_TURN] >= middleRCTurnValue + 30))
    {
      turnValue = convertRCToTurn(RC_elapsed[RC_TURN]);
    }
  }
  if((speedValue != oldSpeedValue) || (turnValue != oldTurnValue))
  {
    moveVehicle(speedValue, turnValue);
    oldSpeedValue = speedValue;
    oldTurnValue = turnValue;
  }
}

/*------------------------------------convertRCToThrottle-------------------------------------*/
// Convert PWM values from Remote Control to corresponding trike specific throttle values 
// Precondition: MIDDLE + 2 * DEAD_ZONE < input < MAX_RCGO
int convertRCToThrottle(int throttleValue)
{
  // Maps to min and max values of wheel. Could it be used for limiting top speed????
  //int mapping = map(input, (MIDDLE + (2 * DEAD_ZONE)), MAX_RC, 80, 150);
  Serial.print("Throttle Input = ");
  Serial.println(throttleValue);
  return map(throttleValue, (middleRCThrottleValue + 30), MAX_THROTTLE_RC, MIN_ACC_OUT, MAX_ACC_OUT);
}

//int middleRCThrottleValue = -1;
//int middleRCTurnValue = -1;
//int middleRCBrakeValue = -1;

int convertRCToBrake(int brakeValue)
{
  Serial.print("Brake Input = ");
  Serial.println(brakeValue);
  return map(brakeValue, MIN_THROTTLE_RC, (middleRCThrottleValue - 30), MAX_BRAKE_US, MIN_BRAKE_US);
}

/*------------------------------------convertRCToTurn-------------------------------------*/
int convertRCToTurn(int turnValue)
{
  Serial.print("Turn Input = ");
  Serial.println(turnValue);
  //  Check if Input is in steer dead zone
  if ((turnValue <= (middleRCTurnValue + DEAD_ZONE)) && (turnValue >= (middleRCTurnValue - DEAD_ZONE)))
  {
    return WHEEL_STRAIGHT_US;
  }
  // On SPEKTRUM, MIN_RC = 1 msec = stick right; MAX_RC = 2 msec = stick left
  // On HI_TEC, MIN_RC = 1 msec = stick left; MAX_RC = 2 msec = stick right
  // LEFT_TURN_MS > RIGHT_TURN_MS
  else
  {
    //int value = map(turnValue, MIN_RC, MAX_RC, WHEEL_MAX_RIGHT_US, WHEEL_MAX_LEFT_US);
    return map(turnValue, MIN_TURN_RC, MAX_TURN_RC, calibratedWheelMaxRightUS, calibratedWheelMaxLeftUS);
    //return map(turnValue, MIN_TURN_RC, MAX_TURN_RC, WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US);
  }
  // @ToDo: Fix this so it is correct in any case.
  // If a controller requires some value to be reversed, then specify that
  // requirement in Settings.h, and use the setting here.
  /* ** PURPOSE OF THIS PART ????
#ifdef RC_HITEC
  turnValue = MAX_RC - (turnValue - MIN_RC);
#endif
*/
}

/*************************** END RC INPUT PROCESSING SECTION ********************************/

void moveVehicle(short speedValue, int turnValue)
{
  // IN THE FUTURE THIS FUNCTION WILL PROCESS OBSTACLE SENSORS
  // AND AVOID THEM. I.E. MOVEMENT BRAIN
  engageWheel(speedValue);
  engageSteering(turnValue);
}



/******************************************************************************\
* ************************* BEGIN INTERRUPT ROUTINES ************************* *
\******************************************************************************/

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
/******************************************************************************\
* *************************** END INTERRUPT ROUTINES ************************* *
\******************************************************************************/




/**********************************************************
 ******************** UNUSED SECTION **********************
 *********************************************************/

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

void computeAngle(struct hist *data)
{
  // Right sensor is a lot less precise
  //int rightSensor = analogRead(A2);
  int leftSensor = analogRead(A3);

  //int right_wms = map(rightSensor, leftsenseleft, leftsenseright, LEFT_TURN_OUT, RIGHT_TURN_OUT); // Left sensor spikes outside of calibrated range between setup() and loop(); temporarily disregard data
  int left_wms = map(leftSensor, rightsenseleft, rightsenseright, LEFT_TURN_OUT, RIGHT_TURN_OUT);
}
