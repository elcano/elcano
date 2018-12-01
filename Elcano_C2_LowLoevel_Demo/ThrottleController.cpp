#include "Settings.h"
#include "ThrottleController.h"


ThrottleController::ThrottleController() :
  speedPID(&speedCyclometerInput_mmPs, &PIDThrottleOutput_pwm, &desiredSpeed_mmPs, proportional_throttle, integral_throttle, derivative_throttle, DIRECT)
{}
  

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

ThrottleController::~ThrottleController()
{
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
void ThrottleController::engageThrottle(int input) {
	if (input != currentThrottlePWM) {
		write(DAC_CHANNEL, input);
		currentThrottlePWM = input;  // Remember most recent throttle PWM value.
	}
}

void ThrottleController::ThrottlePID(int desiredValue) {
	if (desiredValue >= (speedCyclometerInput_mmPs + 10)) {
		speedPID.Compute();
		//currentThrottlePWM = (int)PIDThrottleOutput_pwm;
		engageThrottle(PIDThrottleOutput_pwm);
	}
}

void ThrottleController::write(int address, int value) {

	/*-------------------------------------DAC_Write-------------------------------------------*/
	/* DAC_Write applies value to address, producing an analog voltage.
	  // address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
	  // value: digital value converted to analog voltage
	  // Output goes to mcp 4802 Digital-Analog Converter Chip via SPI
	  // There is no input back from the chip.

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

	int byte1 = ((value & 0xF0) >> 4) | 0x10; // active mode, bits D7-D4
	int byte2 = (value & 0x0F) << 4; // D3-D0
	if (address < 2)
	{
		// take the SS pin low to select the chip:
		digitalWrite(SelectAB, LOW);
		if (address >= 0)
		{
			if (address == 1)
			{
				byte1 |= 0x80; // second channnel
			}
			SPI.transfer(byte1);
			SPI.transfer(byte2);
		}
		// take the SS pin high to de-select the chip:
		digitalWrite(SelectAB, HIGH);
	}
	else
	{
		// take the SS pin low to select the chip:
		digitalWrite(SelectCD, LOW);
		if (address <= 3)
		{
			if (address == 3)
			{
				byte1 |= 0x80; // second channnel
			}
			SPI.transfer(byte1);
			SPI.transfer(byte2);
		}
		// take the SS pin high to de-select the chip:
		digitalWrite(SelectCD, HIGH);
	}
}

void ThrottleController::stop() {
	engageThrottle(0);
}

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
			unsigned long timeDiff = millis() - calcTime_ms[0];
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

double ThrottleController::extrapolateSpeed() {
	double y;
	double t = millis();
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
