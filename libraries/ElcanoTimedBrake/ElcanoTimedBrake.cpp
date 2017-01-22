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
		brakePosition += ms;
		extendOrRetract = BRAKE_EXTEND;
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	
	// retracts the brake for a time up tp 4100 ms
	void retract(double ms)
	{
		brakePosition -= ms;
		extendOrRetract = BRAKE_RETRACT; 
		digitalWrite(extendOrRetract, HIGH);
		setupTimerMillis(ms);
	}
	
	// ensures that the brake is completely retracted then extends to off position
	// extend time needs to be calibrated
	void setupBrake()
	{
		retract(4000);
		delay(5000);
		extend(MIN_BRAKE_OUT);
	}
	
	// sets brake position based on the time it would take to reach
	// that position from fully retracted in ms
	void setBrakePos(int posMs)
	{
		if(posMs > brakePosition) extend(posMs - brakePosition);
		else if (posMs < brakePosition) retract(brakePosition - posMs);
	}
	
	// interrupt to be called once the brakes movement time is up
	ISR(TIMER1_COMPA_vect)
	{
		noInterrupts();
		digitalWrite(extendOrRetract, LOW); // whichever pin is moving the brake is set to low
		if(brakePosition > BRAKE_EXTEND_TIME) brakePosition = BRAKE_EXTEND_TIME;
		if(brakePosition < 0) brakePosition = 0;
		Serial.println(brakePosition);
		stopTimer();
		interrupts();  
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
}
