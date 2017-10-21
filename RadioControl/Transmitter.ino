#include <RH_ASK.h>
// library availible at https://github.com/PaulStoffregen/RadioHead.git

// Parameters are:
// Transmission speed (b/s), rx, tx, transmitter controller (optional), controller inverter (optional)
RH_ASK driver(2000, 7, 8, 10, false);

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
  char c[1] = {'0'};
  char *msg = c;
  int temp = (analogRead(A4) / 4);
  unsigned long longT = (analogRead(A4) / 4) * 1000 + 123;
  msg = itoa(longT, msg, 10);
  char arr[9];
  String strrr = String(temp);
  arr[0] = '1';
  arr[1] = '2';
  arr[2] = '3';
  arr[3] = '4';
  arr[4] = '5';
  arr[5] = '6';
  arr[6] = '7';
  arr[7] = '8';
  arr[8] = '9';
  int a = 1;
  char b[6];
  strrr.toCharArray(b, sizeof(b));
  msg = &b[0];

  Serial.println(temp);
  //  if(digitalRead(6))
  //  {
  //    msg = "1";
  //  }
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
}
