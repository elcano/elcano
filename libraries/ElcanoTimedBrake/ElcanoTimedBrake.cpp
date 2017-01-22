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
		TCCR1A = 0; // set entire TCCR1A register to 0 
		TCCR1B = 0; // same for TCCR1B
		TCNT1 = 0;  //initialize counter value to 0
		// set compare match register for 1hz increments 
		//clockSpeed/(frequency*prescaler)-1
		OCR1A = (CLOCK_SPEED)/(hz*1024) - 1;
		// turn on CTC mode
		TCCR1B |= (1 << WGM12);
		// Set prescaler 
		TCCR1B |= (1 << CS12)|(1 << CS10);
		// enable timer compare interrupt
		TIMSK1 |= (1 << OCIE1A);
		interrupts();
	}
	  
	void setupTimerMillis(double ms)
	{
		setupTimerHz(1000.0/ms);
	}
	void stopTimer()
	{
		TCCR1B = 0;
	}
	ISR(TIMER1_COMPA_vect)
	{
		noInterrupts();
		digitalWrite(extendOrRetract, LOW);
		stopTimer();
		interrupts();  
	}
	void extend(double ms)
	{
		extendOrRetract = BRAKE_EXTEND;
		Serial.println(extendOrRetract);
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	void retract(double ms)
	{
		extendOrRetract = BRAKE_RETRACT;
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	void setupBrake()
	{
		retract(4000);
		delay(5000);
		extend(500);
	}
}
