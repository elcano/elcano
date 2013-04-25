/*
Elcano Contol Module C2: Instrument panel controls and LEDs.
*/
#define MEGA
#define TEST_MODE
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// #include <Serial.h>
#ifdef MEGA
#include <IO_Mega.h>
#else
#include <IO_2009.h>
#endif

/*

There are four principle states. "Enable" is considered a generic state that encompasses the
three switches for Throttle, Brake and Steer. The four main states have additional substates
corresponding to the state of these three systems. The system can simultaneaous be in mixed states,
e.g. "Cruise Go" on Throttle and Brakes and "Manual Go" on Steering.
When Cruise is enabled on a system, "Joystick Motion" means major motion in that axis:
Left to right for Steer, Forward for Throttle, Reverse for Brake. Minor joystick motions will
not take an enabled system into manual mode, but major motion will.

MS: Manual Stop State:  Stop LED On, Cruise LED Off,
  Throttle Off, Brakes Off,  Steer responds to joystick but not to computer.
  Joystick is centered.
  Enable Off.

CS: Cruise Stop State:  Stop LED On, Cruise LED On,
  Throttle Off, Brakes Off,  Steer responds to computer.
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
power is restored. Restoring power is done by turning the key or pressing 
the remote E-stop button (RC4) a second time. There is a dead zone of several seconds between the
first button push and the second push, allowing the vehicle to come to a stop.

States:  */
  const int ManualStop = 0;
  const int CruiseStop = 1;
  const int ManualGo = 3;
  const int CruiseGo = 4;



volatile int Stop = TRUE;
volatile unsigned long TimeOfStopButton_ms;
volatile int Cruise = FALSE;
volatile unsigned long TimeOfCruiseButton_ms;
const int PauseTime_ms = 4000;
unsigned long TimeOfReverseButton_ms;
int Forward;

const int Full = 255;   // fully on for PWM signals
const int Off = 0;
const int FullThrottle =  179;   // 3.5 V
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
     int State;
     int StickMoved;  // bool
     int Joystick;   // position of stick
     int Position;  // last commanded
     int QuiescentCurrent;  // Nominally 120 counts
     int CurrentDraw;  // In counts, with 1 Amp = 12 counts
     int CurrentLimit;  // In counts
     // One count from analog input is about 5mV from ACS758lcb050U
     // The ACS758 has sensitivity of 60 mA/V
  } Instrument[3];
 /*---------------------------------------------------------------------------------------*/ 
void setup()  
{  
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
        pinMode(AccelerateJoystick, INPUT);
        pinMode(SteerJoystick, INPUT);
        pinMode(JoystickCenter, INPUT);
        pinMode(CruiseThrottle, INPUT);
        pinMode(CruiseBrake, INPUT); 
        pinMode(CruiseSteer, INPUT);
#ifdef MEGA
        pinMode(Current36V, INPUT);
        pinMode(CurrentBrake, INPUT);
        pinMode(CurrentSteer, INPUT);
        pinMode(LED1, OUTPUT);
        pinMode(LED3, OUTPUT);
        pinMode(LED4, OUTPUT);
        pinMode(LED5, OUTPUT);
        pinMode(LED6, OUTPUT);
        pinMode(LED7, OUTPUT);
        pinMode(LED8, OUTPUT);  
        initialize();
        attachInterrupt(0, EStopPressed, FALLING);
        attachInterrupt(2, StopPressed, RISING);
        attachInterrupt(3, CruisePressed, RISING);     
        attachInterrupt(4, StopRC2Pressed, RISING);
        attachInterrupt(5, CruiseRC1Pressed, RISING);     
#else
        initialize();
        attachInterrupt(0, StopPressed, FALLING);
        attachInterrupt(1, CruisePressed, FALLING);     
#endif
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
        Instrument[Motor].State = ManualStop;
        Instrument[Brakes].State = ManualStop;
        Instrument[Steering].State = ManualStop;
        Instrument[Motor].Joystick = Off;
        Instrument[Brakes].Joystick = Off;
        Instrument[Steering].Joystick = Straight;
        Instrument[Motor].CurrentLimit = 230;  // about 20A
        Instrument[Brakes].CurrentLimit = 55;  // about 5A
        Instrument[Steering].CurrentLimit = 55;
        TimeOfStopButton_ms = 0;
        TimeOfCruiseButton_ms = 0;
        TimeOfReverseButton_ms = 0;
        Forward = TRUE;
        // Wait for everything to settle down
        delay(2000);  // 2 second
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
}
void CruiseRC1Pressed()
{
    TimeOfCruiseButton_ms = millis();
    Cruise = TRUE;
}
/*---------------------------------------------------------------------------------------
 StopPressed is called by an interrupt
 */  
