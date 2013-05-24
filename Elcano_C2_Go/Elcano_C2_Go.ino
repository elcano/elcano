/*
 Elcano Contol Module C2: Instrument panel controls and LEDs.
 This routine handles simple drive-by-wire, as well as handing control over to the
 computer based on the state of the enable switches.
 */


#include <IO_PCB.h>

// inslude the SPI library:
#include <SPI.h>
#include <DACSPI.cpp>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#define MEG 1000000
//#define DEBUG 1
//#define DEBUG_M 2

volatile int SpeedCyclometer_degPs;  // 20 mph is about 2000 deg / sec
// Values to send over DAC
const int FullThrottle =  227;   // 3.63 V
const int MinimumThrottle = 70;  // Throttle has no effect until 1.2 V
// Values to send on PWM to get response of actuators
const int FullBrake = 167;  // start with a conservative value; could go as high as 255;  
const int NoBrake = 207; // start with a conservative value; could go as low as 127;
// Steering
const int HardLeft = 223; // was 159; // start with a conservative value; could go as high as 255;
const int HalfLeft = 223; // was 159;
const int Straight = 191;
const int HalfRight = 159; // was 223;
const int HardRight = 159; // was 223; // start with a conservative value; could go as low as 127;

// Values on position feedback
const int StraightFB = 501;
const int HalfRightFB = 303;
const int HalfLeftFB = 760;
const int TopSpeed_degPs = 1800;
const int FullBrakeFB = 375;  // Extrapolated 5/23/13
const int NoBrakeFB = 679;  // Observed 5/23/13

// Joystick limits
const int limitStickUp = 893;  // observed
const int limitStickDown = 100;  // observed 0; but make everything from 0 to 100 full brake.
const int limitStickLeft = 900;  // guess
const int limitStickRight = 100; // guess

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
const unsigned long MinTimeStep_usec = 10000;
unsigned long loopTime_usec;
class _Instruments
{
public:
  int StickMoved;  // bool
  int Joystick;   // position of stick
  int LastCommand;  // last commanded (0-255)
  int LastFeedback;  // (0-1023)
  unsigned long LastTime_usec;
  int MinPosition;  // (0-255)
  int MaxPosition;  // (0-255)
  int Feedback;   // actual position, 0-1023
  int maxSlew;  // usec / FBcount; about 1000 for brakes and 500 for steer
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
  // The steering and brakes use the unidirectional CS758lcb050U
  // The 36V supply uses a bidirectional chip
  // Thus the red connectors that plug into the joystick/motor control box can be inserted either
  // way. The software must handle a positive and negative current the same way.
 
