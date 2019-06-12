/*
 *	ElcanoTimedBrake.cpp (WIP)
 *	Written by Jesse Llona
 *
 */


#include "ElcanoTimedBrake.h"
#include <arduino.h>
// #define DEBUG
namespace privateMembers
{
	/*
	 * input min: .24 hz
	 * (range may be expandable by adjusing prescaler (currently 1024) based on input)
	 */
	void setupTimerHz(double hz)
	{
		noInterrupts();
		// sets TCCR1A register to 0
		// Disconnect Pin OC1 from Timer/Counter 1
		// PWM operation disabled
		TCCR1A = 0;
		
		// clears TCCR1B register
		TCCR1B = 0;
		
		// resets counter
		// counter increments on each processor tick
		TCNT1 = 0;
		
		
		// clockSpeed/(frequency*prescaler)-1
		// an interrupt will be run once TCNT1 is equal to OCR1A
		OCR1A = (CLOCK_SPEED)/(hz*1024) - 1;
		
		// turn on CTC mode
		TCCR1B |= (1 << WGM12);
		// Set prescaler 
		TCCR1B |= (1 << CS12)|(1 << CS10);
		
		// enable timer compare interrupt
		TIMSK1 |= (1 << OCIE1A);
		interrupts();
	}
	  
	// clears TCCR1B to stop interrupt from being called after
	void stopTimer()
	{
		TCCR1B = TCCR1B & 0xF7;
	}
	  
	// converts time in milliseconds to frequency in hz
	// to be sent to setupTimerHz(double hz);
	void setupTimerMillis(double ms)
	{
		setupTimerHz(1000.0/ms);
	}
	

	
	// extends the brake for a time up tp 4100 ms
	void extend(double ms)
	{
		Serial.println("extend");
		for(int i = ++numCommands; i >= 1; i--)
		{
			commands[i] = commands[i-1];
		}
		brakePosition += ms;
		commands[0].dist = ms;
		commands[0].type = 'E';
		if(numCommands <= 1)
		{
			extendNoQueue(ms);
		}
	}
	
	void extendNoQueue(double ms)
	{
		extendOrRetract = BRAKE_EXTEND;
		#ifdef DEBUG
		digitalWrite(13, HIGH);
		#endif
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	
	// retracts the brake for a time up tp 4100 ms
	void retract(double ms)
	{
		Serial.println("retract");
		for(int i = ++numCommands; i >= 1; i--)
		{
			commands[i] = commands[i-1];
		}
		brakePosition -= ms;
		commands[0].dist = ms;
		commands[0].type = 'R';
		if(numCommands <= 1)
		{
			retractNoQueue(ms);
		}
	}
	
	void retractNoQueue(double ms)
	{
		extendOrRetract = BRAKE_RETRACT;
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	
	// sets brake position based on the time it would take to reach
	// that position from fully retracted in ms
	void setBrakePos(int posMs)
	{
			Serial.println("Starting pos: " + String(brakePosition));
			if(posMs > brakePosition) extend(posMs - brakePosition);
			else if (posMs <= brakePosition) retract(brakePosition - posMs);
	}
	


	
	// executes next command
	void doCommand()
	{
			switch(commands[numCommands].type)
			{
			case 'e':
			case 'E':
				extendNoQueue(commands[numCommands].dist);
				break;
			case 'r':
			case 'R':
				retractNoQueue(commands[numCommands].dist);
				break;
		}
	}
	
	// interrupt to be called once the brakes movement time is up
	ISR(TIMER1_COMPA_vect)
	{
		noInterrupts();
		digitalWrite(BRAKE_EXTEND, LOW);
		digitalWrite(BRAKE_RETRACT, LOW);
		delay(20);
		#ifdef DEBUG
		digitalWrite(13, LOW);
		#endif
		numCommands--;
		if(brakePosition > BRAKE_EXTEND_TIME) brakePosition = BRAKE_EXTEND_TIME;
		if(brakePosition < 0) brakePosition = 0;
		// Serial.print("pos: ");
		Serial.println(brakePosition);
		stopTimer();
		
		if(numCommands >= 0)
		{
			doCommand();
		}
		interrupts();  
	}
}
namespace elcano
{

	
	void releaseBrakes()
	{
			privateMembers::setBrakePos(MIN_BRAKE_OUT);
	}
	
	// sets brakes to the applyed position as defined by MIN_BRAKE_OUT in Settings.h
	void applyBrakes()
	{
			privateMembers::setBrakePos(MAX_BRAKE_OUT);
	}
	
	// ensures that the brake is completely retracted then extends to off position
	// extend time needs to be calibrated
	void setupBrake()
	{
		privateMembers::retract(4000);
		privateMembers::extend(MIN_BRAKE_OUT);
	}
	
	
	// applys brakes and clears commands queue
	void estop()
	{
		for(int i = 0; i < 50; i++)
		{
			commands[i].type = ' ';
			commands[i].dist = 0;
			numCommands = 0;
		}
		applyBrakes();
	}
}
