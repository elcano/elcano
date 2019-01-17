#include "Settings.h"
#include "ThrottleController.h"


ThrottleController::ThrottleController() :
  speedPID(&speedCyclometerInput_mmPs, &PIDThrottleOutput_pwm, &desiredSpeed_mmPs, proportional_throttle, integral_throttle, derivative_throttle, DIRECT)
{}
  
ThrottleController::~ThrottleController(){
}

void ThrottleController::initialize(){
  speedPID.SetOutputLimits(MIN_ACC_OUT, MAX_ACC_OUT);
  speedPID.SetSampleTime(PID_CALCULATE_TIME);
  speedPID.SetMode(AUTOMATIC);
  pinMode(SelectAB, OUTPUT);
  pinMode(SelectCD, OUTPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  tickTime_ms[0] = 0;
  tickTime_ms[1] = 0;
  calcTime_ms[0] = 0;
  calcTime_ms[1] = 0;
  prevSpeed_mmPs = 0;

}

/*
sets the throttle signal to zero
*/
void ThrottleController::stop() {
	engageThrottle(0);
	currentThrottlePWM = 0;
}

/*
decreases the pwm
new signal = previous (1-strength)
if strength > 1, we use 0
*/
void ThrottleController::stop(int32_t strength) {
	if (strength >= 1)
		stop();
	else {
		int32_t decelerateTo = currentThrottlePWM *(1 - (0.1*strength));
		engageThrottle(decelerateTo);
		currentThrottlePWM = decelerateTo;
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

/*
*/
void ThrottleController::updateSpeed() {
	if (tickTime_ms[1] == 0)
		speedCyclometerInput_mmPs = 0;
	else if (calcTime_ms[0] == 0) {
		speedCyclometerInput_mmPs = WHEEL_CIRCUM_MM * (1000.0 / (tickTime_ms[0] - tickTime_ms[1]));
		prevSpeed_mmPs = speedCyclometerInput_mmPs;
		calcTime_ms[1] = tickTime_ms[1];
		calcTime_ms[0] = tickTime_ms[0];
	}
	else {
		if (calcTime_ms[1] == tickTime_ms[1]) {
			uint32_t timeDiff = millis() - calcTime_ms[0];
			if (timeDiff > MAX_TICK_TIME_ms) {
				speedCyclometerInput_mmPs = 0;
				if (timeDiff > (2 * MAX_TICK_TIME_ms)) {
					prevSpeed_mmPs = 0;
					tickTime_ms[1] = 0;
				}
			}
			else if (prevSpeed_mmPs > speedCyclometerInput_mmPs) {
				speedCyclometerInput_mmPs = extrapolateSpeed();
			}
		}
		else {
			calcTime_ms[1] = calcTime_ms[0];
			calcTime_ms[0] = tickTime_ms[0];
			prevSpeed_mmPs = speedCyclometerInput_mmPs;
			speedCyclometerInput_mmPs = WHEEL_CIRCUM_MM * (1000.0 / (tickTime_ms[0] - tickTime_ms[1]));
		}
	}
}

/*
Uses previous two speeds to extrapolate the current speed
Used to determine when we have stopped
*/
void ThrottleController::tick(uint32_t tick) {
	if ((tick - tickTime_ms[0]) > MIN_TICK_TIME_ms) {
		tickTime_ms[1] = tickTime_ms[0];
		tickTime_ms[0] = tick;
	}
}


//Private functions
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