  //This goes TO a position, if the distance is not too far.
  //Returns the actual sensed position
  int go(int desired_position)
  // desired_position is a 0-255 PWM or analog voltage input.
  // The returned value is 0-1023 position feedback or SpeedCyclometer_degPs
  // The returrned value is also stored in Feedback.
  {
    unsigned long TimeNow_usec;
    desired_position = min(desired_position, MaxPosition);
    desired_position = max(desired_position, MinPosition);
    int Prudent_position;
    int Prudent_feedback;
    int desired_feedback = predictFB(desired_position);
    //  motor is handled differently
    if (FeedbackPin ==  SteerFB || FeedbackPin ==  BrakeFB)
        Feedback = filteredFB();  // 0 to 1023
    else
        Feedback = SpeedCyclometer_degPs;
/*    
    if (Feedback == desired_feedback ||
        (Feedback <= MinPosition && desired_feedback <= MinPosition) ||
        (Feedback >= MaxPosition && desired_feedback >= MaxPosition))
    {   // we have arrived at desired_position
        return(Feedback);
    }
    */
    TimeNow_usec = micros();
    if (TimeNow_usec - LastTime_usec < MinTimeStep_usec)
    {   // not enough time has passed since the last command.
        return(Feedback);
    }
    //  read current
/*    CurrentDraw = abs( analogRead(CurrentPin) - QuiescentCurrent);
    if (CurrentDraw >= CurrentLimit)
    {
     //    Too much current; Pause movement
       if (Feedback == LastFeedback)
       {
           // we are pushing against something; ease back.
           // Are we closer to the high end or the low end?
           if ((FeedbackPin ==  SteerFB || FeedbackPin ==  BrakeFB) &&
               abs(LastCommand - MaxPosition) > abs(LastCommand - MinPosition))
           {   // close to low end
               Prudent_feedback = (Feedback * 11) / 10; // icrease by 10%
           }
           else
           {   // motor or close to high end
               Prudent_feedback = (Feedback * 9) / 10; // cut back to 90%
           }
           Prudent_position = predictPWM( Prudent_feedback);  
       }
       else
           Prudent_position = predictPWM(Feedback);
       analogWrite(SignalPin, Prudent_position);
       LastCommand = Prudent_position;
    }
    else
    {
       if ((TimeNow_usec - LastTime_usec) > maxSlew * abs(LastFeedback - Feedback) ||
           (loopTime_usec > maxSlew * abs(desired_feedback - Feedback)))
       {
          // moving too fast; slow down
            Prudent_feedback = desired_feedback > Feedback?
            Feedback + loopTime_usec / maxSlew:
            Feedback - loopTime_usec / maxSlew; 
            Prudent_position = predictPWM( Prudent_feedback);  
            analogWrite(SignalPin, Prudent_position);
            LastCommand = Prudent_position;
       }
       else  // go for it
       {
*/ 
        //  motor is handled differently
        if (FeedbackPin ==  SteerFB || FeedbackPin ==  BrakeFB)
            analogWrite(SignalPin, desired_position);
        else
            DAC_Write(0, desired_position);
        
            LastCommand = desired_position;
//       }
//    }

#ifdef DEBUG_M
    //debug prints. 
//    Serial.print(desired_position); 
//    Serial.print(", ");
//    Serial.print(Feedback); 
//    Serial.print(", ");
//    Serial.print(desired_position); 
//    Serial.print(", ");
//    Serial.println(TimeNow_usec);
#endif
    LastTime_usec = TimeNow_usec;
    LastFeedback = Feedback;

    return(Feedback);
  }
  // first cut; more sophisticated version would use hysteresis from direction of motion
  int predictFB (int pwm)
  { 
     if (FeedbackPin ==  SteerFB)
     {  //  for steering
       long int product = 714 * pwm - 86300;
       return (product / 100);
     }
     else if (FeedbackPin == BrakeFB)
     {    // to do: case for brakes
       long int product = 714 * pwm - 86300;
       return (product / 100);
     }
     else  // motor; correlate voltage 
     {
         int Speed = (TopSpeed_degPs * (pwm - MinimumThrottle)) / (FullThrottle - MinimumThrottle);
         Speed = max(0, Speed);
         Speed = min(Speed, TopSpeed_degPs);
         return Speed;
     }
     // never get here
     return (511);
  }
  int predictPWM (int fb)
  {  // This works only for steering
     if (FeedbackPin ==  SteerFB)
     {
         long int product = fb * 100 + 86300;
         return (product / 714);
     }
     else if (FeedbackPin == BrakeFB)
     {    // to do: case for brakes
         long int product = fb * 100 + 86300;
         return (product / 714);
     }
     else  // motor; correlate voltage 
     {
         int dac = FullThrottle * (TopSpeed_degPs - fb) * TopSpeed_degPs;
         dac = max(MinimumThrottle, dac);
         dac = min(dac, FullThrottle);
         return dac;
     }
    return 127;
  }
  int filteredFB()
  // side effect: Delay of up tp 1 msec
  {
       int feedbacks[5];
       // feedback signal may have glitches; use of a 5 point median filter is recommended.
       for (int i = 0; i < 5; i++)
       {
           feedbacks[i] = analogRead(FeedbackPin);
           delayMicroseconds(100);             // analogRead takes 100 usec
       }
       int feedback = median_filter(feedbacks);
#ifdef DEBUG_M
       Serial.print("Feedback: ");
       Serial.print(feedback); Serial.print(", ");
#endif
       return feedback;
  }
} Instrument[3];
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
void initialize()
{

  Instrument[Motor].SignalPin = Throttle;  // DEPRICATED: Use MOSI
  Instrument[Motor].FeedbackPin = 0; // Motor feedback is wheel speed
  Instrument[Motor].CurrentPin = Current36V;
  Instrument[Motor].StepSize = 20; // max from range of 1023.
  Instrument[Motor].CloseEnough = 100;  
  Instrument[Motor].Stuck = 3;
  Instrument[Motor].MinPosition = MinimumThrottle;  // position is 0-255
  Instrument[Motor].MaxPosition = FullThrottle;

  Instrument[Steering].SignalPin = Steer;
  Instrument[Steering].FeedbackPin = SteerFB;
  Instrument[Steering].CurrentPin = CurrentSteer;
  Instrument[Steering].StepSize = 20;
  Instrument[Steering].CloseEnough = 20;
  Instrument[Steering].Stuck = 3;
  Instrument[Steering].MinPosition = HardRight;  // position is PWM: 127 to 255
  Instrument[Steering].MaxPosition = HardLeft;
  Instrument[Steering].maxSlew = 500;   // usec / FBcount
  
  Instrument[Brakes].SignalPin = DiskBrake;
  Instrument[Brakes].FeedbackPin = BrakeFB;
  Instrument[Brakes].CurrentPin = CurrentBrake;
  Instrument[Brakes].StepSize = 50;
  Instrument[Brakes].CloseEnough = 20;
  Instrument[Brakes].Stuck = 3;
  Instrument[Brakes].MinPosition = FullBrake;  // position is PWM: 127 to 255
  Instrument[Brakes].MaxPosition = NoBrake;
  Instrument[Brakes].maxSlew = 1000;  // usec / FBcount

  //Load current positions from the actual sensors
  Instrument[Motor].Feedback =
    analogRead(Instrument[Motor].FeedbackPin);
  Instrument[Steering].Feedback =
    analogRead(Instrument[Steering].FeedbackPin);
  Instrument[Brakes].Feedback =
   analogRead(Instrument[Brakes].FeedbackPin);

  // assume that vehicle always stops with brakes on and wheels straight.
  // Otherwise, there is a possibility of blowing a fuse if servos try to
  // get to this state too quickly.
  Instrument[Steering].LastCommand = Straight;

  Instrument[Motor].Joystick = MinimumThrottle;
  Instrument[Brakes].Joystick = FullBrake;
  Instrument[Steering].Joystick = Straight;
  Instrument[Motor].CurrentLimit = 230;  // about 20A
  Instrument[Brakes].CurrentLimit = 55;  // about 5A
  Instrument[Steering].CurrentLimit = 55;
  long counter = 0;
  /*
   do
   { 
      Instrument[Motor].go(MinimumThrottle);
      Instrument[Brakes].go(FullBrake);
      Instrument[Steering].go(Straight);
      counter++;
   } while ((abs( Instrument[Steering].Feedback - StraightFB) > Instrument[Steering].CloseEnough
         || abs( Instrument[Brakes].Feedback - FullBrakeFB) > Instrument[Brakes].CloseEnough
         || abs( Instrument[Motor].Feedback - 0) > Instrument[Motor].CloseEnough)
         && counter < 10000);
 */
  // Read quiesent state of current sensors.
  Instrument[Brakes].CurrentDraw =
    Instrument[Brakes].QuiescentCurrent = analogRead(CurrentBrake);
  Instrument[Steering].CurrentDraw =
    Instrument[Steering].QuiescentCurrent = analogRead(CurrentSteer);
  Instrument[Motor].CurrentDraw =
    Instrument[Motor].QuiescentCurrent = analogRead(Current36V);
 
}
/*---------------------------------------------------------------------------------------*/
void setup()
{
  //Set up pin modes and interrupts, call serial.begin and call initialize.
  Serial.begin(9600);
  
  // SPI: set the slaveSelectPin as an output:
  pinMode (SelectAB, OUTPUT);
  pinMode (SelectCD, OUTPUT);
  pinMode (10, OUTPUT);
  SPI.setDataMode( SPI_MODE0);
  SPI.setBitOrder( MSBFIRST);
  // initialize SPI:
  // The following line should not be neccessary. It uses a system library.
  PRR0 &= ~4;  // turn off PRR0.PRSPI bit so power isn't off
  SPI.begin(); 
  for (int i = 0; i < 4; i++)
      DAC_Write (i, 0);   // reset did not clear previous states
   attachInterrupt(1, WheelRev, RISING);
 
//  Serial.println(SS);
//  Serial.println(MOSI);
//  Serial.println(MISO);
//  Serial.println(SCK); 
//  Serial.println(SPSR);
//  Serial.println(SPCR);
//  Serial.println(DDRB);
// Serial.println("SPSR=%x SPCR=%x\n", SPSR, SPCR); 

        pinMode(Throttle, OUTPUT);
        pinMode(DiskBrake, OUTPUT);
        pinMode(Steer, OUTPUT);
        pinMode(AccelerateJoystick, INPUT);
        pinMode(SteerJoystick, INPUT);
        pinMode(JoystickCenter, INPUT);
        pinMode(Current36V, INPUT);
        pinMode(CurrentBrake, INPUT);
        pinMode(CurrentSteer, INPUT);
        pinMode(BrakeFB, INPUT);
        pinMode(SteerFB, INPUT);
        Serial.println("Start initialization");        
        initialize();
     
//  twitch moves Motor, Brakes and Steering.  5/23/13  TCF        
//       twitch();
     BrakeOff();
     Serial.println("Initialized");
     
}
void twitch()
{

  CalibrateBrakes(500);
  CalibrateSteering(HardRight,10);     
  CalibrateSteering(HardLeft,10);
    DACRamp(200);
}
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/ 
// WheelRev is called by an interrupt. 
static unsigned long OldTick = 0;
    
