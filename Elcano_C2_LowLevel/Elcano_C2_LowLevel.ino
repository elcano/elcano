
#include <SPI.h>
#include <Elcano_Serial.h>
#define LOOP_TIME_MS 200
#define ERROR_HISTORY 20

// New section    TCF 9/17/15
// Indices for information stored in array RC_results
#define VEHICLE_NUMBER 2
// Channel order differs for differernt vehicles
#if (VEHICLE_NUMBER == 1)
#define RC_TURN 1
#define RC_AUTO 2
#define RC_GO   3
#define RC_ESTP 4
#define RC_RDR  5
#define RC_RVS  6

#define NUMBER_CHANNELS 5

//OUTPUT values -  0 to 255
#define MIN_ACC_OUT 40
#define MAX_ACC_OUT 227
#define MIN_BRAKE_OUT 180
#define MAX_BRAKE_OUT 250
#define RIGHT_TURN_OUT 146
#define LEFT_TURN_OUT 230
#define STRAIGHT_TURN_OUT 180
#define DAC_CHANNEL 0
#define STEER_OUT_PIN 7 // Output to steer actuator
#define BRAKE_OUT_PIN 6  // output to brake actuator

/*================ReadTurnAngle 1================*/
// Value measured at analog input A3 from right steering column when wheels pointed straight ahead.
// An analog voltage can be 0 to 5V, which correspond to angles from 0 to 360 degrees.
// Analog input reads this as a value form 0 to 1024.
// We may want to mount the sensors so that straight is close to 500.
// This number is not critical, since actual wheel turn range is much less than 360
#define RIGHT_STRAIGHT_A3 181
// Value measured at analog input A2 from left steering column when wheels pointed straight ahead.
#define LEFT_STRAIGHT_A2  181
// Calibration constants
// Aangle (degrees) = (Value - RIGHT_STRAIGHT_A3) * RIGHT_DEGx1000pCOUNT / 1000
#define RIGHT_DEGx1000pCOUNT  3333
#define LEFT_DEGx1000pCOUNT  3333
// A correct sensor will produce a reading between these values
#define RIGHT_MIN_COUNT  80
#define RIGHT_MAX_COUNT 284
#define LEFT_MIN_COUNT   80
#define LEFT_MAX_COUNT  284

#endif

#if (VEHICLE_NUMBER == 2)

#define RC_AUTO 1
#define RC_ESTP 2
#define RC_RDR  3
#define RC_GO   4
#define RC_TURN 5
#define NUMBER_CHANNELS 5
// Controller has no channel for RC_AUTO
#define RC_RVS  6


//OUTPUT values -  0 to 255
#define MIN_ACC_OUT 40
#define MAX_ACC_OUT 227
#define MIN_BRAKE_OUT 180
#define MAX_BRAKE_OUT 250
#define RIGHT_TURN_OUT 146 // Original value 146
#define LEFT_TURN_OUT 223 // Original value 230
#define STRAIGHT_TURN_OUT 182 // Original value 187

#define DAC_CHANNEL 3
#define STEER_OUT_PIN 7 // Output to steer actuator
#define BRAKE_OUT_PIN 9  // output to brake actuator

/*================ReadTurnAngle 2================*/
#define RIGHT_STRAIGHT_A3 515 // Original value 181
#define LEFT_STRAIGHT_A2  522 // Original value 181
#define RIGHT_DEGx1000pCOUNT  3333
#define LEFT_DEGx1000pCOUNT  3333
#define RIGHT_MIN_COUNT 466 // Original value 80
#define RIGHT_MAX_COUNT 557 // Original value 284
#define LEFT_MIN_COUNT 465  // Original value 80
#define LEFT_MAX_COUNT 559  // Original value 284

#endif


// End new section
// TO DO: Throttle and brake values should be in a header file and depend on vehicle number.
// Values to send over DAC
const int FullThrottle =  227;   // 3.63 V
const int MinimumThrottle = 70;  // Throttle has no effect until 1.2 V
// Values to send on PWM to get response of actuators
const int FullBrake = 167;  // start with a conservative value; could go as high as 255;  
const int NoBrake = 207; // start with a conservative value; could go as low as 127;

