#include "variant.h"
#include <due_can.h>
#define Sensor_CANID 0x0A
#define Drive_CANID  0X05
#define testWatchFor 0X00
#define MAX_CAN_FRAME_DATA_LEN_16   16

void setup() {
  Serial.begin(115200);
  if (CAN.begin(CAN_BPS_500K)) { // initalize CAN with 500kbps baud rate 
    Serial.println("init success");
  }
}

// receive data from low level sensors
void receiveMSG() {
  //filter the other messages rather than sensor 
  CAN.watchFor(Sensor_CANID);  
  
  while (CAN.available() > 0) { // check if CAN message available
    CAN_FRAME incoming;
    CAN.read(incoming);
    Serial.print("Get data from (high level) ID: ");
    Serial.println(incoming.id, HEX);
    Serial.print("Low: ");
    Serial.print((int)incoming.data.low, DEC);  // read the lower four bytes  
    Serial.print("  High: ");
    Serial.println((int)incoming.data.high, DEC); // read the higher four bytes  
  }
}

// send drive command (speed angle) from high level to low level
void sendMSG(int command_speed, int command_angle) {

  CAN_FRAME output; // intial CAN frame to receive CAN message
  output.id = Drive_CANID; // read CAN ID
  output.length = MAX_CAN_FRAME_DATA_LEN_16; // specify CAN message length
  output.data.low = command_speed;  // write lower four byte on data frame 
  output.data.high = command_angle; // write higher four byte on data frame 
  
  CAN.sendFrame(output); // send the CAN message
  
  Serial.println("DUE SENT MESSAGE!!!");
  delay(1000); // a proper delay here is necessay, CAN bus need a time to clear the buffer. delay could be 100 minimum
}

void loop() {
  sendMSG(1000, -30);
  receiveMSG();
}
