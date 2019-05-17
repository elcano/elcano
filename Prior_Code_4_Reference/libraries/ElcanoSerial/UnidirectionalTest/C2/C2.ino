#include <ElcanoSerial.h>

elcano::ParseState ps;
elcano::SerialData dt;

void setup() {
  Serial.begin(9600);
  Serial1.begin(elcano::baudrate);
  Serial2.begin(elcano::baudrate);

  ps.dt = &dt;
  ps.input = &Serial1;
  ps.output = &Serial2;
  ps.capture = (elcano::MsgType::drive | elcano::MsgType::goal);
}

void loop() {
  elcano::ParseStateError err = ps.update();

  Serial.println(static_cast<int8_t>(err));

//  if (err != elcano::ParseStateError::unavailable) {
//    Serial.println("err " + String(static_cast<int8_t>(err)));
//    Serial.println("  kind " + String(static_cast<int8_t>(dt.kind)));
//    Serial.println("  number " + String(dt.number));
//    Serial.println("  speed_cmPs " + String(dt.speed_cmPs));
//    Serial.println("  angle_mDeg " + String(dt.angle_mDeg));
//    Serial.println("  obstacle_mm " + String(dt.obstacle_mm));
//    Serial.println("  probability " + String(dt.probability));
//    Serial.println("  bearing_deg " + String(dt.bearing_deg));
//    Serial.println("  posE_cm " + String(dt.posE_cm));
//    Serial.println("  posN_cm " + String(dt.posN_cm));
//  }
}
