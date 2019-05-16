#include "Settings.h"
#include "Vehicle.h"
#include <mcp_can.h>
#include <Can_Protocol.h>
#ifndef TESTING
#include <Arduino.h>
#include <PinChangeInterrupt.h>
#endif

volatile int32_t Vehicle::desired_speed_mmPs;
volatile int32_t Vehicle::desired_angle;

Brakes Vehicle::brake;
ThrottleController Vehicle::throttle;
MCP_CAN CAN(CAN_SS); // pin for CS on Mega

/****************************************************************************
 * Constructor
 ****************************************************************************/
Vehicle::Vehicle(){
  desired_angle = 0;
	desired_speed_mmPs = 0;
	
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

/*****************************************************************************
 * Destructor
 ****************************************************************************/
Vehicle::~Vehicle(){
}


/*****************************************************************************
 * Struct for sending current speed and angle to high-level board through CAN
 ****************************************************************************/
typedef union{
		struct{
			uint32_t sspeed;
			uint32_t angle;
		};
	}speedAngleMessage;

/*******************************************************************************************************
 * Checks for receipt of new CAN message and updates current 
 * Vehicle speed and steering angle by passing code received from CAN
 * from RC or high-level board to the throttle and steering controllers
 *******************************************************************************************************/
void Vehicle::update() {
  recieveCan(); //check for new message
  int32_t tempDspeed;
  int32_t tempDangle;
  
  noInterrupts();
  tempDspeed = desired_speed_mmPs;  
  tempDangle = desired_angle;
  interrupts();
 
  if(DEBUG) {
    int32_t desA = map(tempDangle,-90000,90000,-90,90);
    int32_t crnA = map(currentAngle,-90000,90000,-90,90);
    Serial.println("DesrdSpd mms: " + String(tempDspeed) + ", CurntSpd: " + String(currentSpeed) + 
              "| DesrdAngl " + String(desA) + ", CrntAngl " + String(crnA));
  }

//*******************************************************************************************
//CHOOSE between the 2 below based on test or actually running the system
  
  //hard_Coded_Test(tempDspeed, tempDangle); //test only, no vehicle results
  real_System(tempDspeed, tempDangle); //real system when using bike, not test
  
//*******************************************************************************************
  
 
  //If speed not less than zero, send current speed and angle to high-level for processing
  if(currentSpeed < 0) //stopped
    return;

   //build struct to send data to Highlevel through CAN
    speedAngleMessage MSG;
    MSG.sspeed = currentSpeed;  
    MSG.angle =  map(currentAngle,-90000,90000,-90,90);
    CAN.sendMsgBuf(Actual_CANID, 0,8, (uint8_t*)&MSG);
    delay(1000);
  
    if(DEBUG)
      Serial.println("Sending Message to DUE");
}

/********************************************************************************************************
 * Actual system for executing results with bike
 *******************************************************************************************************/
void Vehicle::real_System(int32_t tempDspeed, int32_t tempDangle ) {
	brake.Update();  
  int32_t tempcurrentSpeed = throttle.update(tempDspeed);
  currentAngle = steer.update(tempDangle); 
  if(DEBUG){
    if(tempcurrentSpeed != currentSpeed){
    Serial.println("Actual Speed: " + String(currentSpeed) + ",  Changing to: " + String(tempcurrentSpeed));
    }
  }
  currentSpeed = tempcurrentSpeed;
}

/********************************************************************************************************
 * Hard coded to give back feedback to high level as if
 * the lowlevel had responded to the speed and angle requested by Highlevel board
 *******************************************************************************************************/
void Vehicle::hard_Coded_Test(int32_t tempDspeed, int32_t tempDangle) {
  currentSpeed = tempDspeed;
  currentAngle = tempDangle;  
}

/*************************************************************************************
 * Checks for receipt of a message from CAN bus for new 
 * speed/angle/brake instructions from RC or high-level board
 ************************************************************************************/
void Vehicle::recieveCan() {  //need to ADD ALL the other CAN IDs possible (RC instructions etc. 4-23-19)
	noInterrupts();
	unsigned char len = 0;
  unsigned char buf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive()){  //found new instructions
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned int canId = CAN.getCanId();
    if (canId == HiDrive_CANID) { // the drive ID receive from high level 
		  if (DEBUG) 
			  Serial.println("RECEIVED CAN MESSAGE FROM HIGH LEVEL WITH ID: " + String(canId, HEX));

      int low_result = (unsigned int)(buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | (buf[0]);
     
      desired_speed_mmPs = low_result;
		 
      int high_result = (unsigned int)(buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | (buf[4]);
      
      desired_angle= map(high_result,-90,90,-90000,90000); //map to larger range for steering
		  if(DEBUG){
        Serial.print("CAN Speed: " + String(low_result, DEC));
        Serial.print(",  CAN Angle: ");
        Serial.println(high_result, DEC);
        Serial.println("mapped angle: " + String(desired_angle));
		  }
    }		
		else if(canId == HiStatus_CANID){ //High-level Status change (just e-stop for now 4/23/19)
      desired_speed_mmPs = 0;
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
