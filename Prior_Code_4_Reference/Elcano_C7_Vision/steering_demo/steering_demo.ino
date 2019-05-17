#include <Servo.h>
const int led = 13;
Servo myservo;
String inString = "";
int usbRead = 0;
unsigned long Timer;

void setup() {
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  myservo.attach(7);
  myservo.writeMicroseconds(1500);
  Timer = millis();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  while (Serial.available() > 0){
    char inVal = Serial.read();
    if (isDigit(inVal)){
      inString += inVal;
    }
    // end of distance value
    if (inVal == ','){
      int distance = inString.toInt();
      Serial.print(distance);
      Serial.print(", ");
      inString = "";
    }
    // end of degree value
    if (inVal == '\n'){
      usbRead = inString.toInt() * 40 + 1100;
      if(usbRead < 1100){
        usbRead = 1100;
      }
      else if (usbRead > 1900){
        usbRead = 1900;
      }
      Serial.println(usbRead);
      if(millis() - Timer >= 400) {
        Timer = millis();
        myservo.writeMicroseconds(usbRead);
        //Serial.print("turning");
      }
      usbRead = 0;
      inString = "";
    }
  }
}
