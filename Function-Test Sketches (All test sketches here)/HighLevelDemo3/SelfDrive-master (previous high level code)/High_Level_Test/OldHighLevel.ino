#include <Common.h>
#include <SPI.h>
#include <SD.h>
#include <IODue.h>
#include <ElcanoSerial.h>
#include <Serial_Communication.h>
#include <Wire.h>
#include <Adafruit_LSM303_U.h>
#include <FusionData.h>
#include <Adafruit_GPS.h>

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences.
#define GPSECHO  true

#define mySerial Serial3
Adafruit_GPS GPS(&mySerial);


using namespace elcano;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

enum States { STOP, STRAIGHT, ENTER_TURN, LEAVE_TURN, APPROACH_GOAL, LEAVE_GOAL};
States state;

#define GPSRATE 9600
#define DESIRED_SPEED_mmPs 1390 //5mph
#define SLOW_SPEED_mmPs 833 //1-2mph
#define currentlocation  -1 //currentLocation
#define CONES 1

extern bool DataAvailable;
bool got_GPS = false;
const long turn_speed = 835;
const long MIN_TURNING_RADIUS = 1000;
long speed_mmPs = DESIRED_SPEED_mmPs;
const unsigned long LoopPeriod = 100;  // msec

int next = 1; //index to path in a list
//last is the the last index of the Path/goal
int last_index_of_path = 2; //hardcode path of the last index/dest to 3 [cur,loc1,goal]
long current_heading = -1;
//pre-defined goal/destination to get to
long goal_lat[CONES] = {47760934};
long goal_lon[CONES] = { -122189963};
long pre_desired_speed = 0;
long turn_direction = 0;
long pre_turn_angle = 0;
long extractSpeed = 0; //alternative to checksum since it's not implemented ie check for bad incoming data through serial
long turn_radius_mm = 2000;

junction Nodes[MAX_WAYPOINTS]; //Storing the loaded map

//waypoint path[MAX_WAYPOINTS];  // course route to goal/mission
waypoint path[4];
waypoint path0, path1, path2, path3;

waypoint mission[CONES]; //The taget node to reach/goal
waypoint GPS_reading, estimated_position, oldPos, newPos, Start;

//origin is set to the UWB map
Origin origin(47.758949, -122.190746);

SerialData ReceiveData, SendData;
ParseState ps, ps3;

/*---------------------------------------------------------------------------------------*/
/**
   Methods for hardcoding the path in place of C4
*/
/*---------------------------------------------------------------------------------------*/
void populate_path() {


    path0.east_mm = -9432;
    path0.north_mm = -66564;

    path1.east_mm = -15136;
    path1.north_mm = -65716;

    path2.east_mm = -17987;
    path2.north_mm = -63173;

    path[0] = path0;
    path[1] = path1;
    path[2] = path2;

    for (int i = 0; i < 2; i++) {
        path[i].vectors(&path[i+1]);
    }
}
void initializePosition() {
  estimated_position.east_mm = path[0].east_mm;
  estimated_position.north_mm = path[0].north_mm;
  estimated_position.Evector_x1000 = path[0].Evector_x1000;
  estimated_position.Nvector_x1000 = path[0].Nvector_x1000;
  oldPos = estimated_position;
}

/*---------------------------------------------------------------------------------------*/
/**
   All C6 Methods start here
*/
/*---------------------------------------------------------------------------------------*/
void setup_GPS() {
  //Serial 3 is used for GPS
  mySerial.begin(9600);
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);   // 5 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
}
bool AcquireGPS(waypoint &gps_position) {
  float latitude, longitude;

  char c;
  //read atleast 25 characters every loop speed up update time for GPS
  for (int i = 0; i < 25; i++) {
    c = GPS.read();

  }
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return false;  // we can fail to parse a sentence in which case we should just wait for another

    if (GPS.fix) {
      gps_position.latitude = GPS.latitudeDegrees;
      gps_position.longitude = GPS.longitudeDegrees;

      return true;
    }
    return false;
  }
}