//RC input values - pulse widths in microseconds
const int LONG_DEADZONE_THRESHOLD = 10000; // used for syncing to determine if the last down time was the long down zone
const int DEAD_ZONE = 75;
const int MIDDLE = 1500;  // was 1322; new stable value = 1510
// extremes of RC pulse width
const int MIN_RC = 1090;  // was 911;
const int MAX_RC = 1930; // was 1730;
const int CYCLE_MAX = 25000;  // Pulses come at 42.27 Hz, every 21155 us
const int MAX_PULSE = 4000; // Next pulse should be no longer than this time from last.


//ARDUINO PIN SELECTORS
const int RC_STEER_PIN = 44; // Input from RC // was 2;
const int RC_CRUISE_PIN = 42;
const int RC_THROTTLE_PIN = 46; // Input // was 3;
const int RC_ESTOP_PIN = 38;
const int RC_RUDDER_PIN = 40;
const int RC_REVERSE_PIN = 36;
const int RC_INTERRUPT_PIN = 20;  
const int RC_INTERRUPT = 3;
const int PIN_ORDER[8] = {0, RC_STEER_PIN, RC_CRUISE_PIN, RC_THROTTLE_PIN, RC_ESTOP_PIN, RC_RUDDER_PIN, RC_REVERSE_PIN, 0};
const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)

const unsigned long INVALID_DATA = 0;
const int RC_WAIT = 0;  // rc_index is 0, waiting for data
const int RC_PROCESSING = 1;  // rc_index is 1 to 6
const int RC_DONE  = 2;   // rc_index is 7
volatile int rc_index = 0;
volatile  unsigned long RC_results[7];
volatile int rc_state = RC_WAIT;
volatile boolean synced = false;
volatile unsigned long last_fallingedge_time = 4294967295; // max long
volatile int RC_Done[7] = {0,0,0,0,0,0,0};

const int Signals[6] = {1, 2, 3, 4, 5, 0}; // {4, 3, 2, 0, 0, 0};
/* Arduino Mega:
  D2 = Int 0  Chan 6 [closest to ind/Data]
  D3 = Int 1  Wheel Click
  D21 = Int 2 Green wire Chan 2
  D20 = Int 3 Yellow wire Chan 3
  D19 = Int 4 Orange wire Chan 4
  D18 = Int 5 Red wire Chan 5
  Blue wire Chan 1 No Connect
*/
// globals
long sensor_speed_mmPs = 0;
long drive_speed_mmPs = 0;
char IncomingMessage[BUFFER_SIZE];
int  InIndex=0;
int  throttle_control = MinimumThrottle;
int  brake_control = FullBrake;
// int  steer_control = Straight;
long speed_errors[ERROR_HISTORY];
/*================Read Turn Angle=================*/
long old_turn_degx1000, older_turn_degx1000;
long expected_turn_degx1000;

void ProcessRiseOfINT(int intIndex) {
  RC_results[intIndex] = micros();
}

void ISR1_rise() {
  noInterrupts();
  ProcessRiseOfINT(1);
  attachInterrupt(Signals[0], ISR1_fall, FALLING);
  interrupts();
}

void ISR2_rise() {
  noInterrupts();
  ProcessRiseOfINT(2);
  attachInterrupt(Signals[1], ISR2_fall, FALLING);
  interrupts();
}

void ISR3_rise() {
  noInterrupts();
  ProcessRiseOfINT(3);
  attachInterrupt(Signals[2], ISR3_fall, FALLING);
  interrupts();
}

void ISR4_rise() {
  noInterrupts();
  ProcessRiseOfINT(4);
  attachInterrupt(Signals[3], ISR4_fall, FALLING);
  interrupts();
}

void ISR5_rise() {noInterrupts();
  ProcessRiseOfINT(5);
  attachInterrupt(Signals[4], ISR5_fall, FALLING);
  interrupts();
}

void ISR6_rise() {
  noInterrupts();
  ProcessRiseOfINT(6);
  attachInterrupt(Signals[5], ISR6_fall, FALLING);
  interrupts();
}

void ProcessFallOfINT(int intIndex) {
  RC_results[intIndex] = micros() - RC_results[intIndex];
}

