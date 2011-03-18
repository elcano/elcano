/*
Elcano Contol Module C1: Motor and brake control.
This also incorporates control module C2: Steering.
*/

#include "Serial.c"

namespace C1_Motor {
    void setup();
    void loop();
}
/*


[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C3 Pilot
  Serial input is a Gamebot command. Use of this syntax provides compatibility with the simulator.

  DRIVE {Speed ComandedSpinSpeed} {FrontSteer ComandedSteerAngle} 

  Where: 
  {Speed ComandedSpinSpeed} 	ComandedSpinSpeed is a ‘float’ giving the spin speed for the rear wheel. 
  The value is the absolute spin speed, in radians per second. 
  {FrontSteer ComandedSteerAngle} 	ComandedSteerAngle is a ‘float’ that specifies the steer angle 
  of Elcano’s front wheels. The value is the absolute steer angle, in radians. 
  
  Other commands:
  PIDmotor {Kp x} {Ki y} {Kd z}
  PIDsteer {Kp x} {Ki y} {Kd z}
*/

/* [in] Digital Signal 2: J1 pin 3 to Atmega8 pin 4 PD2 (INT0): Input from cyclometer. 
  The signal comes from a reed switch that opens on each revolution of the wheel. 
  The signal is used to compute ActualSpinSpeed (radians/sec). */
const int Tachometer = 2;
 
/* [in] Digital Signal 3: J1 pin 4  (INT1) Halt or reset signal from console. 
*/
const int HaltCmd = 3;

/* [out] Digital Signal 4: J1 pin 5  (D4) Traction motor forward / reverse. */
const int Reverse = 4;

/* [out] Digital Signal 5: J1 pin 6  (D5) Actuator A1. Traction motor throttle. */
const int Throttle = 11;// 5;

/*  Digital Signal 6: J1 pin 7  (D6) Tune PID. 
TO DO: Specify how the PID parameters are tuned.
*/
const int TunePID = 6;

/* [out] Digital Signal 7: J1 pin 8  (D7) 
   LOW: Select traction motor; HIGH: Select steering  
   when TunePID is LOW, this is output; TunePID HIGH makes this an input
*/
const int SelectPID = 7;

const int Ack = 7;  // OBSOLETE: Acknowledge to C3 of reset request.

/* [out] Digital Signal 8: J3 pin 1 (D8) Enable Recombinant braking. 
  Alternatively, this could be hard-wired ON */
const int EnableRecombinant = 8;

/* [out] Digital Signal 9: J3 pin 2 (D9) PWM. Actuator A2: Brake Motor. 
  Controls disk brakes on left and right front wheels. Brakes are mechanically linked to operate in tandem. */
const int DiskBrake = 11; //9;

/* [out] Digital Signal 10: J3 pin 3 (D10) PWM. Recombinant braking. */
const int Recombinant = 10;

/* [out] Digital Signal 11: J3 pin 4 (D11) PWM. Actuator A3: Steering Motor. 
  Turns left or right. If no signal, wheels are locked to a straight ahead position. */
const int Steer = 5; // 11;

/* [out] Digital Signal 12: J3 pin 5 (D12) Enable Drive Motor. 
  Alternatively, this could be hard-wired ON */
const int EnableThrottle = 12;


/* [in] Analog input 0: J2 pin 1 (ADC0) ActualSteerAngle. */
const int SteerFeedback = 0;

/* [in] Analog input 1: J2 pin 2 (ADC1) MotorTemperature. */
const int Temperature = 1;

/* [in] Analog input 2: J2 pin 3 (ADC2) BrakeFeedback. */
const int BrakeFeedback = 2;

/* UNASSIGNED: Analog input 3,4,5 */

/* time (micro seconds) for a wheel revolution */
volatile unsigned long WheelRevMicros = 0;

unsigned long TimeOfCmd_ms = 0;

const int Full = 255;   // fully on for PWM signals
const int Off = 0;
const int FullThrottle =  179;   // 3.5 V
const int MinimumThrottle = 39;  // Throttle has no effect until 0.75 V
const int HardLeft = 0;
const int Straight = 128;
const int HardRight = 255;

#define MICROSEC_PER_REV2_DEGREE_PER_SEC (1000000 * 360)
#define RADIAN2DEGREE 0.0174533
/* The motion flags are logical values.  
   They are not the same as the values written to reverse the motor. */
