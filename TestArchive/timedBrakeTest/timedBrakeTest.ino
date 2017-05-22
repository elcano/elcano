#include <ElcanoTimedBrake.h>
#include <Servo.h>
Servo myServo;
using namespace elcano;
void setup() 
{
  myServo.attach(9);
  Serial.begin(9600);
  pinMode(8, OUTPUT); // extend
  pinMode(6, OUTPUT); // retract
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(8, LOW); // extend
  digitalWrite(6, LOW); // retract
  setupBrake();
  applyBrakes();
  releaseBrakes();
  applyBrakes();
  releaseBrakes();
  applyBrakes();
  releaseBrakes();
  applyBrakes();
  releaseBrakes();
  applyBrakes();
  releaseBrakes();
}
void loop()
{
  for(int i = 1000; i <= 2000; i+=20)
  {
    myServo.writeMicroseconds(i);
    delay(100);
  }
  
  for(int i = 2000; i >= 1000; i-=20)
  {
    myServo.writeMicroseconds(i);
    delay(100);
  }
}

