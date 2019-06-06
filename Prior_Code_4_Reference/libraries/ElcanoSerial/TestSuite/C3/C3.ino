#include <ElcanoSerial.h>

elcano::ParseState ps1;
elcano::SerialData dt1, dt2;

void setup() {
  Serial.begin(9600); // For communication with the reader
  Serial1.begin(elcano::baudrate);
  Serial2.begin(elcano::baudrate);

  ps1.dt = &dt1;
  ps1.input = &Serial1;
  ps1.output = &Serial2;
  ps1.capture = elcano::MsgType::sensor | elcano::MsgType::seg;

  dt2.clear();
  dt2.kind = elcano::MsgType::drive;
  dt2.speed_cmPs = 123456;
  dt2.angle_mDeg = 32;
}

void loop() {
  elcano::ParseStateError err = ps1.update();
  Serial.println(static_cast<int8_t>(err));

  if (err == elcano::ParseStateError::passthru ||
      err == elcano::ParseStateError::unavailable) {
    dt2.write(&Serial2);
  }
}
