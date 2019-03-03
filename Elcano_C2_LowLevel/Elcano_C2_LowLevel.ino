#include <SPI.h>
#include "Settings.h"
#include "Vehicle.h"
#include "mcp_can.h"
MCP_CAN CAN(53); // pin for CS on Mega

Vehicle myTrike = Vehicle();
//myTrike.move(angle, speed) uses PIDS
//myTrike.noPID(angle,speed) does not use PIDS

//Timing stuff
#define LOOP_TIME_MS 100
uint32_t nextTime;
uint32_t endTime;
uint32_t delayTime;
#define ULONG_MAX 0x7FFFFFFF

#define Drive_CANID 0X05

void setup() {
  myTrike.initialize();
  if (DEBUG) {
    Serial.println("Setup complete");
  }
  /***********START OF Communication Section********************************/
  // CAN TEST START HERE
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
	  if (DEBUG) {
		  Serial.println("CAN BUS Shield init fail");
	  }
    delay(1000);
  }
  if(DEBUG)
		Serial.println("CAN BUS init ok!");
  /***********END OF Communication Section**********************************/
}

void loop()
{
  //Timing code
  nextTime = nextTime + LOOP_TIME_MS;
  uint32_t timeStart_ms = millis();

  //Checks if the brakes are on too long
  //If enabled computes the current angle and speed
  myTrike.update();


  /***********START OF Communication Section********************************/
  //This is where the communication section goes.
  //Copied communication code from elcano/elcano/
  static int32_t desired_speed_cmPs, desired_angle;
  static bool e_stop = false, auto_mode = false;

  if (auto_mode) {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
      CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
      unsigned int canId = CAN.getCanId();

      if (canId == Drive_CANID) { // the drive ID receive from high level
		  if (DEBUG) {
			  Serial.print("RECEIVE DRIVE CAN MESSAGE FROM HIGH LEVEL WITH ID: ");
			  Serial.println(canId, HEX);
		  }
        int low_result = (unsigned int)(buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | (buf[0]);
        desired_speed_cmPs = low_result;
		if (DEBUG) {
			Serial.print("Speed: ");
			Serial.print(low_result, DEC);
		}

        int high_result = (unsigned int)(buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | (buf[4]);
        desired_angle = high_result;
		if(DEBUG){
        Serial.print("  Angle: ");
        Serial.println(high_result, DEC);
		}
      }
    }
  }

  //Updated emergency brake section
  if (e_stop)
    myTrike.eStop();
  else {
    if (!auto_mode && desired_speed_cmPs < -10)
      myTrike.eStop();
    else
      myTrike.move(desired_angle, desired_speed_cmPs);
  }
  /***********END OF Communication Section**********************************/

  //Timing code
  endTime = millis();
  delayTime = 0UL;
  if ((nextTime >= endTime) && (((endTime < LOOP_TIME_MS) && (nextTime < LOOP_TIME_MS)) || ((endTime >= LOOP_TIME_MS) && (nextTime >= LOOP_TIME_MS)))) {
    delayTime = nextTime - endTime;
    if (nextTime < LOOP_TIME_MS)
      delayTime = ULONG_MAX - endTime + nextTime;
    else {
      nextTime = endTime;
      delayTime = 0UL;
    }
  }
  if (delayTime > 0UL)
    delay(delayTime);
}
