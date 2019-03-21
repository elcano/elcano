#include "H2Setting.h"

void setup() {
  // stup Serial monitor
  Serial.begin(115200);

  // setup GPS
  mySerial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  mag.enableAutoRange(true); //Enable auto-gain

  // initial compass module (TODO: the compass need to calibrate)
  if (!mag.begin()) {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  }
  mag.begin();

  // get initial heading
  heading = getHeading();
  if (debug) {
    Serial.print("Initial heading: ");
    Serial.println(heading);
  }
}

void loop() {
  // get current location
  AcquireGPS();

  // doing differnet action based on which path we are on
  switch (approaching) {
    case 0:  // 1 -> 2
      if (debug) {
        Serial.println("From 1 -> 2");
      }
      goStraight();
      if (approachTwo()) {
        turn();
      }
      break;
    case 1: // 2 -> 3
      if (debug) {
        Serial.println("From 2 -> 3");
      }
      goStraight();
      if (approachThree()) {
        turn();
      }
      break;

    case 2:  // 3 -> 4
      if (debug) {
        Serial.println("From 3 -> 4");
      }
      goStraight();
      if (approachFour()) {
        turn();
      }
      break;
      
    case 3:  // 4 -> 1
      if (debug) {
        Serial.println("From 4 -> 1");
      }
      goStraight();
      if (approachOne) {
        turn();
      }
      break;
      
    default: // finish
      if (debug) {
        Serial.println("Termination");
      }
      terminate_process();
      break;
  }
}
