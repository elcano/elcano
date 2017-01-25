#include <ElcanoTimedBrake.h>
#include <Servo.h>
using namespace elcano;
using namespace privateMembers;
void setup() 
{
  Serial.begin(9600);
  pinMode(8, OUTPUT); // extend
  pinMode(6, OUTPUT); // retract
  digitalWrite(8, LOW); // extend
  digitalWrite(6, LOW); // retract
  extend(1000);
  retract(1000);
  extend(1000);
  retract(1000);
  extend(1000);
  retract(1000);
  extend(1000);
  retract(1000);
  extend(1000);
  retract(1000);
  extend(1000);
  retract(1000);
//  setupBrake();
//  applyBrakes();
//  retractBrakes();
//  applyBrakes();
//  retractBrakes();
//  applyBrakes();
//  retractBrakes();
//  applyBrakes();
//  retractBrakes();
}
void loop() 
{
//  Serial.println(brakePosition);  
}

