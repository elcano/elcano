#include "C6_Navigator.h"

#include <Adafruit_LSM303_U.h>

//index of hardcoded gps coordinates for testing
double gpsTest[] = {47.760850, -122.190044, 47.9, -122, 51, -123, 50.5, -120};
int gpsIndex = 0; //helper with array above
bool passedInitial = false; //to hardcode GPS only first time

Origin originSt(ORIGIN_LAT, ORIGIN_LONG);
long extractSpeed = 0; //alternative to checksum since it's not implemented ie check for bad incoming data through serial

Waypoint newPos, estimPos, GPS_reading;



Adafruit_GPS GPS(&mySerial);
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

bool got_GPS = false;
CAN_FRAME incoming;


/******************************************************************************************************
 * constructor for C6_Navigator
 *  activates the GPS, Compass and finds initial positoin
 *****************************************************************************************************/
C6_Navigator::C6_Navigator(Waypoint &estimated_position, Waypoint &oldPos) {
  //setting up the GPS rate
  estimPos = estimated_position; //assign global variable to local for editing
  setup_GPS();

  //Enable auto-gain
  mag.enableAutoRange(true);

  if (!mag.begin()) {
    if(DEBUG)  Serial.println("no LSM303 detected ... Check your wiring!");
  }
  mag.begin();
  
  initial_position(oldPos); //getting your initial position from GPS comment out if no GPS available
  passedInitial = true; // got first GPS fail the rest in hard code remove if testing
  //set starting speed to 0 
  newPos.speed_mmPs = 0;
  estimated_position = estimPos; //transfer local data back to global variable
}



void C6_Navigator::setup_GPS() {
  //Serial 3 (mySerial) is used for GPS
  mySerial.begin(GPSRATE);
  GPS.begin(GPSRATE);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 5 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
}

/********************************************************************************************************
 * 
 *******************************************************************************************************/
bool C6_Navigator::AcquireGPS(Waypoint &gps_position) {
  if(DEBUG) Serial.println("Acquire GPS");
  float latitude, longitude;

  //HARD CODED FOR TESTING remove from here to the return true statement down below
  /*gps_position.latitude = gpsTest[gpsIndex];
  gpsIndex++; 
  if(DEBUG)  Serial.println("Latitude: " + String(gps_position.latitude, 6));
  gps_position.longitude = gpsTest[gpsIndex];
  if(DEBUG)  Serial.println("Longitude: " + String(gps_position.longitude, 6));
  if(gpsIndex == 7)
    gpsIndex = 0;
  else
    gpsIndex++; */
  gps_position.longitude = gpsTest[gpsIndex];
  gps_position.latitude = gpsTest[gpsIndex];
  return (!passedInitial); 
  
  char c;
  //read atleast 25 characters every loop speed up update time for GPS
  for (int i = 0; i < 25; i++) {
    c = GPS.read();
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
  //  Serial.println("newNMEArecieved");
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return false;  // we can fail to parse a sentence in which case we should just wait for another

    if (GPS.fix) {
      gps_position.latitude = GPS.latitudeDegrees;
      if(DEBUG)  Serial.println("Latitude: " + String(gps_position.latitude, 6));
      gps_position.longitude = GPS.longitudeDegrees;
      if(DEBUG)  Serial.println("Longitude: " + String(gps_position.longitude, 6));
      return true;
    }
    return false;
  }
  return false;
}


/*******************************************************************************************************
 * Receive actual speed and actual turn angle from the C2_Lowlevel board via CAN BUS
 ******************************************************************************************************/
void C6_Navigator::C6_communication_with_C2() {
 CAN.watchForRange(Actual_CANID, LowStatus_CANID);  //filter for low level communication
  
  while (CAN.available() > 0) { // check if CAN message available
    CAN.read(incoming);

 /*   rec++;
    if(rec > 1000) {
      Serial.println("1k recs at " + String(millis()));
      rec = 0;
    } */
    
    if(DEBUG2)  {
      Serial.println("Get data from (low level) ID: " + String(incoming.id, HEX));
      Serial.print("Low: " + String((int)incoming.data.low, DEC));
      Serial.println(", High: " + String((int)incoming.data.high, DEC));
    }

    if(incoming.id == Actual_CANID) {
      extractSpeed = incoming.data.low;
      if  (extractSpeed >= 0) {
        newPos.speed_mmPs = extractSpeed; //upadte acutal speed from C2
      }
      else
        if(DEBUG)  Serial.println("Got a negative speed from C2");
    }
  else 
    if(DEBUG)  Serial.println("Did not receive actual speed, angle from C2");
  }
}

