#include "Settings.h"
#include "Vehicle.h"


Vehicle myTrike = Vehicle();

//Timing stuff
#define LOOP_TIME_MS 100
unsigned long nextTime;
unsigned long endTime;
unsigned long delayTime;
#define ULONG_MAX 0x7FFFFFFF

//Do we want to print to the serial port?
//Is a define so that it is constant in all code
#define SerialPrint false


void setup(){

	attachInterrupt(digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
	
	myTrike.initialize();
	if (SerialPrint)
		Serial.println("Setup complete");
}

void loop()
{
	//Timing code
	nextTime = nextTime + LOOP_TIME_MS;
	unsigned long timeStart_ms = millis();

	//Checks if the brakes are on too long
	//If enabled computes the current angle and speed
	myTrike.update();

	
  /***********START OF Communication Section********************************/
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