void ISR1_fall() {  
  noInterrupts();
  ProcessFallOfINT(1);
  RC_Done[1] = 1;
  attachInterrupt(Signals[0], ISR1_rise, RISING);
  interrupts();
}

void ISR2_fall() {
  noInterrupts();
  ProcessFallOfINT(2);
  RC_Done[2] = 1;
  attachInterrupt(Signals[1], ISR2_rise, RISING);
  interrupts();
}

void ISR3_fall() {
  noInterrupts();
  ProcessFallOfINT(3);
  RC_Done[3] = 1;
  attachInterrupt(Signals[2], ISR3_rise, RISING);
  interrupts();
}
void ISR4_fall() {  
  noInterrupts();
  ProcessFallOfINT(4);
  RC_Done[4] = 1;
  attachInterrupt(Signals[3], ISR4_rise, RISING);
  interrupts();
}

void ISR5_fall() {
  noInterrupts();
  ProcessFallOfINT(5);
  RC_Done[5] = 1;
  attachInterrupt(Signals[4], ISR5_rise, RISING);
  interrupts();
}

void ISR6_fall() {
  noInterrupts();
  ProcessFallOfINT(6);
  RC_Done[6] = 1;
  attachInterrupt(Signals[5], ISR6_rise, RISING);
  interrupts();
}
void initialize()
{
  for (int i = 0; i < ERROR_HISTORY; i++)
  {
      speed_errors[i] = 0;
  }
 
}
void setup()
{ //Set up pins
      pinMode(BRAKE_OUT_PIN, OUTPUT);
      pinMode(STEER_OUT_PIN, OUTPUT);
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
      for (int channel = 0; channel < 4; channel++)
      DAC_Write (channel, 0); // reset did not clear previous states
      Serial.begin(9600);
      rc_index = 0;
      for (int i = 0; i < 8; i++)
          RC_results[i] = INVALID_DATA;
      rc_state = RC_WAIT;
      initialize();

      setupWheelRev(); // WheelRev4 addition

    attachInterrupt(Signals[0], ISR1_rise, RISING);
    attachInterrupt(Signals[1], ISR2_rise, RISING);
    attachInterrupt(Signals[2], ISR3_rise, RISING);
    attachInterrupt(Signals[3], ISR4_rise, RISING);
    attachInterrupt(Signals[4], ISR5_rise, RISING);
    attachInterrupt(Signals[5], ISR6_rise, RISING);

    old_turn_degx1000 = older_turn_degx1000 = expected_turn_degx1000 = 0; // ReadTurnAngle addition
}
void loop() {
    SerialData Results;
    unsigned long Start_us = micros();
    unsigned long time, endTime;
    time = millis();
    endTime = time + LOOP_TIME_MS ;
    
    startCapturingRCState();
    
    unsigned long local_results[7];
    while (!(RC_Done[2] == 1&& RC_Done[3] == 1&& RC_Done[4] == 1&& RC_Done[5] == 1&& RC_Done[6] == 1))
    ;
    // got data;    
    rc_state = RC_WAIT;  // ignore pulses on data line.
    for (int i = 0; i < 8; i++)
        local_results[i] = RC_results[i];
    unsigned long Elapsed_us = micros() - Start_us;
    Serial.print(Start_us);      Serial.print("\t");
    Serial.print(Elapsed_us);    Serial.print("\t");
    Serial.print(local_results[0]);  Serial.print("\t");
    Serial.print(local_results[1]); Serial.print("\t");
    Serial.print(local_results[2]); Serial.print("\t");
    Serial.print(local_results[3]); Serial.print("\t");
    Serial.print(local_results[4]); Serial.print("\t");
    Serial.print(local_results[5]); Serial.print("\t");
    Serial.println(local_results[6]);
    
    processRC(local_results);
    Results.Clear();
    Results.kind = MSG_SENSOR;
    Results.angle_deg = TurnAngle_degx10() / 10;
    show_speed (&Results);
    while (time < endTime)
    {
        time = millis();
    }

}

void startCapturingRCState()
{
  for (int i = 1; i < 7; i++) {
    RC_Done[i] = 0;
  }
}

