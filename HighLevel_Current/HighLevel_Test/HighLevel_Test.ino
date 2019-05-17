//Outside libraries
#include <IODue.h>
//Elcano Libraries
#include "Settings_HighLevel.h"
#include "C3_Pilot.h"
#include "C6_Navigator.h"
#include "Planner.h"

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
//#define GPSECHO  false //in GPS_HighLevel

C3_Pilot *myPilot;
//C4_Planner *myPlanner;
C6_Navigator *myNav;


Waypoint estimated_position, oldPos;

/******************************************************************************************************
 * main setup method
 *****************************************************************************************************/
void setup() {
  //for the micro SD
  pinMode(chipSelect, OUTPUT);

  Serial.begin(9600);
  //re-include if use serial again for raspberrypi or else
  //Serial2.begin(9600);
  if (CAN.begin(CAN_BPS_500K)) { // initalize CAN with 500kbps baud rate 
    Serial.println("init success");
  }
//may need to do/call planner in navigator
//Kim worried about changing variable names as variables where global and
//supposed to be updated by multiple systems. They should ramain with same name
//possibly originated in one class and used as instance of or made global in Settings_HL.h
 if(DEBUG) Serial.println("Starting Navigation");
 myNav = new C6_Navigator(estimated_position, oldPos);
 
 if(DEBUG) Serial.println("Starting Pilot");
 myPilot = new C3_Pilot(estimated_position);
}

/******************************************************************************************************
 * main loop method
 *****************************************************************************************************/
void loop() {
  myNav->update(estimated_position, oldPos);
  
  //RE-compute path if too far off track (future development) for C4
  
  myPilot->update(estimated_position, oldPos);
  
  //delay(3000); //for testing only.. remove when deploying bike
}
