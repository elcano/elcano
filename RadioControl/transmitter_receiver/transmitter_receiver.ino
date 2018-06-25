#include <RH_ASK.h>
#include <ElcanoSerial.h>
using namespace elcano;

// declare functions
void setupTransmitter(void);
void setupReceiver(void);
void loopTransmitter(void);
void loopReceiver(void);
void sendToLowLevel(int rawSpeed, int rawAngle, int controlData);
void debugPacket(int throttle, int angle, int data, unsigned long timer);

// be sure to check this before compiling a sketch!
// true = transmitter mode, runs on Micro or other boards
// false = receiver mode, does NOT work on a Micro
const bool TRANSMITTER = false;
const bool RECEIVER = !(TRANSMITTER);

const bool DEBUG = false; // verbose debugging output to PC as CSV

#define ESERIAL Serial1
#define PC Serial


const int MSG_SIZE = 3;
RH_ASK driver;
// default: RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);

// LEDs
// analogWrite works on pins 3, 5*, 6*, 9, 10, and 11 of a Micro. * = higher duty cycle
const uint8_t LED_TURN = 3;
const uint8_t LED_THROTTLE = 5;
const uint8_t LED_POWER = 6;
const uint8_t LED_AUTO = 7;
const uint8_t LED_EBRAKE = 8;
const uint8_t RESERVED_PIN = 10; // this is the enable transmitter pin in the default radiohaed config
const uint8_t LED_TRANSMIT = 11; // D11 is also radiohead RX pin but we are not using it on the transmitter
const uint8_t LED_NOSIGNAL = 12; // D12 is also radiohead TX pin but we are not using it on the receiver

void setupTransmitter()
{
  PC.begin(9600);
  if (DEBUG)
  {
    const char HEADER[] = "SystemTime(ms), throttle, turn, data";
    PC.println(HEADER);
  }
  driver.init();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT_PULLUP); // internal pullup keeps values from floating
  pinMode(A3, INPUT_PULLUP); // pin will always read HIGH if circuit/switch to GND is open
  // if the switch is designed to be closed when "ON", invert the value returned by digitalRead()
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_AUTO, OUTPUT);
  pinMode(LED_EBRAKE, OUTPUT);
  pinMode(LED_THROTTLE, OUTPUT);
  pinMode(LED_TURN, OUTPUT);
  pinMode(LED_TRANSMIT, OUTPUT);
}

void loopTransmitter()
{
  unsigned long start = millis();
  digitalWrite(LED_POWER, HIGH);
  uint16_t turn = analogRead(A1) >> 2;
  uint16_t throttle = analogRead(A0) >> 2;
  uint8_t autoMode = digitalRead(A2);
  uint8_t eBrake = digitalRead(A3);
  uint8_t msg[MSG_SIZE] = {0};
  msg[0] = throttle & 0xFF;
  msg[1] = turn & 0xFF;
  msg[2] = (autoMode | (eBrake << 1)) ; // B000000EA
  // make transmit led brighter until uint8_t rollover at 256
  static uint8_t transmitBrightness = 0;
  const uint8_t BRIGHTNESS_STEP = 255 / 4;
  transmitBrightness += BRIGHTNESS_STEP;
  analogWrite(LED_TRANSMIT, transmitBrightness);
  // transmit message
  driver.send((uint8_t*)msg, MSG_SIZE);
  driver.waitPacketSent();
  // write values to remaining output LEDs
  digitalWrite(LED_AUTO, autoMode);
  digitalWrite(LED_EBRAKE, eBrake);
  // turn and throttle are 0-255
  int throttleBrightness = throttle;
  analogWrite(LED_THROTTLE, throttleBrightness);
  int turnBrightness = turn;
  analogWrite(LED_TURN, turnBrightness);
  if (DEBUG)
  {
    debugPacket(msg[0], msg[1], msg[2], millis());
  }
  unsigned long finish = millis();
  const int LOOPTIME_MS = 100;
  unsigned long thisLoopTime = finish - start;
  unsigned long timeToDelay = LOOPTIME_MS - thisLoopTime;
  if (timeToDelay > 0)
  {
    delay(timeToDelay);
  }
}

void setupReceiver()
{
  driver.init();
  driver.setModeRx();
  PC.begin(9600);
  if (DEBUG)
  {
    const char HEADER[] = "\nSystemTime(ms), throttle, turn, data";
    PC.println(HEADER);
  }
}

