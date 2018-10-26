#include "Settings.h"
#include "ElcanoSerial.h"
#include <RH_ASK.h> // external library
using namespace elcano;

/* be sure to check this before compiling a sketch!
  TRANSMITTER 1 = transmitter mode, runs on Micro or other boards
  TRANSMITTER 0 = receiver mode, does NOT work on a Micro.
  This is a source of ElcanoSerial.write() problems because Serial/Serial#
  is not always a HardwareSerial pointer.
*/
#define TRANSMITTER 0
#if TRANSMITTER
#define RECEIVER 0
#else
#define RECEIVER 1
#endif

const bool DEBUG = false; // serial monitor debugging, turn this off for faster execution time

struct Inputs {
  long throttle;
  long angle;
  bool ebrakeOn;
  bool autoOn;
  bool newData;
} inputData;

// declare functions
void setupTransmitter(void);
void setupReceiver(void);
void loopTransmitter(void);
void loopReceiver(void);
void waitInputPWM(int timeout);
void waitInputByte(int timeout);
void sendElcanoSerial(void);
void debugPacket(unsigned long timer);

#define ESERIAL Serial1
#define PC Serial

#define ProcessFallOfINT(Index) RC_elapsed[Index] = (micros() - RC_rise[Index])
#define ProcessRiseOfINT(Index) RC_rise[Index] = micros()
#define RC_NUM_SIGNALS 7 // How many RC signals we receive
volatile unsigned long RC_rise[RC_NUM_SIGNALS];
volatile unsigned long RC_elapsed[RC_NUM_SIGNALS];
/* RC_Done[] tells us when we have started receiving RC data. Until then, we
  ignore RC_rise and RC_elapsed. */
volatile bool RC_Done[RC_NUM_SIGNALS];
volatile bool synced = false;
volatile bool flipping;
#define INVALID_DATA 0L // This is a value that the RC controller can't produce.
volatile int rc_index = 0;

const int MSG_SIZE = 3;
static bool using_us_units = false; // we use byte values(0-255) or microseconds (~1000-2000us)

RH_ASK driver;
/* default config:
  RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false); */

/* LEDs
  analogWrite works on pins 3, 5*, 6*, 9, 10, and 11 of a Micro. * = higher duty cycle */
const uint8_t TX_LED_TURN = 3;
const uint8_t TX_LED_THROTTLE = 5;
const uint8_t TX_LED_POWER = 6;
const uint8_t TX_LED_AUTO = 7;
const uint8_t TX_LED_EBRAKE = 8;
const uint8_t RESERVED_PIN = 10;    // this is the enable transmitter pin in the default radiohaed config
const uint8_t TX_LED_TRANSMIT = 11; // D11 is also radiohead RX pin but we are not using it on the transmitter
const uint8_t RX_LED_NOSIGNAL = 12; // D60 is A6 on the Mega

void setupReceiver() {
  driver.init();
  driver.setModeRx();
  PC.begin(baudrate);
  ESERIAL.begin(baudrate);
}

void waitInputByte(int timeout) {
  if (DEBUG) {
    PC.println(F("waiting for radio packet..."));
  }
  inputData.newData = false;
  static uint8_t buf[MSG_SIZE] = {0};
  uint8_t buflen = sizeof(buf);
  do {
    if (driver.recv(buf, &buflen)) {
      if (DEBUG) {
        PC.print(millis());
      }
      if (DEBUG) {
        PC.println(F(" packet found"));
      }
      // we do have a valid packet so we pull out the data and set newData
      inputData.throttle = buf[0];
      inputData.angle = buf[1];
      inputData.autoOn = bitRead(buf[2], 1);
      inputData.ebrakeOn = bitRead(buf[2], 2);
      inputData.newData = true;
    }
    if (timeout > 0) {
      delay(1);
      timeout--;
    }
  } while (timeout > 0);
}

void waitInputPWM(int timeout) {
  inputData.newData = false;
  for (int i = 0; i < RC_NUM_SIGNALS; i++) {
    RC_rise[i] = INVALID_DATA;
    RC_elapsed[i] = INVALID_DATA;
    RC_Done[i] = 0;
  }
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_rise, RISING);
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_rise, RISING);
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);
  do {
    if (RC_Done[RC_ESTP] == true) {
      // we now have the pulse length so we read that in

      const int THRESHOLD = 2000; // TODO: need to check this value when receiver is working

      inputData.ebrakeOn = (RC_elapsed[RC_ESTP] > THRESHOLD ? true : false);
      // now we reset everything so we can pick up new pulses, if any, while we wait
      RC_Done[RC_ESTP] = false;
      attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);

      // TODO: add other pins when receiver is working
    }
    if (timeout > 0) {
      delay(1);
      timeout--;
    }
  } while (timeout > 0);
}

void loopReceiver()
{
  if (DEBUG) {
    PC.println(F("start loop"));
  }
  int timeout = 100;
  inputData.newData = false;
  if (using_us_units) {
    waitInputPWM(timeout); // check for new data and wait for timeout
  }
  else {
    waitInputByte(timeout); // check for new data and wait for timeout
  }
  if (inputData.newData) {
    sendElcanoSerial();
  }
}

