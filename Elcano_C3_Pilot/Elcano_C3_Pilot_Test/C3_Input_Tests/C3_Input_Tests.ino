// This is a simple test of connecting C4 and C3 and testing if the 
// signal is acutally sent over the Serial connections.
#include <IO.h>
#include <Elcano_Serial.h>
void setup() 
{
  Serial.begin(19200);
  Serial2.begin(19200);
}
void loop() 
{
  // Pull in the data.
  SerialData resultRecieved;
  readSerial(&Serial2, &resultRecieved);
  // Print the data pulled in.
  Serial.println(resultRecieved.kind);
  //Serial.println(resultRecieved.number);
  Serial.println(resultRecieved.speed_cmPs);
  Serial.println(resultRecieved.angle_deg);
  //Serial.println(resultRecieved.bearing_deg);
  //Serial.println(resultRecieved.posE_cm);
  //Serial.println(resultRecieved.posN_cm);
  //Serial.println(resultRecieved.probability);
  //Serial.println(resultRecieved.distance_travelled_cm);
  delay(1000);
}
