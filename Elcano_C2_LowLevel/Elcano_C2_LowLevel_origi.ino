//
////#include "Settings.h"
//#include "pin_settings.h"
//#include "trike_settings.h"
//#include "PID_v1.h"  
//#include <SPI.h>
//#include <Servo.h>
//#include <SD.h>
//#include "ElcanoSerial.h"
//#include "Brakes.h"
//
//#include "history.h"
//#include "Status.h"
//using namespace elcano;
//
///*
// * C2 is the low-level controller that sends control signals to the hub motor,
// * brake servo, and steering servo.  It is (or will be) a PID controller, but
// * may also impose limits on control values for the motor and servos as a safety
// * measure to protect against incorrect PID settings.
// *
// * It receives desired speed and heading from either of two sources, an RC
// * controller operated by a person, or the C3 pilot module.  These are mutually
// * exclusive.
// *
// * RC commands are received directly as interrupts on a bank of pins.  C3 commands
// * are received over a serial line using the Elcano Serial protocol.  Heading and
// * speed commands do not need to be passed through to other modules, but because
// * the Elcano Serial protocol uses a unidirectional ring structure, C2 may need to
// * pass through *other* commands that come from C3 but are intended for modules
// * past C2 on the ring.
// */
//
//
// /* Unused Variables
// #define ERROR_HISTORY 20 //number of errors to accumulate
// long speed_errors[ERROR_HISTORY];
//
// // This is a value that the RC controller can't produce.
// #define INVALID_DATA 0L
//
// long old_turn_degx1000;
// unsigned long stoppedTime_ms;
// unsigned long straightTime_ms;
// float Odometer_m = 0;
// float HubSpeed_kmPh;
// const unsigned long HubAtZero = 1159448;
//
// int oldSpeedValue = MIN_ACC_OUT;
//
// int oldTurnValue = WHEEL_STRAIGHT_US;
// int countz = 0;
// // Sweep communication pin
// const int SWEEP_PIN = 17;
// //Defined in Pin Settings
// // The DAC select pins are the same on all trikes.
// const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
// const int SelectAB = 53; // Select IC 2 DAC (channels A and B)
//
// */
//
//
///*
//Stores data about previous and current tick time, speed, click numbers
//should there be a previous status and a current status instead of one 
//variable holding both??
//*/
//hist history;
//
////Servo for steering
//Servo STEER_SERVO;
//
//// 100 milliseconds -- adjust to accomodate the fastest needed response or
//// sensor data capture.
//#define LOOP_TIME_MS 100
//
//// odometer
//static long distance_mm = 0;  
//
//// ex. throttle_control
//int  currentThrottlePWM = MAX_ACC_OUT; 
//// ex. steer_control
//int  currentSteeringUS = WHEEL_STRAIGHT_US; 
//
//
//void testBrakes();
//void setupWheelRev();
///*========================================================================/
//  ============================WheelRev4 code==============================/
//  =======================================================================*/
//
///* Wheel Revolution Interrupt routine
//   Ben Spencer 10/21/13
//   Modified by Tyler Folsom 3/16/14; 3/3/16
//   A cyclometer gives a click once per revolution.
//   This routine computes the speed.
//*/
//
//unsigned long MinTickTime_ms;
//// MinTickTime_ms = 89 ms
//unsigned long MaxTickTime_ms;
//// MinTickTime_ms = 9239 ms = 9 sec
//
//// Speed in revolutions per second is independent of wheel size.
//float SpeedCyclometer_revPs = 0.0;//revolutions per sec
//
//#define IRQ_NONE 0
//#define IRQ_FIRST 1
//#define IRQ_SECOND 2
//#define IRQ_RUNNING 3
//#define NO_DATA 0xffffffff
//volatile byte InterruptState = IRQ_NONE;  // Tells us if we have initialized.
//
//
//volatile unsigned long TickTime = 0;  // Time from one wheel rotation to the next gives speed.
//volatile unsigned long OldTick = 0;
///**
// * Throttle PID implementation
// */
////PID update frequency in milliseconds
//#define PID_CALCULATE_TIME 50
///* double on Arduino AVR is the same as a float. */
//double speedCyclometerInput_mmPs = 0;
//double PIDThrottleOutput_pwm; //used to tell Throttle and Brake what to do as far as acceleration
//double desiredSpeed_mmPs = 0;
//double proportional_throttle = .0175;
//double integral_throttle = .2;
//double derivative_throttle = .00001;
//double steerAngleUS = WHEEL_STRAIGHT_US; //Steering angle in microseconds used by writeMicroseconds function. Note: doubles on Arduinos are the same thing as floats, 4bytes, single precision
//double PIDSteeringOutput_us; //Output from steerPID.Compute() in microseconds (used by Servo.writeMicroseconds())
//double desiredTurn_us = WHEEL_STRAIGHT_US;
//double proportional_steering = .0175;
//double integral_steering = .5; 
//double derivative_steering = .00001;
//
//// PID setup block
//PID speedPID(&speedCyclometerInput_mmPs, &PIDThrottleOutput_pwm, &desiredSpeed_mmPs, proportional_throttle, integral_throttle, derivative_throttle, DIRECT);
//PID steerPID(&steerAngleUS, &PIDSteeringOutput_us, &desiredTurn_us, proportional_steering, integral_steering, derivative_steering, DIRECT);
//
//int calibratedWheelMaxLeft_us; // CALIBRATED_WHEEL_MAX_LEFT_US
//int calibratedWheelStraight_us; // CALIBRATED_WHEEL_STRAIGHT_US
//int calibratedWheelMaxRight_us; // CALIBRATED_WHEEL_MAX_RIGHT_US
//
//int calibratedWheelSensorMaxLeft; // CALIBRATED_WHEEL_MAX_LEFT_US
//int calibratedWheelSensorStraight;
//int calibratedWheelSensorMaxRight; // CALIBRATED_WHEEL_MAX_RIGHT_US
//
//
//ParseState TxStateHiLevel, RxStateHiLevel, RC_State;   // @@@ cant find parseState Library
//SerialData TxDataHiLevel,  RxDataHiLevel,  RC_Data;	   // @@@ cant find SerailData
///*---------------------------------------------------------------------------------------*/ 
//
//Brakes brake = Brakes();
//void setup()
//{ 
//  //Set up pins
//  STEER_SERVO.attach(STEER_OUT_PIN);
//  /*
//  // SPI: set the slaveSelectPin as an output:
//  pinMode (SelectAB, OUTPUT);
//  pinMode (SelectCD, OUTPUT);
//  SPI.setDataMode(SPI_MODE0);
//  SPI.setBitOrder(MSBFIRST);
//  SPI.begin();  
//  // initialize SPI:
//  // The following line should not be neccessary. It uses a system library.
//  PRR0 &= ~4; // turn off PRR0.PRSPI bit so power isn't off
//  
//  for (int channel = 0; channel < 4; channel++)
//  {
//    DAC_Write(channel, 0); // reset did not clear previous states
//  }
//  // ******* START: System Test and Calibration Cycle ******** \\
//  delay(100);
//  testBrakes();
//  delay(1000);
//  // ******* END: System Test and Calibration Cycle ******** \\
//  
//   Only zeroed out never used
//  for (int i = 0; i < ERROR_HISTORY; i++)
//  {
//    speed_errors[i] = 0;
//  }
//	Serial.begin(9600);
//	Serial3.begin(baudrate);
//	Serial2.begin(baudrate);
//	Serial1.begin(baudrate);
//	// Sweep
//	 //pinMode(SWEEP_PIN, INPUT);
//	
//  pinMode (10, OUTPUT);
//  */
//
//
//  // ******** Initialized but unused since PID has some issues ************
//  speedPID.SetOutputLimits(MIN_ACC_OUT, MAX_ACC_OUT); //useful if we want to change the limits on what values the output can be set to
//  speedPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
//  speedPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute
//  steerPID.SetOutputLimits(WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US); //useful if we want to change the limits on what values the output can be set to
//  steerPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
//  steerPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute
// 
//  // **********************************************************
//  
//    setupWheelRev(); // WheelRev4 addition
//    // Setting up data for sending to high level
//    
//    TxDataHiLevel.clear();
//    TxStateHiLevel.dt  = &TxDataHiLevel;
//    TxStateHiLevel.input = &Serial2;  // not used
//    TxStateHiLevel.output = &Serial3;
//    TxStateHiLevel.capture = MsgType::sensor;
//  
//    //setup for receiving data from High level
//    RxDataHiLevel.clear();
//    RxStateHiLevel.dt  = &RxDataHiLevel;
//    RxStateHiLevel.input = &Serial3;
//    RxStateHiLevel.output = &Serial2; // not used
//    RxStateHiLevel.capture = MsgType::drive;
//
//    // receive data indirectly from RC unit.
//    RC_Data.clear();
//    RC_State.dt  = &RC_Data;
//    RC_State.input = &Serial1;
//    RC_State.output = &Serial1;  // not used
//    RC_State.capture = MsgType::sensor;
//
//  speedCyclometerInput_mmPs = 0;
//
//  
//  
//}
//
//void loop()
//{  
//  // Get the next loop start time. Note this (and the millis() counter) will
//  // roll over back to zero after they exceed the 32-bit size of unsigned long,
//  // which happens after about 1.5 months of operation 
//  unsigned long timeStart_ms = millis();
//  // INSERT ANY LOOP CODE BELOW THIS POINT !!
//
//  static long int desired_speed_cmPs, desired_angle;
//  static bool e_stop = 0, auto_mode = 0;
//  brake.Check();
//
//// get data from serial
//// get desired steering and speed
//   if (auto_mode)
//   {
//   // Receiving data from High Level 
//     ParseStateError r = RxStateHiLevel.update();
//     if (r == ParseStateError::success) {
//       desired_speed_cmPs = RxDataHiLevel.speed_cmPs; 
//       desired_angle = RxDataHiLevel.angle_mDeg; 
//     } 
//   }
//    computeSpeed(&history);
//    computeAngle(); // TO DO: Convert angle to right units for PID and for sending to High Level.
//    
//    // Write data to High Level
//    TxDataHiLevel.speed_cmPs = (speedCyclometerInput_mmPs + 5) / 10;
//    TxDataHiLevel.write(TxStateHiLevel.output);
//
//    // Get data from RC unit
//    ParseStateError r = RC_State.update();
//    if (r == ParseStateError::success) {
//      e_stop = RC_Data.number & 0x01;
//      auto_mode = RC_Data.number & 0x02;
//      if (!auto_mode)
//      {
//        desired_speed_cmPs = RC_Data.speed_cmPs; 
//        desired_angle = RC_Data.angle_mDeg; 
//      }
//    } 
//    if (e_stop)
//    {
//      brake.Stop();
//      engageWheel(0); // Turn off wheel
//    }
//    else
//    { // Control trike to desired speed and angle 
//      SteeringPID(convertHLToTurn(desired_angle));
//      ThrottlePID(desired_speed_cmPs);
//    }
//    
//  // DO NOT INSERT ANY LOOP CODE BELOW THIS POINT !!
//
//   unsigned long delay_ms = millis() - (timeStart_ms + LOOP_TIME_MS);
//  // Did we spend long enough in the loop that we should immediately start
//  // the next pass?
//  if(delay_ms > 0L)
//  {
//    // No, pause til the next loop start time.
//    delay(delay_ms);
//  }
//}
//
///*-------------------------------------testBrakes-------------------------------------------*/
///*
// * Purely for testing during setup if brake actuator is responding. Delay is to allow enough
// * time for actuator to move
//*/
//void testBrakes()
//{
//  brake.Stop();
//  delay(1000);
//  brake.Release();
//  serial.write("**** Brake test Complete! ****");
//}
//
///*-------------------------------------testWheel-------------------------------------------*/
///*
// * Purely for testing during setup if wheel motor is responding. Delay is to allow enough
// * time for motor to run at low speed
//*/
//void testWheel()
//{
//  engageWheel(MIN_ACC_OUT); // Min speed (~85 PWM)
//  delay(2000);
//  engageWheel(0); // Turn off wheel
//  serial.write("**** Wheel test Complete! ****");
//}
//
///*-------------------------------------testSteering-------------------------------------------*/
///*
// * Purely for testing during setup if wheels actuator is responding. Delay is to allow enough
// * time for actuator to move
//*/
//void testSteering()
//{
//  engageSteering(WHEEL_MAX_LEFT_US);
//  delay(3000);
//  engageSteering(WHEEL_MAX_RIGHT_US);
//  delay(6000);
//  engageSteering(WHEEL_STRAIGHT_US);
//  delay(1000);
//  serial.write("**** Steering test Complete! ****");
//}
///*-------------------------------------engageWheel-------------------------------------------*/
///*
// * Used as a more friendly way to engage the back wheel by passing the PWM value to DAC_Write
// * (TCF June 1, 2018) None of this is really PWM. It is a digital value from 0 to 255 that is 
// * converted to a true analog voltage.
// * Input: PWM value corresponding to desired speed (0-255 : min-max)
// * Output: None
//*/
//void engageWheel(int inputPWM)
//{
//    /* !UPDATE THIS OBSERVED INFO! (LAST UPDATE: May 10, 2013, TCF)
//    0.831 V at rest 52 counts
//    1.20 V: nothing 75
//    1.27 V: just starting 79
//    1.40 V: slow, steady 87
//    1.50 V: brisker 94
//    3.63 V: max 227 counts
//    255 counts = 4.08 V
//  */
//  if(inputPWM != currentThrottlePWM)
//  {
//	spi.write(inputPWM);
//	currentThrottlePWM = inputPWM;  // Remember most recent throttle PWM value.
//	serial.write("Wheel engaged = " + currentThrottlePWM);
//  }
//}
//
///*-------------------------------------engageSteering-------------------------------------------*/
///*
// * Engages the steering system by extending or retracting the actuator. Input value is in
// * microseconds (~1000-2000 : min-max), where the min value positions the brake into standby
// * (or near disk grab) position
// * Input: Int for microseconds to move the actuator
// * Output: None
//*/
//void engageSteering(int inputMicroseconds)
//{
//  // 1 sensor tick = 1.7 us of servo
//  if(inputMicroseconds != currentSteeringUS)
//  {
//    STEER_SERVO.writeMicroseconds(inputMicroseconds);
////    Results.angle_mDeg = inputMicroseconds; // Need to do some kind of backwards conversion to C6
//    currentSteeringUS = inputMicroseconds;
//	serial.write("Steering engaged = " + inputMicroseconds);
//  }
//}
//
///*-------------------------------------DAC_Write-------------------------------------------*/
///* DAC_Write applies value to address, producing an analog voltage.
//  // address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
//  // value: digital value converted to analog voltage
//  // Output goes to mcp 4802 Digital-Analog Converter Chip via SPI
//  // There is no input back from the chip.
//
//void DAC_Write(int address, int value)
///*
//  REGISTER 5-3: WRITE COMMAND REGISTER FOR MCP4802 (8-BIT DAC)
//  A/B — GA SHDN D7 D6 D5 D4 D3 D2 D1 D0 x x x x
//  bit 15 bit 0
//  bit 15 A/B: DACA or DACB Selection bit
//  1 = Write to DACB
//  0 = Write to DACA
//  bit 14 — Don’t Care
//  bit 13 GA: Output Gain Selection bit
//  1 = 1x (VOUT = VREF * D/4096)
//  0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
//  bit 12 SHDN: Output Shutdown Control bit
//  1 = Active mode operation. VOUT is available.
//  0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
//  VOUT pin is connected to 500 k (typical)
//  bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.
//  With 4.95 V on Vcc, observed output for 255 is 4.08V.
//  This is as documented; with gain of 2, maximum output is 2 * Vref
//
//{
//  int byte1 = ((value & 0xF0) >> 4) | 0x10; // active mode, bits D7-D4
//  int byte2 = (value & 0x0F) << 4; // D3-D0
//  if (address < 2)
//  {
//    // take the SS pin low to select the chip:
//    digitalWrite(SelectAB, LOW);
//    if (address >= 0)
//    {
//      if (address == 1)
//      {
//        byte1 |= 0x80; // second channnel
//      }
//      SPI.transfer(byte1);
//      SPI.transfer(byte2);
//    }
//    // take the SS pin high to de-select the chip:
//    digitalWrite(SelectAB, HIGH);
//  }
//  else
//  {
//    // take the SS pin low to select the chip:
//    digitalWrite(SelectCD, LOW);
//    if(address <= 3)
//    {
//      if(address == 3)
//      {
//        byte1 |= 0x80; // second channnel
//      }
//      SPI.transfer(byte1);
//      SPI.transfer(byte2);
//    }
//    // take the SS pin high to de-select the chip:
//    digitalWrite(SelectCD, HIGH);
//  }
//}*/
//
///*----------------------------setupWheelRev----------------------------------------------*/
//void setupWheelRev(){
//  MinTickTime_ms = (WHEEL_CIRCUM_MM * 1000.0) / MAX_SPEED_mmPs;
//  MaxTickTime_ms = (WHEEL_CIRCUM_MM * 1000.0) / MIN_SPEED_mmPs;
//  TickTime = millis();
//  // OldTick will normally be less than TickTime.
//  // When it is greater, TickTime - OldTick is a large positive number,
//  // indicating that we have not moved.
//  // TickTime would overflow after days of continuous operation, causing a glitch of
//  // a display of zero speed.  It is unlikely that we have enough battery power to ever see this.
//  OldTick = TickTime;
//  InterruptState = IRQ_NONE;
//  history.oldSpeed_mmPs = history.olderSpeed_mmPs = NO_DATA;
//  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
//}
//
///*----------------------------WheelRev---------------------------------------------------*/
//// WheelRev is called by an interrupt.
//void WheelRev()
//{
//  //static int flip = 0;
//  unsigned long tick;
//  noInterrupts();
//  tick = millis();
//  if(InterruptState != IRQ_RUNNING)
//  {
//    // Need to process 1st two interrupts before results are meaningful.
//    InterruptState++;
//  }
//  
//  if((tick - TickTime) > MinTickTime_ms)
//  {
//    OldTick = TickTime;
//    TickTime = tick;
//  }
//  interrupts();
//}
//
///*----------------------------computeSpeed-----------------------------------------------*/
//void computeSpeed(struct hist *data)
//{
//  unsigned long WheelRev_ms = TickTime - OldTick;
//  float SpeedCyclometer_revPs = 0.0; //revolutions per sec
//  if((InterruptState == IRQ_NONE) || (InterruptState == IRQ_FIRST))
//  { // No data
//    speedCyclometerInput_mmPs = 0;
//    SpeedCyclometer_revPs = 0;
//  }
//  else if(InterruptState == IRQ_SECOND)
//  { //  first computed speed
//    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
//    speedCyclometerInput_mmPs = data->oldSpeed_mmPs = data->olderSpeed_mmPs = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
//    data->oldTime_ms = OldTick;
//    data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
//  }
//  else if(InterruptState == IRQ_RUNNING)
//  { //  new data for second and following computed speeds
//    if(TickTime == data->nowTime_ms)
//    {//no new data
//      //check to see if stopped first
//      unsigned long timeStamp = millis();
//      if((timeStamp - data->nowTime_ms) > MaxTickTime_ms)
//      { // too long without getting a tick
//        speedCyclometerInput_mmPs = 0;
//        SpeedCyclometer_revPs = 0;
//        if((timeStamp - data->nowTime_ms) > (2 * MaxTickTime_ms))
//        {
//          InterruptState = IRQ_FIRST;  //  Invalidate old data
//          data->oldSpeed_mmPs = NO_DATA;
//          data->olderSpeed_mmPs = NO_DATA;
//        }
//        return;
//      }
//      if(data->oldSpeed_mmPs > speedCyclometerInput_mmPs)
//      { // decelerrating, extrapolate new speed using a linear model
////       Serial.println("data->oldSpeed_mmPs > speedCyclometerInput_mmPs");
//       float deceleration = ((float)(data->oldSpeed_mmPs - speedCyclometerInput_mmPs) / (float)(timeStamp - data->nowTime_ms));
//       speedCyclometerInput_mmPs = (data->oldSpeed_mmPs - deceleration * (timeStamp - data->nowTime_ms));
//       if(speedCyclometerInput_mmPs < 0)
//       {
//         speedCyclometerInput_mmPs = 0;
//       }
//       SpeedCyclometer_revPs = (speedCyclometerInput_mmPs / WHEEL_CIRCUM_MM);
//      }
//    }
//    else // data is different from last
//    {
//      //update time block
//      data->olderTime_ms = data->oldTime_ms;
//      data->oldTime_ms = data->nowTime_ms;
//      data->nowTime_ms = TickTime;
//  
//      //update speed block
//      data->olderSpeed_mmPs = data->oldSpeed_mmPs;
//      data->oldSpeed_mmPs = speedCyclometerInput_mmPs;
//      SpeedCyclometer_revPs = (1000.0 / WheelRev_ms);
//      speedCyclometerInput_mmPs  = (WHEEL_CIRCUM_MM * SpeedCyclometer_revPs);
//  
//      data->oldTickMillis = data->tickMillis;
//      data->tickMillis = millis();
//      
//      data->currentSpeed_kmPh = speedCyclometerInput_mmPs/260.0;
//      distance_mm += ((data->oldTime_ms - data->olderTime_ms)/1000.0) * (data->oldSpeed_mmPs);
//  
//      if(data->TickTime_ms-data->OldTick_ms > 1000)
//      {
//        data->currentSpeed_kmPh = 0;
//      }
//    }
//  }
//}
///*************************** START HIGH LEVEL PROCESSING SECTION ********************************/
//
///*------------------------------------convertHLToTurn------------------------------------------*/
//
//int convertHLToTurn(int turnValue)
//{
//  // TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
//  return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US);
//  //return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight);
//}
//
//int convertHLToSpeed(int speedValue)
//{
//  // TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
// // return map(speedValue, 0, MAX_SPEED_CMS, MIN_ACC_OUT, MAX_ACC_OUT);
//}
//
///*---------------------------SteeringPID--------------------------------------*/
//// Computer Steering PID
//// Precondition: None
//// Postcondition: None
//void SteeringPID(int desiredValue)
//{
//  desiredTurn_us = desiredValue;
//  // Input into PID is microseconds and output is US
//  steerPID.Compute();
//  serial.write(" PID STEER OUT = ", PIDSteeringOutput_us);
//  engageSteering((int)PIDSteeringOutput_us);
//}
///*------------------------------------ThrottlePID--------------------------------*/
//// Compute PID 
//// Precondition: None
//// Postcondition: None
//void ThrottlePID(int desiredValue)
//{
//  desiredSpeed_mmPs = desiredValue;
//  serial.write("SPEED MMPS + ", speedCyclometerInput_mmPs);
//  serial.write(" DESIRED SPEED = ", desiredSpeed_mmPs);
//  // Input into PID is PWM and output is PWM
//  if(desiredSpeed_mmPs < (speedCyclometerInput_mmPs + 10))
//  {
//     brake.Stop();
//  }
//  else
//  {
//    speedPID.Compute();
//	serial.write(" PID THROTTLE OUT = ", PIDThrottleOutput_pwm);
//    currentThrottlePWM = (int)PIDThrottleOutput_pwm;
//    brake.Release();
//    engageWheel(currentThrottlePWM);
//  }
//}
//
//void computeAngle()
//{
//  int left = analogRead(A2);
//  int right = analogRead(A3);
//
//   steerAngleUS = map(analogRead(A3), calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight, calibratedWheelMaxLeft_us, calibratedWheelMaxRight_us);
// }