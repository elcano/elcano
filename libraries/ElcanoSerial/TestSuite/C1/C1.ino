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
  ps1.captures = elcano::MsgType::drive;

  ps2.dt = &dt2;
  ps2.input = &Serial;
  ps2.output = &Serial2;
  ps2.captures = elcano::MsgType::none; // Passthru on all msgs

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
    start = finish;
  } else if (err != elcano::ParseStateError::incomplete) {
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

  ps2.update();
}

