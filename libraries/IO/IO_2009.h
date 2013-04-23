/* IO_2009.h:  I/O pin assignments for Arduino DueMilleNove  */

// DIGITAL I/O ----------------------------------------------

/* [in] Digital Signal 0: J1 pin 1 (RxD) Read Serial Data */
const int RxD = 0;
/* [out] Digital Signal 1: J1 pin 2 (TxD). Transmit Serial Data.  */
const int TxD = 1;
/* [out] Digital Signal 13: J3 pin 5 (SCK)  LED on Arduino board. Used for testing.    */
const int LED = 13;

/* [in] Digital Signal 0: J1 pin 1 (RxD) Read Serial Data
The operation of the joystick may be non-linear and depend on speed.
*/
const int ReverseButton = 0;
const int CruiseReverse = 1;
 
/*  
The Stop signal is a momentary button push from either the console or remote.
A rising edge produces an interrupt.
The Cruise button works the same way.
The ~Estop signal is the 5V supply produced by the motor controller.
This supply goes away when either the key switch is turned off or RC4 is pressed.
Lack of 5V from the motor controller is an emergency stop.
The three signals: StopButton, StopRC2 and ~EStop are ORed in hardware to produce 
a single signal.
Similarily, CruiseButton and CruiseRC are ORed together
*/
const int StopButton = 2;
const int CruiseButton = 3;
const int StopRC2 = 2;
const int CruiseRC1 = 3;

const int EnableThrottle = 4;
const int EnableBrake = 5;
const int EnableSteer = 6;
const int Reverse = 7;

// J3
const int CruiseLED = 8;  // J3:1
/* [out] Digital Signal 9: (D9) Actuator A1. Traction motor throttle. */
const int Throttle =  9;  // J3:2
/* [out] Digital Signal 10: (D10) PWM. Actuator A3: Steering Motor. 
  Turns left or right. If no signal, wheels are locked to a straight ahead position. */
const int Steer =  10;  // J3:3
/* [out] Digital Signal 11: (D11) PWM. Actuator A2: Brake Motor. 
  Controls disk brakes on left and right front wheels. Brakes are mechanically linked to operate in tandem. */
const int DiskBrake = 11; // J3:4
const int StopLED = 12;  // J3:5
/* Pin 13 is LED. */
const int ReverseLED = 13;  // J3:6
// Ground  J3:7
// 5 V     J3:8


// ANALOG I/O ----------------------------------------------

/* Analog Input pins 0-5 are also Digital Inputs 14-19. */
/* [in] Analog input 0: J2 pin 1 (ADC0) . */
/* Joystick is what the driver wants to do. */
const int AccelerateJoystick = 0;
/* [in] Analog input 1: J2 pin 2 (ADC1) MotorTemperature. */
const int SteerJoystick = 1;
const int JoystickCenter = 2;

/* These are what the autonomous vehicle wants to do */
const int CruiseThrottle = 3;
const int CruiseBrake = 4;
const int CruiseSteer = 5;

