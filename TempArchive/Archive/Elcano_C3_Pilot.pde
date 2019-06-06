/*
Elcano Contol Module C3: Pilot.

The Pilot program reads a serial line that specifies the desired path and speed 
of the vehicle. It computes the wheel spin speeds and steering angles needed to
follow the planned path. It then sends a DRIVE command to the C1 module over a 
serial line. The format of the DRIVE command is documented in the C1 code.

[in] Digital Signal 0: J1 pin 1 (RxD) Serial signal from C6 Navigator
*/
#ifdef SIMULATOR
#include "..\\Elcano\\Elcano\\Elcano_simulator.h"
#else
#include "Serial.cpp"
#endif

namespace C3_Pilot {
    void setup();
    void loop();
}
using namespace C3_Pilot;

const int Ack = 2;  // OBSOLETE: Acknowledge from C1 of reset.

/* The C3 module has several inputs from the operator console.
   These have not been defined.  They can be set to try various tests.
*/
const int SwitchBit_0 = 3; 
const int SwitchBit_1 = 5; 
const int SwitchBit_2 = 6;

/* There are three operator commands to stop:
   1) HALT. The C1 computer brings the vehicle to a stop, ready to resume present
   operation when commanded.
   HALT is an interrupt to the C1 computer and a status line to the C3 computer.
   HALT not active means GO.
   2) RESET. Clears and restarts all microcontrollers except C1.  When C3 is reset,
   it sends a HALT signal to C1 and waits for the vehicle to stop. After
   enough time has elapsed, C3 sends a RESET signal to C1.
   3) ESTOP. If the operator has pushed the emergency stop, there is no power to the 
   traction motor, brakes or steering. Computers have a separate power supply.
   An E-stop resets all microcontrollers except C1. 
*/
/*  EStop high means that C3 has been reset and C1 has no control of actuators. */
const int E_Stop = 4;  // input from operator console.
const int Halt = 7;   // input from operator console.
const int Reset_C1 = 8;  // output to C1.


unsigned long TimeOfCmd_ms = 0;

const int Full = 255;   // fully on for PWM signals
const int Off = 0;

#define MICROSEC_PER_REV2_DEGREE_PER_SEC (1000000 * 360)
#define RADIAN2DEGREE 0.0174533

// return values
#define OK       0
#define BUFFER_FULL 1

#define NUMBER_BUFFER 12
struct floatText
{
   bool positive;
   int digits;
   int decimal; 
   byte buffer[NUMBER_BUFFER];  
} numberIn;

#ifdef SIMULATOR
namespace C3_Pilot {
#endif

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
        numberIn.positive = true;
        numberIn.digits = 0;
        numberIn.decimal = 0;
        
        digitalWrite( Reset_C1, LOW);
        digitalWrite( LED, LOW);
        
        /* TO DO:
          Wait until the vehicle has stopped.
           Then send a RESET signal to C1.
           Wait until EStop is not active.
        */
}
/*---------------------------------------------------------------------------------------*/ 
void setup() 
{ 
        pinMode(RxD, INPUT);
        pinMode(TxD, OUTPUT);
        pinMode(SwitchBit_0, INPUT);
        pinMode(SwitchBit_1, INPUT);
        pinMode(SwitchBit_2, INPUT);
        pinMode(Halt, INPUT);
        pinMode(E_Stop, INPUT);
        pinMode(Reset_C1, OUTPUT);
        pinMode(LED, OUTPUT); 
        initialize();
      
//     	Serial.begin(9600); 
}	

/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
  static float CommandedSpinSpeed = 0;   // radians / second
  static float CommandedSteerAngle = 0;  // radians
  
#ifdef TEST_MODE
  write_all (LOW);
  wait(500);
  write_all (HIGH);
  wait(500);
  
#else  // not TEST_MODE
/* TO DO:
// Read path and speed profile for next motions.
     GetSerial( &TimeOfCmd_ms);
//   If EStop or halt is active
     {
         CommandedSpinSpeed = 0;
         CommandedSteerAngle = 0;
     }
     else
// Compute how to follow this path.   
     ComputeSpeeds(&CommandedSpinSpeed, &CommandedSteerAngle);
// Send Gamebot command on TxD and put in CommandedSpinSpeed and CommandedSteerAngle
     SendSerial( );
*/ 
#endif  // TEST_MODE 
} 

#ifdef SIMULATOR
} // end namespace
#endif

#ifdef TEST_MODE
/*---------------------------------------------------------------------------------------*/ 
void write_all( int state)
{
      digitalWrite( Reset_C1, state);
      digitalWrite( LED, state);
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
/* The format of the command received over the serial line from C6 Navigator has 
not yet been specified.
The Pilot is provided with a few curve segments and speed profiles that define how
the vehicle travels over the next few meters.

Segments must include a few parameters:
Four points specifying a cubic spline as an Hermite curve [Foley et al, Introduction to Computer Graphics]. 
  - Start point.
  - Start velocity.
  - End point.
  - End velocity.
These points by themselves specify a family of curves. Specifying the parameter t 
for the speed at which the curve is traversed makes the curve unique. This value
is also provided to the Pilot over the serial line.

The points are defined on a coordinate system in meters with the X axis east and the 
Y axis true north.

The four points and t parameter specify a unique path.  They do not specify the speed at 
which the vehicle moves over the path. Vehicle speed is given by a sequence of ordered
pairs consisting of distance traveled on the path in meters and the requested speed at
that point in meters / second.  The first item in this sequence would be zero position and the
the current speed of the vehicle. This item may or may not be explicitly included. 
The distances travelled on the curve are cummulative and the last one should be equal
to the arc length of the curve.

The pilot must also receive the current vehicle position, attitude, velocity and acceleration.

Characters are ASCII. Wide characters are not used.

TO DO: Define the structure used for curve segment and motion profile.
TO DO: Write GetSerial.
*/
void GetSerial( unsigned long *TimeOfCmd)
{
}
/* Entry point for the simulator. */
void C3_Pilot_setup() { C3_Pilot::setup(); }

void C3_Pilot_loop() { C3_Pilot::loop(); }
