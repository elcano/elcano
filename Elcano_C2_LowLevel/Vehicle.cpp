#include "trike_settings.h"
#include "pin_settings.h"
#include <Servo.h>
#include "history.h"
#include "PID_v1.h" 

#include <Arduino.h>
#include "Vehicle.h"

#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_SECOND 2
#define IRQ_RUNNING 3
#define NO_DATA 0xffffffff

Vehicle::Vehicle(){
	distance_mm = 0;
	
}
void Vehicle :: initialize() {
	int MinTickTime_ms = WHEEL_CIRCUM_MM * 1000.0 / MAX_SPEED_mmPs;
	int MaxTickTime_ms = WHEEL_CIRCUM_MM * 1000.0 / MIN_SPEED_mmPs;
	int TickTime = millis();
	int OldTick = TickTime;
	InterruptState = 0;
	history.oldSpeed_mmPs = history.olderSpeed_mmPs = NO_DATA;
	
}

void Vehicle::update() {
	brake.Check();
	//computeSpeed(&history);
	//computeAngle();
	

}

Vehicle::~Vehicle()
{
}

double Vehicle::getAngle() {
	return aSensor.getAngle();
}

void Vehicle :: move(double Angle, double speed, double distance){

	brake.Release();
}

void Vehicle :: compRevolution() {
	noInterrupts();
	unsigned long tick = millis();
	if (InterruptState != IRQ_RUNNING) {
		InterruptState++;
	}
	if (tick - TickTime > MinTickTime_ms) {
		OldTick = TickTime;
		TickTime = tick;
	}
	interrupts();
}

double Vehicle::getSpeed() {
	

}

void Vehicle :: stop(){
	brake.Stop();
}


