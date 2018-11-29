//#include "Settings.h"
//#include "pin_settings.h"
//#include "trike_settings.h"
#include "Settings.h"
#include <SD.h>
#include "ElcanoSerial.h"
#include "Vehicle.h"

using namespace elcano;


Vehicle myTrike = Vehicle();

#define LOOP_TIME_MS 100


ParseState TxStateHiLevel, RxStateHiLevel, RC_State;   // @@@ cant find parseState Library
SerialData TxDataHiLevel, RxDataHiLevel, RC_Data;	   // @@@ cant find SerailData

void setup(){

	//setup serial
	Serial.begin(baud);
	Serial1.begin(baud);
	Serial2.begin(baud);
	Serial3.begin(baud);
	
	attachInterrupt(digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
	
	//Set up to give data to highlevel
	TxDataHiLevel.clear();
	TxStateHiLevel.dt = &TxDataHiLevel;
	TxStateHiLevel.input = &Serial2;  // not used
	TxStateHiLevel.output = &Serial3;
	TxStateHiLevel.capture = MsgType::sensor;

	//setup for receiving data from High level
	RxDataHiLevel.clear();
	RxStateHiLevel.dt = &RxDataHiLevel;
	RxStateHiLevel.input = &Serial3;
	RxStateHiLevel.output = &Serial2; // not used
	RxStateHiLevel.capture = MsgType::drive;

	// receive data indirectly from RC unit.
	RC_Data.clear();
	RC_State.dt = &RC_Data;
	RC_State.input = &Serial1;
	RC_State.output = &Serial1;  // not used
	RC_State.capture = MsgType::sensor;
	
}

void loop()
{
	unsigned long timeStart_ms = millis();
	static long int desired_speed_cmPs, desired_angle;
	static bool e_stop = 0, auto_mode = 0;
	myTrike.update();
	
	//Communication Section
	if (auto_mode){
		ParseStateError r = RxStateHiLevel.update();
		if (r == ParseStateError::success) {
			desired_speed_cmPs = RxDataHiLevel.speed_cmPs;
			desired_angle = RxDataHiLevel.angle_mDeg;
		}
	}
	
	TxDataHiLevel.speed_cmPs = (myTrike.getSpeed() + 5) / 10;
	TxDataHiLevel.write(TxStateHiLevel.output);

	ParseStateError r = RC_State.update();
	if (r == ParseStateError::success) {
		e_stop = RC_Data.number & 0x01;
		auto_mode = RC_Data.number & 0x02;
		if (!auto_mode)
		{
			desired_speed_cmPs = RC_Data.speed_cmPs;
			desired_angle = RC_Data.angle_mDeg;
		}
	}
	
	if (e_stop){
		myTrike.eStop();
	}
	else{ 
		myTrike.move(desired_angle, desired_speed_cmPs);
	}

	unsigned long delay_ms = millis() - (timeStart_ms + LOOP_TIME_MS);
	if (delay_ms > 0L)
		delay(delay_ms);
}


void WheelRev(){
 noInterrupts();
 myTrike.tick();
 interrupts();
}
/*
int convertHLToTurn(int turnValue)
{
	// TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
	return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US);
	//return map(turnValue, -TURN_MAX_DEG, TURN_MAX_DEG, calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight);
}

int convertHLToSpeed(int speedValue)
{
	// TO DO: FIGURE OUT WHAT MIN MAX VALUES ARE INPUTTED
   // return map(speedValue, 0, MAX_SPEED_CMS, MIN_ACC_OUT, MAX_ACC_OUT);
}
*/