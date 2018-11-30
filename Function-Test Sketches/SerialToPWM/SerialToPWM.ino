#define PWMOUT 8
// 130 measured to be minimum value registered (farthest left)
// 254 is the farthest right (and mechanically starts o jam) 255 === 0 and it retracts.
//

void setup() {
  pinMode(PWMOUT, OUTPUT);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("READY!");
}

void loop() {
  if (Serial.available() > 0) {
    int PWM = Serial.parseInt();
    PWM = constrain(PWM, 0, 255);
    Serial.readString();
    analogWrite(PWMOUT, PWM);
    Serial.print("Using PWM value: ");
    Serial.println(PWM);
  }
} 
