/*
  This is a test routine for Elcano.
  It runs on an Arduino Mega 2560 with an Adafruit datalogger shield.
  
  It is based on code by Tom Igoe to use SD card and serial monitor.
  
 The circuit:
 Uses enable switches, joystick, current sensors, and PWM outputs for brakes, steering and motor as
 given in IO_Mega.h

 * SD card attached to SPI bus as follows:
 ** CS - pin 10
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 
 Original SD code created  24 Nov 2010; updated 2 Dec 2010 by Tom Igoe
 
 Adapted 21-29 Feb 2012 by Tyler Folsom
 
 This example code is in the public domain.
 	 
 Status as of 3/13/12:
 Throttle works and is displayed on the 8 LEDs on the front panel.
 All front panel LEDs work except for stopLED.
 Four front panel switches: power and enabling motor, brakes and steering work.
 SD card works, but is sometimes tempermental about displaying on a PC.
 Serial monitor works.
 RTC works with Duemillenove, but RTC hardware does not work on the Mega; use software RTC, which works.
 The throttle controls the servos for steering or brakes.
 Brake servo is physically connected to operate brakes.
 Steering servo has been disconnected from steering linkage. There have been problems 
 with drawing more than 5 amps and blowing the fuse. 
 Traction motor responds to throttle.
 The joystick now works.
 Quiescent current sensors work, though sometimes are zeroed incorrectly.  Increasing current is 
 recognized correctly.
 Traction motor working.
 Joystick is installed and working.
 Quiescent current calculation works on brake and steer; motor current seems high.
 Steering servo works. Steering limits have been set and linkage reattached.
 Wrote routines to ease back on servo requests when too much current is drawn.
 Phsically put everything back together.
 Test all systems on test stand.

 To Do:
 Joystick Center is not read properly. See 2.38V at joystick for center; that voltage does not make it
 to analog input pin 8; continuity problem? Problem bypassed by setting a wide deadband.
 Motor gives jerky response at low speeds; need to smooth it out.
 Brake pusher rod sometimes slips off lever.
 Do a road test.
 Shoot a video and post it.
 */
//#define MEGA
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
//#ifdef MEGA
#include <IO_Mega.h>
//#else
//#include <IO_2009.h> // conflict with chip select
//#endif
#include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

// Date and time functions using just software, based on millis() & timer

#include <Wire.h>
#include "RTClib.h"

RTC_Millis RTC;

#define FILE_NAME "DataLog.csv"
File dataFile;
struct servo
{
  int QuiescentCurrent; // Background reading of current sensor; counts of analog in: 0 to 1023
  int Current;  // counts of analog in - Quiescent: 0 to 1023
  int mAmp;       // current in milliAmp
  int CurrentSum;  // used to recompute QuiescentCurrent
  int OutputChannel;  // a digital PWM pin number
  int OutputValue;    // counts of PWM out: 0 to 255
  int StartPosition;  // servo is assumed to start here.
  int MaxSlew;   // maximum that OutputValue can change in a second.
  int Stick;    // Joystick reading for motor or throttle; center for brakes
  int EnableSwitch;  // HIGH for cruise or LOW for joystick
};
servo Servos[3];

int QuiescentSamples = 0;
String dataString = String("                                                  ");


const int MOTOR = 0;
const int STEER = 1;
const int BRAKE = 2;
/* Steering servo can handle 44 counts per second;
   Brake servo is good for 88 counts per second.
*/
const int MaxBrakeSlew = 88;  // counts / sec
const int MaxSteerSlew = 44;  // counts / sec
const int LoopPeriod = 100;  // msec
/* The mechanical throttle outputs a voltage between 0.84V (32 counts)
   and 3.55 V (181 counts).
   The motor does not respond until about 1.30 V (66 counts)
*/
const int MaxMotorChange = (70*LoopPeriod)/1000;  // counts per step for motor
const int MaxBrakeChange = (MaxBrakeSlew*LoopPeriod)/1000; // counts per step
const int MaxSteerChange = (MaxSteerSlew*LoopPeriod)/1000;
const int MotorFull = 186;   // fully on for PWM signals
const int MotorMin  = 66;
const int MotorOff = 0;
const int FullThrottle =  179;   // 3.5 V
const int MinimumThrottle = 39;  // Throttle has no effect until 0.75 V
const int BrakeOn = 127;  
const int BrakeOff = 255;
const int HardLeft = 255;
const int HalfLeft = 223;
const int Straight = 191;
const int HalfRight = 159;
const int HardRight = 128;
/*  Servo range is 50 mm for brake, 100 mm for steering.
    Servo is fully retracted on a pulse width of 2 ms;
    fully extended at 1 ms and centered at 1.5 ms.
    There is a deadband of 8 us.
    At 12v, servo operating speed is 56mm/s with no load or
    35 mm/s at maximum load.
    
    The Arduino PWM signal is at 490 Hz or 2.04 ms.
    0 is always off; 255 always on. One step is 7.92 us.
    
*/

