/*
Elcano Contol Module C2: Instrument panel controls and LEDs.
This routine handles simple drive-by-wire, as well as handing control over to the
computer based on the state of the enable switches.
*/


#define MEGA
#define STEP_SIZE_ms 5
#define TEST_MODE
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef MEGA
#include "IO_Mega.h"
#else
#include "IO_2009.h"
#endif

void Display (int n);
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
volatile unsigned long TimeOfStopButton_ms; //Records the millis() value when stop was last pressed
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
/*  Elcano Servo range is 50 mm for brake, 100 mm for steering.

    Elcano servo has a hardware controller that moves to a
    particular position based on a PWM signal from Arduino.
    Wikispeed servo is the same, except that controller box has
    buttons for extend, retract, or moving to three preset positions.
    
    Elcano servo is fully retracted on a pulse width of 2 ms;
    fully extended at 1 ms and centered at 1.5 ms.
    There is a deadband of 8 us.
    At 12v, servo operating speed is 56mm/s with no load or
    35 mm/s at maximum load.
    The Arduino PWM signal is at 490 Hz or 2.04 ms.
    0 is always off; 255 always on. One step is 7.92 us.
    
    Either servo has five wires, with observed bahavior of:
    White: 0V
    Yellow: 5V
    Blue: 0-5V depending on position of servo.
    Black: 12V while servo extends; 0V at rest or retracting.
    Red:   12V while retracting; 0V at rest or extending.
    The reading on the Blue line has hysteresis when Elcano sends a PWM signal; 
    there appear to be different (PWM, position) pairs when retracting or extending.
    Motor speed is probably controlled by the current on the red or black line.   
*/

  const int Motor = 0;
  const int Brakes = 1;
  const int Steering = 2;
  class _Instruments
  {
    public:
     int Enabled;  // whether cruise control is enabled.
     int State;
     int StickMoved;  // bool
     int Joystick;   // position of stick
     int LastCommand;  // last commanded
     int MinPosition;
     int MaxPosition;
     int Feedback;   // actual position
     int EnablePin;
     int SignalPin;
     int FeedbackPin;
     int CurrentPin;
     int CloseEnough;  // in counts; Try for 0-1023
     int Stuck;        // max number of time steps for not getting to position before giving up
     int StepSize;     // controls how quickly the servo moves. 2"/sec servo max
     int QuiescentCurrent;  // Nominally 120 counts
     int CurrentDraw;  // In counts, with 1 Amp = 12 counts
     int CurrentLimit;  // In counts
     // One count from analog input is about 5mV from ACS758lcb050U
     // The ACS758 has sensitivity of 60 mA/V
     //This does not actually move TO a position, it moves a distance of StepSize towards the position.
	 //Returns the actual sensed position
     int move(int desired_position)
     {
         int Position = desired_position;    // 0 to 1023
         Feedback = analogRead(FeedbackPin);  // 0 to 1023
		 //If the sensor says the current position is less than the desired position
		 //Than go one step towards it.
         if (desired_position > Feedback)
         {
            Position = min(desired_position, Feedback + StepSize);
			//But do't go any farther than the limit(Use a const here?)
            Position = min (Position, 1023);
         }
		 //Position>Desired
         else
         {
			//Go one step just like above and don't exceed limit.
            Position = max(desired_position,  Feedback - StepSize);
            Position = max(0, Position);
         }
		 //Map the range of the position variable to the expected duty cycle range of the actuator
         int PWMPosition = 127 + Position/8;
         analogWrite(SignalPin, PWMPosition);  // 0 to 255
		 //debug prints. []Should this be in an ifdef?
         Serial.print(desired_position); Serial.print(", ");
         Serial.print(Feedback); Serial.print(", ");
         Serial.print(Position); Serial.print(", ");
         Serial.println(PWMPosition);
        return Position;
     }
	//--Currently non-functioning, is a no-op--
	//Actually go to a position and block util there
     void home(int desired_position)
     {
       return;
       int feedback_after, Position;
       int no_move_count = 0;
    //Loop while not CloseEnough
       do
       {
         Serial.print("Home ");
         Position = move (desired_position);
         delay(STEP_SIZE_ms);
         feedback_after = analogRead(FeedbackPin);
		 //If we detect that we have not moved, increment safety counter
		 //Otherwise reset to zero.
		 //If somehow it just moved back and forth but never got there
		 //This could block forever.
         if (abs(feedback_after - Feedback) <= CloseEnough)
           no_move_count++;
         else

           no_move_count = 0;
		//Return if we are stuck []Return error code?
         if (no_move_count > Stuck)
           return;
       } while ((abs(desired_position - Feedback) > CloseEnough));
     }
  } Instrument[3];
 /*---------------------------------------------------------------------------------------*/ 