//setup Elcano serial communication for recieving data from C2
//Recieving actual_speed and an arbitary angle(for the moment)
void C6_communication_with_C2() {
  //setting up receiving data for C6 elcano communication
  ps.dt = &ReceiveData;
  ps.input = &Serial2;
  ps.output = &Serial2;
  ps.capture = MsgType::drive;
  ReceiveData.clear();
}

long getHeading(void) {
  //Get a new sensor event from the magnetometer
  sensors_event_t event;
  mag.getEvent(&event);

  //Calculate the current heading (angle of the vector y,x)
  //Normalize the heading
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / PIf;

  if (heading < 0)  {
    heading = 360 + heading;
  }
  return heading;
}

//Get your first initial position form the GPS
void initial_position() {

  bool GPS_available = AcquireGPS(estimated_position);

  //Get initial starting position
  while (!GPS_available)
    GPS_available = AcquireGPS(estimated_position);

  estimated_position.time_ms = millis();
  estimated_position.Compute_EandN_Vectors(getHeading()); //get position E and N vector
  estimated_position.Compute_mm(origin);  //initialize north and east coordinates for position

  Serial.print("Estimate E: ");
  Serial.println(estimated_position.east_mm);
  Serial.print("Estimate N: ");
  Serial.println(estimated_position.north_mm);
  
  //oldPos to keep track of previous position for DR
  oldPos = estimated_position;

}

void setup_C6() {
  //setting up the GPS rate
  setup_GPS();

  //Enable auto-gain
  mag.enableAutoRange(true);

  if (!mag.begin()) {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  }
  mag.begin();

  initial_position(); //getting your initial position from GPS

  //hard code for testing the exact start positon in the path
  //initializePosition();
  
  //set default speed to 0 
  newPos.speed_mmPs = 0;

  //for recieving data from C2
  C6_communication_with_C2();
}

void loop_C6() {
  oldPos.time_ms = millis();
  delay(1);
  
  got_GPS = AcquireGPS(GPS_reading);  //try to get a new GPS position
  
  if (got_GPS) {
    GPS_reading.Compute_mm(origin); // get north and east coordinates from originl
  }

  //Recieving data from C2 using Elcano_Serial
  ParseStateError r = ps.update();
  if (r == ParseStateError::success)  {
    extractSpeed = receiveData(ReceiveData.speed_mmPs);

    if (extractSpeed != -1) { //invalid data from C2 if it's -1
      //speed cannot be below 0
      if  (extractSpeed >= 0) {
        newPos.speed_mmPs = extractSpeed; //upadte acutal speed from C2
      }

    }
  }
  newPos.time_ms = millis();
  //get heading coordinates from the compass
  newPos.bearing_deg = getHeading();


  if (got_GPS) { //got both GPS and DeadReckoning

    //to get an esitimation position average between the GPS and Dead Rekoning
    //estimated_position is updated to the current position inside this method
    FindFuzzyCrossPointXY(GPS_reading, newPos, estimated_position);
    
    //calculating the E and N vector by constantly updating everything you move 
    oldPos.vectors(&estimated_position);
     
  }
  else { //Did not get a GPS reading and only DeadReckoning
    
    // calculate position using Dead Reckoning
    ComputePositionWithDR(oldPos, newPos);

    //calculating the E and N unit vector
    oldPos.vectors(&newPos);

    //update new current positon
    estimated_position.east_mm = newPos.east_mm;
    estimated_position.north_mm = newPos.north_mm;
  }
    //update E and N vector of current position 
    estimated_position.Evector_x1000 = oldPos.Evector_x1000;
    estimated_position.Nvector_x1000 = oldPos.Nvector_x1000;

    //update old position to current 
    oldPos = estimated_position;
}

/*---------------------------------------------------------------------------------------*/
/**
   All the Methods for C3 starts here
*/
/*---------------------------------------------------------------------------------------*/
//setup Elcano serial communication for sending data to C2
//Sending speed_mmps that you want to go at, and turn angle for direction
void C3_communication_with_C2() {
  //Setting up for sending data form C3 to C2
  ps3.dt = &SendData;
  ps3.input = &Serial2; //connection to read from
  ps3.output = &Serial2; //connection to write to
  ps3.capture = MsgType::drive;
  SendData.clear();
}

