/*
 *	ElcanoTimedBrake.cpp (WIP)
 *	Written by Jesse Llona
 *
 */


#include "ElcanoTimedBrake.h"
#include <arduino.h>


namespace elcano
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
	  
	// converts time in milliseconds to frequency in hz
	// to be sent to setupTimerHz(double hz);
	void setupTimerMillis(double ms)
	{
		setupTimerHz(1000.0/ms);
	}
	
	// clears TCCR1B to stop interrupt from being called after
	void stopTimer()
	{
		TCCR1B = 0;
	}
	
	// extends the brake for a time up tp 4100 ms
	void extend(double ms)
	{
		
		for(int i = ++numCommands; i >= 1; i--)
		{
			commands[i] = commands[i-1];
		}
		commands[0].dist = ms;
		commands[0].type = 'E';
		if(numCommands <= 1)
		{
			doCommand();
			extendOrRetract = BRAKE_EXTEND;
			digitalWrite(extendOrRetract, HIGH);
			setupTimerMillis(ms);
		}
	}
	
	void extendNoQueue(double ms)
	{
		extendOrRetract = BRAKE_EXTEND;
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	
	// retracts the brake for a time up tp 4100 ms
	void retract(double ms)
	{
		for(int i = ++numCommands; i >= 1; i--)
		{
			commands[i] = commands[i-1];
		}
		commands[0].dist = ms;
		commands[0].type = 'R';
		if(numCommands <= 1)
		{
			doCommand();
			extendOrRetract = BRAKE_RETRACT;
			digitalWrite(extendOrRetract, HIGH);
			setupTimerMillis(ms);
		}
	}
	
	void retractNoQueue(double ms)
	{
		extendOrRetract = BRAKE_RETRACT;
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	
	// ensures that the brake is completely retracted then extends to off position
	// extend time needs to be calibrated
	void setupBrake()
	{
		retract(4000);
		extend(MIN_BRAKE_OUT);
	}
	
	// sets brake position based on the time it would take to reach
	// that position from fully retracted in ms
	void setBrakePos(int posMs)
	{
		if(posMs > brakePosition) extend(posMs - brakePosition);
		else if (posMs < brakePosition) retract(brakePosition - posMs);
	}
	
	// sets brakes to the released position as defined by MIN_BRAKE_OUT in Settings.h
	void releaseBrakes()
	{
		setBrakePos(MIN_BRAKE_OUT);
	}
	
	// sets brakes to the applyed position as defined by MIN_BRAKE_OUT in Settings.h
	void applyBrakes()
	{
		setBrakePos(MAX_BRAKE_OUT);
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
		digitalWrite(extendOrRetract, LOW); // whichever pin is moving the brake is set to low
		numCommands--;
		if(commands[numCommands].type == 'R') brakePosition -= commands[numCommands].dist;
		if(commands[numCommands].type == 'E') brakePosition += commands[numCommands].dist;
		if(brakePosition > BRAKE_EXTEND_TIME) brakePosition = BRAKE_EXTEND_TIME;
		if(brakePosition < 0) brakePosition = 0;
		Serial.print("pos: ");
		Serial.println(brakePosition);
		stopTimer();
		
		if(numCommands > 0)
		{
			doCommand();
		}
		interrupts();  
	}
}
