#include "HardwareSerial.h"

// Arduino core code limits serial buffer size to 64. Increasing it requires more than changing the define.
#define BUFFER_SIZE 64
#define NaN 0x7FFFFFFF
#define MSG_NONE 0
#define MSG_DRIVE 1
#define MSG_SENSOR 2
#define MSG_GOAL 3
#define MSG_SEG 4

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

char * GetWord(char * major, char * str);
float GetNumber(char *minor, char*Args);
void ProcessMessage (char *IncomingMessage, SerialData *SerialD);
void readSerial(HardwareSerial *SerialN, struct SerialData *SerialD );
void writeSerial(HardwareSerial *SerialN, struct SerialData *SerialD );
