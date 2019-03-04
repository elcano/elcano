#include "Settings.h"
#include "ThrottleController.h"
#ifndef Testing
#include <Arduino.h>
#include <PinChangeInterrupt/src/PinChangeInterrupt.h>

#endif

volatile uint32_t ThrottleController::tickTime_ms[2];

ThrottleController::ThrottleController() :
  speedPID(&speedCyclometerInput_mmPs, &PIDThrottleOutput_pwm, &desiredSpeed_mmPs, proportional_throttle, integral_throttle, derivative_throttle, DIRECT)
{
	usePids = false;
}
  
ThrottleController::~ThrottleController(){
}

void ThrottleController::initialize(){
  speedPID.SetOutputLimits(MIN_ACC_OUT, MAX_ACC_OUT);
  speedPID.SetSampleTime(PID_CALCULATE_TIME);
  speedPID.SetMode(AUTOMATIC);
  pinMode(SelectAB, OUTPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  calcTime_ms[0] = 0;
  calcTime_ms[1] = 0;
  prevSpeed_mmPs = 0;
 if (IRPT_WHEEL != 3)
	attachPCINT(digitalPinToPCINT(IRPT_WHEEL), tick, RISING);
 else
	attachInterrupt(digitalPinToInterrupt(IRPT_WHEEL), tick, RISING);//pin 3 on Mega
}

/*
sets the throttle signal to zero
*/
void ThrottleController::stop() {
	engageThrottle(0);
	currentThrottlePWM = 0;
}

void ThrottleController::tick() {
	uint32_t tick = millis();
	noInterrupts();
	if ((tick - tickTime_ms[0]) > MIN_TICK_TIME_ms) {
		tickTime_ms[1] = tickTime_ms[0];
		tickTime_ms[0] = tick;
	}
	interrupts();
}

int32_t ThrottleController::update(int32_t dSpeed) {
	computeSpeed();
	if (usePids)
		ThrottlePID(dSpeed);
	else
		engageThrottle(dSpeed);
	speedCyclometerInput_mmPs;
}


//Private functions

/*
Applies value to adress, producing analog voltage
Address: 0,1,2,3 map to channel a,b,c,d respectivly
Value: digital value converted to analog voltage
output goes to mcp 4802 DAC chip via SPI
no input from chip
Formerly DAC_write
***************************************************
REGISTER 5-3: WRITE COMMAND REGISTER FOR MCP4802 (8-BIT DAC)
		A/B — GA SHDN D7 D6 D5 D4 D3 D2 D1 D0 x x x x
		bit 15 bit 0
		bit 15 A/B: DACA or DACB Selection bit
		1 = Write to DACB
		0 = Write to DACA
		bit 14 — Don’t Care
		bit 13 GA: Output Gain Selection bit
		1 = 1x (VOUT = VREF * D/4096)
		0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
		bit 12 SHDN: Output Shutdown Control bit
		1 = Active mode operation. VOUT is available.
		0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
		VOUT pin is connected to 500 k (typical)
		bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.
		With 4.95 V on Vcc, observed output for 255 is 4.08V.
		This is as documented; with gain of 2, maximum output is 2 * Vref
*/
void ThrottleController::write(int32_t address, int32_t value) {

	int16_t byte1 = ((value & 0xF0) >> 4) | 0x10; 
	int16_t byte2 = (value & 0x0F) << 4;
  if (address == 0){
		digitalWrite(SelectAB, LOW);
			SPI.transfer(byte1);
			SPI.transfer(byte2);
		// take the SS pin high to de-select the chip:
		digitalWrite(SelectAB, HIGH);
  }
	
}



void ThrottleController::ThrottlePID(int32_t desiredValue) {
	if (desiredValue >= (speedCyclometerInput_mmPs + 10)) {
		speedPID.Compute();
		//currentThrottlePWM = (int32_t)PIDThrottleOutput_pwm;
		engageThrottle(PIDThrottleOutput_pwm);
	}
}

/* !UPDATE THIS OBSERVED INFO! (LAST UPDATE: May 10, 2013, TCF)
0.831 V at rest 52 counts
1.20 V: nothing 75
1.27 V: just starting 79
1.40 V: slow, steady 87
1.50 V: brisker 94
3.63 V: max 227 counts
255 counts = 4.08 V
*/
void ThrottleController::engageThrottle(int32_t input) {
  if (input != 0){
    input = map(input, 0, MAX_SPEED_mmPs, MIN_ACC_OUT, MAX_ACC_OUT);
  }
  
	if (input != currentThrottlePWM) {
		write(DAC_CHANNEL, input);
		currentThrottlePWM = input;  // Remember most recent throttle PWM value.

	}
}



int32_t ThrottleController::extrapolateSpeed() {
	int32_t y;
	int32_t t = millis();
	//slope calculation
	y = (speedCyclometerInput_mmPs - prevSpeed_mmPs) / (calcTime_ms[0] - calcTime_ms[1]);
	// * change in time 
	y *= (t - calcTime_ms[0]);
	// + current speed
	y += speedCyclometerInput_mmPs;

	if (y < 0)
		y = 0;
	return y;
}

/*
Uses previous two speeds to extrapolate the current speed
Used to determine when we have stopped
*/
void ThrottleController::computeSpeed() {
	uint32_t tempTick[2];
	noInterrupts();
	tempTick[0] = tickTime_ms[0];
	tempTick[1] = tickTime_ms[1];
	interrupts();
	if (tempTick[1] == 0)
		speedCyclometerInput_mmPs = 0;
	else if (calcTime_ms[0] == 0) {
		speedCyclometerInput_mmPs = WHEEL_CIRCUM_MM * (1000.0 / (tempTick[0] - tempTick[1]));
		prevSpeed_mmPs = speedCyclometerInput_mmPs;
		calcTime_ms[1] = tempTick[1];
		calcTime_ms[0] = tempTick[0];
	}
	else {
		if (calcTime_ms[1] == tempTick[1]) {
			uint32_t timeDiff = millis() - calcTime_ms[0];
			if (timeDiff > MAX_TICK_TIME_ms) {
				speedCyclometerInput_mmPs = 0;
				if (timeDiff > (2 * MAX_TICK_TIME_ms)) {
					prevSpeed_mmPs = 0;
					noInterrupts();
					tickTime_ms[1] = 0;
					interrupts();
				}
			}
			else if (prevSpeed_mmPs > speedCyclometerInput_mmPs) {
				speedCyclometerInput_mmPs = extrapolateSpeed();
			}
		}
		else {
			calcTime_ms[1] = calcTime_ms[0];
			calcTime_ms[0] = tempTick[0];
			prevSpeed_mmPs = speedCyclometerInput_mmPs;
			speedCyclometerInput_mmPs = WHEEL_CIRCUM_MM * (1000.0 / (tempTick[0] - tempTick[1]));
		}
	}
}

