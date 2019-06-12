// Test of data that C4 planner sends. This tests if the physical connection.
// Is correct and there are no board communication issues. This
#include <IO.h>
#include <Elcano_Serial.h>
void setup() 
{
  Serial.begin(19200);
}
void loop() 
{
  SerialData testResult;
  testResult.clear();
  //Set the data of the SerialData. Change kind to test vaiables as needed.
  testResult.kind = 1;
  testResult.number = 2;
  testResult.speed_cmPs = 3;
  testResult.angle_mDeg = 4;
  testResult.bearing_deg = 5;
  testResult.posE_cm = 6;
  testResult.posN_cm = 7;
  testResult.probability = 8;
  testResult.distance_travelled_cm = 9;
  //Write the data over the Serial and print the sent data to the monitor.
  testResult.write(&Serial);
  Serial.println(testResult.kind);
}