void setup()  
{  
        //Set up pin modes and interrupts, call serial.begin and call initialize.
        pinMode(ReverseButton, INPUT);
        pinMode(CruiseReverse, INPUT);
        pinMode(StopButton, INPUT);  // via interrupt
        pinMode(CruiseButton, INPUT); // via interrupt
        pinMode(EnableMotor, INPUT);
        pinMode(EnableBrake, INPUT);
        pinMode(EnableSteer, INPUT);
        pinMode(StopLED, OUTPUT);
        pinMode(CruiseLED, OUTPUT);
        pinMode(TractionMotor, OUTPUT);
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
        Serial.begin(9600);
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

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
        Display (0xFF);  // LED test
        digitalWrite( StopLED, HIGH);
        digitalWrite( CruiseLED, HIGH);
        
        Halt();
        
        Instrument[Motor].EnablePin = EnableMotor;
        Instrument[Motor].SignalPin = TractionMotor;
        Instrument[Motor].FeedbackPin = FeedbackMotor;
        Instrument[Motor].CurrentPin = Current36V;
        Instrument[Motor].StepSize = 20; // max from range of 1023.
        Instrument[Motor].CloseEnough = 1000;  // no feedback
        Instrument[Motor].Stuck = 3;
        Instrument[Motor].MinPosition = 85;  // anolog read range is 0 to 1023
        Instrument[Motor].MaxPosition = 800;
        
        
        Instrument[Steering].EnablePin = EnableSteer;
        Instrument[Steering].SignalPin = Steer;
        Instrument[Steering].FeedbackPin = FeedbackSteer;
        Instrument[Steering].CurrentPin = CurrentSteer;
        Instrument[Steering].StepSize = 20;
        Instrument[Steering].CloseEnough = 1;
        Instrument[Steering].Stuck = 3;
        Instrument[Steering].MinPosition = 85;  // anolog read range is 0 to 1023
        Instrument[Steering].MaxPosition = 800;
        
        Instrument[Brakes].EnablePin = EnableBrake;
        Instrument[Brakes].SignalPin = DiskBrake;
        Instrument[Brakes].FeedbackPin = FeedbackBrake;
        Instrument[Brakes].CurrentPin = CurrentBrake;
        Instrument[Brakes].StepSize = 50;
        Instrument[Brakes].CloseEnough = 1;
        Instrument[Brakes].Stuck = 3;
        Instrument[Brakes].MinPosition = 85;  // anolog read range is 0 to 1023
        Instrument[Brakes].MaxPosition = 800;
      
        //Load current positions from the actual sensors
        Instrument[Motor].Feedback = 
          analogRead(Instrument[Motor].FeedbackPin);
         Instrument[Steering].Feedback = 
          analogRead(Instrument[Steering].FeedbackPin);
         Instrument[Brakes].Feedback = 
          analogRead(Instrument[Brakes].FeedbackPin);
 //       Instrument[Motor].CommandedPosition
 //       Instrument[Steering].CommandedPosition
 //       Instrument[Brakes].CommandedPosition
 
       // assume that vehicle always stops with brakes on and wheels straight.
       // Otherwise, there is a possibility of blowing a fuse if servos try to
       // get to this state too quickly.
        Instrument[Steering].LastCommand = Straight;
        Instrument[Steering].home(Straight);  
        Instrument[Motor].State = CruiseStop;
        Instrument[Brakes].State = CruiseStop;
        Instrument[Steering].State = CruiseStop;
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
        Display (0);
        digitalWrite( StopLED, LOW);
        digitalWrite( CruiseLED, LOW);
        

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
   Instrument[Motor].LastCommand = Off;
   Instrument[Brakes].LastCommand = FullBrake;
   // TODO: make motor off and brakes on simultaneous.
   Instrument[Motor].home(Off);  
   Instrument[Brakes].home(FullBrake);  
   digitalWrite(StopLED, HIGH);
}

