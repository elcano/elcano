#include "ElcanoSerial.h"

using namespace elcano;
int command = 0;
SerialData sd;
ParseState ps;
void setup() 
{
 Serial.begin(9600);
 Serial1.begin(baudrate);
 Serial2.begin(baudrate);
 Serial3.begin(baudrate);
 ps.input = &Serial1;
 ps.capture = MsgType::sensor;
}
void loop() 
{
 sd.kind = MsgType::drive;
 sd.speed_cmPs = 0;
 sd.angle_mDeg = 1;
 sd.write(&Serial);
 delay(100); 
}


