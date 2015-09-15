/*
Program to test Elcano_Serial.cpp
Tyler Folsom         Sept 7, 2015
This test was run an an Arduino Mega, with pin D18 (TX1) connected to pin D0 (RX0).
*/

#include <Elcano_Serial.h>

#define LOOP_TIME_MS 5000

void setup() {
    Serial.begin(9600); 
    Serial.print("\n\n\n");
    Serial1.begin(9600);
}

void loop() {
  SerialData Results;
  Serial.print("\n"); 
  
  Results.Clear();
  Results.kind = MSG_DRIVE;
  Results.angle_deg = 25;    // OK
  writeSerial(&Serial1, &Results);
  delay(100);
  Results.Clear();
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);
  
  Results.Clear();
  Results.kind = MSG_SEG;
  Results.number = 1;
  Results.posE_cm = 14263;
  Results.probability = 59;  // not used
  Results.bearing_deg = 270;
  Results.posN_cm = 123456;
  writeSerial(&Serial1, &Results);
  Results.Clear();
  delay(180);  
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);
  
  Results.Clear();
  Results.kind = MSG_SEG;
  Results.number = 2;
  Results.posE_cm = 4263;
  Results.speed_cmPs = 1599; 
  Results.bearing_deg = 227;
  Results.posN_cm = 789012;
  writeSerial(&Serial1, &Results);
  Results.Clear();
  delay(180);  
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);
    
  Results.Clear();
  Results.kind = MSG_DRIVE;
  Results.speed_cmPs = 1500;    // OK
  writeSerial(&Serial1, &Results);
  delay(50);
  Results.Clear();
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_DRIVE;
  Results.angle_deg = 20;
  Results.speed_cmPs = 1000;  // OK
  writeSerial(&Serial1, &Results);
  Results.Clear();
  delay(90);
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_SENSOR;
  Results.angle_deg = -12;   // OK
  writeSerial(&Serial1, &Results);
  Results.Clear();
  delay(80);  
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_SENSOR;
  Results.speed_cmPs = -500;   // OK
  writeSerial(&Serial1, &Results);
  delay(80);  
  Results.Clear();
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_SENSOR;
  Results.bearing_deg = 3550;  // OK
  Results.angle_deg = 0;   // OK
  Results.speed_cmPs = 1;       // OK
  writeSerial(&Serial1, &Results);
  delay(80);  
  Results.Clear();
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_SENSOR;
  Results.posE_cm = 27540;
  Results.bearing_deg = 679;
  Results.posN_cm = -27540;    // OK
  writeSerial(&Serial1, &Results);
  Results.Clear();
  delay(80);  
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);
 
  Results.Clear();
  Results.kind = MSG_GOAL;
  Results.number = 1;
  Results.posE_cm = -123456;
  Results.posN_cm = -4123450;
  Results.bearing_deg = 2870;
  writeSerial(&Serial1, &Results);
  delay(80);  
  Results.Clear();
  readSerial(&Serial, &Results);     // not read
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_GOAL;
  Results.number = 2;
  Results.posE_cm = 0;
  Results.probability = 59;
  Results.posN_cm = 0;
  writeSerial(&Serial1, &Results);  // OK
  delay(80);  
  Results.Clear();
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);

  Results.Clear();
  Results.kind = MSG_GOAL;
  Results.number = 3;
  Results.posE_cm = -23456;
  Results.posN_cm = 123450;
  writeSerial(&Serial1, &Results);  // OK
  delay(80);  
  Results.Clear();
  readSerial(&Serial, &Results);
  writeSerial(&Serial, &Results);
  

  delay(LOOP_TIME_MS);
  

}
