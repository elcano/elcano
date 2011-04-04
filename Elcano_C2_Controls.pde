/*
Elcano Contol Module C@: Instrument panel controls and LEDs.
*/

#include "Serial.cpp"

/*

There are four principle states. "Enable" is considered a generic state that encompasses the
three switches for Throttle, Brake and Steer. The four main states have additional substates
corresponding to the state of these three systems. The system can simultaneaous be in mixed states,
e.g. Cruise Go on Throttle and Brakes and Manual Go on Steering.
When Cruise is enabled on a system, "Joystick Motion" means major motion in that axis:
Left to right for Steer, Forward for Throttle, Reverse for Brake. Minor joystick motions will
not take an enabled system into manual mode, but major motion will.

MS: Manual Stop State:  Stop LED On, Cruise LED Off,
  Throttle Off, Brakes Off,  Steer responds to joystick but not to computer.
  Joystick is centered.
  Enable Off.

CS: Cruise Stop State:  Stop LED On, Cruise LED Off,
  Throttle Off, Brakes Off,  Steer responds to joystick but not to computer.
  Joystick is centered.
  Enable On.
  
MG: Manual Go State:  Stop LED Off, Cruise LED Off,
  Throttle, Brakes, Steer respond to joystick but not to computer.

CG:Cruisel Go State:  Stop LED Off, Cruise LED On,
  Throttle, Brakes, Steer respond to computer.
  Joystick is centered.
  Enable On.

There are five controls that can transition between states:
Cruise button.
  MS -> MS; CS -> CG; MG -> CG; CG -> CG.
Joystick motion
  MS -> MG; CS -> MG; MG -> MG; CG -> MG.
Enable Switch On
  MS -> CS; CS N/A; MG -> MG; CG N/A.
Enable Switch Off
  MS N/A; CS -> MS; MG -> MG; CG -> MG.
Stop button
  MS -> MS; CS -> CS; MG -> MS; CG -> CS.
E-stop
  MS -> MS; CS -> CS; MG -> MS; CG -> CS. E-stop also removes power to the traction motor.
If the E-stop button has been pushed, transition to MG or CG is blocked until
power is restored. Restoring power is done by releasing the physical E-stop button or pressing 
the remote E-stop button a second time. There is a dead zone of several seconds between the
first button push and the second push, allowing the vehicle to come to a stop.

*/

 
/* [in] Digital Signal 2: J1 pin 3  (INT0) Stop button. 
The Stop signal is a momentary button push from either the console or remote.
It's last state is latched; so it can be polled for change.
The Cruise button works the same way.
*/
const int StopButton = 2;
const int CruiseButton = 4;

const int StopLED = 7;
const int CruiseLED = 8;

/* [out] Digital Signal 9: J1 pin 2  (D9) Actuator A1. Traction motor throttle. */
const int Throttle =  9;

/* [out] Digital Signal 10: J3 pin 3 (D10) PWM. Actuator A2: Brake Motor. 
  Controls disk brakes on left and right front wheels. Brakes are mechanically linked to operate in tandem. */
const int DiskBrake = 10;

/* [out] Digital Signal 11: J3 pin 4 (D11) PWM. Actuator A3: Steering Motor. 
  Turns left or right. If no signal, wheels are locked to a straight ahead position. */
const int Steer =  11;

const int EStop = 12;

/* Pin 13 is LED. */

/* [in] Analog input 0: J2 pin 1 (ADC0) . */
const int MotionJoystick = 0;

/* [in] Analog input 1: J2 pin 2 (ADC1) MotorTemperature. */
const int SteerJoystick = 1;

/* Analog Input pins 0-5 are also Digital Inputs 14-19. */
const int EnableThrottle = 17;
const int EnableBrake = 18;
const int EnableSteer = 19;

/* RESERVED: Pins 1,2,3,5,6,16 */

/* time (micro seconds) for a wheel revolution */
volatile unsigned long WheelRevMicros = 0;

unsigned long TimeOfCmd_ms = 0;

const int Full = 255;   // fully on for PWM signals
const int Off = 0;
const int FullThrottle =  179;   // 3.5 V
const int MinimumThrottle = 39;  // Throttle has no effect until 0.75 V
const int FullBrake = 255;
const int MinimumBrake = 0;
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

 /*---------------------------------------------------------------------------------------*/ 
void setup() 
{ 
        pinMode(RxD, INPUT);
        pinMode(TxD, OUTPUT);
        pinMode(StopButton, INPUT);
        pinMode(CruiseButton, INPUT);
        pinMode(StopLED, OUTPUT);
        pinMode(CruiseLED, OUTPUT);
        pinMode(Throttle, OUTPUT);
        pinMode(DiskBrake, OUTPUT);
        pinMode(Steer, OUTPUT);
        pinMode(EStop, INPUT);
        pinMode(LED, OUTPUT); 
//      analogReference(EXTERNAL);
        pinMode(MotionJoystick, INPUT);
        pinMode(SteerJoystick, INPUT);
        pinMode(EnableThrottle, INPUT);
        pinMode(EnableBrake, INPUT);
        pinMode(EnableSteer, INPUT);

        initialize();
        
}	

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
        Stop();
        
        digitalWrite( EnableThrottle, HIGH);
        analogWrite( Steer, Straight);
        digitalWrite( LED, LOW);
}
/*---------------------------------------------------------------------------------------*/ 

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
   analogWrite(DiskBrake, Full);
}
#define TEST_MODE
/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
  unsigned long TimeSinceCmd_ms = 0;     // radians
  const unsigned long MaxSilence = 2000;  // ms
  int i;
  
#ifdef TEST_MODE
//  write_all (LOW);
//  delay (1000);
//  for (i = MinimumThrottle; i <= MinimumThrottle+5; i++)
//  {
//      ramp(Throttle, i);
//  }
//  write_all (HIGH);
//  delay (1000);
//  for (i = MinimumThrottle+5; i >= MinimumThrottle; i--)
//  {
//      ramp(Throttle, i);
//  }
  
   write_all (LOW);
   delay (1000);
   for (i = MinimumBrake; i <= FullBrake; i++)
  {
      ramp(DiskBrake, i);
  }
  write_all (HIGH);
  delay (1000);
  for (i = FullBrake; i >= MinimumBrake; i--)
  {
      ramp(DiskBrake, i);
  }
  
//   write_all (LOW);
//   delay (1000);
//   for (i = HardLeft; i <= HardRight; i++)
//   {
//      ramp(Steer, i);
//   }
//    write_all (HIGH);
//  delay (1000);
//  for (i = HardRight; i >= HardLeft; i--)
//  {
//      ramp(Steer, i);
//  }
   
#else  // not TEST_MODE
 

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
      digitalWrite( StopLED, state);
      digitalWrite( CruiseLED, state);
      digitalWrite( LED, state);
}
/*---------------------------------------------------------------------------------------*/ 
void ramp (int channel, int state)
{
   analogWrite( channel, state);
   delay(1000); 

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