#define MOTION_STOPPED 0
#define MOTION_FORWARD 1
#define MOTION_REVERSE 2
/* If it has been this many micro seconds since a tick, the wheel is stopped.
   This is a crude method for determining if the wheel has stopped.
   A 10 sec delay between ticks means that a 0.7 m diameter wheel has moved at most 13.6 cm.
   A 0.43 m wheel has moved at most 8.6 cm. */
#define WHEEL_STOPPED 10000000
#define MIN_SAMPLE_MS 100

// return values
#define OK       0
#define BUFFER_FULL 1

#define NUMBER_BUFFER 12
struct floatText
{
   boolean positive;
   int digits;
   int decimal; 
   byte buffer[NUMBER_BUFFER];  
} numberIn;

struct PID_Parameters
{
    float Kp;
    float Ki;
    float Kd;
    long  desired;
    long  actual;
    long  time_ms;
    long  previous_error;
    long  integral;
};
struct PID_Parameters motor;
struct PID_Parameters steer;

#define STATE_INITIAL 0
#define STATE_D       1
#define STATE_DR      2
#define STATE_DRI     3
#define STATE_DRIV    4
#define STATE_DRIVE   5
#define STATE_BRACKET 6
#define STATE_S       7
#define STATE_Sp      8
#define STATE_Spe     9
#define STATE_Spee    10
#define STATE_Speed   11
#define STATE_F       12
#define STATE_Fr      13
#define STATE_Fro     14
#define STATE_Fron    15
#define STATE_Front   16
#define STATE_FrontS  17
#define STATE_FrontSt 18
#define STATE_FrontSte 19
#define STATE_FrontStee 20
#define STATE_FrontSteer 21
#define STATE_CLOSE      22
#define STATE_SPIN       23
#define STATE_STEER      24
#define STATE_LOST       25
int State = STATE_INITIAL;

 /*---------------------------------------------------------------------------------------*/ 
void setup() 
{ 
        pinMode(RxD, INPUT);
        pinMode(TxD, OUTPUT);
        pinMode(Tachometer, INPUT);
        pinMode(HaltCmd, INPUT);
        pinMode(Reverse, OUTPUT);
        pinMode(Throttle, OUTPUT);
        pinMode(SelectPID, OUTPUT);
        pinMode(EnableRecombinant, OUTPUT);
        pinMode(DiskBrake, OUTPUT);
        pinMode(Recombinant, OUTPUT);
        pinMode(Steer, OUTPUT);
        pinMode(EnableThrottle, OUTPUT);
        pinMode(LED, OUTPUT); 
	attachInterrupt (0, WheelRev, RISING);
	attachInterrupt (1, Halt, RISING);

        initialize();
        
       	Serial.begin(9600); 
}	

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
        numberIn.positive = true;
        numberIn.digits = 0;
        numberIn.decimal = 0;
        State = STATE_INITIAL;
        Stop();
        
        motor.previous_error = 0;
        motor.integral = 0;
        steer.previous_error = 0;
        steer.integral = 0;
        motor.Kp = 0;
        digitalWrite( SelectPID, LOW);
        digitalWrite( EnableRecombinant, HIGH);
        digitalWrite( Reverse, LOW);
        digitalWrite( EnableThrottle, HIGH);
        analogWrite( Steer, Straight);
        digitalWrite( LED, LOW);
}
/*---------------------------------------------------------------------------------------*/ 
// WheelRev is called by an interrupt.
void WheelRev()
{
	static unsigned long OldTick;
	static unsigned long TickTime;
	OldTick = TickTime;
	TickTime = micros();
	if (OldTick <= TickTime)
		WheelRevMicros = TickTime - OldTick;
	else // overflow
		WheelRevMicros = TickTime + ~OldTick;
/* TO DO: 
  Replace cyclometer with inputs of Hall sensors in wheel.
*/
}
/*---------------------------------------------------------------------------------------
 Halt is called by an interrupt
 A console reset signal does not immediately reset the C1 motor controller.
 After receiving a halt or reset, the motor controller brings the vehicle to a stop.
 A console reset is immediatly effective on the C3 Pilot.
 A few seconds after C3 is initialized, it sends a reset to C1.
 */
void Halt()
{
    Stop();
    delay (4000);
}
/*---------------------------------------------------------------------------------------*/ 
// Bring the vehicle to a stop.
// Stop may be called from an interrupt or from the loop.
void Stop()
{
   analogWrite(Throttle, Off);
   analogWrite(Recombinant, Full); 
   analogWrite(DiskBrake, Full);
}
#define TEST_MODE
/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
  static float CommandedSpinSpeed = 0;   // radians / second
  static float CommandedSteerAngle = 0;  // radians
  static float CurrentSteerAngle = 0;    // radians
  unsigned long TimeSinceCmd_ms = 0;     // radians
  const unsigned long MaxSilence = 2000;  // ms
  int i;
  unsigned int RevolutionsPerSecond;
  
