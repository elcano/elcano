//Outside libraries
#include <IODue.h>
//Elcano Libraries
#include "Globals.h"
#include "Pilot.h"
#include "Localization.h"

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


Localization *myLocal;
Pilot *myPilot;

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
 if(DEBUG) Serial.println("Starting Localization");
 myLocal = new Localization(origin, estimated_position, old_position);
 if(DEBUG)Serial.println("estimated_position = " + String(estimated_position.latitude));
 if(DEBUG) Serial.println("Starting Pilot");
 myPilot = new Pilot(origin, estimated_position, old_position);
 if(DEBUG)Serial.print("origin after planner is now set to: ");
 if(DEBUG)Serial.print(origin.latitude, 6);
 if(DEBUG)Serial.print(" ");
 if(DEBUG)Serial.println(origin.longitude, 6);
}

/******************************************************************************************************
 * main loop method
 *****************************************************************************************************/
void loop() {
  myLocal->update(origin, estimated_position, old_position);
   if(DEBUG)Serial.println("estimated_position.eastmm = " + String(estimated_position.east_mm));
  //RE-compute path if too far off track (future development) for C4
  
  myPilot->update(estimated_position, old_position);

  //path has been set to first mission cone, once reach it use planner in pilot
  //update myPlanner->mission_index++
  //update myPlanner->Start to current Location
  //make a new plan
  //in pilot call myPlanner->last_index_of_path = myPlanner->PlanPath(origin(fix to origin name), myPlanner->Start, myPlanner->mission[mission_index]);
  //go to next mission index until myPlanner->mission_index > CONES
  //reached end so stop
  
  delay(3000); //for testing only.. remove when deploying bike
}
