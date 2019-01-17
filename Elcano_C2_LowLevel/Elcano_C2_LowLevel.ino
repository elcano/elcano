#include "Settings.h"
#include "Vehicle.h"
#include "ElcanoSerial.h"

using namespace elcano;

Vehicle myTrike = Vehicle();
//myTrike.move(angle, speed) uses PIDS
//myTrike.noPID(angle,speed) does not use PIDS


//Timing stuff
#define LOOP_TIME_MS 100
uint32_t nextTime;
uint32_t endTime;
uint32_t delayTime;
#define ULONG_MAX 0x7FFFFFFF

//Do we want to print to the serial port?
//Is a define so that it is constant in all code

  /***********START OF Communication Section********************************/
ParseState TxState, TxStateHiLevel, RxStateHiLevel, RC_State;
SerialData TxDataHiLevel, RxDataHiLevel, RC_Data;
  /***********END OF Communication Section**********************************/


void setup(){

	attachInterrupt(digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
	
	myTrike.initialize();
	if (DEBUG)
		Serial.println("Setup complete");
  /***********START OF Communication Section********************************/
    TxDataHiLevel.clear();
    TxStateHiLevel.dt = &TxDataHiLevel;
    TxStateHiLevel.input = &Serial2; // not used
    TxStateHiLevel.output = &Serial3;
    TxStateHiLevel.capture = MsgType::sensor;

     //setup for receiving data from High level
    RxDataHiLevel.clear();
    RxStateHiLevel.dt  = &RxDataHiLevel;
    RxStateHiLevel.input = &Serial3;
    RxStateHiLevel.output = &Serial2; // not used
    RxStateHiLevel.capture = MsgType::drive;

    // receive data indirectly from RC unit.
    RC_Data.clear();
    RC_State.dt  = &RC_Data;
    RC_State.input = &Serial1;
    RC_State.output = &Serial1;  // not used
    RC_State.capture = MsgType::drive; // match to RadioControl.INO
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
  static bool e_stop = false, auto_mode=false;
  //get data from serial
  //get desired steering and speed
  if(auto_mode){
	  ParseStateError r = RxStateHiLevel.update();
	  if(r==ParseStateError::success){
		  desired_speed_cmPs=RxDataHiLevel.speed_cmPs;
		  desired_angle=RxDataHiLevel.angle_mDeg;
	  }
  }
  
  //get data from RC unit
  ParseStateError r = RC_State.update();
  if(r==ParseStateError::success){
	  e_stop = RC_Data.number & 0x01;
	  auto_mode = RC_Data.number & 0x02;
	if(!auto_mode){
		desired_speed_cmPs = RC_Data.speed_cmPs;
		desired_angle = RC_Data.angle_mDeg;
	}
  }
 
 //Updated emergency brake section
	if(e_stop)
		myTrike.eStop();
	else{
		if(!auto_mode && desired_speed_cmPs < -10)
			myTrike.eStop();
		else 
			myTrike.move(desired_angle,desired_speed_cmPs);
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


void WheelRev(){
 noInterrupts();
 myTrike.tick();
 interrupts();
}
