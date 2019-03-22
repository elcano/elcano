
// IO.h defines pin usage
//--------------------------------------------------------
// Common to all
// Digital
// D0 is serial monitor in

#define Rx0  0              //------> used in C6 setup but is commented out
// D1 is serial monitor out
#define Tx0  1              //------> used in C6 setup but is commented out
#define DATA_READY 2       //-----> Not used
#define LED 13             //-----> Not used

// digital on C3;  Analog on C6
#define THROTTLE_CMD 9     //-----> Not used
#define BRAKE_CMD   10     //-----> Not used
#define STEER_CMD   11     //-----> Not used

// Analog inputs
#define THROTTLE_SET 0     //-----> Not used
#define BRAKE_SET    1     //-----> Not used
#define STEER_SET    2     //-----> Not used
#define LEFT         4     //-----> Not used
#define RIGHT        5     //-----> Not used

//------ For C3 Pilot -----------------------------------
#define CHAINING 5  //-----> Not used
#define POWER_B  6  //-----> Not used
#define POWER_A  7  //-----> Not used

#define MOVE_OK     12     //-----> Not used

// Analog inputs
#define SPEED_IN     0     //-----> Not used
// DIRECTION is written as 0 to 255 with 0 East and 64 North
// The analog values read are 4 times as much
#define DIRECTION_IN 1     //-----> Not used
#define FRONT        3     //-----> Not used

//----------For C4 Path planner ----------------------------
// C3 gets notice of data
#define C4_DATA_SENT 3     //-----> Not used
// expected image to C7
#define EXPECT_HORIZ 9     //-----> Not used
#define EXPECT_VERT 10     //-----> Not used
#define EXPECT_SIZE 11     //-----> Not used

// Analog inputs
// detected cone from C7
#define HORIZONTAL 0    //-----> Not used
#define VERTICAL   1    //-----> Not used
#define SIZE       2    //-----> Not used
#define CONENESS   3    //-----> Not used

//------------ For C6 Navigator ------------------------------
// pulse from GPS once per second; used to sysnc to GPS time
#define SPEED_OUT     8    //-----> Not used
#define DIRECTION_OUT 9    //-----> Not used

// Pins setup for  Adafruit Micro-SD (4/20/18)
const int chipSelect = 10;  // used in C6 setup and void initialize
#define CARD_DETECT 36   //-----> Not used
#define CS_SD 35         //-----> Not used
/** DI - MOSI
 ** DO - MISO
 ** CLK  - SCK */


//Pins setup for Adafruit GPS breakout board (4/20/18)
// Using Serial 3 
#define GPS_RX  14
#define GPS_TX  15
#define EN 31
//FAdvance features (Not Used)
#define FIX 30 
#define PPS 29


//Pins setup for Adafruit Compass using "I2C" (4/20/18) 
//Must use the modified adafruit library for compass with "wire1" instead of "wire" 
/** SDA - SDA1
 ** SCL - SCL1	*/
//Advance features (Not used)
#define I2 28
#define I1 27
#define DRDY_C 26

// click once per wheel revolution
#define CYCLOMETER 21

//Optical Mouse (Not yet implemented in the software 4/20/18)
#define LED 34
#define NPD 33
#define CS_MOUSE 32

//Gyro using "I2C" (Not yet implemented in the software 4/20/18)
//Must use the modified adafruit library for gyro with "wire1" instead of "wire" 
/**
 ** SCL - SCL1
 ** SDA - SDA1 */
//Advance features (Not used)
#define CS_GYRO 25
#define DARDY_G 24
#define INT1 23

//------------ 3.3V to 5V converter  ------------------------------
#define CS_C5_REAR 38
#define CS_C5_FRONT 37 
//using Serial 2 for C2 
#define C2_RX 18
#define C2_TX 19



// Analog inputs
// 0,1,2,9,10,11