#ifdef TEST_MODE
  write_all (LOW);
  delay (1000);
  for (i = MinimumThrottle; i <= MinimumThrottle+5; i++)
  {
    /* As of 3/15/11 the ramp test spins Elcano's wheel.
       However, we expect to see it on for 20 sec and off for 1 sec.
       Instead, it is only on for 2 sec.
       A manual throttle is also not able to keep the wheel on for an
       extended time.
       
       3/16/11: Elcano is drawing 20 to 25 Amp from a 36V battery.
       It is giving 900W to a wheel capable of 500W.
       Probable cause of motor kicking off is over-current protection
       kicking in.
       Scaling back the amount of throttle helps. 
       There is little load on the test stand.
       TO DO: Monitor speed and control throttle for desired speed.
    */
      ramp(i);
  }
  write_all (HIGH);
 // delay (1000);
  for (i = MinimumThrottle+5; i >= MinimumThrottle; i--)
  {
      ramp(i);
  }
  /* Display speed from cyclometer
  RevolutionsPerSecond = (unsigned int) (WheelRevMicros / 1000000 + 0.5F);
  FlashMorse(RevolutionsPerSecond);
*/
  
#else  // not TEST_MODE
 // Read Gamebot command on RxD and put in CommandedSpinSpeed and CommandedSteerAngle
 // When a DRIVE {Speed x} {FrontSteer y} command is read,
 // TimeOfCmd is set to 0 and the other arguments read.
   GetSerial( &TimeOfCmd_ms, &CommandedSpinSpeed, &CommandedSteerAngle);
 
  // TO DO: Use differences in commanded and actual spin speed to control Drive, DiskBrake and Recombinant.
   ControlSpin(CommandedSpinSpeed);
 
 // TO DO: Read CurrentSteerAngle from SteerFeedback.
 // TO DO: Use differences in commanded and actual steer angle to control Steer.

 // If no new Gamebot command received in 2 sec, stop.
  TimeSinceCmd_ms = millis() - TimeOfCmd_ms;
  if (TimeSinceCmd_ms > MaxSilence)
 {  // Watchdog time out.
   Stop();
 } 
