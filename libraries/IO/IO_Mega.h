/* IO_Mega.h:  I/O pin assignments for Arduino Mega 2560 

The Mega is designed to be used with a data-logging shield.
This shiels uses A4 and A5 for RTC and D10,11,12,and 13 for data logging.
Data on all pins are logged to an SD card in CSV format.
*/

#define Serial1 Serial
#define Serial2 Serial
#define Serial3 Serial

// DIGITAL I/O ----------------------------------------------

// D0-7 Connector -------------------------------
// On the Mega, any of D0 to D13 can be PWM.
/* D0 is (RxD) Read Serial Data. */
const int RxD = 0;      // external input
/* [out] Digital Signal 1: (TxD). Transmit Serial Data.  */
const int TxD = 1;      // external output
// const int CruiseRC1 = 2;       // relay input
// const int StopRC2 = 3;         // relay input

// D3 is not used.  This is a spare interrupt.
// D4 is a spare PWM
// D3 and 4 are avoided since some code may want to use them as red and green LEDs.
/*  Actuator A1. Traction motor (aka Throttle). */
const int TractionMotor =  5;  // external PWM output
/* Actuator A3: Steering Motor. 
  Turns left or right. Default is wheels locked to a straight ahead position. */
const int Steer =  6;    // external PWM output
/* Actuator A2: Brake Motor. 
  Controls disk brakes on left and right front wheels. Brakes are mechanically linked to operate in tandem. */
const int DiskBrake = 7;  // external PWM output

// D8-13 Connector ----------------------
// The shield does not provide a socket for D8-13
// D8 and D9 are not used.
const int chipSelect = 10;  // for SD data logging shield from Adafruit.
/** MOSI - pin 11
 ** MISO - pin 12
 ** CLK  - pin 13 */
const int LED = 13;

// D14 - 21 Connector -------------------------------------
const int RxD2 = 14;      // reserved external input
const int TxD2 = 15;      // reserved external output
const int RxD3 = 16;      // reserved
const int TxD3 = 17;      // reserved
/*  
The Stop signal is a momentary button push from either the console or remote.
A rising edge produces an interrupt.
The Cruise button works the same way.
The ~Estop signal is the 5V supply produced by the motor controller.
This supply goes away when either the key switch is turned off or RC4 is pressed.
Lack of 5V from the motor controller is an emergency stop.
Interrupts are on 2,3,18,19,20 and 21.
*/
const int CruiseRC1 = 2;       // relay input
const int StopRC2 = 3;         // relay input
const int EStop = 19;         // external input
const int StopButton = 20;      // panel input
const int CruiseButton = 21;    // panel input

// D22-53 Connector ----------------------------
// Cable for D22-41; odd on lower, even upper
const int LED1 = 23;      // external output
const int Reverse = 25;   // external output; reserved
const int LED3 = 27;      // external output
const int LED4 = 29;      // external output
const int LED5 = 31;      // external output
const int LED6 = 33;      // external output
const int LED7 = 35;      // external output
const int LED8 = 37;      // external output
#define redLEDpin 35
#define greenLEDpin 37

const int EnableMotor = 26;  // panel input TCF 2/25/12
const int EnableBrake = 24;     // panel input
const int EnableSteer = 22;     // panel input TCF 2/25/12
const int CruiseLED = 28;       // panel output
const int StopLED = 30;         // panel output 
const int ReverseLED = 32;      // panel output
const int ReverseButton = 34;   // external input; reserved
const int CruiseReverse = 36;   // external input; reserved


// ANALOG Input ----------------------------------------------
// A0-7 Connector -----------------------------
/* Current sensing lets us ease back if we are drawing too much power */
const int Current36V = 0;      // lower box input
const int CurrentSteer = 1;   // lower box input
const int CurrentBrake = 2;   // lower box input
// Shield uses 4, 5 for RTC.
const int FeedbackMotor = 5;  // to be determined; not implemented.
// A3, 6 and 7 are spare

// A8-15 Connector ----------------------------
/* Joystick is what the driver wants to do. */
const int SteerJoystick = 8;       // external input
const int JoystickCenter = 9;      // external input
const int AccelerateJoystick = 11;  // external input

/* These are what the autonomous vehicle wants to do */
const int FeedbackBrake = 10;           // external input
const int CruiseThrottle = 15;  // external input
const int CruiseBrake = 14;     // external input
const int CruiseSteer = 13;     // external input
const int FeedbackSteer = 12;      // external input