void loopReceiver()
{
  static unsigned long start = millis();
  static uint8_t buf[MSG_SIZE] = {0};
  uint8_t buflen = sizeof(buf);
  static bool isNewPacket = false;
  static unsigned long lastPacketMillis = millis();
  static int qos[3] = {0}; //quality of service(found packets, dropped packets, nosignal)
  if (driver.recv(buf, &buflen))
  {
    lastPacketMillis = millis();
    isNewPacket = true;
    digitalWrite(LED_NOSIGNAL, LOW);
  }
  const unsigned long LOOPTIME_MS = 100;
  unsigned long const SIGNAL_LOST_MILLIS = 4 * LOOPTIME_MS;
  if ((millis() - start) >= LOOPTIME_MS)
  {
    start = millis();
    if (isNewPacket)
    {
      isNewPacket = false;
      qos[0]++;
      sendToLowLevel(buf[0], buf[1], buf[2]);
      if (DEBUG)
      {
        debugPacket(buf[0], buf[1], buf[2], millis());
      }
    }
    else if ((millis() - lastPacketMillis) < SIGNAL_LOST_MILLIS)
    {
      if (DEBUG)
      {
        PC.print(millis(), DEC);
        PC.println(",,,,nopacket");
      }
      qos[1]++;
    }
    if ((millis() - lastPacketMillis) >= SIGNAL_LOST_MILLIS)
    {
      const int EBRAKE[] = {0, 0, 2};
      sendToLowLevel(EBRAKE[0], EBRAKE[1], EBRAKE[2]);
      for (int i = 0; i < MSG_SIZE; i++)
      {
        buf[i] = EBRAKE[i];
      }
      digitalWrite(LED_NOSIGNAL, HIGH);
      if (DEBUG)
      {
        PC.print(millis(), DEC);
        qos[2]++;
        PC.println(",,,,nosignal");
      }
    }
    if (DEBUG)
    {
      while (PC.available() > 0)
      {
        char temp = PC.read();
        if (temp == '\n')
        {
          continue;
        }
        if (temp == 'r') // type 'r' into ardiono serial monitor to reset QOS data
        {
          // reset qos data
          for (int i = 0; i < 3; i++)
          {
            qos[i] = 0;
          }
        }
        PC.println("packets found, packets lost, nosignal");
        for (int i = 0; i < 3; i++)
        {
          PC.print(qos[i], DEC);
          PC.print(",\t");
        }
        PC.print('\n');
      }
    }
  }
}

void sendToLowLevel(int rawSpeed, int rawAngle, int controlData)
{
  SerialData command;
  command.kind = MsgType::drive;
  const int T_OFFSET = 0; // throttle trim
  const int THROTTLE_CENTER = 127 - T_OFFSET;
  int throttle = rawSpeed - THROTTLE_CENTER;
  const int A_OFFSET = 3; // angle trim
  const int ANGLE_CENTER = 127 - A_OFFSET;
  int angle = rawAngle - ANGLE_CENTER;
  const int DEADZONE = 2;
  if (abs(throttle) < DEADZONE)
  {
    throttle = 0;
  }
  else
  {
    const int THROTTLE_SCALE = 2;
    throttle *= THROTTLE_SCALE; // 127 * 2 = 254 cm/s
  }

  if (abs(angle) < DEADZONE)
  {
    angle = 0;
  }
  else
  {
    const int INVERT = -1; // control is reversed
    const int ANGLE_SCALE = 230; //127 * 230 = 29210 mDeg
    angle *= INVERT * ANGLE_SCALE;
  }
  command.speed_cmPs = throttle;
  command.angle_mDeg = angle;
  command.number = controlData & B11;
#if RECEIVER
  // this precompiler if is necessary to compile transmitter code for an Arduino Micro
  //    without splitting code into two files
  // the Arduino Micro does not allow HardwareSerial on "Serial", only "Serial1"
  command.write(&ESERIAL);
#endif
}

void debugPacket(int throttle, int angle, int data, unsigned long timer)
{
  PC.print(timer, DEC);
  PC.print(",\t");
  PC.print(throttle, DEC);
  PC.print(",\t");
  PC.print(angle, DEC);
  PC.print(",\t");
  PC.println(data, DEC);
}

void setup()
{
  if (TRANSMITTER)
  {
    setupTransmitter();
  }
  else
  {
    setupReceiver();
  }
}

void loop()
{
  if (TRANSMITTER)
  {
    loopTransmitter();
  }
  else
  {
    loopReceiver();
  }
}