/*******************************************************************************************************
 * 
 ******************************************************************************************************/
long C6_Navigator::getHeading() {
  //Get a new sensor event from the magnetometer
  sensors_event_t event;
  mag.getEvent(&event);      
  
  //Calculate the current heading (angle of the vector y,x)
  //Normalize the heading
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / PIf;
  if(DEBUG)  Serial.println("acquired heading: " + String(heading));
  if (heading < 0)  {
    heading = 360 + heading;
  }
  if(DEBUG3) Serial.println("Heading is: " + String(heading));
  return heading;
}

/*******************************************************************************************************
 * 
 ******************************************************************************************************/
void C6_Navigator::findPosition(bool got_GPS, Waypoint &oldPos) {
  newPos.time_ms = millis(); //mark current time
  
  //get heading coordinates from the compass
  newPos.bearing_deg = getHeading();
  if(DEBUG)  Serial.println("loop6 newPos.bearing_deg = " + String(newPos.bearing_deg));
  
  if (got_GPS) { 
    if(DEBUG)  Serial.println("got gps and deadreckoning");
    
    //to get an esitimation position average between the GPS and Dead Rekoning
    //estimPos is updated to the current position inside this method
    
    FindFuzzyCrossPointXY(GPS_reading, newPos, estimPos);
    
    //calculating the E and N vector by constantly updating everything you move 
    oldPos.vectors(&estimPos);
  }
  else { 
    if(DEBUG)  Serial.println("Only got dead reckoning");
    // calculate position using Dead Reckoning
    ComputePositionWithDR(oldPos, newPos);

    //calculating the E and N unit vector
    oldPos.vectors(&newPos);

    //update new current positon
    estimPos.east_mm = newPos.east_mm;
    estimPos.north_mm = newPos.north_mm;
  }
  
  //update E and N vector of current position 
  estimPos.Evector_x1000 = oldPos.Evector_x1000;
  estimPos.Nvector_x1000 = oldPos.Nvector_x1000;
  if(DEBUG3) Serial.println("--- North: " + String(estimPos.north_mm) + ", East: " + String(estimPos.east_mm) + "\n");
  
  //update old position to current 
  oldPos = estimPos;
}


/*******************************************************************************************************
 * Get your first initial position form the GPS
 ******************************************************************************************************/
void C6_Navigator::initial_position(Waypoint &oldPos) {
  
  bool GPS_available = AcquireGPS(estimPos);

  //This makes an infinite loop when the GPS is not available, comment out or hard code for testing 4-8-19 Mel
  while (!GPS_available) {
    GPS_available = AcquireGPS(estimPos); 
  }
  
  if(DEBUG)  Serial.println("Acquired GPS position");
  estimPos.time_ms = millis();
  estimPos.Compute_EandN_Vectors(getHeading()); //get position E and N vector
  //Assign Origin GPS position
  //Origin tmpOrg(estimPos.latitude, estimPos.longitude);
  //originSt = tmpOrg; //assign originSt to starting position
  
  if(DEBUG)  Serial.println("Computed Vectors in initial position");
  estimPos.Compute_mm(originSt);  //initialize north and east coordinates for position
  if(DEBUG) {
    Serial.print("Estimate E: ");
    Serial.println(estimPos.east_mm);
    Serial.print("Estimate N: ");
    Serial.println(estimPos.north_mm);
  }
  //oldPos to keep track of previous position for DR
  oldPos = estimPos;
}

/******************************************************************************************************
 * update loop for C6_Navigator
 *  gets a new GPS signal, Deadreckoning data from the C2_Lowlevel
 *  and uses this data to estimate the current position of the trike
 *****************************************************************************************************/
void C6_Navigator::update(Waypoint &estimated_position, Waypoint &oldPos) {
  estimPos = estimated_position; //assign value at estimate_position reference to local var
  oldPos.time_ms = millis();
  delay(1);
  
  got_GPS = AcquireGPS(GPS_reading);  //try to get a new GPS position
  
  C6_communication_with_C2(); // Receiving speed data from C2 using CAN Bus
  
  if (got_GPS) {
    GPS_reading.Compute_mm(originSt); // get north and east coordinates from originStl
    if(DEBUG)  Serial.println("Got and computed GPS");
  }
  else {
    if(DEBUG)  Serial.println("Failed to get got_GPS");
  }

  findPosition(got_GPS, oldPos); //determine location based on dead reckoning and GPS
  estimated_position = estimPos; //return local variable data to estimated_position reference
}   
