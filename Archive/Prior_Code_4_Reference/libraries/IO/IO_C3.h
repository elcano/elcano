/* IO_Mega.h:  I/O pin assignments for Arduino Mega 2560 

The Mega is designed to be used with a data-logging shield.
This shiels uses A4 and A5 for RTC and D10,11,12,and 13 for data logging.
Data on all pins are logged to an SD card in CSV format.
*/

// DIGITAL I/O ----------------------------------------------

// D0-7 Connector -------------------------------
// On the Mega, any of D0 to D13 can be PWM.
/* D0 is (RxD) Read Serial Data. */
const int C3_RxD = 0;      // external input
#define Rx0 C3_RxD
/* [out] Digital Signal 1: (TxD). Transmit Serial Data.  */
const int C3_TxD = 1;      // external output
#define Tx0  C3_TxD
const int C4AVL = 2; 
#define   DATA_READY C4AVL
const int CHN2 = 3;
const int CHN1 = 4;
const int CRS_BRK = 5;
#define BRAKE_CMD CRS_BRK
const int CRS_STR = 6;
#define STEER_CMD CRS_STR
const int PWR_SEL = 7;
const int CRS_REV = 8;
#define DIRECTION_IN CRS_REV
const int CRS_THR = 9;
#define THROTTLE_CMD CRS_THR
const int MOV_OK = 10;
#define MOVE_OK MOV_OK
const int SPEED = 11;
#define SPEED_IN SPEED
const int D39 = 12;
const int C3_LED = 13;

//analog signals
const int F_RNG = 0;
#define FRONT F_RNG
const int FL_RNG = 1;
const int FR_RNG = 2;
const int L_RNG = 3;

#define LEFT L_RNG
const int R_RNG = 4;
#define RIGHT R_RNG
const int F2RNG = 5;
const int L2RNG = 6;
const int R2RNG = 7;


