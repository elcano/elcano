
// IO.h defines pin usage
//--------------------------------------------------------
// Common to all
// Digital
// D0 is serial monitor in
#define Rx0  0
// D1 is serial monitor out
#define Tx0  1
#define DATA_READY 2
#define LED 13

// digital on C3;  Analog on C6
#define THROTTLE_CMD 9
#define BRAKE_CMD   10
#define STEER_CMD   11

// Analog inputs
#define THROTTLE_SET 0
#define BRAKE_SET    1
#define STEER_SET    2
#define LEFT         4
#define RIGHT        5

//------ For C3 Pilot -----------------------------------
#define CHAINING 5
#define POWER_B  6
#define POWER_A  7

#define MOVE_OK     12

// Analog inputs
#define SPEED_IN     0
// DIRECTION is written as 0 to 255 with 0 East and 64 North
// The analog values read are 4 times as much
#define DIRECTION_IN 1
#define FRONT        3

//----------For C4 Path planner ----------------------------
// C3 gets notice of data
#define C4_DATA_SENT 3
// expected image to C7
#define EXPECT_HORIZ 9
#define EXPECT_VERT 10
#define EXPECT_SIZE 11

// Analog inputs
// detected cone from C7
#define HORIZONTAL 0
#define VERTICAL   1
#define SIZE       2
#define CONENESS   3

//------------ For C6 Navigator ------------------------------
#define GPS_POWER     4
#define GPS_GREEN_LED 5
#define GPS_RED_LED   6
// pulse from GPS once per second; used to sysnc to GPS time
#define GPS_PPS       7
#define SPEED_OUT     8
#define DIRECTION_OUT 9
// D10 to D13 are for SD card on shield
const int chipSelect = 10;  // for SD data logging shield from Adafruit.
/** MOSI - pin 11
 ** MISO - pin 12
 ** CLK  - pin 13 */
// Serial 3
#define GPS_RX  14
#define GPS_TX  15
// Serial 2 for intended state and estimated state
#define C4_TX   16
#define C4_RX   17
// Serial 1 for digital compass and inertial measurement unit
#define INU_TX  18
#define INU_RX  19
// click once per wheel revolution
#define CYCLOMETER 21

// Analog inputs
// 0,1,2,9,10,11


