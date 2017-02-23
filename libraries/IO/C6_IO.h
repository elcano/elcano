// I/O pins for C6 Navigator on Arduino Mega

// Analog inputs
#define C2_MOTOR_COMMANDED 8
#define C2_STEER_COMMANDED 9
#define C2_BRAKE_COMMANDED 10

// Digital
// D0 is serial monitor in
#define Rx0  0
// D1 is serial monitor out
#define Tx0  1

// pulse from GPS once per second; used to sysnc to GPS time
// Define as 2 if use interrupt
// If no interrupt is needed, defining as 7 allows using a jumper connector
#define GPS_PPS 2
// click once per wheel revolution
// This signal can be pin 3 on a base Arduino.
// When using a Mega, pin 21 offers easier wiring.
#define CYCLOMETER 21
#define GPS_POWER  4
#define GPS_GREEN_LED 5
#define GPS_RED_LED 6
#define SPEED_PIN  9
// D10 to D13 are for SD card on shield
const int chipSelect = 10;  // for SD data logging shield from Adafruit.
/** MOSI - pin 11
 ** MISO - pin 12
 ** CLK  - pin 13 */
const int LED = 13;

// Serial 3
#define GPS_RX  14
#define GPS_TX  15
// Serial 2 for intended state and estimated state
#define C4_TX   16
#define C4_RX   17
// Serial 1 for digital compass and inertial measurement unit
#define INU_TX  18
#define INU_RX  19
// pin 21 is CYCLOMETER

// Analog inputs
const int Front = 0;
const int Right = 1;
const int Left = 2;

