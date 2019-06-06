#include <SPI.h>
#include "mcp_can.h"
#define RCStatus_CANID 0x50
#define HiStatus_CANID 0x100
#define LowStatus_CANID 0x200
#define RCDrive_CANID 0x300
#define HiDrive_CANID 0x350
#define Actual_CANID 0x500

MCP_CAN CAN(49); // chip selection pin for CAN. 53 for mega, 49 for our new low level board

int actual_speed = 0; //to be changed by input from CAN bus
int actual_angle = 0; //to be changed by input from CAN bus

void setup() // intialization CAN bus and serial monitor
{
  Serial.begin(115200);

  while (CAN_OK != CAN.begin(CAN_500KBPS)) // inital CAN bus with 500KBPS baud rate (CAN_500KBPS is the baud rate)
  {
    Serial.println("CAN BUS Shield init fail");
    delay(1000);
  }
  Serial.println("CAN BUS init ok!");
}

//convert 2 32 bits integer to 8 bytes array
typedef union {
  struct {
    uint32_t sspeed;
    uint32_t angle;
  };
  uint8_t bytes[8];
} speedAngleMessage;

//convert 4 byte array to 32 bits integer
typedef union {
  byte arr[4];
  uint32_t integer;
} bufToInt;



void receiveMSG() {
  unsigned char len = 0; // CAN message length 
  unsigned char buf[8];	 // 8 bytes buffer to store CAN message

  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned int canId = CAN.getCanId();
    
    if (canId == HiDrive_CANID) { // the drive ID receive from high level
      Serial.print("Receive DRIVE CAN message from high level with ID: ");
      Serial.println(canId, HEX);
      
	  // reads lower 4 bytes and reformatting to one integer value
      int low_result = (unsigned int)(buf[3]<<24)|(buf[2]<<16)|(buf[1]<<8)|(buf[0]);
      Serial.print("Speed: ");
      Serial.print(low_result, DEC);
      actual_speed = low_result; //change the speed
	   
	  // reads higher 4 bytes and reformatting to one integer value
      int high_result = (unsigned int)(buf[7]<<24)|(buf[6]<<16)|(buf[5]<<8)|(buf[4]);
      Serial.print("  Angle: ");
      Serial.println(high_result, DEC);
      actual_angle = high_result; //change angle
      /*
      bufToInt low_converter;
      low_converter.arr[0] = buf[0];
      low_converter.arr[1] = buf[1];
      low_converter.arr[2] = buf[2];
      low_converter.arr[3] = buf[3];
      Serial.print("Speed: ");
      Serial.print(low_converter.integer, DEC);

      bufToInt high_converter;
      high_converter.arr[0] = buf[4];
      high_converter.arr[1] = buf[5];
      high_converter.arr[2] = buf[6];
      high_converter.arr[3] = buf[7];
      Serial.print("  Angle: ");
      Serial.println(high_converter.integer, DEC);
      */
    }
    else if (canId == HiStatus_CANID) {  //the E-stop ID received from high level
    Serial.print("Receive E-STOP CAN message from high level with ID: ");
    Serial.println(canId, HEX);
    actual_speed = 0;
    Serial.println("STOP!");
    }
  }
}

//send actual speed and angle to high level through can bus
void sendMSG() {
  
  if (actual_speed < 0) { // check if the speed valid
    Serial.println("Speed can not be negative!!! Ignored");
  }
  else {
    
    speedAngleMessage MSG;
  // lower four bytes CAN data 
    MSG.sspeed = actual_speed;
  // higher four bytes CAN data 
    MSG.angle = actual_angle;

    speedAngleMessage MSG2;
    MSG2.sspeed = actual_speed * 2;
    MSG2.angle = actual_angle * 2;
  
  // send two CAN message from low level to high level for testing
    CAN.sendMsgBuf(Actual_CANID, 0, 8, (uint8_t*)&MSG);
    delay(1000); // a proper delay here is necessay, CAN bus need a time to clear the buffer. delay could be 100 minimum
    //CAN.sendMsgBuf(Actual_CANID, 0, 8, (uint8_t*)&MSG2); 
    //delay(1000);
    Serial.println("MEGA Messages SENT spd" + String(MSG.sspeed)+ ", ang: " + String(MSG.angle));
  }
}
void loop(){
  sendMSG();
  delay(1000);
  Serial.println("");
  receiveMSG();
  delay(5000);
}