/**
   This method computes the turning radius for the trike
   Not using the speed at the moment. Needs improvment for futher development
   by taking in the speed
*/
long turning_radius_mm(long speed_mmPs) {
  return  MIN_TURNING_RADIUS; //4000mm or 4m
}

/**
   This method checks to see if we have passed the destination and missed the turn.
   Also takes in consider of the type of path shape such as rectagle, skinny, square from current location
   to the intersection when determing if we passed the destination.

   param : n = next -> index of the intersection you're approaching
   return true = past the destination , false = Have not past the destination

*/
bool test_past_destination(int n) {
  if (abs(path[n - 1].east_mm - path[n].east_mm) > abs(path[n - 1].north_mm - path[n].north_mm)) {
    if (path[n].east_mm > path[n - 1].east_mm && estimated_position.east_mm > path[n].east_mm) {
      return true;
    } else if (path[n - 1].east_mm > path[n].east_mm && estimated_position.east_mm < path[n].east_mm) {
      return true;
    }
  } else {
    if (path[n].north_mm > path[n - 1].north_mm && estimated_position.north_mm > path[n].north_mm) {
      return true;
    } else if (path[n - 1].north_mm > path[n].north_mm && estimated_position.north_mm < path[n].north_mm) {
      return true;
    }
  }
  return false;
}

/**
   This method test for intersection from straight to entering turn

   param : turning radius of the vehicle
   param : n = next -> index of the intersection you're approaching
   return : true -> if approached intersection to entering turn, false -> otherwise
*/
bool test_approach_intersection(long turn_radius_mm, int n) {
  if (abs(path[n - 1].east_mm - path[n].east_mm) > abs(path[n - 1].north_mm - path[n].north_mm)) {
    if (path[n].east_mm > path[n - 1].east_mm && estimated_position.east_mm >= path[n].east_mm - turn_radius_mm) {
      return true;
    } else if (path[n - 1].east_mm > path[n].east_mm &&
               estimated_position.east_mm <= path[n].east_mm + turn_radius_mm) {
      return true;
    }
  } else {
    if (path[n].north_mm > path[n - 1].north_mm &&
        estimated_position.north_mm >= path[n].north_mm - turn_radius_mm) {
      return true;
    } else if (path[n - 1].north_mm > path[n].north_mm &&
               estimated_position.north_mm <= path[n].north_mm + turn_radius_mm) {
      return true;
    }
  }
  return false;
}

/**
   This method test for intersection from leaving turn to straight

   param : turning radius of the vehicle
   param : n = next -> index of the intersection you're approaching
   return : true -> if approached intersection to leave turn, false -> otherwise
*/
bool test_leave_intersection(long turning_radius_mm, int n) {
    //more change in east
    if (abs(path[n - 1].east_mm - path[n].east_mm) > abs(path[n - 1].north_mm - path[n].north_mm)) {
        if (path[n].east_mm > path[n - 1].east_mm) {
            if(abs((abs(path[n].north_mm) + turning_radius_mm) - (abs(estimated_position.north_mm) + turning_radius_mm)) >= turning_radius_mm/2){
            return true;
            }
        }
        else if(path[n - 1].east_mm > path[n].east_mm) {
            if(abs((abs(path[n].north_mm) + turning_radius_mm) - (abs(estimated_position.north_mm) + turning_radius_mm)) >= turning_radius_mm/2) {
            return true;
            }
        }
    }
    else {
        //more change in north
        if(path[n].north_mm > path[n - 1].north_mm){
            if(abs((abs(path[n].east_mm) + turning_radius_mm) - (abs(estimated_position.east_mm) + turning_radius_mm)) >= turning_radius_mm/2){
                return true;
            }
        }
        else if(path[n - 1].north_mm > path[n].north_mm){
            if(abs((abs(path[n].east_mm) + turning_radius_mm) - (abs(estimated_position.east_mm) + turning_radius_mm)) >= turning_radius_mm/2) {
                return true;
            }
        }
    }
    return false;
}

