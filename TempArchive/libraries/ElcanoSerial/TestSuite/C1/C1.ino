#include <ElcanoSerial.h>

elcano::ParseState ps1, ps2;
elcano::SerialData dt1, dt2;
unsigned long start, finish;

void setup() {
  Serial.begin(9600); // For communication with the reader
  Serial1.begin(elcano::baudrate);
  Serial2.begin(elcano::baudrate);

  ps1.dt = &dt1;
  ps1.input = &Serial1;
  ps1.output = &Serial2;
  ps1.capture = elcano::MsgType::drive;

  ps2.dt = &dt2;
  ps2.input = &Serial;
  ps2.output = &Serial2;
  ps2.capture = elcano::MsgType::none; // Passthru on all msgs

  start = millis();
}

void loop() {
  elcano::ParseStateError err = ps1.update();
  if (err == elcano::ParseStateError::success) {
    finish = millis();
    dt1.write(&Serial);
    Serial.print("Loop completed in ");
    Serial.print(finish - start);
    Serial.println("ms");
    start = millis();
  } else if (err != elcano::ParseStateError::passthru && err != elcano::ParseStateError::unavailable) {
    dt1.write(&Serial);
    Serial.println(static_cast<int8_t>(err));
  }

  err = ps2.update();
  if (err == elcano::ParseStateError::passthru) {
    start = millis();
  } else if (err != elcano::ParseStateError::unavailable) {
    dt1.write(&Serial);
    Serial.println(static_cast<int8_t>(err));
  }
}
