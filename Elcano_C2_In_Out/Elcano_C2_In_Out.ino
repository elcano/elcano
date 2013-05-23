

/*
Elcano Contol Module C2: Tests for steering and brake servos
*/


#define MEGA
#define STEP_SIZE_ms 5
#define TEST_MODE
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


#include <IO_PCB.h>


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

const int FullBrake = 167;  // start with a conservative value; could go as high as 255;  
const int NoBrake = 207; // start with a conservative value; could go as low as 127;

const int HardLeft = 223; // was 159; // start with a conservative value; could go as high as 255;
const int HalfLeft = 223; // was 159;
const int Straight = 191;
const int HalfRight = 159; // was 223;
const int HardRight = 159; // was 223; // start with a conservative value; could go as low as 127;
const int StraightFB = 501;
const int HalfRightFB = 303;
const int HalfLeftFB = 760;
/* There is hysteresis on steering.
  E.g. Move to the left until PWM 223, getting FB of 760.
  If moves to the right are then commanded, feedback will not change until PWM of about 217.
*/
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
    The Blue wire is 
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
     int Stuck;        // max number of time steps for not getting to position
     int StepSize;     // controls how quickly the servo moves. 2"/sec servo max
     int QuiescentCurrent;  // Nominally 120 counts
     int CurrentDraw;  // In counts, with 1 Amp = 12 counts
     int CurrentLimit;  // In counts
     // One count from analog input is about 5mV from ACS758lcb050U
     // The ACS758 has sensitivity of 60 mA/V
     int move(int desired_position)
     {
         int Position = desired_position;    // 0 to 1023
         Feedback = analogRead(FeedbackPin);  // 0 to 1023
         if (desired_position > Feedback)
         {
            Position = min(desired_position, Feedback + StepSize);
            Position = min (Position, 1023);
         }
         else
         {
            Position = max(desired_position,  Feedback - StepSize);
            Position = max(0, Position);
         }
         int PWMPosition = 127 + Position/8;
         analogWrite(SignalPin, PWMPosition);  // 0 to 255
         Serial.print(desired_position); Serial.print(", ");
         Serial.print(Feedback); Serial.print(", ");
         Serial.print(Position); Serial.print(", ");
         Serial.println(PWMPosition);
        return Position;
     }
     void home(int desired_position)
     {
       return;
       int feedback_after, Position;
       int no_move_count = 0;
       do
       {
         Serial.print("Home ");
         Position = move (desired_position);
         delay(STEP_SIZE_ms);
         feedback_after = analogRead(FeedbackPin);
         if (abs(feedback_after - Feedback) <= CloseEnough)
           no_move_count++;
         else
           no_move_count = 0;
         if (no_move_count > Stuck)
           return;
       } while ((abs(desired_position - Feedback) > CloseEnough));
     }
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
        
        CalibrateBrakes(400);
/*        
       CalibrateSteering(HardRight,3);     
       CalibrateSteering(HardLeft,3);
       delay(1000);
       CalibrateSteering(HardRight,1);
       CalibrateSteering(HardLeft,1);
       delay(1000);
       CalibrateSteering(HardRight,10);       
*/       
}	