void sendElcanoSerial() {
  SerialData command;
  command.kind = MsgType::drive;
  command.speed_cmPs = inputData.throttle;
  command.angle_mDeg = inputData.angle;
  command.number =
    (long(using_us_units) << 2) + (long(inputData.ebrakeOn) << 1) + long(inputData.autoOn);

#if RECEIVER
  /*this #if is necessary to compile transmitter code for an Arduino Micro
    without splitting code into two files.
    SerialData.write() requires a pointer to a HardwareSerial type (like "Serial") but
    the Arduino Micro does not allow HardwareSerial on "Serial", only "Serial1" */
  command.write(&ESERIAL);
  if (DEBUG) {
    PC.print(millis());
  }
  if (DEBUG) {
    PC.println(F(" sent ES packet: "));
  }
#endif
}

void debugPacket(int throttle, int angle, int data, unsigned long timer) {
  // this is useful for identifying problems at the transmitter
#define separator F(",\t")
  PC.print(timer, DEC);
  PC.print(separator);
  PC.print(throttle, DEC);
  PC.print(separator);
  PC.print(angle, DEC);
  PC.print(separator);
  PC.println(data, DEC);
}

void setup() {
  if (TRANSMITTER) {
    setupTransmitter();
  }
  else {
    setupReceiver();
  }
}

void loop() {
  if (TRANSMITTER) {
    loopTransmitter();
  }
  else {
    loopReceiver();
  }
}

void setupTransmitter() {
  PC.begin(9600);
  if (DEBUG) {
    PC.println(F("SystemTime(ms), throttle(byte), turn(byte), data(byte)"));
  }
  driver.init();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT_PULLUP); // internal pullup keeps values from floating
  pinMode(A3, INPUT_PULLUP); // pin will always read HIGH if circuit/switch to GND is open
  // if the switch is designed to be closed when "ON", invert the value returned by digitalRead()
  pinMode(TX_LED_POWER, OUTPUT);
  pinMode(TX_LED_AUTO, OUTPUT);
  pinMode(TX_LED_EBRAKE, OUTPUT);
  pinMode(TX_LED_THROTTLE, OUTPUT);
  pinMode(TX_LED_TURN, OUTPUT);
  pinMode(TX_LED_TRANSMIT, OUTPUT);
}

void loopTransmitter() {
  unsigned long start = millis();
  digitalWrite(TX_LED_POWER, HIGH);
  int16_t turn = analogRead(A1) >> 2;
  int turn_trim = -3;
  turn -= turn_trim;
  if (turn < 0){turn = 0;}
  if (turn > 255){turn = 255;}
  int16_t throttle = analogRead(A0) >> 2;
  int thr_trim = 0;
  throttle -= thr_trim;
  if (throttle < 0){throttle = 0;}
  if (throttle > 255){throttle = 255;}
  uint8_t autoMode = digitalRead(A2);
  uint8_t eBrake = digitalRead(A3);
  uint8_t msg[MSG_SIZE] = {0};
  msg[0] = throttle & 0xFF;
  msg[1] = turn & 0xFF;
  msg[2] = (autoMode | (eBrake << 1)); // B000000EA
  // make transmit led brighter until uint8_t rollover at 256
  static uint8_t transmitBrightness = 0;
  const uint8_t BRIGHTNESS_STEP = 255 / 4;
  transmitBrightness += BRIGHTNESS_STEP;
  analogWrite(TX_LED_TRANSMIT, transmitBrightness);
  // transmit message
  driver.send((uint8_t *)msg, MSG_SIZE);
  driver.waitPacketSent();
  // write values to remaining output LEDs
  digitalWrite(TX_LED_AUTO, autoMode);
  digitalWrite(TX_LED_EBRAKE, eBrake);
  // turn and throttle are 0-255
  int throttleBrightness = throttle;
  analogWrite(TX_LED_THROTTLE, throttleBrightness);
  int turnBrightness = turn;
  analogWrite(TX_LED_TURN, turnBrightness);
  if (DEBUG) {
    debugPacket(msg[0], msg[1], msg[2], millis());
  }
  unsigned long finish = millis();
  const int LOOPTIME_MS = 100;
  unsigned long thisLoopTime = finish - start;
  unsigned long timeToDelay = LOOPTIME_MS - thisLoopTime;
  if (timeToDelay > 0) {
    delay(timeToDelay);
  }
}

void ISR_TURN_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_TURN);
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_fall, FALLING);
  interrupts();
}
void ISR_BRAKE_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_BRAKE);
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_fall, FALLING);
  interrupts();
}
void ISR_ESTOP_rise() { //Should be bound to the red switch
  noInterrupts();
  ProcessRiseOfINT(RC_ESTP);
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_fall, FALLING);
  interrupts();
}

void ISR_GO_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_GO);
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_fall, FALLING);
  interrupts();
}
void ISR_GO_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_GO);
  RC_Done[RC_GO] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_rise, RISING);
  interrupts();
}
void ISR_TURN_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_TURN);
  RC_Done[RC_TURN] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_rise, RISING);
  interrupts();
}
void ISR_BRAKE_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_BRAKE);
  RC_Done[RC_BRAKE] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);
  interrupts();
}
void ISR_ESTOP_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_ESTP);
  RC_Done[RC_ESTP] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);
  interrupts();
}
