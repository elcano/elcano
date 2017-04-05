#include "ElcanoSerial.h"

using namespace elcano;
int command = 0;

void setup() 
{
 Serial.begin(9600);
 Serial1.begin(baudrate);
}
SerialData sd;
void loop() 
{
  if(Serial.available() > 0) 
  {
    command = Serial.read();
    Serial1.print((char)command);
    Serial.print((char)command);
  }
}
