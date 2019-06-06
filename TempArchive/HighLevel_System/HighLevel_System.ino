#include "Settings.h"
#include "C3_Pilot.h"
#include "C6_Navigator.h"
#include "C4_Planner.h"
#include <IODue.h>
//#include <Adafruit_Sensor.h>
//#include <SPI.h>
//#include <SD.h>
//#include <ElcanoSerial.h>
//#include <Serial_Communication.h>
//#include <Wire.h>
//#include <FusionData.h>

//******************* TO DO***************************************
//    Communicate path from C4 to C3, passing the array of paths most likely
//    but waiting on C4 to be finished

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to DEBUG and listen to the raw GPS sentences.
#define GPSECHO  false

C3_Pilot *myC3;
C4_Planner *myC4;
C6_Navigator *myC6;


Waypoint estimated_position, oldPos;

/******************************************************************************************************
 * main setup method
 *****************************************************************************************************/
void setup() {
  //for the micro SD
  pinMode(chipSelect, OUTPUT);

  Serial.begin(9600);
  Serial2.begin(9600);
  if (CAN.begin(CAN_BPS_500K)) { // initalize CAN with 500kbps baud rate 
    Serial.println("init success");
  }

 if(DEBUG) Serial.println("Starting C6");
 myC6 = new C6_Navigator(estimated_position, oldPos);
 if(DEBUG) Serial.println("Starting C4");
 //myC4 = new C4_Planner(estimated_position);
 if(DEBUG) Serial.println("Starting C3");
 myC3 = new C3_Pilot();
}

/******************************************************************************************************
 * main loop method
 *****************************************************************************************************/
void loop() {
  myC6->update(estimated_position, oldPos);
  
  //RE-compute path if too far off track (future development) for C4
  
  myC3->update(estimated_position, oldPos);

}