/*---------------------------------------------------------------------------------------*/ 
// Bring the vehicle to a stop.
// Stop may be called from an interrupt or from the loop.
void Halt()
{
   analogWrite(TractionMotor, MotorOff);
   analogWrite(DiskBrake, BrakeOn);
   Servos[MOTOR].OutputValue = MotorOff;
   Servos[BRAKE].OutputValue = BrakeOn;
   digitalWrite(StopLED, HIGH);
}
/*---------------------------------------------------------------------------------------*/ 

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing throttle SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);
  pinMode(53, OUTPUT);  // Unused CS on Mega
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.\n");

  // following line sets the RTC to the date & time this sketch was compiled
  RTC.begin(DateTime(__DATE__, __TIME__));
     DateTime now = RTC.now();
  
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
  pinMode(EnableMotor, INPUT);
  pinMode(EnableBrake, INPUT);
  pinMode(EnableSteer, INPUT);
  pinMode(Steer, OUTPUT);
  pinMode(DiskBrake, OUTPUT);
  pinMode(TractionMotor, OUTPUT);
  
  Halt();
  delay(1000);
  analogWrite( Steer, Straight);
  Servos[STEER].OutputValue = Straight;
  BackgroundCurrent();
}
void RecomputeQuiescent()
{
  // all switches off means recompute Quiescent currents
  // assume no load on servos when all switches are off
  int n;
  for (n = 0; n < 3; n++)
  {
    Servos[n].CurrentSum +=  analogRead(n);
  }
  if (++QuiescentSamples == 20)
  {   
    for (n = 0; n < 3; n++)
    {
      Servos[n].QuiescentCurrent = Servos[n].CurrentSum / QuiescentSamples;
      Servos[n].CurrentSum = 0;
    }
    QuiescentSamples = 0;
   }
}