void WheelRev()
{
    unsigned long TickTime;
    unsigned long WheelRevMicros;
    TickTime = micros();
    if (OldTick == TickTime)
        return;
    if (OldTick <= TickTime)
    {
      	WheelRevMicros = TickTime - OldTick;
        if (WheelRevMicros < 100)
            return;  // switch bounce
    }

    else // overflow
      	WheelRevMicros = TickTime + ~OldTick;
    SpeedCyclometer_degPs = (360 * MEG) / WheelRevMicros;
    OldTick = TickTime;
}
void NoWheelRev()  // let the speed slow down if there has been no motion
{
    unsigned long NoTickTime;
    unsigned long WheelRevMicros;
    unsigned long EntryOldTick = OldTick;
    NoTickTime = micros();
    if (OldTick == NoTickTime)
        return;
    if (OldTick > NoTickTime)
    {
        OldTick = 0;  // overflow
        return;
    }
    unsigned long TimeToNextClick = OldTick + SpeedCyclometer_degPs * MEG / 360;
    if (NoTickTime < TimeToNextClick)
        return;
    WheelRevMicros = NoTickTime - OldTick;
    // To do: lock out interrupts at this point
    if (EntryOldTick == OldTick)
        SpeedCyclometer_degPs = (360 * MEG) / WheelRevMicros;
 
}

