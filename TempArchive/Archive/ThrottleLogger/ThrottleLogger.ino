/*
  This is a test routine for Elcano.
  It runs on an Arduino Mega 2560 with an Adafruit datalogger shield.
  
  It is based on code by Tom Igoe to use SD card and serial monitor.
  
 The circuit:
 Uses enable switches, current sensors, and PWM outputs for brakes, steering and motor as
 given in IO_Mega.h
 A throttle is attached to analog input pin 8.
 * SD card attached to SPI bus as follows:
 ** CS - pin 10
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 
 Original created  24 Nov 2010; updated 2 Dec 2010 by Tom Igoe
 
 Adapted 21 Feb 2012 by Tyler Folsom
 
 This example code is in the public domain.
 	 
 Status as of 2/26/12:
 Throttle works and is displayed on the 8 LEDs on the front panel.
 All front panel LEDs work except for stopLED.
 Four front panel switches: power and enabling motor, brakes and steering work.
 SD card works, but is sometimes tempermental about displaying on a PC.
 Serial monitor works.
 RTC works with Duemillenove, but RTC hardware does not work on the Mega; use software RTC, which works.
 The throttle controls the servos for steering or brakes.
 Brake servo is physically connected to operate brakes.
 Steering servo has been disconnected from steering linkage. There have been problems 
 with drawing more than 5 amps and blowing the fuse. Need to analyze phsysical limits of motion
 and limit motion if drawing too much current.
 Traction motor does not respond to throttle. Motor had worked previously and an alternative motor was
 recently tested as good; neither presently responds to computer. Suspect that the phase wires to wheel 
 are wrong.
 The joystick was defective and returned to manufacturer. Presently in return shipment.
 Quiescent current sensors read about 111 counts, when this is subtracted off, there are still about 
 12-13 counts left, which is 1 Amp.  Increasing current appears to be recognized correctly.
 
 To Do:
 Set steering limits and reattach linkage.
 Write routines to ease back on servo requests when too much current is drawn. Maybe a PID controller.
 Get traction motor working.
 Reinstall joystick.
 Phsically put everything back together.
 Test all systems on test stand.
 Do a road test.
 Shoot a video and post it.
 */
#define MEGA
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#ifdef MEGA
#include "IO_Mega.h"
#else
#include "IO_2009.h" // conflict with chip select
#endif
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
// Background readings of current sensors
int Quiescent36count = 0; 
int Quiescent12Bcount = 0; 
int Quiescent12Scount = 0; 

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing throttle SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);
  pinMode(53, OUTPUT);  // Unused CS on Mega
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
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
  BackgroundCurrent();
}
void BackgroundCurrent()
{
  int i;
  const int samples = 10;
  // wait for any power-up servo motion to stop; flash LED while waiting.
  for (i = 1; i < 12; i++)
  {
    digitalWrite(CruiseLED,HIGH);   
    delay(700);
    digitalWrite(CruiseLED,LOW);   
    delay(700);
    // 2/26/12 Do not see this LED flash.  
  }
  // 2/26/12  Do we need to wait longer for current to stabilize?
  // Shoud we periodically recalculate the quiescent values?
  digitalWrite(CruiseLED,HIGH);   

  for (i=1; i < samples; i++)
  {
    Quiescent36count += analogRead(Current36V); 
    Quiescent12Bcount += analogRead(CurrentBrake); 
    Quiescent12Scount += analogRead(CurrentSteer); 
    delay(100);
  }
    Quiescent36count /= samples; 
    Quiescent12Bcount /= samples; 
    Quiescent12Scount /= samples; 
    digitalWrite(CruiseLED,LOW);   
    Serial.print(Quiescent36count, DEC);
    Serial.print(',');
    Serial.print(Quiescent12Bcount, DEC);
    Serial.print(',');
    Serial.print(Quiescent12Scount, DEC);
    Serial.println();

}
void loop()
{
  const unsigned long pause = 500; // msec
  unsigned long time = millis();
  unsigned long endTime = time + pause;

// Read throttle and 3 switches; Send throttle to enabled servos.
  int throttle = testThrottle();  // 0 to 255
  int MotorSwitch = digitalRead(EnableMotor);
  int BrakeSwitch = digitalRead(EnableBrake);
  int SteerSwitch = digitalRead(EnableSteer);
  if (MotorSwitch == HIGH)
  {
    analogWrite(TractionMotor, throttle);
  }
  if (BrakeSwitch == HIGH)
  {
    analogWrite(DiskBrake, 128 + throttle/2);
  }
  if (SteerSwitch == HIGH)
  {
    analogWrite(Steer, 128 + throttle/2);
  }
  // To do:  if all switches are low and it has been a while since last calculation,
  // call BackgroundCurrent().
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
  int current36 = analogRead(Current36V) - Quiescent36count; 
  int current12B = analogRead(CurrentBrake) - Quiescent12Bcount; 
  int current12S = analogRead(CurrentSteer) - Quiescent12Scount; 
  int mAmp36 = current36 * mAmpsPerCount;
  int mAmp12B = current12B * mAmpsPerCount;
  int mAmp12S = current12S * mAmpsPerCount;
  
  // now log the information
  // make a string for assembling the data to log:
  // !!! Original version fails when string > 13 characters !!! TCF 2/21/12
  // String dataString = "";  // ORIGINAL
  String dataString = String("                                                  ");
//  dataString += String(i);  // ORIGINAL
  dataString = String(throttle);  
  dataString += (MotorSwitch==HIGH? ",1,": ",0,"); 
  dataString += (BrakeSwitch==HIGH? "1,": "0,"); 
  dataString += (SteerSwitch==HIGH? "1,": "0,"); 
  dataString += String(current36);
  dataString += ","; 
  dataString += String(current12B);
  dataString += ","; 
  dataString += String(current12S);
  dataString += ", "; 
  dataString += String(mAmp36);
  dataString += ","; 
  dataString += String(mAmp12B);
  dataString += ","; 
  dataString += String(mAmp12S);
  dataString += ", "; 
    
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

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open(FILE_NAME, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
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
int  testThrottle()
 {
  
  int throttle = analogRead(AccelerateJoystick);
  const int MINIMUM = 0.86/ 5 * 1023; // 175
  const int MAXIMUM = 3.99/5 * 1023;  //816
  float scale = 255.0 / (float)(MAXIMUM - MINIMUM);
  throttle = max (0, throttle - MINIMUM);
  throttle *= scale;
  
   Display (throttle);
   return throttle;
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