void BackgroundCurrent()
{
  int i,n;
  const int samples = 20;
  // wait for any power-up servo motion to stop; 
  delay(1000);
   for (n = 0; n < 3; n++) 
   { 
      Servos[n].QuiescentCurrent =  0;
      Servos[n].CurrentSum = 0;
   }
  for (i=0; i < samples; i++)
  {
      RecomputeQuiescent();
      delay(100);
  }
   for (n = 0; n < 3; n++) 
   { 
      Servos[n].QuiescentCurrent /= samples;
      Serial.print(Servos[n].QuiescentCurrent, DEC);
      Serial.print(',');
   }
   Serial.print('\n');
}
void SetThrottle(int throttle) // -512 to 511
{
  int ChangeMotor = 0;
  int ChangeBrake = 0;
  int TargetMotor = MotorOff;  // 0
  int TargetBrake = BrakeOff;  // 255
  if (throttle < -84)
  {
     TargetMotor = MotorOff;
     TargetBrake = BrakeOff + (throttle + 84) / 3;
  }
  else if (throttle < 45)
  {  // deadband
     TargetMotor = MotorOff;
     TargetBrake = BrakeOff;
  }
  else if (throttle < 111)
  {  // deadband
     TargetMotor = throttle-45;
     TargetBrake = BrakeOff;
  }
  else 
  {
    TargetBrake = BrakeOff;
    TargetMotor = MotorMin + (throttle - 111) / 3;
  }
  ChangeBrake = TargetBrake - Servos[BRAKE].OutputValue;
  /* if (Servos[BRAKE].mAmp > 4700)
  { // ease back to keep from blowing fuse
      ChangeBrake = (ChangeBrake > 0)? -2: 2;
  }
  else if (Servos[BRAKE].mAmp > 3000 && (ChangeBrake > 2 || ChangeBrake < -2))
  {
      ChangeBrake = (ChangeBrake>0)? 2: -2;
  }
  else */
  if (ChangeBrake > MaxBrakeChange)
  {
      ChangeBrake = MaxBrakeChange;
  }
  else if (ChangeBrake < -MaxBrakeChange)
  {
      ChangeBrake = -MaxBrakeChange;
  }
  ChangeMotor = TargetMotor - Servos[MOTOR].OutputValue;
  int EffectiveMotorChange = ChangeMotor > 0?
      min(TargetMotor, MotorFull) - max(MotorMin, Servos[MOTOR].OutputValue):
      min(Servos[MOTOR].OutputValue, MotorFull) - max(MotorMin, TargetMotor);
  if (EffectiveMotorChange > MaxMotorChange)
  {
      ChangeMotor = MaxMotorChange;
  }
  else if (EffectiveMotorChange < -MaxMotorChange)
  {
      ChangeMotor = -MaxMotorChange;
  }
  Servos[BRAKE].OutputValue += ChangeBrake;
  Servos[BRAKE].OutputValue = max(BrakeOn /*128 */, Servos[BRAKE].OutputValue);
  Servos[BRAKE].OutputValue = min(BrakeOff /* 255 */, Servos[BRAKE].OutputValue);
  analogWrite(DiskBrake, Servos[BRAKE].OutputValue);
  Servos[MOTOR].OutputValue += ChangeMotor;
  Servos[MOTOR].OutputValue = max(MotorOff /* 0 */, Servos[MOTOR].OutputValue);
  Servos[MOTOR].OutputValue = min(MotorFull /* 255 */, Servos[MOTOR].OutputValue);
  analogWrite(TractionMotor, Servos[MOTOR].OutputValue);
}
void SetSteer(int steer)
{ // Joystick has central deadband, flanked by fine control, then coarse control
  int Change;
  int TargetSteer = Straight;
  if (steer < -468) // && not too fast)  // TODO when speed is available.
  {
      TargetSteer = HalfLeft /* 223 */ - (steer + 468);
  }
  else if (steer < -84)
  {
      TargetSteer = Straight - (steer + 84) / 12;
  }
  else if (steer < 84)
  {
      TargetSteer = Straight;  // 191
  }
  else if (steer < 468)
  {
      TargetSteer = Straight - (steer - 84) / 12;
  }
  else
  {
      TargetSteer = HalfRight /* 159 */ - (steer - 468);
  }
  Change = TargetSteer - Servos[STEER].OutputValue;
/*  if (Servos[STEER].mAmp > 4700)
  { // ease back to keep from blowing fuse
      if (Servos[STEER].OutputValue < HalfRight)
        Change = 2;
      else if (Servos[STEER].OutputValue > HalfLeft)
        Change = -2;
      else
        Change = 0;
  }
  else if (Servos[STEER].mAmp > 3000 && (Change > 2 || Change < -2))
  {
      Change = (Change>0)? 2: -2;
  }
  else */
  if (Change > MaxSteerChange)
  {
      Change = MaxSteerChange;
  }
  else if (Change < -MaxSteerChange)
  {
      Change = -MaxSteerChange;
  }
  Servos[STEER].OutputValue += Change;
  Servos[STEER].OutputValue = max(HardRight /*128 */, Servos[STEER].OutputValue);
  Servos[STEER].OutputValue = min(HardLeft /* 255 */, Servos[STEER].OutputValue);
  analogWrite(Steer, Servos[STEER].OutputValue);
}
void formDataString()
{
  // now log the information
  // make a string for assembling the data to log:
  // !!! Original version fails when string > 13 characters !!! TCF 2/21/12
  // String dataString = "";  // ORIGINAL
  dataString = String("                                                  ");
//  dataString += String(i);  // ORIGINAL
  dataString = String(QuiescentSamples);  
  dataString += ","; 
    
  DateTime now = RTC.now();
  dataString += String(now.hour());
  dataString += ","; 
  dataString += String(now.minute());
  dataString += ","; 
  dataString += String(now.second());
  dataString += ","; 
  dataString += String(now.day());
  dataString += ","; 
  dataString += String(now.month());
  dataString += ","; 
  dataString += String(now.year());
  dataString += ","; 

  for (int n = 0; n < 3; n++)
  {
    dataString += String(Servos[n].Stick);
    dataString += (Servos[n].EnableSwitch==HIGH? ",1,": ",0,");
    dataString += String(Servos[n].OutputValue);
    dataString += ","; 
    dataString += String(Servos[n].Current);
    dataString += ", "; 
    dataString += String(Servos[n].mAmp);
    dataString += ","; 
    dataString += String(Servos[n].QuiescentCurrent);
    dataString += ", "; 
    dataString += String(Servos[n].CurrentSum);
    dataString += ","; 
  }
}