/*---------------------------------------------------------------------------------------*/
void DACRamp(int pause)
{
/* 
Use SPI interface to output signal to motor.

Initial test looked at the voltage on this pin.  
May 10, 2013 TCF: Observed ramp on all four DAC output pins.
On PCB #1
Ramp on DAC channel A (IC2 pin 8) is seen on DB15 X3-12
Channel A is the output to motor, and is the only DAC channel currently used.
Ramp on DAC channel B (IC2 pin 6) is seen on DB15 X3-13
Ramp on DAC channel D (IC3 pin 6) is seen on DB9M X5-3

No ramp is seen on DB9F X4-3.  There is no continuity between IC3 pin 8 and X4-3.

We attached the DB15 cable and drove the motor from PCB #1

The DB-25 connector from Cruise supplies the wheel revolution click.  Since PCB #1 and #2 have
reversed genders, the present wiring only plugs into PCB #2. When PCB #2 is used, there is no
signal on IC2 pin 8.
   
*/
 // Only use motor control:
  int channel = 0;
  { 
      /* Observed behavior with manual throttle, no load (May 10, 2013, TCF)
      0.831 V at rest       52 counts
      1.20 V: nothing       75
      1.27 V: just starting 79
      1.40 V: slow, steady  87
      1.50 V: brisker       94
      3.63 V: max          227 counts
      
      255 counts = 4.08 V
      
      */
      // change the voltage on this channel from min to max:
      for (int level = MinimumThrottle; level < FullThrottle/2; level+=4) 
      {
        DAC_Write(channel, level);
        Serial.print(SpeedCyclometer_degPs); Serial.print(", ");
        Serial.println(level);
        delay(pause);
      }
      // wait at the top:
      delay(pause);
      // change the voltage on this channel from max to min:
      for (int level = FullThrottle/2; level > MinimumThrottle; level-=4) 
      {
        DAC_Write(channel, level);
        Serial.print(SpeedCyclometer_degPs); Serial.print(", ");
        Serial.println(level);
        delay(pause);
      }
 }    

}
/*---------------------------------------------------------------------------------------*/