#endif  // TEST_MODE 
} 
#ifdef TEST_MODE
/*---------------------------------------------------------------------------------------*/ 
void write_all( int state)
{
      digitalWrite( SelectPID, state);
      digitalWrite( EnableRecombinant, state);
      digitalWrite( Reverse, state);
      digitalWrite( EnableThrottle, state);
      digitalWrite( LED, state);
}
/*---------------------------------------------------------------------------------------*/ 
void ramp (int state)
{
   analogWrite( Steer, state);
   analogWrite(Throttle, state);
   analogWrite(Recombinant, state); 
   analogWrite(DiskBrake, state);
   delay(500); 

}
/*---------------------------------------------------------------------------------------
Flash a number in Morse Code
1  . ----
2  ..---
3  ...--
4  ....-
5  .....
6  -....
7  --...
8  ---..
9  ----.
0  -----
*/ 
#define BASE 300
#define DOT BASE
#define DASH (4*BASE)
#define BIT_SPACE (2*BASE)
#define NUMBER_SPACE (10*BASE)
void FlashMorse (unsigned int number)
{
  unsigned int quotient, remainder;
  unsigned int i;
  quotient = number / 10;
  remainder = number % 10;
  if (quotient != 0)
    FlashMorse (quotient);
  if (remainder <= 5)
  {
    for (i = 0; i < remainder; i++)
      Flash(DOT);
    for (; i < 5; i++)
      Flash(DASH);
  }
  else
  {
    for (i = 5; i < remainder; i++)
      Flash(DASH);
    for (; i < 10; i++)
      Flash(DOT);
   }
   digitalWrite( LED, LOW);
   delay (NUMBER_SPACE);

}
void Flash(int DashDot)
{
  digitalWrite( LED, LOW);
  delay (BIT_SPACE);
  digitalWrite( LED, HIGH);
  delay (DashDot);
}
#endif  // TEST_MODE 
/*---------------------------------------------------------------------------------------*/ 
// Adjust the throttle to hit the desired wheel speed.
void ControlSpin (float SpinSpeed_RadianPSecond)
{
/*  CurrentSpinSpeed = 2 * PI / (WheelRevMicros * 1,000,000)  (radians)
    Degrees / sec lets us use integers over the desired range.
    Wheel diam (m)     min speed @ 1 deg/s    high speed @ 57,300 deg/sec
    0.05 (2 in)        0.001 kph              44 kph
    0.70 (27 in)       0.012 kph             600 kph  

    We do not have sensors that tell whether the robot is moving forward or backward.
    Assume that it is moving in the desired direction.
    When the desired direction changes, the robot must stop before we reverse the motor.    
*/
    static int motion = MOTION_STOPPED;
    static unsigned long LastSample_ms = 0;
    unsigned long TimeNow_ms;
    long DesiredSpinSpeed;
    
    TimeNow_ms = millis();
    if (TimeNow_ms - LastSample_ms < MIN_SAMPLE_MS &&
         TimeNow_ms < LastSample_ms) 
         return;
         
    DesiredSpinSpeed = SpinSpeed_RadianPSecond * RADIAN2DEGREE;     
    if (DesiredSpinSpeed > 0 && motion == MOTION_REVERSE ||
        DesiredSpinSpeed < 0 && motion == MOTION_FORWARD)
     {
          Stop();
          // TO DO: Extrapolate speeds for a better time to stop.
          if (WheelRevMicros < WHEEL_STOPPED)
            return;  // wait for robot to stop
          motion = MOTION_STOPPED;
     }
     if (DesiredSpinSpeed > 0)
     {
           digitalWrite( Reverse, LOW);
           motion = MOTION_FORWARD;
     }
     if (DesiredSpinSpeed < 0)
     {
           digitalWrite( Reverse, HIGH);
           motion = MOTION_REVERSE;
     }
     /* TO DO: Retain a history of times, accelerations, actual speeds, commanded speeds and distances.
           Write a PID controller to move the actual speed to the commanded speed. */
     motor.desired = DesiredSpinSpeed;
     motor.actual = MICROSEC_PER_REV2_DEGREE_PER_SEC / WheelRevMicros;
     motor.time_ms = TimeNow_ms - LastSample_ms;
     // TO DO: Output a new throttle value
}
 /*---------------------------------------------------------------------------------------*/ 

// Serial communications routines

 /*---------------------------------------------------------------------------------------*/ 
