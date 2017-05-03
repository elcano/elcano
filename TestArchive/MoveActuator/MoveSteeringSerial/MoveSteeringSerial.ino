#include <Settings.h>
#include <Servo.h>

Servo steer, brake;
void setup() {
  // put your setup code here, to run once:
  pinMode(BRAKE_OUT_PIN, OUTPUT);
  steer.attach(STEER_OUT_PIN);
  Serial.begin(9600);
}
int sig = 1500;
bool h = false;
void loop() {
  
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    sig = Serial.parseInt();
    Serial.println(sig);
    
    if(h){
      steer.writeMicroseconds(sig);
      h = false;
    }
    else{
      brake.writeMicroseconds(sig);
      h = true;
    }
  }  
}