// processRC modified by TCF  9/17/15
void processRC (unsigned long *RC_results)
{   
    // 1st pulse is aileron (position 5 on receiver; controlled by Right left/right joystick on transmitter)
    //     used for Steering
    int aileron = RC_results[RC_TURN];
//    RC_results[RC_TURN] = liveTurn(aileron)? convertTurn(aileron): STRAIGHT_TURN_OUT;
    RC_results[RC_TURN] = convertTurn(aileron);
    
    /* 2nd pulse is aux (position 1 on receiver; controlled by flap/gyro toggle on transmitter) 
       will be used for selecting remote control or autonomous control. */
    if (NUMBER_CHANNELS > 5) 
        RC_results[RC_AUTO] = (RC_results[RC_AUTO] > MIDDLE? HIGH: LOW);

    /* 4th pulse is gear (position 2 on receiver; controlled by gear/mode toggle on transmitter) 
    will be used for emergency stop. D38 */
    RC_results[RC_ESTP] = (RC_results[RC_ESTP] > MIDDLE? HIGH: LOW);
    
    if (RC_results[RC_ESTP] == HIGH)
    {
        E_Stop();
        if (RC_results[RC_AUTO] == HIGH  && NUMBER_CHANNELS > 5) // under RC control
            steer(RC_results[RC_TURN]); 
        return;
    }
    if ( RC_results[RC_AUTO] == LOW  && NUMBER_CHANNELS > 5)
        return;  // not under RC control

    /*  6th pulse is marked throttle (position 6 on receiver; controlled by Left up/down joystick on transmitter). 
    It will be used for shifting from Drive to Reverse . D40
    */
    RC_results[RC_RVS] = (RC_results[RC_RVS] > MIDDLE? HIGH: LOW);
        
// TO DO: Select Forward / reverse based on RC_results[RC_RVS]

       
    /*   3rd pulse is elevator (position 4 on receiver; controlled by Right up/down.  
       will be used for throttle/brake: RC_Throttle
    */
    // Braking or Throttle
    if (liveBrake(RC_results[RC_GO])) 
        brake(convertBrake(RC_results[RC_GO]));
    else
        brake(MIN_BRAKE_OUT);
    //Accelerating
    if(liveThrottle(RC_results[RC_GO]))
        moveVehicle(convertThrottle(RC_results[RC_GO]));
    else
        moveVehicle(MIN_ACC_OUT);
        
    steer(RC_results[RC_TURN]); 
    
    /* 5th pulse is rudder (position 3 on receiver; controlled by Left left/right joystick on transmitter) 
    Not used */
    RC_results[RC_RDR] = (RC_results[RC_RDR] > MIDDLE? HIGH: LOW);  // could be analog

}
//Converts RC values to corresponding values for the PWM output
int convertTurn(int input)
{
     long int steerRange, rcRange;
     long output;
     int trueOut;
//  Check if Input is in steer dead zone
      if (input <= MIDDLE + DEAD_ZONE && input >= MIDDLE - DEAD_ZONE)
       return STRAIGHT_TURN_OUT;
       // MIN_RC = 1 msec = right; MAX_RC = 2 msec = left
      // LEFT_TURN_OUT > RIGHT_TURN_OUT
      
      if (input > MIDDLE + DEAD_ZONE)
      {  // left turn
         steerRange = LEFT_TURN_OUT - STRAIGHT_TURN_OUT;
         rcRange = MAX_RC - (MIDDLE + DEAD_ZONE);
        input = input - MIDDLE - DEAD_ZONE; // originally input = middle + dead_zone
        output = STRAIGHT_TURN_OUT + input * steerRange / rcRange;
        //set max and min values if out of range
        trueOut = (int)output;
        if(trueOut > LEFT_TURN_OUT)
            trueOut = LEFT_TURN_OUT;
        if(trueOut < STRAIGHT_TURN_OUT)
            trueOut = STRAIGHT_TURN_OUT;
        return trueOut;
    }
      if (input < MIDDLE - DEAD_ZONE)
      {  // right turn
         steerRange = STRAIGHT_TURN_OUT - RIGHT_TURN_OUT;
         rcRange = MIDDLE - DEAD_ZONE - MIN_RC;
        input = input - DEAD_ZONE - MIDDLE;  // input is negative
        output = STRAIGHT_TURN_OUT + input * steerRange / rcRange;
        //set max and min values if out of range
        trueOut = (int)output;
        if(trueOut < RIGHT_TURN_OUT)
            trueOut = RIGHT_TURN_OUT;
        if(trueOut > STRAIGHT_TURN_OUT)
            trueOut = STRAIGHT_TURN_OUT;
        return trueOut;
    }
}
int convertBrake(int input)
{
      if (input < (MIDDLE + MIN_RC)/2)
           return MAX_BRAKE_OUT;
      else
           return MIN_BRAKE_OUT;     
}
int convertThrottle(int input)
{
      //full throttle = 227, min = 40
      const int dacRange = MAX_ACC_OUT - MIN_ACC_OUT;
      const int rcRange = MAX_RC - (MIDDLE + DEAD_ZONE);
      input -= (MIDDLE + DEAD_ZONE);
      double output = (double)input /(double)rcRange;
      output *= dacRange;
      output += MIN_ACC_OUT;
      //set max values if out of range
      int trueOut = (int)output;
      if(trueOut > MAX_ACC_OUT)
          trueOut = MAX_ACC_OUT;
      return trueOut;
}
//Tests for inputs
// Input not in throttle dead zone
boolean liveThrottle(int acc)
{
      return (acc > MIDDLE + DEAD_ZONE);
}
// Input is not in brake dead zone
boolean liveBrake(int brake)
{
      return (brake < MIDDLE - DEAD_ZONE);
}
// Emergency stop
void E_Stop()
{
    brake(MAX_BRAKE_OUT);
    moveVehicle(MIN_ACC_OUT);
    // TO DO: disable 36V power
}
//Send values to output pin
void steer(int pos)
{
      analogWrite(STEER_OUT_PIN, pos);
}
void brake(int amount)
{
      analogWrite(BRAKE_OUT_PIN, amount);
      Serial.println(amount);
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
      int byte1 = ((value & 0xF0)>>4) | 0x10; // acitve mode, bits D7-D4
      int byte2 = (value & 0x0F)<<4; // D3-D0
      if (address < 2)
      {
          // take the SS pin low to select the chip:
          digitalWrite(SelectAB,LOW);
          if (address >= 0)
          {
              if (address == 1)
                  byte1 |= 0x80; // second channnel
              SPI.transfer(byte1);
              SPI.transfer(byte2);
          }
          // take the SS pin high to de-select the chip:
          digitalWrite(SelectAB,HIGH);
      }
      else
      {
          // take the SS pin low to select the chip:
          digitalWrite(SelectCD,LOW);
          if (address <= 3)
          {
              if (address == 3)
                  byte1 |= 0x80; // second channnel
              SPI.transfer(byte1);
              SPI.transfer(byte2);
          }
          // take the SS pin high to de-select the chip:
          digitalWrite(SelectCD,HIGH);
      }
}
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
}

