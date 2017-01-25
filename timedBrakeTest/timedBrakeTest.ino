#include <ElcanoTimedBrake.h>
#include <Servo.h>
using namespace elcano;
void setup() 
{
  Serial.begin(9600);
  pinMode(8, OUTPUT); // extend
  pinMode(6, OUTPUT); // retract
  digitalWrite(8, LOW); // extend
  digitalWrite(6, LOW); // retract
}
void loop() 
{
//  Serial.println(brakePosition);  
}

