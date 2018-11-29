
#include "Settings.h"
#include <SD.h>
#include "ElcanoSerial.h"
#include "Vehicle.h"

using namespace elcano;


Vehicle myTrike = Vehicle();

#define LOOP_TIME_MS 100
unsigned long SectionStart;
int section = 0;

void setup() {

	//setup serial
	Serial.begin(baud);
	SectionStart = floor(millis()/1000); //seconds
	attachInterrupt(digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 3 on Mega
	section = 0;
}

void loop(){
	unsigned long timeStart_ms = millis();
	static long int desired_speed_cmPs, desired_angle;
	static bool e_stop = 0;
	myTrike.update();
	
	unsigned long secs = SectionStart - floor(timeStart_ms / 1000);
	if (secs >= 20) {
		section++;
	}
	if (e_stop) {
		myTrike.eStop();
	}
	else {
		desired_angle = angle(section);
		desired_speed_cmPs = speed(section);
		myTrike.move(desired_angle, desired_speed_cmPs);
	}

	unsigned long delay_ms = millis() - (timeStart_ms + LOOP_TIME_MS);
	if (delay_ms > 0L)
		delay(delay_ms);
}


void WheelRev() {
	noInterrupts();
	myTrike.tick();
	interrupts();
}

long int speed(int section) {
	if (section % 2)
		return 5 * KmPh_mmPs;
	else
		return 3 * KmPh_mmPs;
}

long int angle(int section) {
	if (section % 2 == 0)
		return CENTER_TURN;
	else if (section % 4 == 1)
		return CENTER_TURN + 12;
	else 
		return CENTER_TURN - 12;
}