#include "variant.h"
#include <due_can.h>
#define HiStatus_CANID 0x100
#define LowStatus_CANID 0x200
#define HiDrive_CANID 0x350
#define Actual_CANID 0x500
#define MAX_CAN_FRAME_DATA_LEN_16   16

int code = 0; //added for testing different messages
int value = 0; //added for testing different messages

void setup() {
  Serial.begin(115200);
  if (CAN.begin(CAN_BPS_500K)) { // initalize CAN with 500kbps baud rate 
    Serial.println("init success");
  }
}

// receive data from low level sensors
void receiveMSG() {
  //filter the other messages rather than sensor 
  CAN.watchForRange(Actual_CANID, LowStatus_CANID);  
  
  while (CAN.available() > 0) { // check if CAN message available
    CAN_FRAME incoming;
    CAN.read(incoming);
    Serial.print("Get data from (low level) ID: ");
    Serial.println(incoming.id, HEX);
    Serial.print("Low: ");
    Serial.print((int)incoming.data.low, DEC);  // read the lower four bytes  
    Serial.print("  High: ");
    Serial.println((int)incoming.data.high, DEC); // read the higher four bytes  
    Serial.println("");
  }
}


//test code for sending a variety of outputs to lowlevel board through CAN
void sendMSG() {
  CAN_FRAME output; //initial CAN frame to add CAN message
  output.length = MAX_CAN_FRAME_DATA_LEN_16; // specify CAN message length
  if(code % 5 != 0) { //DRIVE Code
    output.id = HiDrive_CANID;
    if(value % 3 !=0) { //common speed angle
      output.data.low = 9500; //Speed, lower 4 bytes on data frame
      output.data.high = 27; //Angle, higher 4 bytes on data frame
    }
    else { //less common, change
      output.data.low = 100; //Speed, lower 4 bytes on data frame
      output.data.high = -40; //Angle, higher 4 bytes on data frame
    }
  }
  else { //Status Code (E-stop)
   output.id = HiStatus_CANID;
   output.data.low = 10; //estop data 
  }
  CAN.sendFrame(output); //send the Message
  Serial.print("DUE sent msg: ");
  Serial.println(output.id, HEX);
  code++;
  value++;
  delay(1000); // a proper delay here is necessay, CAN bus need a time to clear the buffer. delay could be 100 minimum
}


void loop() {
  sendMSG();
  delay(1000);
  Serial.println("");
  receiveMSG();
  delay(5000);
}
