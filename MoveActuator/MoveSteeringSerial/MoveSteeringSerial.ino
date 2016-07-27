#include <Settings.h>

void setup() {
  // put your setup code here, to run once:
//  pinMode(7, OUTPUT);
//  //pinMode(8, OUTPUT);
//  pinMode(6, OUTPUT);
  Serial.begin(9600);
  
}
int sig = 210;
void loop() {
  
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    sig = Serial.parseInt();
    Serial.println(sig);
    analogWrite(STEER_OUT_PIN, sig);
    analogWrite(BRAKE_OUT_PIN, sig);
  }  
}
