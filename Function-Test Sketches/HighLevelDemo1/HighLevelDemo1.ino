#include <due_can.h>
#include "variant.h"
#include <Wire.h>
extern TwoWire Wire1;
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>

#define Sensor_CANID 0X0A
#define Drive_CANID 0X05
#define MAX_CAN_FRAME 16
#define EStop_CANID 0x00

/* map speed from 0~20 kph to actuator value */
int mapSpeed(int speed)
{ 
  return map(speed, 0, 20, 55, 5540);
}

/* map speed from -30(left) to 30 (right) to steering value */
int mapAngle(int angle)
{ 
  return map(angle, -90, 90, 0, 255);
}

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
int Straight_speed = mapSpeed(8);
int Turn_speed = mapSpeed(5);
int Straight_angle = 255/2;
int Right_turn_angle = 255;
int Left_turn_angle = 0;
float Pi = 3.14159;

unsigned long currentSpeed = 0;
unsigned long totalDistance = 0;
unsigned long t = 0;

/* receive CAN message from low level */
void receiveMSG()
{
  //filter the other messages rather than sensor readings
  CAN.watchFor(Sensor_CANID);

  while (CAN.available() > 0)
  {
    CAN_FRAME incoming;
    CAN.read(incoming);
    Serial.print("Get data from ID: ");
    Serial.println(incoming.id, HEX);
    Serial.print("Low: ");
    Serial.print((int)incoming.data.low, DEC);
    Serial.print("  High: ");
    Serial.println((int)incoming.data.high, DEC);
  }
}

/* send drive command CAN message with given speed and angle */
void sendMSG(int command_speed, int command_angle)
{
  CAN_FRAME output;
  output.id = Drive_CANID;
  output.length = MAX_CAN_FRAME;
  output.data.low = command_speed;
  output.data.high = command_angle;
  CAN.sendFrame(output);
}

/* send emergency stop CAN message */
void sendEStop(){
  CAN_FRAME output;
  output.id = EStop_CANID;
  output.length = MAX_CAN_FRAME;
  output.data.low = 0;
  output.data.high = 0;
  CAN.sendFrame(output);
}

/* do the a square routine */
void test_high_level()
{
  for (int i = 0; i < 4; i++)
  {
    // send straight message then turn message
    sendMSG(Straight_speed, Straight_angle);
    delay(6000);
    sendMSG(Turn_speed, Right_turn_angle);
    delay(1000);
  }
}

/* stop the demo */
void terminate_process()
{
  sendMSG(0, 0);
//  delay(3000);
//  sendEStop();
  while (true){}
}

void setup()
{
  Serial.begin(115200);
  CAN.begin(CAN_BPS_500K);
  delay(3000);
}

void loop()
{
  //receiveMSG(); 
  test_high_level();
  terminate_process();
}