void loop()
{
  unsigned long time = millis();
  unsigned long endTime = time + LoopPeriod;

// Read joy stick and 3 switches; Send stick to enabled servos.
  int  n;
  readJoystick();
  if (!Servos[MOTOR].EnableSwitch && !Servos[STEER].EnableSwitch && !Servos[BRAKE].EnableSwitch)
  {
    // all switches off means recompute Quiescent currents
    // assume no load on servos when all switches are off
    analogWrite(TractionMotor, Servos[MOTOR].OutputValue);
    analogWrite(Steer, Servos[STEER].OutputValue);
    analogWrite(DiskBrake, Servos[BRAKE].OutputValue);
    RecomputeQuiescent();
  }
  else
  {
    SetThrottle(Servos[MOTOR].Stick);
    SetSteer(Servos[STEER].Stick);    
  }
  if (Servos[STEER].EnableSwitch == HIGH)
  {
    Display (Servos[STEER].OutputValue);
  }
  else if (Servos[BRAKE].EnableSwitch == HIGH)
  {
     Display (Servos[BRAKE].OutputValue);
  }
  else 
     Display (Servos[MOTOR].OutputValue);
     
  formDataString();

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open(FILE_NAME, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) 
  {
      dataFile.println(dataString);
      dataFile.close();
      if (Servos[MOTOR].EnableSwitch || Servos[STEER].EnableSwitch || Servos[BRAKE].EnableSwitch)
      {
        // print to the serial port too:
        Serial.println(dataString);
      }
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  } 
   
  // delay, but don't count time in loop
  while (time < endTime)
  {
    time = millis();
  }
}
void readJoystick()
// Motor or steer is a value between -512 and 511.
// A negative value is slowing or a left turn.
{
  int n;
  Servos[BRAKE].Stick = 512; // analogRead(JoystickCenter);
  Servos[MOTOR].Stick  = analogRead(AccelerateJoystick) - Servos[BRAKE].Stick;
  Servos[STEER].Stick  = analogRead(SteerJoystick) - Servos[BRAKE].Stick;
  Servos[MOTOR].EnableSwitch = digitalRead(EnableMotor);
  Servos[STEER].EnableSwitch  = digitalRead(EnableSteer);
  Servos[BRAKE].EnableSwitch  = digitalRead(EnableBrake);
  
  /*  Current sensors are Allegro ACS758LCB-50U
      Quiescent reading is about 550 mV, increasing by 60mV/A.
      Data sheet gives 600 mV as typical quiescent.
      +/- 3 sigma noise voltage is 15 mV.
      It uses 5V Vcc; Thus a reading of 1023 would be about 72 A.
      One count is 4.8 mV; One amp is 12 counts;
      Servos are fused for 5 amps; 
      must keep current < 60 counts above quiescent.
  */
  const int mAmpsPerCount = 80; 
  // read a voltage proportional to current.
  for (n= 0; n < 3; n++)
  {
    Servos[n].Current = analogRead(n) - Servos[n].QuiescentCurrent; 
    Servos[n].mAmp = Servos[n].Current * mAmpsPerCount;
  }
}
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








