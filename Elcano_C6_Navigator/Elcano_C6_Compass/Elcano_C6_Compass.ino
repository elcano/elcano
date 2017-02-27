#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"
#include <ElcanoSerial.h>
using namespace elcano;
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag mag = Adafruit_LSM303_Mag(1366123);

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
SerialData data;
ParseState ps;

void setup(void) 
{
  ps.dt = &data;
  ps.input = &Serial1;
  ps.output = &Serial2;
  ps.capture = MsgType::drive;
  data.clear();
  Serial1.begin(baudrate);
  Serial2.begin(baudrate);
  pinMode(16,OUTPUT);
  delay(5000);

  Serial.begin(9600);
//  Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  /* Display some basic information on this sensor */
  displaySensorDetails();
}


void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
//  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
//  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  
  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / M_PI;
  
  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
//  Serial.print("Compass Heading: ");
  Serial.println(heading);
  data.kind = MsgType::sensor;
  data.bearing_deg = heading;
  data.speed_cmPs = 0;
  data.angle_deg = 0;
  data.posE_cm = event.magnetic.x;
  data.posN_cm = event.magnetic.y;
  data.write(&Serial2);

  ParseStateError r = ps.update();
  if(r == ParseStateError::success) {
    Serial.println("speed = " + String(data.speed_cmPs));
  }

  delay(500);
}

//  Serial.print("Compass Heading: ");
//  Serial.println(heading);