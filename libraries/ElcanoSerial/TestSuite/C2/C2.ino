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
  ps1.capture = elcano::MsgType::goal;

  dt2.clear();
  dt2.kind = elcano::MsgType::seg;
  dt2.number = 4;
  dt2.bearing_deg = 80;
  dt2.speed_cmPs = 160;
  dt2.posE_cm = 3200;
  dt2.posN_cm = -64000;
}

void loop() {
  elcano::ParseStateError err = ps1.update();
  Serial.println(static_cast<int8_t>(err));

  if (err == elcano::ParseStateError::passthru ||
      err == elcano::ParseStateError::unavailable) {
    dt2.write(&Serial2);
  }
}