void StopPressed()
{
    Halt();
    TimeOfStopButton_ms = millis();
    Stop = TRUE;
}
void StopRC2Pressed()
{
    Halt();
    TimeOfStopButton_ms = millis();
    Stop = TRUE;
}
void EStopPressed()
{
    Halt();
    TimeOfStopButton_ms = millis();
    Stop = TRUE;
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
void loop() 
{
  unsigned long TimeSinceCmd_ms = 0;     // radians
  const unsigned long MaxSilence = 2000;  // ms
  int TimeNow = millis();
  int NextLoopTime_ms = TimeNow + 100;
  
#ifdef TEST_MODE
//  testSwitches();
//  testRamp();
    testQuick();
   
#else  // not TEST_MODE
 
  JoystickMotion();
  Instrument[Motor].Enabled = digitalRead(EnableThrottle);
  Instrument[Brakes].Enabled = digitalRead(EnableBrake);
  Instrument[Steering].Enabled = digitalRead(EnableSteer);
  StateTransition(Motor);
  StateTransition(Brakes);
  StateTransition(Steering);
  
  if (Instrument[Motor].State == ManualGo)
  {
   analogWrite(Throttle, Instrument[Motor].Joystick);
   Instrument[Motor].Position = Instrument[Motor].Joystick;
  }
  else if (Instrument[Motor].State == CruiseGo)
  {
    Instrument[Motor].Position = analogRead(CruiseThrottle) / 4;   
    analogWrite(Throttle, Instrument[Motor].Position);
  }
  else
  {
    Instrument[Motor].Position = MinimumThrottle;   
    analogWrite(Throttle, Instrument[Motor].Position);
  }
  
  if (Instrument[Brakes].State == ManualGo)
  {
   analogWrite(DiskBrake, Instrument[Brakes].Joystick);
   Instrument[Brakes].Position = Instrument[Brakes].Joystick;
  }  
  else if (Instrument[Brakes].State == CruiseGo)
  {
    Instrument[Brakes].Position = analogRead(CruiseBrake) / 4;   
    analogWrite(DiskBrake, Instrument[Brakes].Position);
  }
  else
  {
    Instrument[Brakes].Position = FullBrake;   
    analogWrite(DiskBrake, Instrument[Brakes].Position);
  }
  
   if (Instrument[Steering].State == ManualGo)
  {
   analogWrite(Steer, Instrument[Steering].Joystick);
   Instrument[Steering].Position = Instrument[Steering].Joystick;
  }  
  else if (Instrument[Steering].State == CruiseGo)
  {
    Instrument[Steering].Position = analogRead(CruiseSteer) / 4;   
    analogWrite(Steer, Instrument[Steering].Position);
  }
   if (Stop)
     digitalWrite(StopLED, HIGH);
   else
     digitalWrite(StopLED, LOW);
   if (TimeNow > TimeOfCruiseButton_ms + 400 &&
   (Instrument[Motor].StickMoved || Instrument[Brakes].StickMoved || Instrument[Steering].StickMoved)
       Cruise = FALSE;
   if (!Instrument[Motor].Enabled && !Instrument[Motor].Enabled && !Instrument[Motor].Enabled)
       Cruise = False
   if (Cruise)
     digitalWrite(CruiseLED, HIGH);
   else
     digitalWrite(CruiseLED, LOW);

  checkReverse();   
  do  //  Delay so that loop is not faster than 10 Hz.
  {
    TimeNow = millis();
  } while (TimeNow < NextLoopTime_ms);
 

#endif  // TEST_MODE 
} 
/*---------------------------------------------------------------------------------------*/ 
void JoystickMotion()
{
  int stick;
  int center;
  const int deadBandLow = 32;  // out of 1023
  const int deadBandHigh = 32;
  center = analogRead(JoystickCenter);
  int tinyDown = center - deadBandLow;
  int tinyUp = center + deadBandHigh;

    if (Stop && (millis() < TimeOfStopButton_ms + PauseTime_ms))
    {
      Instrument[Motor].StickMoved = FALSE;
      Instrument[Brakes].StickMoved = FALSE;
      Instrument[Steering].StickMoved = FALSE;
      return;
    }
    stick = analogRead(AccelerateJoystick);
    if (stick <= tinyUp)
    {
      Instrument[Motor].Joystick = 0;
      Instrument[Motor].StickMoved = FALSE;
    }
    else
    {
      Instrument[Motor].Joystick = 255 * (stick - tinyUp) / (1023 - tinyUp);
      Instrument[Motor].StickMoved =  TRUE;
      Stop = FALSE;
    }
 
    if (stick >= tinyDown)
    {
      Instrument[Brakes].Joystick = 0;
      Instrument[Brakes].StickMoved =  FALSE;
    }
    else
    {
      Instrument[Brakes].Joystick = MinimumBrake + (FullBrake - MinimumBrake) * (-stick + tinyDown) / tinyDown;
      Instrument[Brakes].StickMoved =  TRUE;
      Stop = FALSE;
    }

    stick = analogRead(SteerJoystick);
    if (stick > tinyDown && stick <= tinyUp)
    {
      Instrument[Steering].Joystick = Straight;
      Instrument[Steering].StickMoved =  FALSE;
    }
    else if (stick > tinyUp)
    {
      Instrument[Steering].Joystick = Straight + (HardRight - Straight) * (stick - tinyUp) / (1023 - tinyUp);
      Instrument[Steering].StickMoved =  TRUE;
      Stop = FALSE;
    }
    else 
    {
      Instrument[Steering].Joystick = Straight + (Straight - HardLeft) * (-stick + tinyDown) / tinyDown;
      Instrument[Steering].StickMoved = TRUE;
      Stop = FALSE;
    }
}
/*---------------------------------------------------------------------------------------*/ 
void StateTransition( int system)
{
  unsigned long Time_ms = millis();  
  switch(Instrument[system].State)
  {
    case ManualStop:
    if (TimeOfStopButton_ms + PauseTime_ms > Time_ms)
      break;
      if (Instrument[system].Enabled)
      {
        Instrument[system].State = CruiseStop;
        break;
      }
//  if (joystick motion) set state.
    if (Instrument[system].StickMoved)
    {
      Instrument[system].State = ManualGo;
    }
    break;
    
    case ManualGo:
    if (Stop)
    {
      Instrument[system].State = ManualStop;
      break;
    }
    if (Cruise)
    {
      Instrument[system].State = CruiseGo;
      break;
      // respond to joystick
    }
    break;
      
    case CruiseStop:
    if (TimeOfStopButton_ms + PauseTime_ms > Time_ms)
      break;
    if (Instrument[system].StickMoved)
    {
       Instrument[system].State = ManualGo;
       break;
    }
    if (!Instrument[system].Enabled)
    {
      Instrument[system].State = ManualStop;
      break;
    }
    if (Cruise)
    {
      Instrument[system].State = CruiseGo;
    }
    break;
    
    case CruiseGo:
    if (Stop)
    {
      Instrument[system].State = CruiseStop;
      break;
    }
    if (Instrument[system].StickMoved)
    {
      Instrument[system].State = ManualGo;
      break;
    }
    if (!Instrument[system].Enabled)
    {
      Instrument[system].State = ManualGo;
      break;
    }
    break;
  }
}
/*---------------------------------------------------------------------------------------*/ 
void  checkReverse()
{
  unsigned long TimeNow;
  int ReversePushed;
  
  if (Instrument[Motor].State == ManualStop)
 { 
   ReversePushed = digitalRead(ReverseButton);
   if (ReversePushed)
   {
     TimeNow = millis();
     if (TimeNow > TimeOfReverseButton_ms + 2000)
     {
         TimeOfReverseButton_ms = TimeNow;
         Forward = !Forward;
     }
   }
  digitalWrite(ReverseLED, !Forward);
  digitalWrite(Reverse, !Forward);
 }
   if (Instrument[Motor].State == CruiseStop || Instrument[Motor].State == CruiseGo)
   {
     Forward = !digitalRead(CruiseReverse);
     digitalWrite(ReverseLED, !Forward);
     digitalWrite(Reverse, !Forward);
   }
}


/*********************************************************** 
                        TEST ROUTINES
 ***********************************************************/
 

#ifdef TEST_MODE

/*---------------------------------------------------------------------------------------*/ 
void testRamp()
{
  // test passed for brakes and steering 5/21/11.
  // Computer commanded braking, steering and throttle works!
  
    int i, throttle;
    unsigned int count = 0;

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
  write_all (HIGH);
  delay (1000);
  
   write_all (LOW);
   delay (1000);
   FlashMorse (count++);
 
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
  
   write_all (LOW);
   delay (1000);
   for (i = Straight; i <= HardRight; i++)
   {
      ramp(Steer, i);
   }
    write_all (HIGH);
  delay (1000);
  for (i = HardRight; i >= Straight; i--)
  {
      ramp(Steer, i);
  }
   write_all (LOW);
   
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

   delay (1000);
   for (i = Straight; i <= HardLeft; i++)
   {
      ramp(Steer, i);
   }
    write_all (HIGH);
    
    
  delay (1000);
  for (i = HardLeft; i >= Straight; i--)
  {
      ramp(Steer, i);
  }
}
/*---------------------------------------------------------------------------------------*/ 
void testSwitches()
{
  // test failed 5/2/11.  Panel LEDs do not light.
  int SwThrottle, SwBrake, SwSteer;
  SwThrottle = digitalRead(EnableThrottle);
  SwBrake = digitalRead(EnableBrake);
  SwSteer = digitalRead(EnableSteer);
  digitalWrite( StopLED, SwBrake);
  digitalWrite( CruiseLED, SwThrottle);
  digitalWrite( ReverseLED, SwSteer);
}
/*---------------------------------------------------------------------------------------*/ 
// testQuick: fast operation of brakes and steering to measure peak power demand
void testQuick()
{
  int i, steer, brake;
  for (i = 0; i < 5; i++)
  {
     steer = Straight + i * (HalfRight - Straight)/4;
     analogWrite( Steer, steer);
     brake = MinimumBrake + i * (FullBrake - MinimumBrake)/4;
     analogWrite( DiskBrake, brake);
   }
  for (i = 5; i >= 0; i--)
  {
     steer = Straight + i * (HalfRight - Straight)/4;
     analogWrite( Steer, steer);
     brake = MinimumBrake + i * (FullBrake - MinimumBrake)/4;
     analogWrite( DiskBrake, brake);
   }

}

/*---------------------------------------------------------------------------------------*/ 
void write_all( int state)
{
      digitalWrite( StopLED, state);
      digitalWrite( CruiseLED, state);
      digitalWrite( ReverseLED, state);
}
/*---------------------------------------------------------------------------------------*/ 
void ramp (int channel, int state)
{
   analogWrite( channel, state);
   delay(100); 

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
   digitalWrite( ReverseLED, LOW);
   delay (NUMBER_SPACE);

}
void Flash(int DashDot)
{
  digitalWrite( ReverseLED, LOW);
  delay (BIT_SPACE);
  digitalWrite( ReverseLED, HIGH);
  delay (DashDot);
}
#endif  // TEST_MODE 

