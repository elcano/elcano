#include <Settings.h>

#include <PID_v1.h>
#include <SPI.h>
#include <Elcano_Serial.h>
#include <Servo.h>

// Set up state variables for the Elcano_Serial parsing ring
elcano::ParseState ps;
elcano::SerialData dt;

//#include <SoftwareSerial.h>
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

#define LOOP_TIME_US 400
#define ERROR_HISTORY 20 //number of errors to accumulate
#define TEN_SECONDS_IN_MICROS 10000000

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

// RC_rise contains the time value collected in the rising edge interrupts.
// RC_elapsed contains the width of the pulse. The rise and fall interrupts
// should alternate.
#define ProcessFallOfINT(Index)  RC_elapsed[Index]=(micros()-RC_rise[Index])
#define ProcessRiseOfINT(Index) RC_rise[Index]=micros()

// @ToDo: Do these differ per trike? If so, move to Settings.h.
const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)

const unsigned long INVALID_DATA = 0;
volatile int rc_index = 0;
volatile unsigned long RC_rise[7];
volatile unsigned long RC_elapsed[7];
volatile boolean synced = false;
volatile unsigned long last_fallingedge_time = 4294967295; // max long
volatile bool RC_Done[7] = {0, 0, 0, 0, 0, 0, 0};
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
  unsigned long old_phase_rise = RC_rise[RC_RDR];
  ProcessRiseOfINT(RC_RDR);
  RC_elapsed[RC_RDR] = RC_rise[RC_RDR] - old_phase_rise;
  // The phase frequency is proportional to wheel rotation.
  // An e-bike hub is powered by giving it 3 phased 36 V lines
  // The e-bike controller needs feeback from the hub.
  // The hub supplies 3 Hall Phase sensors; each is a 5V square wave and tells how fast the wheel rotates.
  // The square wave feedback has sone noise, which is cleaned up by an RC low pass filter
  //  with R = 1K, C = 100 nF
  //Serial.println("RDR");
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_GO_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_GO);
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
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
  //Serial.println("TURN");
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_GO_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_GO);
  RC_Done[RC_GO] = 1;
  //Serial.println("GO");
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_ESTOP_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_ESTP);
  RC_Done[RC_ESTP] = 1;
  //  if (RC_elapsed[RC_ESTP] > MIDDLE)
  //     E_Stop();
  //Serial.println("ESTOP");
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

void ISR_SWITCH_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_RDR);
  attachInterrupt(digitalPinToInterrupt(IRPT_SWITCH), ISR_SWITCH_fall, FALLING);
  //RC_Done[RC_RDR] = 1;
  interrupts();
}

void ISR_SWITCH_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_RDR);
  RC_Done[RC_RDR] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_SWITCH), ISR_SWITCH_rise, RISING);
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
  //Serial.println("MOTOR");
  interrupts();
}

/*---------------------------------------------------------------------------------------*/
//----------------------------------------------------------------------------
void setup()
{ //Set up pins
  STEER_SERVO.attach(STEER_OUT_PIN);
  pinMode(BRAKE_OUT_PIN, OUTPUT);

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
  brake(MAX_BRAKE_OUT);
  moveVehicle(MIN_ACC_OUT);
  //setup7seg();    // Initialize 7 segment display for speedometer
  delay(500);   // let vehicle stabilize
  //brake(MIN_BRAKE_OUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  rc_index = 0;
  for (int i = 0; i < 8; i++)
  {
    RC_rise[i] = INVALID_DATA;
    RC_elapsed[i] = INVALID_DATA;
  }
  for (int i = 0; i < ERROR_HISTORY; i++)
  {
    speed_errors[i] = 0;
  }
  //setupWheelRev(); // WheelRev4 addition
  CalibrateTurnAngle(32, 20);
  calibrationTime_ms = millis();
        attachInterrupt(digitalPinToInterrupt(IRPT_TURN),  ISR_TURN_rise,  RISING);
        //attachInterrupt(digitalPinToInterrupt(IRPT_RDR),   ISR_RDR_rise,   RISING);
        attachInterrupt(digitalPinToInterrupt(IRPT_GO),    ISR_GO_rise,    RISING);
        attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);
        //attachInterrupt(digitalPinToInterrupt(IRPT_RVS),   ISR_RVS_rise,   RISING);
        attachInterrupt(digitalPinToInterrupt(IRPT_SWITCH), ISR_SWITCH_rise, RISING);
        attachInterrupt(digitalPinToInterrupt(IRPT_MOTOR_FEEDBACK), ISR_MOTOR_FEEDBACK_rise, RISING);
        //Print7headers(false);
  //PrintHeaders();
  
  // Set up Elcano_Serial parsing ring
  ps.dt = &dt;
  ps.dev = &Serial1;
  dt.clear();
}
/*---------------------------------------------------------------------------------------*/
void loop1() {
    elcano::SerialData results;
    results.kind = elcano::MsgType::sensor;
    results.angle_deg = TurnAngle_degx10() / 10;
}