// Input is a character code, which is stored in the numberIn structure.
// Returned value OK or  BUFFER_FULL
int getNumber(byte digit)
{
  int result = OK;
  if (numberIn.digits == 0 && digit == '-')
    numberIn.positive = false;
  else if (digit >= '0' && digit <= '9')
  {
    numberIn.buffer[numberIn.digits++] = digit;
    if (numberIn.digits >= NUMBER_BUFFER)
      result = BUFFER_FULL; 
  }
  else if (digit == '.')
    numberIn.decimal = numberIn.digits;
  return result;
}
/*---------------------------------------------------------------------------------------*/ 
// Convert the numberIn structure to a float and return it.
float convert()
{
  float result;
  long numerator = 0;
  long divisor = 1;
  int i;
  for (i = 0; i < numberIn.digits; i++)
  {
    numerator *= 10;
    numerator += numberIn.buffer[i];
  }
  for (i = 0; i < numberIn.decimal; i++)
    divisor *= 10;
  if (!numberIn.positive)
    numerator = -numerator; 
   result = float(numerator) / float(divisor); 
  return result;
}
/*---------------------------------------------------------------------------------------*/ 
/* There is only one command that is received over the serial line. It is
DRIVE {Speed x} {FrontSteer y}
where x and y are floats. 
Characters are ASCII. Wide characters are not used.
*/
void GetSerial( unsigned long *TimeOfCmd, float *CommandedSpinSPeed, float *CommandedSteerAngle)
{

  static float nextSpin = 0;
  static float nextSteer = 0;
  byte nextByte;
  
  while (Serial.available() > 0)
  {
    nextByte = byte(Serial.read());
    switch( State)
    {
    case STATE_INITIAL: 
       State = (nextByte == 'D' || nextByte == 'd')? STATE_D: STATE_INITIAL;
       break;  
    case STATE_D: 
       State = (nextByte == 'R' || nextByte == 'r')? STATE_DR: STATE_INITIAL;
       break; 
    case STATE_DR: 
       State = (nextByte == 'I' || nextByte == 'i')? STATE_DRI: STATE_INITIAL;
       break; 
    case STATE_DRI: 
       State = (nextByte == 'V' || nextByte == 'v')? STATE_DRIV: STATE_INITIAL;
       break; 
    case STATE_DRIV: 
       State = (nextByte == 'E' || nextByte == 'e')? STATE_DRIVE: STATE_INITIAL;
       break; 
    case STATE_DRIVE: 
       if (nextByte == ' ' || nextByte == '\t')
         break;
       State = (nextByte == '{')? STATE_BRACKET: STATE_INITIAL;
       break; 
    case STATE_BRACKET: 
       if (nextByte == 'S' || nextByte == 's') State = STATE_S;
       else if (nextByte == 'F' || nextByte == 'f') State = STATE_F;
      break; 
    case STATE_S: 
       State = (nextByte == 'P' || nextByte == 'p')? STATE_Sp: STATE_LOST;
       break; 
    case STATE_Sp: 
       State = (nextByte == 'E' || nextByte == 'e')? STATE_Spe: STATE_LOST;
       break; 
    case STATE_Spe: 
       State = (nextByte == 'E' || nextByte == 'e')? STATE_Spee: STATE_LOST;
       break; 
    case STATE_Spee: 
       State = (nextByte == 'D' || nextByte == 'd')? STATE_Speed: STATE_LOST;
       break; 
    case STATE_Speed: 
       if (nextByte == ' ' || nextByte == '\t')
         break;
       else if (nextByte == '}')
       {  // command is acted on without waiting for {FrontSteer}
         State = STATE_CLOSE;
         nextSpin = convert();
         *TimeOfCmd = 0;      
       }
       else
       {    
        if (getNumber( nextByte) == BUFFER_FULL)
        {  
          State = STATE_LOST;
          nextSpin = convert();
          *TimeOfCmd = 0;      
        }
       }
       break; 
    case STATE_F: 
       State = (nextByte == 'R' || nextByte == 'r')? STATE_Fr: STATE_LOST;
       break; 
    case STATE_Fr: 
       State = (nextByte == 'O' || nextByte == 'o')? STATE_Fro: STATE_LOST;
       break; 
    case STATE_Fro: 
       State = (nextByte == 'N' || nextByte == 'n')? STATE_Fron: STATE_LOST;
       break; 
    case STATE_Fron: 
       State = (nextByte == 'T' || nextByte == 't')? STATE_Front: STATE_LOST;
       break; 
    case STATE_Front: 
       State = (nextByte == 'S' || nextByte == 's')? STATE_FrontS: STATE_LOST;
       break; 
    case STATE_FrontS: 
       State = (nextByte == 'T' || nextByte == 't')? STATE_FrontSt: STATE_LOST;
       break; 
    case STATE_FrontSt: 
       State = (nextByte == 'E' || nextByte == 'e')? STATE_FrontSte: STATE_LOST;
       break; 
    case STATE_FrontSte: 
       State = (nextByte == 'E' || nextByte == 'e')? STATE_FrontStee: STATE_LOST;
       break; 
    case STATE_FrontStee: 
       State = (nextByte == 'R' || nextByte == 'r')? STATE_FrontSteer: STATE_LOST;
       break; 
    case STATE_FrontSteer: 
       if (nextByte == ' ' || nextByte == '\t')
         break;
       if (nextByte == '}')
       {  // command is complete.  Speed and FrontSteer can be in either order.
         State = STATE_CLOSE;
         nextSteer = convert();
         *TimeOfCmd = 0;      
       }
       else
       {    
        if (getNumber( nextByte) == BUFFER_FULL)
        {  
          State = STATE_LOST;
          nextSteer = convert();
          *TimeOfCmd = 0;      
        }
       }
       break; 
    case STATE_CLOSE:
       if (nextByte == ' ' || nextByte == '\t')
         break;
       if (nextByte == '{') State = STATE_BRACKET;
       else if (nextByte == 'D' || nextByte == 'd') State = STATE_D;
       else if (nextByte == 'S' || nextByte == 's') State = STATE_S;
       else if (nextByte == 'F' || nextByte == 'f') State = STATE_F;
       else State = STATE_LOST;
       break; 
     case STATE_LOST:
       if      (nextByte == 'D' || nextByte == 'd') State = STATE_D;
       else if (nextByte == 'S' || nextByte == 's') State = STATE_S;
       else if (nextByte == 'F' || nextByte == 'f') State = STATE_F;
       break; 
     }
  }
}

