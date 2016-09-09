#include "HardwareSerial.h"

// Arduino core code limits serial buffer size to 64. Increasing it requires more than changing the define.
#define BUFFER_SIZE 64
#define NaN 0x7FFFFFFF
#define MSG_NONE 0
#define MSG_DRIVE 1
#define MSG_SENSOR 2
#define MSG_GOAL 3
#define MSG_SEG 4

// The following list outlines the data that can be sent on the serial lines to the various parts of the
// system. 
/*
Drive - 1 (C3 to C2)
   - speed_cmPs
   - angle_deg
Sensor - 2 (C5 to C3)
   - speed_cmPs
   - angle_deg
   - posE_cm
   - posN_cm
   - bearing_deg
Goal - 3 (C6 to C4)
   - number
   - posE_cm
   - posN_cm
   - bearing_deg
   - probability
Segment - 4 (C4 to C3)
   - number
   - posE_cm
   - posN_cm
   - bearing_deg
   - speed_cmPs
*/

struct SerialData
{
    int kind;
    long int number;
    long int speed_cmPs;
    long int angle_deg;    // front wheels
    long int bearing_deg;  // compass direction
    long int posE_cm ;
    long int posN_cm;
    long int probability;
    long int distance_travelled_cm;
    void Clear();
};

int readSerial(HardwareSerial *SerialN, struct SerialData *SerialD );
int writeSerial(HardwareSerial *SerialN, struct SerialData *SerialD );