void loop2() {
    
}

void loop() {
    elcano::ParseStateError r = ps.update();
    if (r == elcano::ParseStateError::success) {
        if (dt.kind == elcano::MsgType::drive) {
            loop1();
        } else {
            dt.write(&Serial2);
        }
    }
    loop2();
}
#if 0
void loop() {
  // Save start time for performance report.
  unsigned long startTime = micros();
  // Get the next loop start time.
  unsigned long nextTime = startTime + LOOP_TIME_US;

  digitalWrite(27, LOW);
  digitalWrite(33, LOW);
  startCapturingRCState();

  unsigned long local_results[7];
  //  PrintDone();

  while (micros() < nextTime &&
         ~((RC_Done[RC_ESTP] == 1) && (RC_Done[RC_GO] == 1) && (RC_Done[RC_TURN] == 1) && (RC_Done[RC_RDR] == 1)))
  {
    ;  //wait
  }
  // got data;
  for (int i = 0; i < 8; i++)
    local_results[i] = RC_elapsed[i];
  //Print7(false, local_results);
  //Serial.println("Processing RC data...");
  byte automate = processRC(local_results);
  if (automate == 0x01) //remember to tell Pat to fix this, was = instead of ==
  {
    //Serial.println("Processing high-level data");
    processHighLevel(&Results);
  }
  //    Print7( true, local_results);
    
  //Serial.println(nextTime); //Old sanity check line; testing new pin output sanity check.
  //Serial.println("Clearing Results");
//  Results.Clear();
//  Results.kind = MSG_SENSOR;
//  Results.angle_deg = TurnAngle_degx10() / 10;
  //show_speed (&Results);

  // Report how long the loop took.
  
  digitalWrite(27, HIGH);
  if(nextTime < startTime || nextTime > startTime + LOOP_TIME_US) {
    digitalWrite(33, HIGH);
  }
  //Serial.println(nextTime); //Old sanity check line; testing new pin output sanity check.
  //Serial.println("Clearing Results");
  //Results.clear();
  Results.kind = MSG_SENSOR;
  Results.angle_deg = TurnAngle_degx10() / 10;
  //show_speed (&Results);

  //LogData(local_results, &Results);  // data for spreadsheet

  calibrationTime_ms += LOOP_TIME_US;
  straightTime_ms = (steer_control == STRAIGHT_TURN_OUT) ? straightTime_ms + LOOP_TIME_US : 0;
  stoppedTime_ms = (throttle_control == MIN_ACC_OUT) ? stoppedTime_ms + LOOP_TIME_US : 0;
  if (calibrationTime_ms > 40000 && straightTime_ms > 3000 && stoppedTime_ms > 3000)
  {
    //       int oldBrake = brake_control;
    //       brake(MAX_BRAKE_OUT);  // put on brakes
    CalibrateTurnAngle(16, 10);  // WARNING: No response to controls while calibrating
    //       brake(oldBrake);       // restore brake state
    calibrationTime_ms = 0;
  }
  
  unsigned long endTime = micros();
  // unsigned long elapsedTime = endTime - startTime;
  // Serial.print("loop elapsed time = ");
  // Serial.println(elapsedTime);
  // Did we spend long enough in the loop that we should immediately
  // start the next pass?
  //Serial.print("Time: "); Serial.print(endTime); Serial.print(", Next: "); Serial.println(nextTime);
  if (nextTime > endTime) {
    // No, pause til the next loop start time.
    Serial.print("Start Sanity: "); Serial.println(startTime);
    Serial.print("Const Sanity: "); Serial.println(LOOP_TIME_US);
    Serial.print("Next Sanity: "); Serial.println(nextTime);
    Serial.print("End Sanity: "); Serial.println(endTime);
    Serial.print("Delaying: "); Serial.println(nextTime - endTime);
    delay(nextTime - endTime);
  } else {
    // Yes, we overran the expected loop interval. Extend the time.
    nextTime = endTime + LOOP_TIME_US;
  }
  
  //Serial.println("End of loop");
}
#endif
/*---------------------------------------------------------------------------------------*/
void PrintDone()
{
  Serial.print("RC_Done values: RC_ESTP "); Serial.print(RC_Done[RC_ESTP]);
  Serial.print(" RC_GO "); Serial.print(RC_Done[RC_GO]);
  Serial.print(" RC_TURN "); Serial.print(RC_Done[RC_TURN]);
  //Serial.print(" RC_RVS "); Serial.print(RC_Done[RC_RVS]);
  // Not currently using RC_RVS, and it is always zero.

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
void LogData(unsigned long commands[7], elcano::SerialData *sensors)  // data for spreadsheet
{
  show7seg(HubSpeed_kmPh);
  /*Serial.print("(ms) Time\t");
    Serial.print("(cm/s) Speed\t");
    Serial.print("(km/h) Speed\t");
    Serial.print("(km/h) Hub Speed\t");
    Serial.print("(deg) Angle\t");
    Serial.print("Right\t");
    Serial.print("Left\t");
    Serial.print("Throttle\t");
    Serial.print("Brake\t");
    Serial.print("Steer\t");
    Serial.println("(m) Distance");*/
  Serial.print(millis()); Serial.print("\t");                        //(ms) Time
  Serial.print(sensors->speed_cmPs); Serial.print("\t");             //(cm/s) Speed
  Serial.print(sensors->speed_cmPs * 36.0 / 1000.); Serial.print("\t"); //(km/h) Speed
  Serial.print(HubSpeed_kmPh); Serial.print("\t");                   //(km/h) Hub Speed
  Serial.print(sensors->angle_deg); Serial.print("\t");              //(deg) Angle
  int right = analogRead(A3);
  int left = analogRead(A2);
  Serial.print(right); Serial.print("\t");                           //Right turn sensor
  Serial.print(left); Serial.print("\t");                            //Left turn sensor
  Serial.print(throttle_control); Serial.print("\t");                //Throttle
  Serial.print(brake_control); Serial.print("\t");                   //Brake
  Serial.print(steer_control); Serial.print("\t");                   //Steer
  Serial.println(Odometer_m);                                        //(m) Distance
  //     Print7 (true, commands);
}
/*---------------------------------------------------------------------------------------*/
void PrintHeaders (void)
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

/*---------------------------------------------------------------------------------------*/
//circleRoutine
void circleRoutine(unsigned long seconds, unsigned long &rcAuto) {
  rcAuto = HIGH;
  
  steer(LEFT_TURN_OUT);
  delay(1000);
  seconds = seconds * 1000;
  unsigned long loopTime = millis();
  while (millis() < (loopTime + seconds)) {
    moveVehicle(112);
  }
  rcAuto = LOW;
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
    //steer(LEFT_TURN_OUT);
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
/*---------------------------------------------------------------------------------------*/
void startCapturingRCState(){
  for (int i = 1; i < 7; i++) {
    RC_Done[i] = 0;
  }
}

///*---------------------------------------------------------------------------------------*/
////done in setup, calibrate RC values for MIDDLE, MIN_RC, and MAX_RC at startup
//void calibrateRC(unsigned long mic) {
//
//  Serial.print(mic);
//  Serial.println("Calibration Started");
//  //Step 1: Wait for controller to turn on, and leave joysticks in neutral
//  //    while (micros() < TEN_SECONDS_IN_MICROS ||
//  //    ~((RC_Done[RC_ESTP]) && (RC_Done[RC_GO]) && (RC_Done[RC_TURN]) ))
//  //    digitalWrite(LED_PIN_OUT, HIGH); //wait for 10 seconds to receive radio signal to Calibrate Neutral Positions on RC controller, otherwise proceed
//
//  //Step 2: Calibrate MIDDLE
//  Serial.println(RC_elapsed[RC_GO]);
//  mid = RC_elapsed[RC_GO];
//  Serial.print("MIDDLE \t");
//  Serial.println(mid);
//  mic = micros();
//
//  //Step 3: Turn on LED and wait for Right joystick DOWN
//  //digitalWrite(LED_PIN_OUT, HIGH);
//  while (micros() < (mic + (TEN_SECONDS_IN_MICROS / 4)))
//    //digitalWrite(LED_PIN_OUT, HIGH);//wait
//    //if
//    //digitalWrite(LED_PIN_OUT, LOW);
//    //Set MIN_RC
//    min_rc = RC_elapsed[RC_GO];
//  Serial.print("MIN_RC \t");
//  Serial.println(min_rc);
//  mic = micros();
//
//  //Step 4:Turn on LED again and wait for Right joystick UP
//  //digitalWrite(LED_PIN_OUT, HIGH);
//  while (micros() < (mic + (TEN_SECONDS_IN_MICROS / 4)))
//    //digitalWrite(LED_PIN_OUT, HIGH);//wait
//
//    max_rc = RC_elapsed[RC_GO];
//  Serial.print("MAX_RC \t");
//  Serial.println(max_rc);
//  //digitalWrite(LED_PIN_OUT, LOW);
//}

/*---------------------------------------------------------------------------------------*/
byte processRC (unsigned long *results){
  // 1st pulse is aileron (position 5 on receiver; controlled by Right left/right joystick on transmitter)
  //     used for Steering
    //    Serial.print("\tTurn Cmd "); Serial.println(results[RC_TURN]);
  /* 2nd pulse is aux (position 1 on receiver; controlled by flap/gyro toggle on transmitter)
     will be used for selecting remote control or autonomous control. */
  //    Serial.print("In processRC, received results[RC_AUTO] = "); Serial.println(results[RC_AUTO]);
  if (NUMBER_CHANNELS > 5)
    results[RC_AUTO] = (results[RC_AUTO] > MIDDLE ? HIGH : LOW);
  //    Serial.print("processed results[RC_AUTO] = "); Serial.println(results[RC_AUTO]);

  /* 4th pulse is gear (position 2 on receiver; controlled by gear/mode toggle on transmitter)
    will be used for emergency stop. D38 */
  Serial.println(results[RC_ESTP]);
  results[RC_ESTP] = (results[RC_ESTP] > MIDDLE ? HIGH : LOW);

  if (results[RC_ESTP] == HIGH){
    Serial.println("Exiting processRC due to E-stop.");
    E_Stop();  // already done at interrupt level
    if ((results[RC_AUTO] == LOW)  && (NUMBER_CHANNELS > 5)) // under RC control
      ;//steer(results[RC_TURN]);
    return 0x00;
  }


  if ((results[RC_AUTO] == HIGH)  && (NUMBER_CHANNELS > 5))
  {
            Serial.println("Calling processHighLevel.");
    return 0x01;  // not under RC control
  } else {
         Serial.println("Continuing processRC as under RC control.");
  }
  
  /*  6th pulse is marked throttle (position 6 on receiver; controlled by Left up/down joystick on transmitter).
    It will be used for shifting from Drive to Reverse . D40
  */
  //results[RC_RVS] = (results[RC_RVS] > MIDDLE? HIGH: LOW);

  // TO DO: Select Forward / reverse based on results[RC_RVS]

  /*   3rd pulse is elevator (position 4 on receiver; controlled by Right up/down.
     will be used for throttle/brake: RC_Throttle
  */
  int aileron = results[RC_TURN];
  //    Serial.print("\tTurn input "); Serial.print(aileron);
  results[RC_TURN] = convertTurn(aileron);

  
  // Braking or Throttle
  Serial.println(results[RC_GO]);
  if (liveBrake(results[RC_GO])){
    Serial.print("Braking: "); Serial.println(results[RC_GO]);
    brake(convertBrake(results[RC_GO]));
  }
  else {
    brake(MIN_BRAKE_OUT);
  }

  //Accelerating
  if (liveThrottle(results[RC_RDR])){
    int going = convertThrottle(results[RC_RDR]);
    moveVehicle(going);
  }
  else if(doRoutine(results[RC_RDR])){
    moveVehicle(MIN_ACC_OUT);
    squareRoutine(5, results[RC_AUTO]);
  }
  else {
    moveVehicle(MIN_ACC_OUT);
  }

  steer(results[RC_TURN]);

  /* 5th pulse is rudder (position 3 on receiver; controlled by Left left/right joystick on transmitter)
    Not used */
  //    results[RC_RDR] = (results[RC_RDR] > MIDDLE? HIGH: LOW);  // could be analog
  //    Serial.println(results[RC_RDR]);
  //    if (results[RC_RDR] >= HubAtZero)
  //        HubSpeed_kmPh = 0;
  //    else
  //        HubSpeed_kmPh = HubSpeed2kmPh / results[RC_RDR];

  //  Serial.println("");  // New line
  return 0x00;
}
/*---------------------------------------------------------------------------------------*/
void processHighLevel(elcano::SerialData * results)
{
  //results->update();
  //Steer
  int turn_signal = convertDeg(results->angle_deg);
  steer(turn_signal);
  //End Steer
  //Throttle
  Throttle_PID(10*results->speed_cmPs);
  //End Throttle
//  results->write(&Serial3);
}
/*---------------------------------------------------------------------------------------*/
//Converts RC values to corresponding values for the PWM output
int convertTurn(int input)
{
  long int steerRange, rcRange;
  long output;
  int trueOut;
  //     Serial.print("\tconvertTurn: input = \t"); Serial.print(input);
  //  Check if Input is in steer dead zone
  if ((input <= MIDDLE + DEAD_ZONE) && (input >= MIDDLE - DEAD_ZONE))
    return STRAIGHT_TURN_OUT;
  // On SPEKTRUM, MIN_RC = 1 msec = stick right; MAX_RC = 2 msec = stick left
  // On HI_TEC, MIN_RC = 1 msec = stick left; MAX_RC = 2 msec = stick right
  // LEFT_TURN_OUT > RIGHT_TURN_OUT
  else
    return input;

  // @ToDo: Fix this so it is correct in any case.
  // If a controller requires some value to be reversed, then specify that
  // requirement in Settings.h, and use the setting here.
#ifdef RC_HITEC
  input = MAX_RC - (input - MIN_RC);
#endif

  //      if (input > MIDDLE + DEAD_ZONE)
  //      {  // left turn
  //         steerRange = LEFT_TURN_OUT - STRAIGHT_TURN_OUT;
  //         rcRange = MAX_RC - (MIDDLE + DEAD_ZONE);
  //        input = input - MIDDLE - DEAD_ZONE; // originally input = middle + dead_zone
  //        output = STRAIGHT_TURN_OUT + input * steerRange / rcRange;
  //        //set max and min values if out of range
  //        trueOut = (int)output;
  //        if(trueOut > LEFT_TURN_OUT)
  //            trueOut = LEFT_TURN_OUT;
  //        if(trueOut < STRAIGHT_TURN_OUT)
  //            trueOut = STRAIGHT_TURN_OUT;
  //        return trueOut;
  //    }
  //      if (input < MIDDLE - DEAD_ZONE)
  //      {  // right turn
  //         steerRange = STRAIGHT_TURN_OUT - RIGHT_TURN_OUT;
  //         rcRange = MIDDLE - DEAD_ZONE - MIN_RC;
  //        input = input - DEAD_ZONE - MIDDLE;  // input is negative
  //        output = STRAIGHT_TURN_OUT + input * steerRange / rcRange;
  //        //set max and min values if out of range
  //        trueOut = (int)output;
  //        if(trueOut < RIGHT_TURN_OUT)
  //            trueOut = RIGHT_TURN_OUT;
  //        if(trueOut > STRAIGHT_TURN_OUT)
  //            trueOut = STRAIGHT_TURN_OUT;
  //        return trueOut;
  //    }
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
  //full throttle = 227, min = 40
  const int dacRange = MAX_ACC_OUT - MIN_ACC_OUT;
  const int rcRange = MAX_RC - (MIDDLE + DEAD_ZONE);
  input -= (MIDDLE + DEAD_ZONE);
  double output = (double)input / (double)rcRange;
  output *= dacRange;
  output += MIN_ACC_OUT;
  //set max values if out of range
  int trueOut = (int)output;
  if (trueOut > MAX_ACC_OUT)
    trueOut = MAX_ACC_OUT;
  return trueOut;
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
  brake(MAX_BRAKE_OUT);
  moveVehicle(MIN_ACC_OUT);
  delay (2000);   // inhibit output
  // TO DO: disable 36V power
}
/*---------------------------------------------------------------------------------------*/
//Send values to output pin
void steer(int pos)
{
  STEER_SERVO.writeMicroseconds(pos);
  //      Serial.print("\tSteering to: \t"); Serial.print(pos);
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
//  if(amount > (MIDDLE + MIN_RC)
//    amount = MAX_BRAKE_OUT;
//  else if (amount > (MAX_RC - DEAD_ZONE))
//    amount = MIN_BRAKE_OUT;
  return result;
}
/*---------------------------------------------------------------------------------------*/
void brake (int amount)
{
  analogWrite(BRAKE_OUT_PIN, amount);
  //     Serial.print("\tBraking to: \t"); Serial.print(*amount);
  brake_control = amount;
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
// ((WHEEL_DIAMETER_MM * 3142) / MAX_SPEED_mmPs)
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 3000
// A speed of less than 0.3 KPH is zero.
unsigned long MaxTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MIN_SPEED_mmPs)
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

static struct hist {
  long olderSpeed_mmPs;  // older data
  unsigned long olderTime_ms;   // time stamp of older speed

  long oldSpeed_mmPs;  // last data from the interrupt
  byte oldClickNumber;
  unsigned long oldTime_ms;  // time stamp of old speed

  byte nowClickNumber;  // situation when we want to display the speed
  byte InterruptState;
  unsigned long nowTime_ms;
  unsigned long TickTime_ms;  // Tick times are used to compute speeds
  unsigned long OldTick_ms;   // Tick times may not match time stamps if we don't process
  // results of every interrupt
} history;

/*---------------------------------------------------------------------------------------*/
// WheelRev is called by an interrupt.
// This is all WAY TOO LONG for an interrupt
void WheelRev()
{
  //static int flip = 0;
  unsigned long tick;
  noInterrupts();
  tick = millis();
  if (InterruptState != IRQ_RUNNING)
    // Need to process 1st two interrupts before results are meaningful.
    InterruptState++;

  if (tick - TickTime > MinTickTime_ms)
  {
    OldTick = TickTime;
    TickTime = tick;
    ++ClickNumber;
  }
//  if (flip)
//    digitalWrite(13, LOW);
//  else
//    digitalWrite(13, HIGH);
//  flip = !flip;
  interrupts();
}
/*---------------------------------------------------------------------------------------*/

void setupWheelRev()
{

  //    SerialOdoOut.begin(115200); // C6 to C4
  //    pinMode(13, OUTPUT); //led
  //    digitalWrite(13, LOW);//turn LED off
  //
  pinMode(IRPT_WHEEL, INPUT);//pulls input HIGH
  float MinTick = WHEEL_DIAMETER_MM * PI;
  //    SerialMonitor.print (" MinTick = ");
  //    SerialMonitor.println (MinTick);
  MinTick *= 1000.0;
  MinTick /= MAX_SPEED_mmPs;
  //    SerialMonitor.print (MinTick);
  MinTickTime_ms = MinTick;
  //    SerialMonitor.print (" MinTickTime_ms = ");
  //    SerialMonitor.println (MinTickTime_ms);

  //    SerialMonitor.print (" MIN_SPEED_mPh = ");
  //    SerialMonitor.print (MIN_SPEED_mPh);
  float MIN_SPEED_mmPs =  ((MIN_SPEED_mPh * 1000.0) / 3600.0);
  // MIN_SPEED_mmPs = 135 mm/s
  //    SerialMonitor.print (" MIN_SPEED_mmPs = ");
  //    SerialMonitor.print (MIN_SPEED_mmPs);
  float MaxTick = (WHEEL_DIAMETER_MM * PI * 1000.0) / MIN_SPEED_mmPs;
  //    SerialMonitor.print (" MaxTick = ");
  //    SerialMonitor.print (MaxTick);
  MaxTickTime_ms = MaxTick;
  //    SerialMonitor.print (" MaxTickTime = ");
  //    SerialMonitor.println (MaxTickTime_ms);
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
  //    SerialMonitor.print("TickTime: ");
  //    SerialMonitor.print(TickTime);
  //    SerialMonitor.print(" OldTick: ");
  //    SerialMonitor.println(OldTick);

  //    SerialMonitor.println("WheelRev setup complete");

}
/*---------------------------------------------------------------------------------------*/
void ComputeSpeed( struct hist *data)
{
  if (data->InterruptState == IRQ_NONE || data->InterruptState == IRQ_FIRST)
  { // No data
    SpeedCyclometer_mmPs = 0;
    SpeedCyclometer_revPs = 0;
    return;
  }
  float Circum_mm = (WHEEL_DIAMETER_MM * PI);
  long WheelRev_ms = data->TickTime_ms - data->OldTick_ms;
  if (data->InterruptState >= IRQ_SECOND && data->oldSpeed_mmPs == NO_DATA && WheelRev_ms > 0)
  { //  first computed speed
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  =
      data->oldSpeed_mmPs = Circum_mm * SpeedCyclometer_revPs;
    data->oldTime_ms = data->TickTime_ms;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber;
    return;
  }
  if (data->InterruptState == IRQ_RUNNING && data->olderSpeed_mmPs == NO_DATA && WheelRev_ms > 0
      && data->nowClickNumber != data->oldClickNumber)
  { //  new data for second computed speed
    data->olderSpeed_mmPs = data->oldSpeed_mmPs;
    data->olderTime_ms = data->oldTime_ms;

    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  =
      data->oldSpeed_mmPs = Circum_mm * SpeedCyclometer_revPs;
    data->oldTime_ms = data->TickTime_ms;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber;
    return;
  }
  if (data->InterruptState == IRQ_RUNNING && data->olderSpeed_mmPs != NO_DATA && WheelRev_ms > 0)
  { // Normal situation after initialization
    if (data->nowClickNumber == data->oldClickNumber)
    { // No new information; extrapolate the speed if decelerating; else keep old speed
      if (data->olderSpeed_mmPs > data->oldSpeed_mmPs)
      { // decelerrating
        float deceleration = (float) (data->olderSpeed_mmPs - data->oldSpeed_mmPs) /
                             (data->oldTime_ms - data->olderTime_ms);
        SpeedCyclometer_mmPs = data->oldSpeed_mmPs - deceleration *
                               (data->nowTime_ms - data->oldTime_ms);
        if (SpeedCyclometer_mmPs < 0)
          SpeedCyclometer_mmPs = 0;
        SpeedCyclometer_revPs = SpeedCyclometer_mmPs / Circum_mm;
      }
      else
      { // accelerating; should get new data soon

      }
      if (data->nowTime_ms - data->oldTime_ms > MaxTickTime_ms)
      { // too long without getting a tick
        SpeedCyclometer_mmPs = 0;
        SpeedCyclometer_revPs = 0;
        if (data->nowTime_ms - data->oldTime_ms > 2 * MaxTickTime_ms)
        {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
        }
      }
    }
    else
    { // moving; use new data to compute speed
      data->olderSpeed_mmPs = data->oldSpeed_mmPs;
      data->olderTime_ms = data->oldTime_ms;

      SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
      SpeedCyclometer_mmPs  =
        data->oldSpeed_mmPs = Circum_mm * SpeedCyclometer_revPs;
      data->oldTime_ms = data->TickTime_ms;  // time stamp for oldSpeed_mmPs
      data->oldClickNumber = data->nowClickNumber;
    }
  }
  if (data->nowTime_ms - data->TickTime_ms > WheelRev_ms)
  { // at this speed, should have already gotten a tick?; If so, we are slowing.
    float SpeedSlowing_revPs = 1000.0 / (data->nowTime_ms - data->TickTime_ms);
    long SpeedSlowing_mmPs  = Circum_mm * SpeedCyclometer_revPs;
    SpeedCyclometer_revPs = min(SpeedCyclometer_revPs, SpeedSlowing_revPs);
    SpeedCyclometer_mmPs  = min(SpeedCyclometer_mmPs, SpeedSlowing_mmPs);
  }
  return;
}
/*---------------------------------------------------------------------------------------*/
void PrintSpeed( struct hist *data)
{
  Serial.print(SpeedCyclometer_mmPs); Serial.print("\t");
  Serial.print(data->InterruptState); Serial.print("\t");
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
void show_speed(elcano::SerialData *Results)
{
  history.nowTime_ms = millis();
  noInterrupts();
  history.TickTime_ms = TickTime;
  history.OldTick_ms = OldTick;
  history.nowClickNumber = ClickNumber;
  history.InterruptState = InterruptState;
  interrupts();

  ComputeSpeed (&history);
  //   PrintSpeed(&history);

  Odometer_m += (float)(LOOP_TIME_US * SpeedCyclometer_mmPs) / MEG;
  // Since Results have not been cleared, angle information will also be sent.
  Results->speed_cmPs = SpeedCyclometer_mmPs / 10;
//  Results->write(&Serial3);  // Send speed to C6

  show7seg( SpeedCyclometer_mmPs);   // Show speed on 7 segment LEDs

}
/*---------------------------------------------------------------------------------------*/
/*========================CalibrateTurnAngle======================*/
/*  This routine should only be called when
          - Wheels are pointed straight ahead, and have been for a while.
          - Trike is not moving, and is stable.
    Calibration will block any response to controls; there will be no turning or movemnet during calibration.
    Angle sensors can drift, and should periodically be zeroed.
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
  //    Serial.print("\tCALIBRATE: Left Straight\t"); Serial.print(LeftStraight_A3);
  //    Serial.print("\tRight Straight\t"); Serial.println(RightStraight_A2);
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
  //    Serial.print("Left"); Serial.print("\t"); Serial.print(left);
  //    Serial.print("\tRight"); Serial.print("\t"); Serial.print(right);
  //    Serial.print("LeftStraight_A3"); Serial.print("\t"); Serial.print(LeftStraight_A3);
  //    Serial.print("\tRightStraight_A2"); Serial.print("\t"); Serial.print(RightStraight_A2);
  if ((Right_Min_Count <= right) && (right <= Right_Max_Count))
    OK_right = true;
  if ((Left_Min_Count <= left) && (left <= Left_Max_Count))
    OK_left = true;
  long right_degx1000 = (right - RightStraight_A2) * RIGHT_DEGx1000pCOUNT;
  long left_degx1000  = (left -  LeftStraight_A3) *  LEFT_DEGx1000pCOUNT;
  //    Serial.print("Left"); Serial.print("\t"); Serial.print(left_degx1000);
  //    Serial.print("\tRight"); Serial.print("\t"); Serial.print(right_degx1000);

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
  //  Serial.print("\tnew_turn_degx10"); Serial.print("\t"); Serial.println(new_turn_degx10);
  return new_turn_degx10;
}
/*---------------------------------------------------------------------------------------*/
void newThrottlePID(){
  
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
  // @ToDo: PID parameters are different per trike, and should be moved to Settings.h.
  const float P_tune = 0.4;
  const float I_tune = 0.5;
  const float D_tune = 0.1;
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
  PID_error = P_tune * error_speed_mmPs
              + I_tune * mean_speed_error
              + D_tune * extrapolated_error;
  if (PID_error > speed_tolerance_mmPs)
  { // too fast
    long throttle_decrease = (MAX_ACC_OUT - MIN_ACC_OUT) * PID_error / max_error_mmPs;
    throttle_control -= throttle_decrease;
    if (throttle_control < MIN_ACC_OUT)
      throttle_control = MIN_ACC_OUT;
    moveVehicle(throttle_control);

    long brake_increase = (MAX_BRAKE_OUT - MIN_BRAKE_OUT) * PID_error / max_error_mmPs;
    // MIN_BRAKE_OUT = 180; MAX_BRAKE_OUT = 250;
    brake_control -= brake_increase;
    if (brake_control > MAX_BRAKE_OUT)
      brake_control = MAX_BRAKE_OUT;
    brake(brake_control);
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
    // MIN_BRAKE_OUT = 180; MAX_BRAKE_OUT = 250;
    brake_control += brake_decrease;
    if (brake_control < MIN_BRAKE_OUT)
      brake_control = MIN_BRAKE_OUT;
    brake(brake_control);
  }
  // else maintain current speed
  //  Serial.print("\tThrottle Brake \t");  // csv for spreadsheet
  //  Serial.print(throttle_control);
  //  Serial.print("\t");
  //  Serial.print(brake_control);
  //  Serial.print("\t");
  //  Serial.print(drive_speed_mmPs);  Serial.print("\t");
  //  Serial.println(sensor_speed_mmPs);
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
  //  setBrightness(127);  // Medium brightness
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
  //  Serial.println(temp3);

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
