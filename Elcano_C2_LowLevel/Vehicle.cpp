#include "Settings.h"
#include "Vehicle.h"
#include <mcp_can.h>
#ifndef TESTING
#include <Arduino.h>
#include <PinChangeInterrupt.h>
#endif
//********* current CAN Protocol (4-23-19)************* 
#define RCStatus_CANID 0x50
#define HiStatus_CANID 0x100
#define LowStatus_CANID 0x200
#define RCDrive_CANID 0x300
#define HiDrive_CANID 0x350
#define Actual_CANID 0x500
//*****************************************************
volatile int32_t Vehicle::desired_speed_cmPs;
volatile int32_t Vehicle::desired_angle;
Brakes Vehicle::brake;
ThrottleController Vehicle::throttle;
MCP_CAN CAN(CAN_SS); // pin for CS on Mega

//Constructor
Vehicle::Vehicle(){
  desired_angle = 0;
	desired_speed_cmPs = 0;
	
  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
	  if (DEBUG) {
		  Serial.println("CAN BUS Shield init fail");
	  }
    delay(1000);
  }
  if(DEBUG)
		Serial.println("CAN BUS init ok!");

	 //attachPCINT(digitalPinToPCINT(IRPT_ESTOP), eStop, RISING);
   //attachPCINT(digitalPinToPCINT(IRPT_CAN), recieveCan, RISING);
}

//Destructor
Vehicle::~Vehicle(){
}

//Struct for sending current speed and angle to high-level board through CAN
typedef union{
		struct{
			uint32_t sspeed;
			uint32_t angle;
		};
	}speedAngleMessage;

/*
 * Checks for receipt of new CAN message and updates current 
 * Vehicle speed and steering angle by passing code received from CAN
 * from RC or high-level board to the throttle and steering controllers
 */
void Vehicle::update() {
  recieveCan();
  if(DEBUG)
    Serial.println("Vehicle update");
	int32_t tempDspeed;
	int32_t tempDangle;
	noInterrupts();
	tempDspeed = desired_speed_cmPs;
	tempDangle = desired_angle;
	interrupts();
 if(DEBUG) {
    Serial.println("Desired Speed cms: " + String(tempDspeed) + ", CurrentSpeed: " + String(currentSpeed));
  }
	brake.Update();
	int32_t tempcurrentSpeed = throttle.update(tempDspeed);
	currentAngle = steer.update(tempDangle);
  if(DEBUG){
    if(tempcurrentSpeed != currentSpeed){
    Serial.print("Actual Speed: ");
    Serial.print(currentSpeed/10);
    Serial.print(",  Changing to: ");
    Serial.println(tempcurrentSpeed/10);
    }
  }
  if(DEBUG) {
    Serial.println("2nd: tempSpd: " + String(tempcurrentSpeed) + ", Curr: " + String(currentSpeed));
  }
  currentSpeed = tempcurrentSpeed;
  
//If speed not zero, send current speed and angle to high-level for processing
	if(currentSpeed< 0) //stopped
		return;
	else{ 
		speedAngleMessage MSG;
		MSG.sspeed = currentSpeed;
		MSG.angle = currentAngle;
		CAN.sendMsgBuf(Actual_CANID, 0,8, (uint8_t*)&MSG);
	}
}

/*
 * Checks for receipt of a message from CAN bus for new 
 * speed/angle/brake instructions from RC or high-level board
 */
void Vehicle::recieveCan() {  //need to ADD ALL the other CAN IDs possible (RC instructions etc. 4-23-19)
	noInterrupts();
	unsigned char len = 0;
  unsigned char buf[8];
	
  if (CAN_MSGAVAIL == CAN.checkReceive()){  //found new instructions
      
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned int canId = CAN.getCanId();
    
    if (canId == HiDrive_CANID) { // the drive ID receive from high level 
		  if (DEBUG) 
			  Serial.println("RECEIVE DRIVE CAN MESSAGE FROM HIGH LEVEL WITH ID: " + String(canId, HEX));
		  
      int low_result = (unsigned int)(buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | (buf[0]);
     
      desired_speed_cmPs = low_result;
		  if (DEBUG) {
			  Serial.println("CAN Speed Dec: " + String(low_result, DEC) + " Std: " + String(low_result));
		  }
      Serial.print("desired Speed in cms: ");
      Serial.println(desired_speed_cmPs);
      int high_result = (unsigned int)(buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | (buf[4]);
      desired_angle= map(high_result,0,255,-90000,90000);
		  if(DEBUG){
			  Serial.print("  CAN Angle: ");
			  Serial.println(high_result, DEC);
        Serial.println("mapped angle: " + String(desired_angle));
		  }
    }
		
		else if(canId == HiStatus_CANID){ //High-level Status change (just e-stop for now 4/23/19)
      desired_speed_cmPs = 0;
			eStop();
    }
  }
	interrupts();
}

//Estop method for high or RC calls
void Vehicle::eStop() {
  if(DEBUG)
    Serial.println("E-Stop!");
  noInterrupts();
  brake.Stop();
  throttle.stop();
  interrupts();
}
