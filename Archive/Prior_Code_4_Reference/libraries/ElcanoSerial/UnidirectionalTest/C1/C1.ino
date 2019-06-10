#include <ElcanoSerial.h>

elcano::SerialData dt;

//void setupData() {
//  dt.kind = elcano::MsgType::drive;
//  dt.speed_cmPs = 11;
//  dt.angle_mDeg = -21;
//}
//void loopData() {
//  dt.speed_cmPs += 1;
//  dt.angle_mDeg -= 1;
//}

void setupData() {
  dt.kind = elcano::MsgType::goal;
  dt.posE_cm = 31;
  dt.posN_cm = -41;
}
void loopData() {
  dt.posE_cm += 1;
  dt.posN_cm -= 1;
}

//void setupData() {
//  dt.kind = elcano::MsgType::sensor;
//  dt.angle_mDeg = 51;
//  dt.obstacle_mm = -61;
//}
//void loopData() {
//  dt.angle_mDeg += 1;
//  dt.obstacle_mm -= 1;
//}

void setup() {
  Serial.begin(9600);
  Serial2.begin(elcano::baudrate);

  dt.clear();
  setupData();
}

void loop() {
  loopData();  
  dt.write(&Serial2);
  dt.write(&Serial);

  // Adjust this to a realistic period.
  // Use a large number like 1000 if the recipient (C2) is printing a lot of characters.
  delay(10);
}
