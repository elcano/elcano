#include <Settings.h>
#include <Servo.h>

Servo steer;
void setup() {
  // put your setup code here, to run once:
  pinMode(BRAKE_OUT_PIN, OUTPUT);
  steer.attach(STEER_OUT_PIN);
  Serial.begin(9600);
  
}
int sig = 1500//210;
void loop() {
  
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    sig = Serial.parseInt();
    Serial.println(sig);
    steer.writeMicroseconds(sig);
    //analogWrite(BRAKE_OUT_PIN, sig);
  }  
}
