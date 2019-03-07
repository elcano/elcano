#include "Settings.h"
#include "Vehicle.h"
#include <mcp_can.h>
#ifndef TESTING
#include <Arduino.h>
#include <PinChangeInterrupt.h>
#endif

#define Drive_CANID 0X05
#define EStop_CANID 0x00
#define FEEDBACKLOW_CANID 0x0A
volatile int32_t Vehicle::desired_speed_cmPs;
volatile int32_t Vehicle::desired_angle;
Brakes Vehicle::brake;
ThrottleController Vehicle::throttle;
SteeringController Vehicle::steer;
MCP_CAN CAN(CAN_SS); // pin for CS on Mega


Vehicle::Vehicle(){
    desired_angle = 0;
	desired_speed_cmPs = 0;
	
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

	attachPCINT(digitalPinToPCINT(IRPT_ESTOP), eStop, RISING);
    attachPCINT(digitalPinToPCINT(IRPT_CAN), recieveCan, RISING);
}


Vehicle::~Vehicle(){
}


void Vehicle::eStop() {
	noInterrupts();
	brake.Stop();
	throttle.stop();
	interrupts();
}



typedef union{
		struct{
			uint32_t sspeed;
			uint32_t angle;
		};
	}speedAngleMessage;


void Vehicle::update() {
	int32_t tempDspeed;
	int32_t tempDangle;
	noInterrupts();
	tempDspeed = desired_speed_cmPs;
	tempDangle = desired_angle;
	interrupts();
	brake.Update();
	currentSpeed = throttle.update(tempDspeed);
	currentAngle = steer.update(tempDangle);
	if (tempDspeed < (currentSpeed*10))
		brake.Stop();
	else 
		brake.Release();

	if(currentSpeed< 0)
		return;
	else{
		speedAngleMessage MSG;
		MSG.sspeed = currentSpeed;
		MSG.angle = currentAngle;
		CAN.sendMsgBuf(FEEDBACKLOW_CANID, 0,8, (uint8_t*)&MSG);
	}
}




void Vehicle::recieveCan() {
	noInterrupts();
	unsigned char len = 0;
    unsigned char buf[8];
	
	//estop can message
	
    if (CAN_MSGAVAIL == CAN.checkReceive()){
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
		if(canId == EStop_CANID){
			eStop();
      }
    }

	interrupts();
}
}