/*---------------------------------------------------------------------------------------*/ 
void BrakeOn()
{
     analogWrite(DiskBrake, FullBrake);
}
void BrakeOff()
{
     analogWrite(DiskBrake, NoBrake);
}
void CalibrateBrakes( int delaySmall)
{
  // This routine establishes the correct values for NoBrake and FullBrake.
  // Verify that the value for FullBrake stops the vehicle.
  // It allows us to establish the constants to map the feedback to the commanded position.
  
  // PWM signal is only effective from 127 to 255.
  // Some of the extreme values produce no motion in an unattached servo.
  // When the servo is atached to the brake lever, pushing too hard can draw too much power and blow a fuse.
  
  // Perform this test first with an unattached servo.
  // Repeat with an attached servo.
  
 // It would be a good idea to put an ammeter on the brake servo to verify that excess current is not being drawn;
 // In case current gets too high, abort by unplugging the servo.
 
 // Use the serial monitor, and save the results for analysis.
 
 // Mechanically adjust the brake cables so that the motion of the servo is appropriate
       int feedbacks[5];
       Serial.print(2*delaySmall);
       Serial.println(" Calibrate brakes");
       for (int i = NoBrake; i > FullBrake; i -= 15)
     {
       analogWrite(DiskBrake, i);
//       int current =  analogRead(CurrentBrake);
       delay(delaySmall);  // allow tiime for brake to respond
       feedbacks[0] = analogRead(BrakeFB);
       delay(1);
       feedbacks[1] = analogRead(BrakeFB);
       delay(1);
       feedbacks[2] = analogRead(BrakeFB);
       delay(1);
       feedbacks[3] = analogRead(BrakeFB);
       delay(1);
       feedbacks[4] = analogRead(BrakeFB);
       int feedback = median_filter(feedbacks);
       // feedback signal may have glitches; use of a 5 point median filter is recommended.
       Serial.print(i); Serial.print(", ");
//       Serial.print(current); Serial.print(", ");
       Serial.println(feedback);
       delay(delaySmall);   // hold each brake position 1 second.
     }
}
void moveSteer(int i)
{
       int feedbacks[5];
       analogWrite(Steer, i);
//       int current =  analogRead(CurrentBrake);
       delay(95);  // allow tiime for steer to respond
       // feedback signal may have glitches; use of a 5 point median filter is recommended.
       feedbacks[0] = analogRead(SteerFB);
       delay(1);
       feedbacks[1] = analogRead(SteerFB);
       delay(1);
       feedbacks[2] = analogRead(SteerFB);
       delay(1);
       feedbacks[3] = analogRead(SteerFB);
       delay(1);
       feedbacks[4] = analogRead(SteerFB);
       int feedback = median_filter(feedbacks);
       Serial.print(i); Serial.print(", ");
//       Serial.print(current); Serial.print(", ");
       Serial.println(feedback);
       delay(900);   // hold each steer position 1 second.

}
/*---------------------------------------------------------------------------------------*/ 
void CalibrateSteering(int SteerPosition, int delta)
{
  // This routine establishes the correct values for HardLeft, HardRight, Straight, etc.
  // Verify that the values for left are those that turn the vehicle to the let.
  // It allows us to establish the constants to map the feedback to the commanded position.
  
  // PWM signal is only effective from 127 to 255.
  // Some of the extreme values produce no motion in an unattached servo.
  // When the servo is atached to the steering link, pushing too hard can draw too much power and blow a fuse.
  
  // Perform this test first with an unattached servo.
  // Repeat with an attached servo.
  
 // It would be a good idea to put an ammeter on the steering servo to verify that excess current is not being drawn;
 // In case current gets too high, abort by unplugging the servo.
  // Use the serial monitor, and save the results for analysis.
  
  // Mechanically position the steering link so that servo motion half-way between extremes is straight ahead.

       delta = abs(delta);
       int i;
       Serial.println("Calibrate steering");
       if (SteerPosition >= Straight)
       { 
         for (i = Straight; i <= SteerPosition; i += delta)
             moveSteer(i);
         for (i = SteerPosition; i >= Straight; i -= delta)
             moveSteer(i);
       }
       else
      { 
         for (i = Straight; i >= SteerPosition; i -= delta)
             moveSteer(i);
         for (i = SteerPosition; i <= Straight; i += delta)
             moveSteer(i);

       }       
      moveSteer(Straight);    
    
}

