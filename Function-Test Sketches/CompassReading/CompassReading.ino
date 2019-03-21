#include <Wire.h>
extern TwoWire Wire1;
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
 
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
 
void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Magnetometer Test"); Serial.println("");
  if(!mag.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}
 
void loop(void) 
{
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / PI;
  
  //Normalize to 0-360
  
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  Serial.print("Compass Heading: ");
  Serial.println(heading);
  delay(500);
}
