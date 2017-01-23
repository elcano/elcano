#include <ElcanoTimedBrake.h>
using namespace elcano;
void setup() 
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  extend(2000);
  retract(1000);
  extend(2000);
  retract(1000);
  extend(2000);
  retract(1000);
  extend(2000);
  retract(1000);
}
void loop() 
{
//  Serial.println(brakePosition);  
}

