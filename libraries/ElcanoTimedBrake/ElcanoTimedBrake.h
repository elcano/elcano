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
 *	- make a queue of commands for if a command is given before
 *    the current one is completed
 *  - make estop function that will apply brakes regardles of
 *    commands queue
 */




#ifndef _LIB_
#define _LIB_

#include <Settings.h>
// THESE NEED TO BE CHANGED TO MATCH THE PINS AVAILABLE
#define BRAKE_EXTEND 13
#define BRAKE_RETRACT 12
#define BRAKE_EXTEND_TIME 4000 // needs to be calibrated

struct command
{
	int dist = 0;
	char type = 'N';
};

// queue of up to 50 commands
static command commands[50];
static int numCommands = 0;



namespace elcano
{
	// clock speed of arduino mega 16Mhz
	const long CLOCK_SPEED = 16000000l;
	
	volatile static int brakePosition = 0;
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
	
	// ensures that the brake is completely retracted then extends to off position
	// extend time needs to be calibrated
	void setupBrake();
	
	// sets brake position based on the time it would take to reach
	// that position from fully retracted in ms
	void setBrakePos(int);
	
	// sets brakes to the released position as defined by MIN_BRAKE_OUT in Settings.h
	void releaseBrakes();
	
	// sets brakes to the applyed position as defined by MIN_BRAKE_OUT in Settings.h
	void applyBrakes();
	
	// applys brakes and clears commands queue
	void estop();
	
	// executes next command
	void doCommand();
	
	void retractNoQueue(double);
	void extendNoQueue(double);
}
#endif