/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
  unsigned long TimeSinceCmd_ms = 0;     // radians
  const unsigned long MaxSilence = 2000;  // ms
  unsigned long TimeNow = millis();
  unsigned long NextLoopTime_ms = TimeNow + STEP_SIZE_ms;
  static unsigned long EndTime = 0;
  static unsigned long OutsideTime;
  
//Calculate how much time was spent outside the loop
  OutsideTime = EndTime - TimeNow;
//Read the user inputs for the enable switches
  Instrument[Motor].Enabled = digitalRead(EnableMotor);
  Instrument[Brakes].Enabled = digitalRead(EnableBrake);
  Instrument[Steering].Enabled = digitalRead(EnableSteer);
#ifdef TEST_MODE
//  testSwitches();
//  testRamp();
//    testQuick();
//    RampTest();
    ServoData();
   
#else  // not TEST_MODE
 
  JoystickMotion();
  StateTransition(Motor);
  StateTransition(Brakes);
  StateTransition(Steering);
  
  //Motor stuff
  
  //This logic determines the source of LastCommand
  if (Instrument[Motor].State == ManualGo)
  {
   Instrument[Motor].LastCommand = Instrument[Motor].Joystick;
  }
  else if (Instrument[Motor].State == CruiseGo)
  {
    Instrument[Motor].LastCommand = analogRead(CruiseThrottle) / 4;   
  }
  else
  {
    Instrument[Motor].LastCommand = MinimumThrottle;   
  }
   //Whatever the logic says, move a step towards it.
   Instrument[Motor].move(Instrument[Motor].LastCommand);
//Brakes stuff
  if (Instrument[Brakes].State == ManualGo)
  {
   Instrument[Brakes].LastCommand = Instrument[Brakes].Joystick;
  }  
  else if (Instrument[Brakes].State == CruiseGo)
  {
    Instrument[Brakes].LastCommand = analogRead(CruiseBrake) / 4;   
  }
  else
  {
    Instrument[Brakes].LastCommand = FullBrake;   
  }
   Instrument[Brakes].move(Instrument[Brakes].LastCommand);
  
//Steering stuff
   if (Instrument[Steering].State == ManualGo)
  {
   Instrument[Steering].LastCommand = Instrument[Steering].Joystick;
  }  
  else if (Instrument[Steering].State == CruiseGo)
  {
    Instrument[Steering].LastCommand = analogRead(CruiseSteer) / 4;   
  }
  Instrument[Steering].move(Instrument[Steering].LastCommand);

   if (Stop)
     digitalWrite(StopLED, HIGH);
   else
     digitalWrite(StopLED, LOW);
   if (TimeNow > TimeOfCruiseButton_ms + 400 &&
   (Instrument[Motor].StickMoved || Instrument[Brakes].StickMoved || Instrument[Steering].StickMoved))
       Cruise = FALSE;
   if (!Instrument[Motor].Enabled && !Instrument[Motor].Enabled && !Instrument[Motor].Enabled)
       Cruise = FALSE;
   if (Cruise)
     digitalWrite(CruiseLED, HIGH);
   else
     digitalWrite(CruiseLED, LOW);

  checkReverse();  //Logic to handle the reverse button.
#endif  // TEST_MODE 
 // TODO: Check out loop speed;
 // make loop faster
 // limit servo motion during loop to step size 
  unsigned long fred = millis();  
//  Serial.print(fred - TimeNow);
//  Serial.print(",");
  do  //  Delay so that loop is not faster than 10 Hz.
  {
    EndTime = millis();
  } while (EndTime < NextLoopTime_ms);