/*========================================================================/
 *============================WheelRev4 code==============================/
 *=======================================================================*/
 
/* Wheel Revolution Interrupt routine
   Ben Spencer 10/21/13
   Modified by Tyler Folsom 3/16/14
   
   A cyclometer gives a click once per revolution. 
   This routine computes the speed.
*/
// CLICK_IN defined: use interrupt; not defined: simulate with timer
#define CLICK_IN 1
#define LOOP_TIME_MS 1000
#define CLICK_TIME_MS 1000
#define SerialOdoOut  Serial3
#define SerialMonitor Serial

#define WHEEL_DIAMETER_MM 397
#define MEG 1000000
#define MAX_SPEED_KPH 50
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
unsigned long MinTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MAX_SPEED_mmPs)
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 500
// A speed of less than 0.5 KPH is zero.
unsigned long MaxTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MIN_SPEED_mmPs)
// MinTickTime_ms = 9239 ms = 9 sec

float Odometer_m = 0;
long SpeedCyclometer_mmPs = 0;
// Speed in revolutions per second is independent of wheel size.
float SpeedCyclometer_revPs = 0.0;//revolutions per sec
volatile unsigned long TickTime = 0;
long WheelRev_ms = 0;
unsigned long OldTick = 0;
#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_RUNNING 2
volatile int InterruptState = IRQ_NONE;
unsigned long ShowTime_ms;

