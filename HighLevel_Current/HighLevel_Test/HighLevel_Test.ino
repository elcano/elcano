//Outside libraries
#include <IODue.h>
//Elcano Libraries
#include "Settings_HighLevel.h"
#include "C3_Pilot.h"
#include "C6_Navigator.h"

using namespace elcano;

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


C6_Navigator *myNav;
C3_Pilot *myPilot;

Origin origin; //origin on the selected map
Waypoint estimated_position, old_position;

/******************************************************************************************************
 * main setup method
 *****************************************************************************************************/
void setup() {
   if(DEBUG)Serial.println("origin is first set to :" + String(origin.latitude) + " " + String(origin.longitude));
 
   Serial.begin(9600);
  //re-include if use serial again for raspberrypi or else
  //Serial2.begin(9600);
  if (CAN.begin(CAN_BPS_500K)) { // initalize CAN with 500kbps baud rate 
    Serial.println("init success");
  }
 if(DEBUG)Serial.println("estimated_position before process = " + String(estimated_position.latitude));
 if(DEBUG) Serial.println("Starting Navigation");
 myNav = new C6_Navigator(origin, estimated_position, old_position);
 if(DEBUG)Serial.println("estimated_position = " + String(estimated_position.latitude));
 if(DEBUG) Serial.println("Starting Pilot");
 myPilot = new C3_Pilot(origin, estimated_position, old_position);
 if(DEBUG)Serial.print("origin after planner is now set to: ");
 if(DEBUG)Serial.print(origin.latitude, 6);
 if(DEBUG)Serial.print(" ");
 if(DEBUG)Serial.println(origin.longitude, 6);
}

/******************************************************************************************************
 * main loop method
 *****************************************************************************************************/
void loop() {
  myNav->update(origin, estimated_position, old_position);
   if(DEBUG)Serial.println("estimated_position.eastmm = " + String(estimated_position.east_mm));
  //RE-compute path if too far off track (future development) for C4
  
  myPilot->update(estimated_position, old_position);
  
  //delay(3000); //for testing only.. remove when deploying bike
}
