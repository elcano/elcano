#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define SERVOch 0
// 130 measured to be minimum value registered (farthest left)
// 254 is the farthest right (and mechanically starts o jam) 255 === 0 and it retracts.

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  while (!Serial);
  Serial.println("READY! Give % extension ('50')");
}

void loop() {
  if (Serial.available() > 0) {
    uint8_t extension = Serial.parseInt();
    extension = constrain(extension, 0, 100);
    Serial.readString(); // clear serial
    Serial.print("Using extension: "); Serial.print(extension); Serial.println("%");
    
    double pulse = map(extension, 0, 100, 240, 474); // min 240, 474

    pwm.setPWM(SERVOch, 0, pulse);
  }
} 
