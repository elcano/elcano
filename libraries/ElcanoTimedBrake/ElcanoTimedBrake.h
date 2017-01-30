/*
 *	ElcanoTimedBrake.h (WIP)
 *	Written by Jesse Llona
 *	
 *	Description: ElcanoTimedBrake library to use with modified
 *  brake actuator. Estimates position of the brake based on time.
 *	Uses the AVR chip's timer 1
 *  
 *  IMPORTANT NOTE: many types of arduinos use timer 1 for 
 *	build in arduino library functions such as delay() or millis().
 *	Since these functions are used in the library, this library
 *	will not work on arduinos that use timer1 for built in functions
 *	(such as the arduino uno). This WILL work on the arduino Mega.
 * 
 *	For more information on the AVR chips timer and timed interrupts visit:
 *	http://www.avrbeginners.net/architecture/timers/timers.html#ocr
 *	http://www.instructables.com/id/Arduino-Timer-Interrupts/
 *
 *	See timedBrakeGuide.md or timedBrakeGuide.pdf for information
 *	on how to use this library
 *
 *	TODO: 
 *	- CALIBRATE
 	- Change distance to time
 */




#ifndef _LIB_
#define _LIB_

#include <Settings.h>
// THESE NEED TO BE CHANGED TO MATCH THE PINS AVAILABLE
#define BRAKE_EXTEND 8
#define BRAKE_RETRACT 6
// #define BRAKE_EXTEND_TIME 2000 // needs to be calibrated
#define BRAKE_EXTEND_TIME 10000

struct command
{
	int dist = 0;
	char type = 'N';
};

// queue of up to 50 commands
static command commands[50];
static int numCommands = 0;
volatile static int brakePosition = 0;


namespace privateMembers
{
	// clock speed of arduino mega 16Mhz
	const long CLOCK_SPEED = 16000000l;
	// stores pin for extending or retracting
	// for interrupt to know what to write low when done
	static int extendOrRetract;
	
	// sets up the interrupt to be called at a frequency down to .26 Hz
	// (will only be called once, however)
	void setupTimerHz(double);		
	
	// sets up the interrupt to be run after a time in ms up tp 4100 ms
	void setupTimerMillis(double);	
	
	// extends the brake for a time up tp 4100 ms
	void extend(double);			
	
	// retracts the brake for a time up tp 4100 ms
	void retract(double);			
	
	// stops interrupt from being called again
	void stopTimer();				
	
	// sets brake position based on the time it would take to reach
	// that position from fully retracted in ms
	void setBrakePos(int);
	

	

	
	// executes next command
	void doCommand();
	
	void retractNoQueue(double);
	void extendNoQueue(double);
	
}
	
namespace elcano
{
	// sets brakes to the released position as defined by MIN_BRAKE_OUT in Settings.h
	void releaseBrakes();
	
	// sets brakes to the applyed position as defined by MIN_BRAKE_OUT in Settings.h
	void applyBrakes();
	
	// ensures that the brake is completely retracted then extends to off position
	// extend time needs to be calibrated
	void setupBrake();
	
	// applys brakes and clears commands queue
	void estop();
}
#endif
