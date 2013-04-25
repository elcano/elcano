/*
Elcano Motor control Test routine
March 1, 2013    T.C. Folsom
*/
#define MEGA
//#define TEST_MODE
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef MEGA
//#include "IO_Mega.h"
#include <IO_PCB.h>
#else
#include <IO_2009.h>
#endif


volatile int Stop = TRUE;
volatile unsigned long TimeOfStopButton_ms;
volatile int Cruise = FALSE;
volatile unsigned long TimeOfCruiseButton_ms;
const int PauseTime_ms = 4000;

const int Full = 255;   // fully on for PWM signals
const int Off = 0;
const int FullThrottle =  255;   // 3.5 V
const int MinimumThrottle = 39;  // Throttle has no effect until 0.75 V
const int FullBrake = 255;  
const int MinimumBrake = 127;
const int HardLeft = 127;
const int HalfLeft = 159;
const int Straight = 191;
const int HalfRight = 223;
const int HardRight = 255;
/*  Servo range is 50 mm for brake, 100 mm for steering.
    Servo is fully retracted on a pulse width of 2 ms;
    fully extended at 1 ms and centered at 1.5 ms.
    There is a deadband of 8 us.
    At 12v, servo operating speed is 56mm/s with no load or
    35 mm/s at maximum load.
    
    The Arduino PWM signal is at 490 Hz or 2.04 ms.
    0 is always off; 255 always on. One step is 7.92 us.
    
*/

  const int Motor = 0;
  const int Brakes = 1;
  const int Steering = 2;
  struct _Instruments
  {
     int Enabled;  // whether cruise control is enabled.
//   int State;
     int StickMoved;  // bool
     int Joystick;   // position of stick
     int Position;  // last commanded
     int QuiescentCurrent;  // Nominally 120 counts
     int CurrentDraw;  // In counts, with 1 Amp = 12 counts
     int CurrentLimit;  // In counts
     // One count from analog input is about 5mV from ACS758lcb050U
     // The ACS758 has sensitivity of 60 mA/V
  } Instrument[3];
  
  int throttle = MinimumThrottle;
  int delta = 1;
 /*---------------------------------------------------------------------------------------*/ 
void setup()  
{  
        pinMode(LED, OUTPUT);
        pinMode(ReverseButton, INPUT);
        pinMode(CruiseReverse, INPUT);
        pinMode(StopButton, INPUT);  // via interrupt
        pinMode(CruiseButton, INPUT); // via interrupt
        pinMode(EnableThrottle, INPUT);
        pinMode(EnableBrake, INPUT);
        pinMode(EnableSteer, INPUT);
        
        pinMode(StopLED, OUTPUT);
        pinMode(CruiseLED, OUTPUT);
        pinMode(Throttle, OUTPUT);
        pinMode(DiskBrake, OUTPUT);
        pinMode(Steer, OUTPUT);
        pinMode(Reverse, OUTPUT);
        pinMode(ReverseLED, OUTPUT); 

 #ifdef MEGA
        pinMode(LED1, OUTPUT);
        pinMode(LED3, OUTPUT);
        pinMode(LED4, OUTPUT);
        pinMode(LED5, OUTPUT);
        pinMode(LED6, OUTPUT);
        pinMode(LED7, OUTPUT);
        pinMode(LED8, OUTPUT); 
        return; 
 
        initialize();
        attachInterrupt(0, CruiseRC1Pressed, RISING);     
        attachInterrupt(1, StopRC2Pressed, RISING);
        attachInterrupt(3, EStopPressed, FALLING);
        attachInterrupt(4, StopPressed, RISING);
        attachInterrupt(5, CruisePressed, RISING);     
#else
        initialize();
        attachInterrupt(0, StopPressed, FALLING);
        attachInterrupt(1, CruisePressed, FALLING);     
#endif
}	
//-------------------------------------------------------
// Motor test: Slow ramp up and down.

void loop()
{
  analogWrite(Throttle, throttle);  
  throttle += delta;
  if (throttle >  FullThrottle)
  {  
    delta = -delta;
    throttle =  FullThrottle;
  }
  if (throttle <  MinimumThrottle)
  {  
    delta = -delta;
    throttle =  MinimumThrottle;
  }
  if (delta > 0)
  {
   digitalWrite(LED, HIGH);
   delay(100);
  }
  else
  {
   digitalWrite(LED, LOW);
   delay(100);
  } 
}
/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
        Halt();
       // assume that vehicle always stops with brakes on and wheels straight.
       // Otherwise, there is a possibility of blowing a fuse if servos try to
       // get to this state too quickly.       
        analogWrite( Steer, Straight);
        Instrument[Steering].Position = Straight;
        digitalWrite( StopLED, HIGH);
        digitalWrite( CruiseLED, LOW);
        Instrument[Motor].Joystick = Off;
        Instrument[Brakes].Joystick = Off;
        Instrument[Steering].Joystick = Straight;
        Instrument[Motor].CurrentLimit = 230;  // about 20A
        Instrument[Brakes].CurrentLimit = 55;  // about 5A
        Instrument[Steering].CurrentLimit = 55;
        TimeOfStopButton_ms = 0;
        TimeOfCruiseButton_ms = 0;
        // Wait for everything to settle down
 //     delay(2000);  // 2 second
        // Read quiesent state of current sensors.
        Instrument[Brakes].CurrentDraw = 
        Instrument[Brakes].QuiescentCurrent = analogRead(CurrentBrake);
        Instrument[Steering].CurrentDraw = 
        Instrument[Steering].QuiescentCurrent = analogRead(CurrentSteer);
        Instrument[Motor].CurrentDraw = 
        Instrument[Motor].QuiescentCurrent = analogRead(Current36V);

}
/*---------------------------------------------------------------------------------------*/ 
/*---------------------------------------------------------------------------------------
 CruisePressed is called by an interrupt
 */  
void CruisePressed()
{
    TimeOfCruiseButton_ms = millis();
    Cruise = TRUE;
    Stop = FALSE;
}
void CruiseRC1Pressed()
{
    TimeOfCruiseButton_ms = millis();
    Cruise = TRUE;
    Stop = FALSE;
}
/*---------------------------------------------------------------------------------------
 StopPressed is called by an interrupt
 */  
void StopPressed()
{
    Halt();
    TimeOfStopButton_ms = millis();
    Stop = TRUE;
    Cruise = FALSE;
}
void StopRC2Pressed()
{
    Halt();
    TimeOfStopButton_ms = millis();
    Stop = TRUE;
    Cruise = FALSE;
}
void EStopPressed()
{
    Halt();
    TimeOfStopButton_ms = millis();
    Stop = TRUE;
    Cruise = FALSE;
}
/*---------------------------------------------------------------------------------------*/ 
// Bring the vehicle to a stop.
// Stop may be called from an interrupt or from the loop.
void Halt()
{
   analogWrite(Throttle, Off);
   analogWrite(DiskBrake, FullBrake);
   Instrument[Motor].Position = Off;
   Instrument[Brakes].Position = FullBrake;
   digitalWrite(StopLED, HIGH);
}

/*---------------------------------------------------------------------------------------*/ 
