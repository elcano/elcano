#include <Settings.h>
#include <Servo.h>
Servo steer;
//Servo brake;
void setup() {
  // put your setup code here, to run once:
  steer.attach(7);
  //brake.attach(6);
  Serial.begin(9600);
}
int sig = 1500;
void loop(){
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    sig = Serial.parseInt();
    Serial.println(sig);
    steer.writeMicroseconds(sig);
    //brake.writeMicroseconds(1500);
  }
}