//  Serial.print(EndTime - fred);
//  Serial.print(",");
//  Serial.print(NextLoopTime_ms);
//  Serial.print(",");
//  Serial.println(OutsideTime);
 

} 
/*---------------------------------------------------------------------------------------*/ 
void JoystickMotion()
{
  int stick;  // 0-1023
  int center;
  const int deadBandLow = 32;  // out of 1023
  const int deadBandHigh = 32;
  center = 511; // analogRead(JoystickCenter);
  //Calculate deadband info
  int tinyDown = center - deadBandLow;
  int tinyUp = center + deadBandHigh;

 //This appears to make it pause(ignore input?) for PauseTime_ms after the button press
    if (Stop && (millis() < TimeOfStopButton_ms + PauseTime_ms))
    {
      Instrument[Motor].StickMoved = FALSE;
      Instrument[Brakes].StickMoved = FALSE;
      Instrument[Steering].StickMoved = FALSE;
      return;
    }
    stick = 511; // analogRead(AccelerateJoystick);
//If stick did not move more than the "up"	deadband.
    if (stick <= tinyUp)  // 543
    {
		//stick not outside deadBand
      Instrument[Motor].Joystick = 0;
      Instrument[Motor].StickMoved = FALSE;
    }
    else
    {
		//stick outside of deadband
      Instrument[Motor].Joystick = MinimumThrottle + (FullThrottle - MinimumThrottle)
       * (stick - tinyUp) / (1023 - tinyUp);
      Instrument[Motor].StickMoved =  TRUE;
      Stop = FALSE;
    }
 
	//Check if Stick moved beyone "down" deadBand
    if (stick >= tinyDown)
    {
		//Stick still in deadband
      Instrument[Brakes].Joystick = 0;
      Instrument[Brakes].StickMoved =  FALSE;
    }
    else
    {
      Instrument[Brakes].Joystick = MinimumBrake + (FullBrake - MinimumBrake) 
       * (-stick + tinyDown) / tinyDown;
      Instrument[Brakes].StickMoved =  TRUE;
      Stop = FALSE;
    }
//This line appears to be changed in for testing
//read the steering pot in the joystick

    stick = 511; // 1023 - analogRead(SteerJoystick);
	//If the stick is still in the left-right deadband
    if (stick > tinyDown && stick <= tinyUp)
    {
      Instrument[Steering].Joystick = Straight;
      Instrument[Steering].StickMoved =  FALSE;
    }
    else if (stick > tinyUp)
    {
		//Stick moved past "up" deadband limit
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
      break;  // Pause time is big enough to let vehicle come to stop
      // if user pushed the stop button, let it stop before we do the next thing.
    if (Instrument[system].Enabled )
    {  // Under cruise control
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
	//If we are in motor=manual stop and the reverse button is pressed
     TimeNow = millis();
	 //If the reverse button has not been pressed in the last 2000ms
     if (TimeNow > TimeOfReverseButton_ms + 2000)
     {
         TimeOfReverseButton_ms = TimeNow;
		 //Toggle reversed state
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
/*   Front Panel

        On/Off            Stop              Cruise
 
                ---------Enable-----------        
        Motor            Brake              Steer
        
          ----------------LED -------------------
          1      3      4        Reverse    Stop
          5      6      7          8        Cruise
          
For diagnostics, display an amalog input based on switches.

  Motor  Brake  Steer  Octal  Signal       
    0      0      0      0    AccelerateJoystick - JoystickCenter
    0      0      1      1    Steer
    0      1      0      2    DiskBrake
    0      1      1      3    Current36V
    1      0      0      4    TractionMotor
    1      0      1      5    CurrentBrake
    1      1      0      6    CurrentSteer
    1      1      1      7    SteerJoystick
*/
 void diagnostic()
 {
   int value;
   int Pin_in = (digitalRead(EnableSteer) << 2);
   Pin_in +=    (digitalRead(EnableBrake) << 1);
   Pin_in +=     digitalRead(EnableMotor);
    switch (Pin_in)
   {
   case 0:
   default:
      value = analogRead(AccelerateJoystick);
      value -= analogRead(JoystickCenter);
      value = value >> 2;
      break;
   case 1:
      value = Instrument[Steering].LastCommand;
      break;
   case 2:
      value = Instrument[Brakes].LastCommand;
      break;
   case 3:
      value = (analogRead(Current36V) >> 2);
      break;
   case 4:
      value = Instrument[Motor].LastCommand;
      break;
   case 5:
      value = (analogRead(CurrentBrake) >> 2);
      break;
   case 6:
      value = (analogRead(CurrentSteer) >> 2);
      break;
   case 7:
      value = analogRead(SteerJoystick) >> 2;
      break;
   }
   Display (value);
 }
 /*-------------------------------------------------------*/
void CruiseDiagnostic()
 {
   int value = 0;
   if (digitalRead(EnableMotor) == HIGH)
      value = analogRead(CruiseThrottle);
   else if (digitalRead(EnableBrake) == HIGH)
       value = analogRead(CruiseBrake);
   else if (digitalRead(EnableSteer) == HIGH)
      value = analogRead(CruiseSteer);
   value = value >> 2;
   Display (value);
 }
 /*-------------------------------------------------------*/
 #endif  // TESTMODE
 void Display (int n)
 {
    int Bit;
    Bit = (n & 0x01)? HIGH: LOW;
    digitalWrite(LED8, Bit);   
    Bit = (n & 0x02)? HIGH: LOW;
    digitalWrite(LED7, Bit);   
    Bit = (n & 0x04)? HIGH: LOW;
    digitalWrite(LED6, Bit);   
    Bit = (n & 0x08)? HIGH: LOW;
    digitalWrite(LED5, Bit);   
    Bit = (n & 0x10)? HIGH: LOW;
    digitalWrite(ReverseLED, Bit);   
    Bit = (n & 0x20)? HIGH: LOW;
    digitalWrite(LED4, Bit);   
    Bit = (n & 0x40)? HIGH: LOW;
    digitalWrite(LED3, Bit);   
    Bit = (n & 0x080)? HIGH: LOW;
    digitalWrite(LED1, Bit);   
 }

/*---------------------------------------------------------------------------------------*/ 
#ifdef TEST_MODE
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
//      ramp(TractionMotor, i);
//  }
//  write_all (HIGH);
//  delay (1000);
//  for (i = MinimumThrottle+5; i >= MinimumThrottle; i--)
//  {
//      ramp(TractionMotor, i);
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
  SwThrottle = digitalRead(EnableMotor);
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
//========================================================================
void ServoWrite(int i, int Delay_ms)
{
     analogWrite(DiskBrake, i);
     for (int d = 0; d < Delay_ms; d++)
     {
       int feedback = analogRead(FeedbackBrake);
       int current =  analogRead(CurrentBrake);
       Serial.print(Delay_ms); Serial.print(", ");
       Serial.print(i); Serial.print(", ");
       Serial.print(feedback); Serial.print(", ");
       Serial.print(current); Serial.print(", ");
       Serial.println(millis());

       delay(1);
    }
}
//=========================================================================
void ServoData()
{
  int Delay_ms = 20;
  int incr = 8;
  int i;
  for (Delay_ms = 20; Delay_ms > 0; Delay_ms--)
  {
   for (i = 0; i < 255; i+= incr)
      ServoWrite(i, Delay_ms);
   for (i = 255; i >= 0; i-= incr)
      ServoWrite(i, Delay_ms);
  } 
}
//==============================================================================
void RampTest()
{
  static int Position[] ={ 0, 0, 0};
  const int i = Brakes;
    for (int j = 80; j <800; j += 20) /*Instrument[i].MinPosition; j<= Instrument[i].MaxPosition;
    j += Instrument[i].StepSize) */
    {
        Serial.print("Move up ");
        Instrument[i].move(j);
      
//       if( digitalRead(Instrument[i].EnablePin) == HIGH)
//       {
//           if (abs(Instrument[i].move(Position[i]) - Position[i]) < Instrument[i].CloseEnough)
//           {
//             if (Position[i] = Position[i]? 0: 1023;
//           }
//       }
        delay(500);
    }
    for (int j = Instrument[i].MaxPosition; j>= Instrument[i].MinPosition;
    j -= 20)
    {
         Serial.print("Move down ");
        Instrument[i].move(j);
        delay(500);
    }
 
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

