#include <ElcanoTimedBrake.h>
using namespace elcano;
void setup() 
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  setupBrake();
  delay(10000);
  applyBrakes();
  delay(1100);
  releaseBrakes();
}

void loop() 
{
//  Serial.println(brakePosition);  
}

