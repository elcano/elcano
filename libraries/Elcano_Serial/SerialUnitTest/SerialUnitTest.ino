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
  
  Results.clear();
  Results.kind = MSG_DRIVE;
  Results.angle_deg = 25;    // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);
  
  Results.clear();
  Results.kind = MSG_SEG;
  Results.number = 1;
  Results.posE_cm = 14263;
  Results.probability = 59;  // not used
  Results.bearing_deg = 270;
  Results.posN_cm = 123456;
  Results.write(&Serial1);
  Results.clear(); 
  readSerial(&Serial, &Results);
  Results.write(&Serial);
  
  Results.clear();
  Results.kind = MSG_SEG;
  Results.number = 2;
  Results.posE_cm = 4263;
  Results.speed_cmPs = 1599; 
  Results.bearing_deg = 227;
  Results.posN_cm = 789012;
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);
    
  Results.clear();
  Results.kind = MSG_DRIVE;
  Results.speed_cmPs = 1500;    // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_DRIVE;
  Results.angle_deg = 20;
  Results.speed_cmPs = 1000;  // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_SENSOR;
  Results.angle_deg = -12;   // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_SENSOR;
  Results.speed_cmPs = -500;   // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_SENSOR;
  Results.bearing_deg = 3550;  // OK
  Results.angle_deg = 0;   // OK
  Results.speed_cmPs = 1;       // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_SENSOR;
  Results.posE_cm = 27540;
  Results.bearing_deg = 679;
  Results.posN_cm = -27540;    // OK
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);
 
  Results.clear();
  Results.kind = MSG_GOAL;
  Results.number = 1;
  Results.posE_cm = -123456;
  Results.posN_cm = -4123450;
  Results.bearing_deg = 2870;
  Results.write(&Serial1);
  Results.clear();
  readSerial(&Serial, &Results);     // not read
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_GOAL;
  Results.number = 2;
  Results.posE_cm = 0;
  Results.probability = 59;
  Results.posN_cm = 0;
  Results.write(&Serial1);  // OK
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);

  Results.clear();
  Results.kind = MSG_GOAL;
  Results.number = 3;
  Results.posE_cm = -23456;
  Results.posN_cm = 123450;
  Results.write(&Serial1);  // OK
  Results.clear();
  readSerial(&Serial, &Results);
  Results.write(&Serial);
  

  delay(LOOP_TIME_MS);
  

}