void DAC_Write(int address, int value)
// address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
// value: digital value converted to analog voltage
// Output goes to mcp 4802 DAC
// No input
/*
REGISTER 5-3: WRITE COMMAND REGISTER FOR MCP4802 (8-BIT DAC)
A/B  —  GA  SHDN  D7 D6 D5 D4 D3 D2 D1 D0 x x x x
bit 15                                       bit 0

bit 15   A/B: DACA or DACB Selection bit
         1 = Write to DACB
         0 = Write to DACA
bit 14   — Don’t Care
bit 13   GA: Output Gain Selection bit
         1 = 1x (VOUT = VREF * D/4096)
         0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
bit 12   SHDN: Output Shutdown Control bit
         1 = Active mode operation. VOUT is available. 
         0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
         VOUT pin is connected to 500 k (typical)
bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.


With 4.95 V on Vcc, observed output for 255 is 4.08V.
This is as documented; with gain of 2, maximum output is 2 * Vref

*/

{
  int byte1 = ((value & 0xF0)>>4) | 0x10; // acitve mode, bits D7-D4
  int byte2 = (value & 0x0F)<<4;           // D3-D0
  if (address < 2)
  {
      // take the SS pin low to select the chip:
      digitalWrite(SelectAB,LOW);
      if (address >= 0)
      { 
        if (address == 1)
          byte1 |= 0x80;  // second channnel
        SPI.transfer(byte1);
        SPI.transfer(byte2);
 //       Serial.print(byte1);  Serial.print(", ");
 //       Serial.println(byte2);
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
          byte1 |= 0x80;  // second channnel
        SPI.transfer(byte1);
        SPI.transfer(byte2);
      }
       // take the SS pin high to de-select the chip:
      digitalWrite(SelectCD,HIGH);
  }
}

