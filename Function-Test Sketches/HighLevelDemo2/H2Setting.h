#include <due_can.h>
#include "variant.h"
#include <Wire.h>
extern TwoWire Wire1;
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>

#define mySerial Serial3
#define Sensor_CANID 0X0A
#define Drive_CANID 0X05
#define MAX_CAN_FRAME 16
#define EStop_CANID 0x00
#define debug false

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

int Straight_speed = mapSpeed(8);
int Turn_speed = mapSpeed(5);
int Straight_angle = 255/2;
int Right_turn_angle = 255;
int Left_turn_angle = 0;

long latitude = 0;
long longitude= 0;
float heading = 0;

Adafruit_GPS GPS(&mySerial);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

/*  This is the geocoordinate of UWB playgroud                             
 *   
 *                               (lat diff 652)
 *  3.(47.761253, -122.189872)--------------------------------4.(47.760601, -122.189434)
 *      |                                                           |
 *      | (long diff 659)                                           |  (long diff 757) 
 *      |                                                           |
 *  2.(47.761136, -122.190531)--------------------------------1.(47.760433, -122.190191)  * Start point
 *                             (lat diff 703)
 */

long TwoLat = 47761001;
long ThreeLongt = -122189971;
long FourLat = 47760635;
long OneLongt = -122190383;
int approaching = 0;
long offset = 80; // The distance from target coordinate

 
// get gps coordinates and update latitude and longitude
void AcquireGPS() {
  char c;
  for (int i = 0; i < 25; i++) {
    c = GPS.read();
  }
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
    if (GPS.fix) {
      latitude = GPS.latitudeDegrees * 1000000;
      longitude = GPS.longitudeDegrees * 1000000;
      // 47.78823090 | -122.22163391
      if(debug){
        Serial.print(GPS.latitudeDegrees, 8);
        Serial.print("  ||  ");
        Serial.println(GPS.longitudeDegrees, 8); 
        Serial.print(latitude);
        Serial.print("  |  ");
        Serial.println(longitude); 
      }
    }
  }
}

// return where the trike is heading
float getHeading(void) {
  sensors_event_t event;
  mag.getEvent(&event);
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / PI;
  if (heading < 0)  {
    heading = 360 + heading;
  }
  return heading;
}

/*---------------------------------------------------------------------------------------*/
/* receive CAN message from low level
/*---------------------------------------------------------------------------------------*/
void receiveMSG()
{
  //filter the other messages rather than sensor
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

/*---------------------------------------------------------------------------------------*/
/* send drive command CAN message with given speed and angle
/*---------------------------------------------------------------------------------------*/
void sendMSG(int command_speed, int command_angle)
{
  CAN_FRAME output;
  output.id = Drive_CANID;
  output.length = MAX_CAN_FRAME;
  output.data.low = command_speed;
  output.data.high = command_angle;
  CAN.sendFrame(output);
}

void sendEStop(){
  CAN_FRAME output;
  output.id = EStop_CANID;
  output.length = MAX_CAN_FRAME;
  output.data.low = 0;
  output.data.high = 0;
  CAN.sendFrame(output);
}

/*---------------------------------------------------------------------------------------*/
/* sending go straight CAN message
/*---------------------------------------------------------------------------------------*/
void goStraight(){
  sendMSG(Straight_speed, Straight_angle);
}

/*---------------------------------------------------------------------------------------*/
/* sending turn CAN message
/*---------------------------------------------------------------------------------------*/
void turn(){
  sendMSG(Turn_speed, Right_turn_angle);
  delay(2000); // $$$ hardcode since compass doesnt work
}

void terminate_process()
{
  sendMSG(0, 0);
  delay(3000);
  sendEStop();
  while (true){}
}

/*---------------------------------------------------------------------------------------*/
/*  below test if we are approaching each corner
/*---------------------------------------------------------------------------------------*/
bool approachTwo(){
  if (abs(TwoLat - latitude) <= offset){
    approaching++;
    return true;
  }
  return false;
}

bool approachThree(){
  if (abs(ThreeLongt - longitude) <= offset){
    approaching++;
    return true;
  }
  return false;
}

bool approachFour(){
  if (abs(FourLat - latitude) <= offset){
    approaching++;
    return true;
  }
  return false;
}

bool approachOne(){
  if (abs(OneLongt - longitude) <= offset){
    approaching++;
    return true;
  }
  return false;
}
