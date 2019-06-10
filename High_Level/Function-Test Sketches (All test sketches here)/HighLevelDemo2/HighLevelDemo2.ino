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
  mag.enableAutoRange(true); //Enable auto-gain for the compass

  // /* initial compass module (TODO: the compass need to calibrate) */
  //  if (!mag.begin()) {
  //    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  //  }
  //  mag.begin();
  
  //  heading = getHeading(); // get initial heading
  //  if (debug) {
  //    Serial.print("Initial heading: ");
  //    Serial.println(heading);
  //  }
  delay(3000);
  sendMSG(0, Straight_angle); // let the wheel point to straight
  delay(3000);
}
`
/*  This is the geocoordinate of UWB playgroud                             
 *   
 *                               (lat diff 652)
 *  3.(47.761253, -122.189872)--------------------------------4.(47.760601, -122.189434)
 *      |                                                           |
 *      | (long diff 659)                                           |  (long diff 757) 
 *      |                                                           |
 *  2.(47.761136, -122.190531)--------------------------------1.(47.760433, -122.190191)  * Start point
 *                             (lat diff 703)
 */
void loop() {
  // get current location
  AcquireGPS();

  // doing differnet action based on which path we are on
  switch (approaching) {
	  
    case 0:  // 1 -> 2
      if (debug) {
        Serial.println("From 1 -> 2");
      }
	  
      if (old != approaching){ // send the command only if the current state changes
        goStraight();
        old = approaching;
      }
	  
      if (approachTwo()) { // if we are approaching first point 
        turn(); // turn 90 degree to the right so it can do square movement
      }
      break;
	  
    case 1: // 2 -> 3
      if (debug) {
        Serial.println("From 2 -> 3");
      }
      if (old != approaching){
        goStraight();
        old = approaching;
      }
      if (approachThree()) {
        turn();
      }
      break;

    case 2:  // 3 -> 4
      if (debug) {
        Serial.println("From 3 -> 4");
      }
      if (old != approaching){
        goStraight();
        old = approaching;
      }
      if (approachFour()) {
        turn();
      }
      break;
      
    case 3:  // 4 -> 1
      if (debug) {
        Serial.println("From 4 -> 1");
      }
      if (old != approaching){
        goStraight();
        old = approaching;
      }
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