/*---------------------------------------------------------------------------------------*/
void loop()
{
  static unsigned long time_us;
  unsigned long timeNow_us = micros();
  if (timeNow_us < time_us)
  {      // roll-over
    time_us = 0;
    if (timeNow_us < MinTimeStep_usec)
      return;
  }
  unsigned long ElapsedTime_us = timeNow_us - time_us;
  if (ElapsedTime_us < MinTimeStep_usec)
    return;
  loopTime_usec = ElapsedTime_us;

  JoystickMotion();
  // apply throttle
  int desired_position =  MinimumThrottle;
  if (Instrument[Motor].StickMoved)
  {
     desired_position = Instrument[Motor].Joystick;
     Serial.print(" FB: "); Serial.print(Instrument[Motor].Feedback);
     Serial.print(" Motor: "); Serial.print(desired_position);
  }
  Instrument[Motor].go(desired_position);
  NoWheelRev();  // let speed slow down
 
  // apply brakes
  desired_position =  NoBrake;
  if (Instrument[Brakes].StickMoved)
  {
     desired_position = Instrument[Brakes].Joystick;
     Serial.print(" FB: "); Serial.print(Instrument[Brakes].Feedback);
     Serial.print(" Brakes: "); Serial.print(desired_position);
  }
  Instrument[Brakes].go(desired_position);

  
 // apply steering
  desired_position =  Straight;
  if (Instrument[Steering].StickMoved)
  {
     desired_position = Instrument[Steering].Joystick;
     Serial.print(" FB: "); Serial.print(Instrument[Steering].Feedback);
     Serial.print(" Steering: "); Serial.print(desired_position);
  }
  Instrument[Steering].go(desired_position); 
  if (Instrument[Motor].StickMoved || Instrument[Brakes].StickMoved ||
     Instrument[Steering].StickMoved)
     Serial.println(" ");
}
/*---------------------------------------------------------------------------------------*/
void JoystickMotion()
// All inputs are 0-1023
{
    long int x;
    int upStick = analogRead(AccelerateJoystick);
    int center  = analogRead(JoystickCenter);
    int sideStick = analogRead(SteerJoystick);
    if (center < 420 || center > 460)
    {
      Serial.print(" Center: "); Serial.print(center);
      Serial.print(" Up: "); Serial.print(upStick);
      Serial.print(" Side: "); Serial.println(sideStick);
      // Joystick may have lost power
      Instrument[Motor].Joystick = MinimumThrottle;
      Instrument[Motor].StickMoved = FALSE;
      Instrument[Steering].Joystick = Straight;
      Instrument[Steering].StickMoved =  FALSE;
      Instrument[Brakes].Joystick = NoBrake;
      Instrument[Brakes].StickMoved =  FALSE;
      return; 
   }
    const int deadBandLow = 80;  // out of 1023
    const int deadBandHigh = 80;
     //Calculate deadband info
     int tinyDown = center - deadBandLow;
     int tinyUp = center + deadBandHigh;
     
       //If stick did not move more than the "up"    deadband.
  if (upStick <= tinyUp)  
  {
    //stick not outside deadBand
    Instrument[Motor].Joystick = 0;
    Instrument[Motor].StickMoved = FALSE;
  }
  else
  {
    //stick outside of deadband
    if (upStick > limitStickUp)
        upStick = limitStickUp;
    x = (upStick - tinyUp);
    x *= (FullThrottle - MinimumThrottle);
    x /= (limitStickUp - tinyUp);
    x  += MinimumThrottle;
    Instrument[Motor].Joystick = x;
    Instrument[Motor].StickMoved =  TRUE;
#ifdef DEBUG_M
    Serial.print(" upStick: "); Serial.print(upStick);
    Serial.print(" tinyUp: "); Serial.print(tinyUp);
    Serial.print(" Min: "); Serial.print(MinimumThrottle);
    Serial.print(" Full: "); Serial.print(FullThrottle);
    Serial.print(" x: "); Serial.print(x);
#endif
  }

  //Check if Stick moved beyond "down" deadBand
  if (upStick >= tinyDown)
  {
    //Stick still in deadband
    Instrument[Brakes].Joystick = NoBrake;
    Instrument[Brakes].StickMoved =  FALSE;
  }
  else
  {
    if (upStick < limitStickDown)
        upStick = limitStickDown;
    x = (tinyDown - upStick);
    x *= (NoBrake - FullBrake);  // NoBrake = 207; FullBrake = 167
    x /= (tinyDown - limitStickDown);
    Instrument[Brakes].Joystick = NoBrake - x;
    Instrument[Brakes].StickMoved =  TRUE;
#ifdef DEBUG
    Serial.print(" upStick: "); Serial.print(upStick);
    Serial.print(" tinyDown: "); Serial.print(tinyDown);
    Serial.print(" center: "); Serial.print(center);
    Serial.print(" Min: "); Serial.print(NoBrake);
    Serial.print(" Full: "); Serial.print(FullBrake);
    Serial.print(" x: "); Serial.print(x);
#endif
  }

  //read the steering pot in the joystick
  if (sideStick > tinyDown && sideStick <= tinyUp)
  {  //  The stick is still in the left-right deadband
    Instrument[Steering].Joystick = Straight;
    Instrument[Steering].StickMoved =  FALSE;
  }
  else if (sideStick > tinyUp)
  {
    //Stick moved past "up" deadband limit
    if (sideStick > limitStickLeft)
        sideStick = limitStickLeft;
    x = (sideStick - tinyUp);  // positive
    x *= (HardLeft - Straight);  // positive
    x /= (limitStickLeft - tinyUp);  // x is positive
    Instrument[Steering].Joystick = Straight + x;   
    Instrument[Steering].StickMoved =  TRUE;
  }
  else
  {
    if (sideStick < limitStickRight)
        sideStick = limitStickRight;
    x = (tinyDown - sideStick);  // positive
    x *= (Straight - HardRight);  // positive
    x /= (tinyDown - limitStickRight);  // x is positive
    Instrument[Steering].Joystick = Straight - x;
#ifdef DEBUG
    Serial.print(" sideStick: "); Serial.print(sideStick);
    Serial.print(" tinyDown: "); Serial.print(tinyDown);
    Serial.print(" HardRight: "); Serial.print(HardRight);
    Serial.print(" limitStickRight: "); Serial.print(limitStickRight);
    Serial.print(" x: "); Serial.print(x); 
#endif  
    Instrument[Steering].StickMoved = TRUE;
  }
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