/*---------------------------------------------------------------------------------------*/ 
// WheelRev is called by an interrupt.
void WheelRev()
{
    static int flip = 0;
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
    }
    if (flip)
        digitalWrite(13, LOW);
    else
        digitalWrite(13, HIGH);
    flip =!flip;  
    
    interrupts();
}
/*---------------------------------------------------------------------------------------*/ 

void setupWheelRev() 
{ 

    SerialOdoOut.begin(115200); // C6 to C4        
    pinMode(13, OUTPUT); //led
    digitalWrite(13, LOW);//turn LED off
    
    pinMode(2, INPUT);//pulls input HIGH
    float MinTick = WHEEL_DIAMETER_MM * PI;
//    SerialMonitor.print (" MinTick = ");
//    SerialMonitor.println (MinTick);
    MinTick *= 1000.0;
    MinTick /= MAX_SPEED_mmPs;
//    SerialMonitor.print (MinTick);
    MinTickTime_ms = MinTick;
    SerialMonitor.print (" MinTickTime_ms = ");
    SerialMonitor.println (MinTickTime_ms);

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
    ShowTime_ms = TickTime;
    InterruptState = IRQ_NONE;

    attachInterrupt (0, WheelRev, RISING);//pin 2 on Mega
    SerialMonitor.print("TickTime: ");
    SerialMonitor.print(TickTime);
    SerialMonitor.print(" OldTick: ");
    SerialMonitor.println(OldTick);
     
    SerialMonitor.println("setup complete");

}
/*---------------------------------------------------------------------------------------*/ 

void show_speed(SerialData *Results)
{

   ShowTime_ms = millis();       
   if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)  // no OR 1 interrupts
   {
       SpeedCyclometer_mmPs = 0;
       SpeedCyclometer_revPs = 0;
   } 

  //check if velocity has gone to zero
  else
  {
    if(ShowTime_ms - TickTime > MaxTickTime_ms)
    {  // stopped
/*        SerialMonitor.print("Stop. Showtime: ");
        SerialMonitor.print(ShowTime_ms);
        SerialMonitor.print(" Tick: ");
        SerialMonitor.println(TickTime); */
        SpeedCyclometer_mmPs = 0;
        SpeedCyclometer_revPs = 0;
    }
    else
    {  // moving
        WheelRev_ms = max(TickTime - OldTick, ShowTime_ms - TickTime);
        if (InterruptState == IRQ_RUNNING)
        {  // have new data
      
            float Circum_mm = (WHEEL_DIAMETER_MM * PI);
            if (WheelRev_ms > 0)
            {
                SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
                SpeedCyclometer_mmPs  = Circum_mm * SpeedCyclometer_revPs;
            }
            else
            {
                SpeedCyclometer_mmPs = 0;
                SpeedCyclometer_revPs = 0;
            }
        }

      }
    }
    Odometer_m += (float)(LOOP_TIME_MS * SpeedCyclometer_mmPs) / MEG;
    Results->speed_cmPs = SpeedCyclometer_mmPs / 10;
    writeSerial(&Serial3, Results);

    // Show on monitor
/*    SerialMonitor.print("\nWheelRev (ms): ");
    SerialMonitor.print(WheelRev_ms);
    SerialMonitor.print(" SENSOR ");
    SerialMonitor.print("{Speed ");
    SerialMonitor.print(SpeedCyclometer_revPs);
    SerialMonitor.println("}\0");
 */
}

