#include <RH_ASK.h>
// library availible at https://github.com/PaulStoffregen/RadioHead.git

// RH_ASK Parameters are:
// Transmission speed (b/s), rx, tx, transmitter controller (optional), controller inverter (optional)
RH_ASK driver(2000, 7, 8, 10, false);

#define THROTTLE_PIN A5
#define TURN_PIN A4
const int EBRAKE_PIN = 9;
const int AUTO_PIN = 6;
const unsigned char MSG_LENGTH = 9;
const int THROTTLE_CENTER = 512;
const int THROTTLE_PADDING = 10000;
const int TURN_PADDING = 1000;

void setup()
{
  Serial.begin(9600);
  delay(1);
  if (!driver.init())
  {
    Serial.println("init failed");
  }
}

void loop()
{
  //// declare and init char buffer
  uint8_t buf[MSG_LENGTH];
  memset(buf, 0, sizeof(buf));
  //// get values
  int turn = (analogRead(TURN_PIN) / 4) + TURN_PADDING;
  char turnchars[4];
  int throttle = analogRead(THROTTLE_PIN) - THROTTLE_CENTER;
  if (throttle < 0)
  {
    throttle = 0;
  }
  throttle += THROTTLE_PADDING;
  char throttlechars[5];
  digitalWrite(EBRAKE_PIN, HIGH); // integral pull-up resistor is active, pulls pin high
  digitalWrite(AUTO_PIN, HIGH);
  char ebrake = digitalRead(EBRAKE_PIN) + '0';
  char auto_mode = digitalRead(AUTO_PIN) + '0';
  //// construct message
  //Message structure (char array)
  //[0] : '0' = ebrake disengaged, else ebrake is engaged
  //[1] : '0' = autonomous, else manual control
  //Turn:
  //[2] : x100 (most significant turn digit)
  //[3] : x10
  //[4] : x1
  //Throttle:
  //[5] : x1,000 (most significant throttle digit)
  //[6] : x100
  //[7] : x10
  //[8] : x1

  itoa(throttle, throttlechars, 10);
  itoa(turn, turnchars, 10);
  buf[0] = ebrake;
  buf[1] = auto_mode;
  int i;
  int j = 1; // MSD turn
  for (i = 2; i < 5; i++)
  {
    buf[i] = turnchars[j];
    j++;
  }
  j = 1; // MSD throttle
  for (i = 5; i < 9; i++)
  {
    buf[i] = throttlechars[j];
    j++;
  }
  //// send message as a frame
  const unsigned char *msg = buf;
  driver.send(msg, MSG_LENGTH);
  //// debugging output
  if (ebrake == '1') // ebrake is active
  {
    Serial.print("E-Stop");
    if (auto_mode == '0')
    {
      Serial.print(" & ");
    }
    else
    {
      Serial.print(" on!\n");
    }
  }
  if (auto_mode == '0') // autonomous mode is active
  {
    Serial.print("Autonomous on!\n");
  }
  Serial.print("Tx Payload: ");
  for (i = 0; i < MSG_LENGTH; i++)
    Serial.print(char(buf[i]));
  Serial.print("\n");
  //// resume loop once data frame is sent
  driver.waitPacketSent();
}
