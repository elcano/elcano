#include <Settings.h>
#include <Servo.h>
Servo steer;
int leftCalib;
int rightCalib;
bool hardLeft = true; //direction of wheels in last sensor catch
unsigned long curMicros;
unsigned long lastMicros;
int runningAvg [10] = {};
int runningAvgIter = 1;



void setup() {
  // put your setup code here, to run once:

  steer.attach(7);
  Serial.begin(9600);
  
  steer.writeMicroseconds(LEFT_TURN_OUT);
  delay(5000); //wait for wheels to turn
  leftCalib = analogRead(A2);
  Serial.print("Left Calibration: "); Serial.println(leftCalib);
  
  steer.writeMicroseconds(RIGHT_TURN_OUT);  
  delay(5000); //wait for wheels to turn
  rightCalib = analogRead(A2);
  Serial.print("Right Calibration: "); Serial.println(rightCalib);

  lastMicros = micros();
  runningAvg[0] = lastMicros;
}

void loop() {
  // put your main code here, to run repeatedly:
    int sensor = analogRead(A2);
    if (sensor > leftCalib - 5 && !hardLeft)//sensor catch at far left
    {
        hardLeft = true;
        curMicros = micros();
        Serial.println(curMicros-lastMicros); //Current time and end line
        lastMicros = curMicros;
        
        steer.writeMicroseconds(RIGHT_TURN_OUT);
        delay(100); //wait for wheels to turn out of calibrated range; stops excessive microsecond writes
    }
    else if (sensor < rightCalib + 5 && hardLeft)//sensor catch at far right
    {
        hardLeft = false;
        curMicros = micros();
        Serial.println(curMicros-lastMicros); //Current time and end line
        lastMicros = curMicros;
        
        steer.writeMicroseconds(LEFT_TURN_OUT);
        delay(100); //wait for wheels to turn out of calibrated range; stops excessive microsecond writes
    }
}
