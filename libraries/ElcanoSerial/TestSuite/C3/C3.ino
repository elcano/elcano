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
  if (err == elcano::ParseStateError::success) {
    dt2.write(&Serial2);
  } else if (err != elcano::ParseStateError::passthru) {
    Serial.print("Error: recieved ");
    switch (err) {
    case elcano::ParseStateError::inval_comb:
      Serial.println("an invalid combination of attributes for a type!");
      break;
    case elcano::ParseStateError::bad_type:
      Serial.println("a bad type!");
      break;
    case elcano::ParseStateError::bad_lcurly:
      Serial.println("a bad left `{'!");
      break;
    case elcano::ParseStateError::bad_attrib:
      Serial.println("a bad attribute!");
      break;
    case elcano::ParseStateError::bad_number:
      Serial.println("a bad number!");
      break;
    default:
      Serial.println("an unknown error!");
    }
  }
}