/*======================ReadTurnAngle======================*/
int TurnAngle_degx10()
{
    long new_turn_degx1000;
    int new_turn_degx10;
    long min_ang, max_ang;
    bool OK_right = false;
    bool OK_left = false;
    int right = analogRead(A3);
    int left = analogRead(A2);
    Serial.print("Left"); Serial.print("\t"); Serial.println(left);
    Serial.print("Right"); Serial.print("\t"); Serial.println(right);
    if (RIGHT_MIN_COUNT <= right && right <= RIGHT_MAX_COUNT)
       OK_right = true;    
    if (LEFT_MIN_COUNT <= left && left <= LEFT_MAX_COUNT)
       OK_left = true;    
    long right_degx1000 = (right - RIGHT_STRAIGHT_A3) * RIGHT_DEGx1000pCOUNT;  
    long left_degx1000  = (left -  LEFT_STRAIGHT_A2) *  LEFT_DEGx1000pCOUNT;  
 
    expected_turn_degx1000 = 2 * old_turn_degx1000 - (older_turn_degx1000);
    if (OK_left && OK_right)
    {  // use the median
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
        else  // TO DO; limit this value to keep it bounded when both sensors fail
            new_turn_degx1000 = expected_turn_degx1000;
    }
    else if (OK_left)
    {
        new_turn_degx1000 = left_degx1000;
    }
    else if (OK_right)
    {
        new_turn_degx1000 = right_degx1000;
    }
    else
    {
        new_turn_degx1000 = expected_turn_degx1000;
    }
    new_turn_degx10 = (int) (new_turn_degx1000 / 100);
    older_turn_degx1000 = old_turn_degx1000;
    old_turn_degx1000 = new_turn_degx1000;
    return new_turn_degx10;
}
/*---------------------------------------------------------------------------------------*/
void Throttle_PID(long error_speed_mmPs)

/* Use throttle and brakes to keep vehicle at a desired speed.
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
  static long speed_errors[ERROR_HISTORY];
  static int error_index = 0;
  int i;
  static long error_sum = 0;
  long mean_speed_error = 0;
  long extrapolated_error = 0;
  long PID_error;
  const float P_tune = 0.4;
  const float I_tune = 0.5;
  const float D_tune = 0.1;
  const long speed_tolerance_mmPs = 75;  // about 0.2 mph
  // setting the max_error affacts control: anything bigger gets maximum response
  const long max_error_mmPs = 2500; // about 5.6 mph

  error_sum -= speed_errors[error_index];
  speed_errors[error_index] = error_speed_mmPs;
  error_sum += error_speed_mmPs;
  mean_speed_error = error_sum / ERROR_HISTORY;
  i = (error_index-1) % ERROR_HISTORY;
  extrapolated_error = 2 * error_speed_mmPs - speed_errors[i];
  
  PID_error = P_tune * error_speed_mmPs 
            + I_tune * mean_speed_error
            + D_tune * extrapolated_error;
  if (PID_error > speed_tolerance_mmPs)
  {  // too fast
    long throttle_decrease = (FullThrottle - MinimumThrottle) * PID_error / max_error_mmPs;
    throttle_control -= throttle_decrease;
    if (throttle_control < MinimumThrottle)
        throttle_control = MinimumThrottle;
    moveVehicle(throttle_control);
    
    long brake_increase = (NoBrake - FullBrake) * PID_error / max_error_mmPs;
    // NoBrake = 207; FullBrake = 167; 
    brake_control -= brake_increase;
    if (brake_control < FullBrake)
        brake_control = FullBrake;
    brake(brake_control);
  }
  else if (PID_error < speed_tolerance_mmPs)
  {  // too slow
    long throttle_increase = (FullThrottle - MinimumThrottle) * PID_error / max_error_mmPs;
    throttle_control += throttle_increase;
    if (throttle_control > FullThrottle)
        throttle_control = FullThrottle;
    moveVehicle(throttle_control);
    
    // release brakes
    long brake_decrease = (NoBrake - FullBrake) * PID_error / max_error_mmPs;
    // NoBrake = 207; FullBrake = 167; 
    brake_control += brake_decrease;
    if (brake_control > NoBrake)
        brake_control = NoBrake;
    brake(brake_control);
  }
  // else maintain current speed
  //Serial.print("Thottle Brake,");  // csv for spreadsheet
  Serial.print(throttle_control);
  Serial.print(",");
  Serial.print(brake_control);
  Serial.print(",");
  Serial.print(drive_speed_mmPs);  Serial.print(",");
  Serial.println(sensor_speed_mmPs); 
}

/*---------------------------------------------------------------------------------------*/

