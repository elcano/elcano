#pragma once
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_GPS.h>
#include <SPI.h>
#include <Wire.h>
#include "Common.h"
#include "Settings_HighLevel.h"

namespace elcano {

   #define GPSECHO  true
   #define mySerial Serial3
   #define GPSRATE 9600
   Adafruit_GPS GPS(&mySerial);
    
class GPS_HighLevel{ 
  
  public:
  
    // Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
    // Set to 'true' if you want to debug and listen to the raw GPS sentences.
   
    bool DataAvailable;
    bool got_GPS;
    
    GPS_HighLevel():DataAvailable(false),got_GPS(false) {//default constructor
     // this->DataAvailable = false; 
     // this->got_GPS = false;
    } 
    
    ~GPS_HighLevel(){} //destructor
    void setup_GPS();
    boolean AcquireGPS(Waypoint &gps_position);
};
} // namespace elcano
