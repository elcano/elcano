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
    // "secret" feature; go to the full range of positions slowly
    if (Serial.peek() == 's') {
      Serial.readString();
      uint8_t pos = 45;
      int8_t delta = 2;
      for (uint8_t count = 0; count < 150; count++) {
        pos += delta;
        if (pos > 142 || pos < 45) delta = -delta;
        pos = constrain(pos, 45, 142);
        actuator.write(pos);
        delay(100);
      }
      actuator.write(94);
      return;
    }
    
    uint8_t extension = Serial.parseInt();
    extension = constrain(extension, 0, 180);
    Serial.readString(); // clear serial
    Serial.print("Using extension: "); Serial.println(extension);
    actuator.write(extension);
  }
} 
