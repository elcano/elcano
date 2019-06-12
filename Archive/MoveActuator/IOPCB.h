/* IO_PCB.h:  I/O pin assignments for Arduino Mega 2560 on MegaShieldDB

The Mega is designed to be used with a data-logging shield.
The nonMega shield uses A4 and A5 for RTC and D10,11,12,and 13 for MOSI data logging.

7/1/15 TCF  Removed BASSACKWARDS, put in VEHICLE_NUMBER
*/
// Orange trike
//#define VEHICLE_NUMBER 1
// Yellow trike
#define VEHICLE_NUMBER 2

// DIGITAL I/O ----------------------------------------------

// D0-7 Connector -------------------------------
// On the Mega, any of D0 to D13 can be PWM.
/* D0 is (Rx0) Read Serial Data. */
const int Rx0 = 0;      // external input
/* [out] Digital Signal 1: (Tx0). Transmit Serial Data.  */
const int Tx0 = 1;      // external output

// This is the 5V supply produced by the E-bike controller.
//  If it goes away, 36V power has been turned off (presumably by the rider's key switch).
const int EStop =   2;         // external input
const int WheelClick = 3;      //  interrupt; Reed switch generates one pulse per rotation.

/*  Actuator A1. Traction motor throttle. */
const int Throttle =  5;  // external PWM output  DEPRICATED: Use MOSI
/* Actuator A3: Steering Motor. 
  Turns left or right. Default is wheels locked to a straight ahead position. */

const int Steer =  7;    // external PWM output
#if (VEHICLE_NUMBER == 1)
const int DiskBrake = 6;  // external PWM output
const int ThrottleChannel = 0;  // Use DAC A
#elif (VEHICLE_NUMBER == 2)
const int DiskBrake = 8;  // external PWM output
const int ThrottleChannel = 3;  // Use DAC D
#endif

// D8-13 Connector ----------------------
// The shield does not provide a socket for D8-13
// D8 is spare on X4-2;  D9 is spare on X5-2.
// Signals D10-13 are not brought out on connectors, and may be used by a shield.
const int SDchipSelect = 10;  // for some shields
const int LED = 13;

// D14 - 21 Connector -------------------------------------
const int RxD2 = 14;      // reserved external input on X2-19
// Message in Gamebots format giving wheel spin rate and steering angle.
const int TxD2 = 15;      // Cruise Drive Command
const int RxD3 = 16;      // available on X2-22
const int TxD3 = 17;      // available on X2-20
/*  
The Stop signal is a momentary button push from either the console or remote.
A rising edge produces an interrupt.
The Cruise button works the same way.
The ~Estop signal is the 5V supply produced by the motor controller.
This supply goes away when either the key switch is turned off or RC4 is pressed.
Lack of 5V from the motor controller is an emergency stop.
Interrupts are on 2,3,18,19,20 and 21.
*/
// A remote stop button generates an interrupt to stop the vehicle.
const int StopRC2 =   18;      // wireless input
// A remote cruise button starts enabled cruise systems
const int CruiseRC1 = 19;       // wireless input
// Panel stop button generates an interrupt to stop the vehicle.
const int StopButton = 20;      // panel input
// Panel cruise button starts enabled cruise systems
const int CruiseButton = 21;    // panel input

// D22-53 Connector ----------------------------

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

// Panel switch: Whether steering can be autonomous.
const int EnableSteer = 22;     // panel input
// Whether brakes can be autonomous.
const int EnableBrake = 24;     // panel input
// Whether the throttle can be autonomous.
const int EnableThrottle = 26;  // panel input
// Panel LED lights when under autonomous control.
const int CruiseLED = 28;       // panel output
// Panel LED lights when vehicle stop is commanded.
const int StopLED = 30;         // panel output 
// Vehicle is in reverse (not implemented)
const int ReverseLED = 32;      // panel output
// Button requesting reverse (not implemented)
const int ReverseButton = 34;   // external input; reserved
// Autonomous request for reverse (not implemented)
const int CruiseReverse = 36;   // external input; reserved
// D38 - D48 are available and brought out on various connectors


const int SelectCD     = 49;  // Select IC 3 DAC (channels C and D)
const int ThrottleMISO = 50;
const int ThrottleMOSI = 51;
const int ThrottleSCK  = 52;
const int SelectAB     = 53;  // Select IC 2 DAC (channels A and B)

// ANALOG Input ----------------------------------------------

// A8-15 Connector ----------------------------
const int SpeedLimit = 8;  // not implemented
/* Joystick is what the driver wants to do. */
const int JoystickCenter = 9;      // Center voltage from joystick is normally 2.5 V
const int SteerJoystick = 10;       // Left-right joystick motion turns vehicle
const int AccelerateJoystick = 11;  // Joystick up is throttle; down is brake.

/* These are what the autonomous vehicle wants to do */
const int Speed = 12;           // Actual speed (not implemented; Use WheelClick instead.)
const int CruiseSteer = 13;     // Position of steering commanded by AI
const int CruiseBrake = 14;     // Position of brake commanded by AI
const int CruiseThrottle = 15;  // Position of throttle commanded by AI




