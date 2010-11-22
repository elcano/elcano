/*
Elcano Contol Module C1: Motor and brake control.
This also incorporates control module C2: Steering.


[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C3 Pilot
  Serial input is a Gamebot command. Use of this syntax provides compatibility with the simulator.

  DRIVE {Speed ComandedSpinSpeed} {FrontSteer ComandedSteerAngle} 

  Where: 
  {Speed ComandedSpinSpeed} 	ComandedSpinSpeed is a ‘float’ giving the spin speed for the rear wheel. The value is the absolute spin speed, in radians per second. 
  {FrontSteer ComandedSteerAngle} 	ComandedSteerAngle is a ‘float’ that specifies the steer angle of Elcano’s front wheels. The value is the absolute steer angle, in radians. 
*/
const int RxD = 0;

/* [out] Digital Signal 1: J1 pin 2 (TxD). Reserved.  */
const int TxD = 1;

/* [in] Digital Signal 2: J1 pin 3 to Atmega8 pin 4 PD2 (INT0): Input from cyclometer. 
  The signal comes from a reed switch that opens on each revolution of the wheel. 
  The signal is used to compute ActualSpinSpeed (radians/sec). */
const int Tachometer = 2;

/* [in] Digital Signal 3: J1 pin 4  (INT1) Reset request from C3. */
const int Reset = 3;

/* [out] Digital Signal 4: J1 pin 5  (D4) Traction motor forward / reverse. */
const int Reverse = 4;

/* [out] Digital Signal 5: J1 pin 6  (D5) Actuator A1. Traction motor throttle. */
const int Throttle = 5;

/*  Digital Signal 6: J1 pin 7  (D6) Spare. */

/* [out] Digital Signal 7: J1 pin 8  (D7) Acknowledge to C3 of reset request. */
const int Ack = 7;

/* [out] Digital Signal 8: J3 pin 1 (D8) Enable Recombinant braking. */
const int EnableRecombinant = 8;

/* [out] Digital Signal 9: J3 pin 2 (D9) PWM. Actuator A2: Brake Motor. 
  Controls disk brakes on left and right front wheels. Brakes are mechanically linked to operate in tandem. */
const int DiskBrake = 9;

/* [out] Digital Signal 10: J3 pin 3 (D10) PWM. Recombinant braking. */
const int Recombinant = 10;

/* [out] Digital Signal 11: J3 pin 4 (D11) PWM. Actuator A3: Steering Motor. 
  Turns left or right. If no signal, wheels are locked to a straight ahead position. */
const int Steer = 11;

/* [out] Digital Signal 12: J3 pin 5 (D12) Enable Drive Motor. 
  Controls motor on rear wheel.  Three wires for throttle. */
const int EnableThrottle = 12;

/* [out] Digital Signal 13: J3 pin 5 (SCK)  LED on Arduino board. Used for testing. */
const int LED = 13;   

/* [in] Analog input 0: J2 pin 1 (ADC0) ActualSteerAngle. */
const int SteerFeedback = 0;

/* [in] Analog input 1: J2 pin 2 (ADC1) MotorTemperature. */
const int Temperature = 1;

/* UNASSIGNED: Analog input 2,3,4,5 */

/* time (micro seconds) for a wheel revolution */
volatile unsigned long WheelRevMicros = 0;

unsigned long TimeOfCmd_ms = 0;

const int Full = 255;   // fully on for PWM signals
const int HardLeft = 0;
const int Straight = 128;
const int HardRight = 255;

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

void setup() 
{ 
        pinMode(RxD, INPUT);
        pinMode(TxD, OUTPUT);
        pinMode(Tachometer, INPUT);
        pinMode(Reset, INPUT);
        pinMode(Reverse, OUTPUT);
        pinMode(Throttle, OUTPUT);
        pinMode(Ack, OUTPUT);
        pinMode(EnableRecombinant, OUTPUT);
        pinMode(DiskBrake, OUTPUT);
        pinMode(Recombinant, OUTPUT);
        pinMode(Steer, OUTPUT);
        pinMode(EnableThrottle, OUTPUT);
        pinMode(LED, OUTPUT); 
	attachInterrupt (0, WheelRev, RISING);

        numberIn.positive = true;
        numberIn.digits = 0;
        numberIn.decimal = 0;
	Serial.begin(9600); 

        digitalWrite( Ack, LOW);
        digitalWrite( EnableRecombinant, HIGH);
        analogWrite( Recombinant, Full); 
        analogWrite( DiskBrake, Full);
        digitalWrite( Reverse, LOW);
        digitalWrite( EnableThrottle, HIGH);
        analogWrite( Throttle, 0);
        analogWrite( Steer, Straight);
        digitalWrite( LED, LOW);
}	
void loop() 
{
  static float CommandedSpinSpeed = 0;
  static float CommandedSteerAngle = 0;
  static float CurrentSpinSpeed = 0;
  static float CurrentSteerAngle = 0;
  unsigned long TimeSinceCmd_ms = 0;
  const unsigned long MaxSilence = 2000;  // ms
  
 // Read Gamebot command on RxD and put in CommandedSpinSpeed and CommandedSteerAngle
 // When a DRIVE {Speed x} {FrontSteer y} command is read,
 // TimeOfCmd is set to 0 and the other arguments read.
 GetSerial( &TimeOfCmd_ms, &CommandedSpinSpeed, &CommandedSteerAngle);
 
 // TO DO: Use WheelRevMicros to compute CurrentSpinSpeed.
 // TO DO: Use differences in commanded and actual spin speed to control Drive, DiskBrake and Recombinant.
 // TO DO: Read CurrentSteerAngle from SteerFeedback.
 // TO DO: Use differences in commanded and actual steer angle to control Steer.

 // If no new Gamebot command received in 2 sec, stop.
  TimeSinceCmd_ms = millis() - TimeOfCmd_ms;
  if (TimeSinceCmd_ms > MaxSilence)
 {  // Watchdog time out.
   Stop();
 } 
} 
void Stop()
{
   analogWrite(Throttle, 0);
   analogWrite(Recombinant, Full); 
   analogWrite(DiskBrake, Full);
 
}

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
/* There is only one command that is sent over the serial line. It is
DRIVE {Speed x} {FrontSteer y}
where x and y are floats. 
Characters are ASCII. Wide characters are not used.
*/
void GetSerial( unsigned long *TimeOfCmd, float *CommandedSpinSPeed, float *CommandedSteerAngle)
{
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
  static int State = STATE_INITIAL;
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
}


