/*
Record joystick values to the serial port. With this running, move the joystick
in a recognizable pattern. Would be good to also record an external signal that
could serve as a mark for when a motion starts.
*/

#include <IO_PCB.h>

// Limit loop rate.
#define LOOP_TIME_MS 100

/*---------------------------------------------------------------------------------------*/
void setup()
{
  //pinMode(AccelerateJoystick, INPUT);
  //pinMode(SteerJoystick, INPUT);
  //pinMode(JoystickCenter, INPUT);
  Serial.begin(9600);
}

/*---------------------------------------------------------------------------------------*/

int accelerateJoystickValue;
int steerJoystickValue;
int centerJoystickValue;

unsigned long currentTime;
unsigned long nextLoopStart = 0;
unsigned long pauseTime;
void loop()
{
  // Delay til time to capture next value.
  currentTime = millis();
  pauseTime = nextLoopStart - currentTime;
  if (pauseTime > 0) {
    Serial.print("Waiting ");
    Serial.print(pauseTime);
    Serial.println(" ms");
    delay(pauseTime);
  }  

  //Read the joystick values.
  currentTime = millis();
  accelerateJoystickValue = analogRead(AccelerateJoystick);
  steerJoystickValue = analogRead(SteerJoystick);
  centerJoystickValue = analogRead(JoystickCenter);

  Serial.print("Time: ");
  Serial.print(currentTime);
  Serial.print(", ");
  Serial.print("Accelerate: ");
  Serial.print(accelerateJoystickValue);
  Serial.print(", ");
  Serial.print("Steer: ");
  Serial.print(steerJoystickValue);
  Serial.print(", ");
  Serial.print("Center: ");
  Serial.println(centerJoystickValue);
  
  nextLoopStart = nextLoopStart + LOOP_TIME_MS;
}