#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;
int median_filter(int arr[])
{ // return the median of 5 numbers
  // on exit, the order of the five numbers is unspecified.
  // median filter has been tested and verified on 300 random arrays.
	register int temp;
	if (arr[0] > arr[1])
	{
		SWAP(arr[0],arr[1])
	}
	if (arr[1] > arr[2])
	{
		SWAP(arr[1],arr[2])
		if (arr[0] > arr[1])
		{
			SWAP(arr[0],arr[1])
		}
	}
	if (arr[4] < arr[3])
	{
		SWAP(arr[4],arr[3])	
	}
	if (arr[2] <= arr[3])
		return arr[2];	
	if (arr[4] <= arr[0])
		return arr[0];
	if (arr[0] < arr[3])
	{
		SWAP(arr[0],arr[3])	// arr[3] is the min
	}
	if (arr[2] > arr[4])
	{
		SWAP(arr[2],arr[4])	// arr[4] is the max
	}
	if (arr[0] >= arr[1])
		return arr[0];	
	if (arr[2] <= arr[1])
		return arr[2];
	return arr[1];
}
/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
        
        Halt();
        
        Instrument[Motor].EnablePin = EnableThrottle;
        Instrument[Motor].SignalPin = Throttle;
 //       Instrument[Motor].FeedbackPin = FeedbackMotor;
        Instrument[Motor].CurrentPin = Current36V;
        Instrument[Motor].StepSize = 20; // max from range of 1023.
        Instrument[Motor].CloseEnough = 1000;  // no feedback
        Instrument[Motor].Stuck = 3;
        Instrument[Motor].MinPosition = 85;  // anolog read range is 0 to 1023
        Instrument[Motor].MaxPosition = 800;
        
        
        Instrument[Steering].EnablePin = EnableSteer;
        Instrument[Steering].SignalPin = Steer;
        Instrument[Steering].FeedbackPin = SteerFB;
        Instrument[Steering].CurrentPin = CurrentSteer;
        Instrument[Steering].StepSize = 20;
        Instrument[Steering].CloseEnough = 1;
        Instrument[Steering].Stuck = 3;
        Instrument[Steering].MinPosition = 85;  // anolog read range is 0 to 1023
        Instrument[Steering].MaxPosition = 900;
        
        Instrument[Brakes].EnablePin = EnableBrake;
        Instrument[Brakes].SignalPin = DiskBrake;
        Instrument[Brakes].FeedbackPin = BrakeFB;
        Instrument[Brakes].CurrentPin = CurrentBrake;
        Instrument[Brakes].StepSize = 50;
        Instrument[Brakes].CloseEnough = 1;
        Instrument[Brakes].Stuck = 3;
        Instrument[Brakes].MinPosition = 85;  // anolog read range is 0 to 1023
        Instrument[Brakes].MaxPosition = 800;
      
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
        digitalWrite( StopLED, LOW);
        digitalWrite( CruiseLED, LOW);
        

}
/*---------------------------------------------------------------------------------------*/ 
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
  int center;
 
 /* 
  for (int i = 300; i> 0; i-=10)
  {
          CalibrateBrakes(i);
          delay(500);
  } */
  // With no load, there is no current limit on how fast the brakes can move.
  // If we go from NoBrake (207) to FullBrake (167) with a delay of 250 ms between commands,
  // maximum power draw is about 3.4 Amp; 
  // but actuator has only a 0.3 inch throw - it is not getting to the extremes
  // If we use delays of 500 ms, power draw is 2.5 A; throw is 0.5 inch.
  // With a 1 sec delay, draw is 2.2 A.
  // Full throw is 0.7 inch
  // 
  BrakeOn();
  delay (500);
  BrakeOff();
  delay (500);
  
  return;
  
  OutsideTime = EndTime - TimeNow;
  Instrument[Motor].Enabled = digitalRead(EnableThrottle);
  Instrument[Brakes].Enabled = digitalRead(EnableBrake);
  Instrument[Steering].Enabled = digitalRead(EnableSteer);

   // ServoData();
   
 

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

/*********************************************************** 
                        TEST ROUTINES
 ***********************************************************/
 
/*---------------------------------------------------------------------------------------*/ 
#ifdef TEST_MODE

//========================================================================
void BrakeWrite(int i, int Delay_ms)
{
     static int BrakeFBhistory[5];
     analogWrite(DiskBrake, i);
     for (int d = 0; d < Delay_ms; d++)
     {
       int feedback = analogRead(BrakeFB);
       BrakeFBhistory[4] = feedback;
//       int current =  analogRead(CurrentBrake);
       delay(1);
       feedback = median_filter(BrakeFBhistory);
       Serial.print(d); Serial.print(", ");
       Serial.print(i); Serial.print(", ");
       Serial.print(feedback); Serial.print(", ");
//       Serial.print(current); Serial.print(", ");
       Serial.println(millis());


    }
}
//========================================================================
void SteerWrite(int i, int Delay_ms)
{
     static int SteerFBhistory[5];
     analogWrite(Steer, i);
     for (int d = 0; d < Delay_ms; d++)
     {
      int feedback = analogRead(SteerFB);
      SteerFBhistory[4] = feedback;
 //      int current =  analogRead(CurrentSteer);
       feedback = median_filter(SteerFBhistory);
       Serial.print(d); Serial.print(", ");
       Serial.print(i); Serial.print(", ");
//       Serial.print(feedback); Serial.print(", ");
 //      Serial.print(current); Serial.print(", ");
       Serial.println(millis());

       delay(1);
       return;
    }
}//=========================================================================
void ServoData()
{
  int Delay_ms = 20;
  int incr = 2;
  int i;
  for (Delay_ms = 20; Delay_ms > 0; Delay_ms--)
  {
   for (i = 127; i < 255; i+= incr)
   {
      SteerWrite(i, Delay_ms);
      BrakeWrite(i, Delay_ms);  // brake works 4/18/13  // extent is 43 mm
   }
   for (i = 255; i >= 127; i-= incr)
   {
      SteerWrite(i, Delay_ms);
      BrakeWrite(i, Delay_ms);
   }
  } 
}


#endif  // TEST_MODE 

