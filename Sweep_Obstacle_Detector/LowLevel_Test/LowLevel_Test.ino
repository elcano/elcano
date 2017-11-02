#include <Servo.h>
#include <ElcanoSerial.h>

const uint16_t SERIAL_BAUDRATE      = 9600;
const uint8_t  STEER_OUT_PIN        = 47;
const uint8_t  BRAKE_OUT_PIN        = 49;
const uint8_t  SWEEP_INPUT_PIN      = 22;
const uint8_t  LED_PIN              = 13;
const uint16_t LED_DELAY_MS         = 100;
const uint16_t SWEEP_INPUT_DELAY_MS = 50;
const uint16_t BRAKE_TIME_MS        = 2500;
const uint16_t RELEASE_BRAKE        = 1400;
const uint16_t ENGAGE_BRAKE         = 1950;

Servo BRAKE_SERVO;

elcano::ParseState parser;
elcano::SerialData data;

void led()
{
  digitalWrite(LED_PIN, HIGH);
  delay(LED_DELAY_MS);
  digitalWrite(LED_PIN, LOW);
  delay(LED_DELAY_MS);
}

void setup()
{
  BRAKE_SERVO.attach(BRAKE_OUT_PIN);
  pinMode(SWEEP_INPUT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  // Serial
  Serial.begin(SERIAL_BAUDRATE);
  // Serial1
  Serial1.begin(elcano::baudrate);
  // Parser
  parser.dt = &data;
  parser.input = &Serial1;
  parser.output = nullptr;
  parser.capture = elcano::MsgType::sensor;
  // Data
  data.clear();
  data.kind = elcano::MsgType::sensor;
}

void loop()
{
  while (parser.update() != elcano::ParseStateError::success)
  {
    delay(SWEEP_INPUT_DELAY_MS);
  }
  data.write(&Serial);
  led();
  BRAKE_SERVO.writeMicroseconds(ENGAGE_BRAKE);
  delay(BRAKE_TIME_MS);
  BRAKE_SERVO.writeMicroseconds(RELEASE_BRAKE);
  delay(BRAKE_TIME_MS);
}
