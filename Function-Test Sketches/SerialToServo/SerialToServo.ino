#include <Servo.h>

Servo actuator;

void setup() {
  Serial.begin(115200);
  actuator.attach(8);
  while (!Serial);
  Serial.println("READY! Give Servo.h extension ('0-180')");
  Serial.println("45 = 1ms, 142 = 2ms, 94 = 1.5ms");
}

void loop() {
  if (Serial.available() > 0) {
    uint8_t extension = Serial.parseInt();
    extension = constrain(extension, 0, 180);
    Serial.readString(); // clear serial
    Serial.print("Using extension: "); Serial.println(extension);
    
    

    actuator.write(extension);
  }
} 
