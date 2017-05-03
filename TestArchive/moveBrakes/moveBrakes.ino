#include <Settings.h>
#include <Servo.h>
int pin = BRAKE_OUT_PIN;
Servo STEER_SERVO;
void setup() {
  STEER_SERVO.attach(9); 
  pinMode(pin, OUTPUT);
}

void loop() 
{
  for(int i = 1000; i < 2000; i++)
  {
    STEER_SERVO.writeMicroseconds(i);
    delay(10);
  }
}