/**
   This method determines the direction of turns of the vehicle of either left 
   or right once approached a turn intersection. Angle returned is either 90 for right 
   and -90 for left.  

   param : n = next -> index of the intersection you're approaching
   return : pos = right, neg = left, 0 = straight
*/
int get_turn_direction_angle(int n) {

  if (state == ENTER_TURN) {
    int turn_direction_angle = 0;
    double turn_direction = (estimated_position.Evector_x1000 * path[n].Evector_x1000) + (estimated_position.Nvector_x1000 * path[n].Nvector_x1000);
    double x_magnitude = sqrt((estimated_position.Evector_x1000 * estimated_position.Evector_x1000) + (estimated_position.Nvector_x1000 * estimated_position.Nvector_x1000));
    double y_magnitude = sqrt ((path[n].Evector_x1000 * path[n].Evector_x1000) + (path[n].Nvector_x1000 * path[n].Nvector_x1000));
    double dot_product = (turn_direction / (x_magnitude * y_magnitude));
    dot_product /= 1000000.0;
    turn_direction = acos(dot_product); // angle in radians
    double cross = (estimated_position.Evector_x1000 * path[n].Nvector_x1000) - (estimated_position.Nvector_x1000 * path[n].Evector_x1000);
    turn_direction_angle = turn_direction * 180 / PIf; //angle is degrees
    if (cross > 0)
      turn_direction_angle = -turn_direction_angle;

    return turn_direction_angle;
  }
  return 0;
}

/**
   The method find and set the state of the trike and
   also set the speed of the trike depending on the state it's in

   param : turning radius
   param : n = next -> index of the intersection you're approaching
*/
void find_state(long turn_radius_mm, int n) {

  switch (state) {
    case STRAIGHT:
      speed_mmPs = DESIRED_SPEED_mmPs;
      if (test_approach_intersection(turn_radius_mm, n)) {
        //last index of path/goal
        if (n == last_index_of_path) {
          state = APPROACH_GOAL;
        }
        else {
          state = ENTER_TURN;
        }
      }
      break;

    case STOP:
      speed_mmPs = 0;
      break;

    case ENTER_TURN:
      //setting to turning speed
      speed_mmPs = turn_speed;

      if (test_leave_intersection(turn_radius_mm, n)) {
        state = STRAIGHT;
        next++;

      }
      break;

    case APPROACH_GOAL:
      if (test_past_destination(n)) {
        state = STOP;
      }
      break;
  }

}

void setup_C3() {
  //Trike state starts Straight
  state = STRAIGHT;
  //the path is set to approach the first intersection at index 1
  next = 1;

  //Setting up for sending data from C3 to C2
  C3_communication_with_C2();
}

void loop_C3() {

  //Determining the state of the Trike
  find_state(turn_radius_mm, next);

  //Determining the turn direction for the trike "left, right or straight"
  turn_direction = get_turn_direction_angle(next);

  //Send speed and angle to C2 to diplay the Led on the test stance
  //only send data to C2 if we get new data. Avoid sending the same data
  if (pre_desired_speed != speed_mmPs || pre_turn_angle != scaleDownAngle(turn_direction)) {
    SendData.clear();
    SendData.kind = MsgType::drive;
    //chheck this
    SendData.speed_mmPs = sendData(speed_mmPs);
    SendData.angle_mDeg = scaleDownAngle(turn_direction);
    SendData.write(&Serial2);

    pre_desired_speed = speed_mmPs;
    pre_turn_angle = scaleDownAngle(turn_direction);
  }

}
void setup() {
  //for the micro SD
  pinMode(chipSelect, OUTPUT);

  Serial.begin(9600);
  Serial2.begin(9600);

  //populate_path(); //hard code path in replacement of C4
  setup_C6();
  setup_C3();
}

void loop() {
  loop_C6();
  loop_C3();


}