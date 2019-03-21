#include "variant.h"
#include <due_can.h>
#define Sensor_CANID 0x0A
#define Drive_CANID  0X05
#define testWatchFor 0X00
#define MAX_CAN_FRAME_DATA_LEN_16   16

void setup() {
  Serial.begin(115200);
  if (CAN.begin(CAN_BPS_500K)) {
    Serial.println("init success");
  }
}

// receive data from low level sensors
void receiveMSG() {
  //filter the other messages rather than sensor 
  CAN.watchFor(Sensor_CANID);  
  
  while (CAN.available() > 0) {
    CAN_FRAME incoming;
    CAN.read(incoming);
    Serial.print("Get data from (high level) ID: ");
    Serial.println(incoming.id, HEX);
    Serial.print("Low: ");
    Serial.print((int)incoming.data.low, DEC);
    Serial.print("  High: ");
    Serial.println((int)incoming.data.high, DEC);
  }
}

// send drive command (speed angle) from high level to low level
void sendMSG(int command_speed, int command_angle) {

  CAN_FRAME output;
  output.id = Drive_CANID;
  output.length = MAX_CAN_FRAME_DATA_LEN_16;
  output.data.low = command_speed; 
  output.data.high = command_angle;
  
  CAN.sendFrame(output);
  Serial.println("DUE SENT MESSAGE!!!");
  delay(1000);
}

void loop() {
  sendMSG(1000, -30);
  //receiveMSG();
}
