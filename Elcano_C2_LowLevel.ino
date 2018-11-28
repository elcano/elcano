//#include "Settings.h"
#include "pin_settings.h"
#include "trike_settings.h"
#include "PID_v1.h"  
#include <SPI.h>
#include <Servo.h>
#include <SD.h>
#include "ElcanoSerial.h"
#include "Brakes.h"
#include "ThrottleController.h"
#include "SteeringController.h"
#include "history.h"

//Added include vehicle
#include "Vehicle.h"
using namespace elcano;


#define PID_CALCULATE_TIME 50
Vehicle myTrike = Vehicle();
ThrottleController throttle(MIN_ACC_OUT, MAX_ACC_OUT, PID_CALCULATE_TIME, SelectAB, SelectCD, DAC_CHANNEL);
SteeringController steer(WHEEL_MAX_LEFT_US, WHEEL_MAX_RIGHT_US, PID_CALCULATE_TIME, STEER_OUT_PIN);




unsigned long MinTickTime_ms;
unsigned long MaxTickTime_ms;

float SpeedCyclometer_revPs = 0.0;

#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_SECOND 2
#define IRQ_RUNNING 3
#define NO_DATA 0xffffffff
volatile byte InterruptState = IRQ_NONE;  // Tells us if we have initialized.


volatile unsigned long TickTime = 0;  // Time from one wheel rotation to the next gives speed.
volatile unsigned long OldTick = 0;

//All of these still need to be defined
//They should be stored in trike settings probably
int calibratedWheelMaxLeft_us; 
int calibratedWheelStraight_us;
int calibratedWheelMaxRight_us; 

int calibratedWheelSensorMaxLeft; 
int calibratedWheelSensorStraight;
int calibratedWheelSensorMaxRight; 

hist history;


//These definatly stay in this code 
#define LOOP_TIME_MS 100
int baud = 9600;

ParseState TxStateHiLevel, RxStateHiLevel, RC_State;   // @@@ cant find parseState Library
SerialData TxDataHiLevel, RxDataHiLevel, RC_Data;	   // @@@ cant find SerailData

void setup(){
	//attach servo--	--in steeringController Constructor
	//setup spi--		--in throttleController Constructor

	//setup serial
	Serial.begin(baud);
	Serial1.begin(baud);
	Serial2.begin(baud);
	Serial3.begin(baud);
	//Pid params--		--in Controller Constructors
	//setupWheelRev		
		//minTickTime = circum *1000/max_speed
		//maxTickTime = circum *1000/min_speed
		//TickTime = millis();
		//OldTick = TickTime;
		//Istate = 0;
	
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
	
	//set speedInput to 0--	--initialized to 0 in Throttle Controller
}

void loop()
{
	unsigned long timeStart_ms = millis();
	static long int desired_speed_cmPs, desired_angle;
	static bool e_stop = 0, auto_mode = 0;
  myTrike.update();
	if (auto_mode){
		ParseStateError r = RxStateHiLevel.update();
		if (r == ParseStateError::success) {
			desired_speed_cmPs = RxDataHiLevel.speed_cmPs;
			desired_angle = RxDataHiLevel.angle_mDeg;
		}
	}
	computeSpeed(&history);
	computeAngle();
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
		steer.setDesiredTurn(convertHLToTurn(desired_angle));
		myTrike.move(desired_angle, desired_speed_cmPs);
	}

	unsigned long delay_ms = millis() - (timeStart_ms + LOOP_TIME_MS);
	if (delay_ms > 0L)
		delay(delay_ms);
}

void setupWheelRev(){
 MinTickTime_ms = (WHEEL_CIRCUM_MM * 1000.0) / MAX_SPEED_mmPs;
 MaxTickTime_ms = (WHEEL_CIRCUM_MM * 1000.0) / MIN_SPEED_mmPs;
 TickTime = millis();
 OldTick = TickTime;
 InterruptState = IRQ_NONE;
 history.oldSpeed_mmPs = NO_DATA;
 attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
}

void WheelRev(){
 unsigned long tick;
 noInterrupts();
 tick = millis();
 if(InterruptState != IRQ_RUNNING){
   InterruptState++;
 }
 
 if((tick - TickTime) > MinTickTime_ms){
   OldTick = TickTime;
   TickTime = tick;
 }
 interrupts();
}

void computeSpeed(struct hist *data){
	unsigned long WheelRev_ms = TickTime - OldTick;
	float SpeedCyclometer_revPs = 0.0; //revolutions per sec
	if ((InterruptState == 0) || (InterruptState == 1))
	{ // No data
		throttle.setSpeedInput_mmPs(0);
		SpeedCyclometer_revPs = 0;
	}
	else if (InterruptState == 2)
	{ //  first computed speed
		SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
		double speed = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
		throttle.setSpeedInput_mmPs(WHEEL_CIRCUM_MM * SpeedCyclometer_revPs);
		data->oldSpeed_mmPs  = speed;
		data->oldTime_ms = OldTick;
		data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
	}
	else if (InterruptState == 3){ //  new data for second and following computed speeds
		if (TickTime == data->nowTime_ms){//no new data
		  //check to see if stopped first
			unsigned long timeStamp = millis();
			if ((timeStamp - data->nowTime_ms) > MaxTickTime_ms){ // too long without getting a tick
				throttle.setSpeedInput_mmPs(0);
				SpeedCyclometer_revPs = 0;
				if ((timeStamp - data->nowTime_ms) > (2 * MaxTickTime_ms)){
					InterruptState = 1;  //  Invalidate old data
					data->oldSpeed_mmPs = NO_DATA;
				}
				return;
			}
			if (data->oldSpeed_mmPs >throttle.getSpeedInput_mmPs()){ 
				throttle.setSpeedInput_mmPs(throttle.getSpeedInput_mmPs() + data->oldSpeed_mmPs*(1 - timeStamp + data->nowTime_ms));
				if (throttle.getSpeedInput_mmPs() < 0){
					throttle.setSpeedInput_mmPs(0);
				}
				SpeedCyclometer_revPs = (throttle.getSpeedInput_mmPs() / WHEEL_CIRCUM_MM);
			}
		}
		else // data is different from last
		{
			//update time block
			data->olderTime_ms = data->oldTime_ms;
			data->oldTime_ms = data->nowTime_ms;
			data->nowTime_ms = TickTime;

			//update speed block
			data->oldSpeed_mmPs = throttle.getSpeedInput_mmPs();
			SpeedCyclometer_revPs = (1000.0 / WheelRev_ms);
			throttle.setSpeedInput_mmPs(WHEEL_CIRCUM_MM * SpeedCyclometer_revPs);

			data->oldTickMillis = data->tickMillis;
			data->tickMillis = millis();

			data->currentSpeed_kmPh = throttle.getSpeedInput_mmPs() / 260.0;
			myTrike.distance_mm += ((data->oldTime_ms - data->olderTime_ms) / 1000.0) * (data->oldSpeed_mmPs);

			if (data->TickTime_ms - data->OldTick_ms > 1000)
			{
				data->currentSpeed_kmPh = 0;
			}
		}
	}
}



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


void computeAngle(){
	steer.setSteerAngleUS(map(analogRead(A3), calibratedWheelSensorMaxLeft, calibratedWheelSensorMaxRight, calibratedWheelMaxLeft_us, calibratedWheelMaxRight_us));
}
