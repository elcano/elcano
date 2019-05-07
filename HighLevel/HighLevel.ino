#include <Common.h>
#include <SPI.h>
#include <SD.h>
#include <IODue.h>
#include <Can_Protocol.h>
#include <ElcanoSerial.h>
#include <Serial_Communication.h>
#include <Wire.h>
#include <Adafruit_LSM303_U.h>
#include <FusionData.h>
#include <Adafruit_GPS.h>
#include <due_can.h>

#define MAX_CAN_FRAME_DATA_LEN_16   16

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences.
#define GPSECHO  false

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
CAN_FRAME incoming;

int next = 1; //index to path in a list
//last is the the last index of the Path/goal
int last_index_of_path = 2; //hardcode path of the last index/dest to 3 [cur,loc1,goal]
//int last_index_of_path = 0;
long current_heading = -1;
long pre_desired_speed = 0;
long turn_direction = 0;
long pre_turn_angle = 0;
long extractSpeed = 0; //alternative to checksum since it's not implemented ie check for bad incoming data through serial
long turn_radius_mm = 2000;
extern int map_points =  5;//16;

junction Nodes[MAX_WAYPOINTS]; //Storing the loaded map

//waypoint path[MAX_WAYPOINTS];  // course route to goal/mission
waypoint path[3]; //3 is hardcoded
waypoint path0, path1, path2, path3;

waypoint mission[CONES]; //aka MDF //The target Nodes to hit
waypoint GPS_reading, estimated_position, oldPos, newPos, Start;

//origin is set to the UWB map
Origin origin(47.758949, -122.190746);
double gpsTest[] = {47.758951, -122.2, 47.9, -122, 51, -123, 50.5, -120};
int gpsIndex = 0; 
int speeds[] = {1000, 2000, 4000, 3000, 1000, 1000};
int angg[] = {0, 100, 255, 10, 125, 230};
int speedIndex = 0;
//Origin origin; //set origin later in intial_position

SerialData ReceiveData, SendData;
ParseState ps, ps3;

#define CONES 1
//pre-defined goal/destination to get to
long goal_lat[CONES] = {47760934};
long goal_lon[CONES] = { -122189963};
//long goal_lat[CONES] = {  47621881,   47621825,   47623144,   47620616,   47621881};
//long goal_lon[CONES] = {-122349894, -122352120, -122351987, -122351087, -122349894};
/*  mph   mm/s
     3    1341f
     5    2235
     8    3576
     10   4470
     15   6706
     20   8941
     25  11176
     30  13411
     45  20117
     60  26822
*/

struct AStar
{
  int ParentID;
  long CostFromStart;
  long CostToGoal;
  long TotalCost;
} Open[MAX_WAYPOINTS];


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
/*THIS is the hard coded method for testing not currently used 4/10/19 Mel ********************
  void initializePosition() {
  estimated_position.east_mm = path[0].east_mm;
  estimated_position.north_mm = path[0].north_mm;
  estimated_position.Evector_x1000 = path[0].Evector_x1000;
  estimated_position.Nvector_x1000 = path[0].Nvector_x1000;
  oldPos = estimated_position;
} */

/*---------------------------------------------------------------------------------------*/
/**
   All C6 Methods start here
*/
/*---------------------------------------------------------------------------------------*/
void setup_GPS() {
  //Serial 3 (mySerial) is used for GPS
  mySerial.begin(9600);
  GPS.begin(9600);

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
bool AcquireGPS(waypoint &gps_position) {
  Serial.println("Acquire GPS");
  float latitude, longitude;

  //HARD CODED FOR TESTING remove from here to the return true statement down below
  gps_position.latitude = gpsTest[gpsIndex];
  gpsIndex++; 
  Serial.print("Latitude: ");
  Serial.println(gps_position.latitude, 6);
  gps_position.longitude = gpsTest[gpsIndex];
  Serial.print("Longitude: ");
  Serial.println(gps_position.longitude, 6);
  if(gpsIndex == 7)
    gpsIndex = 0;
  else
    gpsIndex++;
  return true;
  
  char c;
  //read atleast 25 characters every loop speed up update time for GPS
  for (int i = 0; i < 25; i++) {
    c = GPS.read();
  }
  delay(1000);
  
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
      Serial.print("Latitude: ");
      Serial.println(gps_position.latitude, 6);
      gps_position.longitude = GPS.longitudeDegrees;
      Serial.print("Longitude: ");
      Serial.println(gps_position.longitude, 6);
      return true;
    }
    return false;
  }
  return false;
}

//setup CAN Bus communication for recieving data from C2
//Recieving actual_speed and an arbitary angle(for the moment)
void C6_communication_with_C2() {
 CAN.watchForRange(Actual_CANID, LowStatus_CANID);  //filter for low level communication
  
  while (CAN.available() > 0) { // check if CAN message available
    CAN.read(incoming);
    Serial.println("Get data from (low level) ID: " + String(incoming.id, HEX));
    Serial.println("Low: " + String((int)incoming.data.low, DEC));
    Serial.println("High: " + String((int)incoming.data.high, DEC));

    if(incoming.id == Actual_CANID) {
    //extractSpeed = receiveData(incoming.data.low);
      extractSpeed = incoming.data.low;
      if  (extractSpeed >= 0) {
        newPos.speed_mmPs = extractSpeed; //upadte acutal speed from C2
      }
      else
        Serial.println("Got a negative speed from C2");
    }
  else 
    Serial.println("Did not receive actual speed, angle from C2");
  }
  
  //Outdated Serial communication replaced by CAN
  /*//setting up receiving data for C6 elcano communication
  ps.dt = &ReceiveData;
  ps.input = &Serial2;
  ps.output = &Serial2;
  ps.capture = MsgType::drive;
  ReceiveData.clear(); */
}

long getHeading(void) {
  //Get a new sensor event from the magnetometer
  sensors_event_t event;
  mag.getEvent(&event);      
  
  //Calculate the current heading (angle of the vector y,x)
  //Normalize the heading
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / PIf;
  Serial.println("acquired heading: " + String(heading));
  if (heading < 0)  {
    heading = 360 + heading;
  }
  delay(3000);
  return heading;
}

//Get your first initial position form the GPS
void initial_position() {
  bool GPS_available = AcquireGPS(estimated_position);

  //This makes an infinite loop when the GPS is not available, comment out or hard code for testing 4-8-19 Mel
  while (!GPS_available) {
    GPS_available = AcquireGPS(estimated_position); 
  }
  
  //Serial.println("Acquired GPS position");
  estimated_position.time_ms = millis();
  estimated_position.Compute_EandN_Vectors(getHeading()); //get position E and N vector
  //Assign Origin GPS position
  //Origin tmpOrg(estimated_position.latitude, estimated_position.longitude);
  //origin = tmpOrg; //assign origin to starting position
  
  Serial.println("Computed Vectors in initial position");
  estimated_position.Compute_mm(origin);  //initialize north and east coordinates for position
  Serial.print("Estimate E: ");
  Serial.println(estimated_position.east_mm);
  Serial.print("Estimate N: ");
  Serial.println(estimated_position.north_mm);
  
  //oldPos to keep track of previous position for DR
  oldPos = estimated_position;
delay(3000);
}

void setup_C6() {
  //setting up the GPS rate
  setup_GPS();

  //Enable auto-gain
  mag.enableAutoRange(true);

  if (!mag.begin()) {
    Serial.println("no LSM303 detected ... Check your wiring!");
  }
  
  mag.begin();
  
  initial_position(); //getting your initial position from GPS comment out if no GPS available
  
  //hard code for testing the exact start positon in the path
  //initializePosition();
  //Serial.println("passed hardcode GPS,  initializePosition()");
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
    Serial.println("Got and computed GPS");
  }
  else {
    Serial.println("Failed to get got_GPS");
  }
  
  /* Receiving data from C2 using CAN Bus */
  C6_communication_with_C2();
  

  /*
   * OUT DATED SERIAL tranfer replaced by CAN
  //Recieving data from C2 using Elcano_Serial
  ParseStateError r = ps.update();
  Serial.print("C6Loop, ParseState Error: ");
  if (r == ParseStateError::success)  {
    Serial.println("Start step2");
    extractSpeed = receiveData(ReceiveData.speed_mmPs);

    if (extractSpeed != -1) { //invalid data from C2 if it's -1
      //speed cannot be below 0
      if  (extractSpeed >= 0) {
        newPos.speed_mmPs = extractSpeed; //upadte acutal speed from C2
      }

    } */
    
  newPos.time_ms = millis();
  //get heading coordinates from the compass
  newPos.bearing_deg = getHeading();
  Serial.println("loop6 newPos.bearing_deg = " + String(newPos.bearing_deg));
  delay(3000);
  if (got_GPS) { //got both GPS and DeadReckoning
    Serial.println("got gps and deadreckoning");
    //to get an esitimation position average between the GPS and Dead Rekoning
    //estimated_position is updated to the current position inside this method
    FindFuzzyCrossPointXY(GPS_reading, newPos, estimated_position);
    Serial.println("-------------- estimated position: " + String(estimated_position.latitude) + ", " + String(estimated_position.longitude));
    
    //calculating the E and N vector by constantly updating everything you move 
    oldPos.vectors(&estimated_position);
     Serial.println("finished gps deadreck");
  }
  else { //Did not get a GPS reading and only DeadReckoning
    Serial.println("Only got dead reckoning");
    // calculate position using Dead Reckoning
    ComputePositionWithDR(oldPos, newPos);

    //calculating the E and N unit vector
    oldPos.vectors(&newPos);

    //update new current positon
    estimated_position.east_mm = newPos.east_mm;
    estimated_position.north_mm = newPos.north_mm;
    Serial.println("finished just deadreck");
  }
    //update E and N vector of current position 
    estimated_position.Evector_x1000 = oldPos.Evector_x1000;
    estimated_position.Nvector_x1000 = oldPos.Nvector_x1000;

    //update old position to current 
    oldPos = estimated_position;
}

/*---------------------------------------------------------------------------------------*/
/**
   All the Methods for C4 starts here
*/
/*---------------------------------------------------------------------------------------*/
// Fill the distances of the junctions in the MAP
void ConstructNetwork(junction *Map, int MapPoints) {
  double deltaX, deltaY;
  int destination;
  for (int i = 0; i < MapPoints; i++) {
    if ( Map[i].east_mm == INVALID)  continue;
    for (int j = 0;  j < 4; j++)  {
      destination = Map[i].destination[j];
      if (destination == END) continue;
      deltaX = Map[i].east_mm;
      deltaX -= Map[destination].east_mm;
      deltaY = Map[i].north_mm;
      deltaY -= Map[destination].north_mm;

      Map[i].Distance[j] += sqrt(deltaX * deltaX + deltaY * deltaY); //in rough scale
    }
  }
}
/*---------------------------------------------------------------------------------------*/
// Set up mission structure from cone latitude and longitude list.
void GetGoals(junction *nodes , int Goals)  {
  double deltaX, deltaY, Distance;
  for (int i = 0; i < CONES; i++) {
    mission[i].latitude = goal_lat[i];
    mission[i].longitude = goal_lon[i];
    mission[i].Compute_mm(origin);
    mission[i].speed_mmPs = DESIRED_SPEED_mmPs;
    mission[i].index = 1 | GOAL;
    mission[i].sigma_mm = 1000;
    mission[i].time_ms = 0;

    if (i == 0) { //If CONE == 1
      mission[i].Evector_x1000 = 1000;
      mission[i].Nvector_x1000 = 0;
    }
    else  {
      deltaX = mission[i].east_mm - mission[i - 1].east_mm;
      deltaY = mission[i].north_mm - mission[i - 1].north_mm;
      Distance = sqrt(deltaX * deltaX + deltaY * deltaY);
      mission[i - 1].Evector_x1000 = (deltaX * 1000.) / Distance;
      mission[i - 1].Nvector_x1000 = (deltaY * 1000.) / Distance;
    }
    if (i == CONES - 1) {
      mission[i].Evector_x1000 = mission[i - 1].Evector_x1000;
      mission[i].Nvector_x1000 = mission[i - 1].Nvector_x1000;
      mission[i].index |= END;
    }
  }
}
/*---------------------------------------------------------------------------------------*/
// Find the distance from (east_mm, north_mm) to a road segment Nodes[i].distance[j]
// return distance in mm, and per cent of completion from i to j.
// distance is negative if (east_mm, north_mm) lies to the left of the road
// when road direction is from i to j

// Compare this routine to distance() in C3 Pilot
//k =  index into Nodes[]
//east_mm : current
long distance(int cur_node, int *k,  long cur_east_mm, long cur_north_mm, int* perCent) {
  float deltaX, deltaY, dist_mm;
  int cur , destination;
  long Eunit_x1000, Nunit_x1000;
  long closest_mm = MAX_DISTANCE;
  long Road_distance, RoadDX_mm, RoadDY_mm;

  long pc; //per cent of completion from i to j.

  *perCent = 0;
  *k = 0;
  closest_mm = MAX_DISTANCE;


  for (cur = 0; cur < 4; cur++) { // Don't make computations twice.
    destination = Nodes[cur_node].destination[cur];
    if (destination == 0 || destination < cur_node) continue;  //replace Destination with END
    // compute road unit vectors from i to cur
    RoadDX_mm = Nodes[destination].east_mm - Nodes[cur_node].east_mm;

    //      Serial.println("RoadX_mm " + String(RoadDX_mm));
    //      Serial.println("Destination " + String(destination));
    //      Serial.println("Nodes[destination].east_mm " + String(Nodes[destination].east_mm));
    //      Serial.println("-Nodes[cur_loc].east_mm " + String(-Nodes[cur_node].east_mm));
    //
    int Eunit_x1000 = RoadDX_mm  * 1000 / Nodes[cur_node].Distance[cur];

    RoadDY_mm =  Nodes[destination].north_mm - Nodes[cur_node].north_mm;
    //
    //      Serial.println("RoadY_mm " + String(RoadDY_mm));
    //      Serial.println("Nodes[destination].north_mm " + String(Nodes[destination].north_mm));
    //      Serial.println("-Nodes[cur_loc].north_mm " + String(-Nodes[cur_node].north_mm));
    //
    int Nunit_x1000 = RoadDY_mm * 1000 / Nodes[cur_node].Distance[cur];
    //
    //      // normal vector is (Nunit, -Eunit)
    //      //Answers: What would be the change in X/Y from my current Node.
    //      deltaX = cur_east_mm - Nodes[cur_node].east_mm;
    //      deltaY = cur_north_mm - Nodes[cur_node].north_mm;
    //
    //
    //      // sign of return value gives which side of road it is on.
    //      Road_distance = (-deltaY * Eunit_x1000 + deltaX * Nunit_x1000) / 1000;
    Road_distance = sqrt( (RoadDX_mm * RoadDX_mm) + (RoadDY_mm * RoadDY_mm));
    //Why do percentage computation like this?
    pc = (deltaX * Eunit_x1000 + deltaY * Nunit_x1000) / (Nodes[cur_node].Distance[cur] * 10);

    //     Serial.println("Closest_mm " + String(closest_mm) + "\t Road_distance " + String(Road_distance));
    //     Serial.println("Road Distance " + String(Road_distance));
    //     Serial.println("closest Distance " + String(closest_mm));
    if (abs(Road_distance) < abs(closest_mm) && pc >= 0 && pc <= 100) {
      closest_mm = Road_distance;
      *k = destination;
      *perCent = pc;

    }
  }
  return closest_mm;
}
/*---------------------------------------------------------------------------------------*/
//Figuring out a path to get the road network
void FindClosestRoad(waypoint *start, waypoint *road) {  //populate road with best road from start
  long closest_mm = MAX_DISTANCE;
  long dist;
  int close_index;
  int perCent;
  long done = 1;//2000;
  int i, node_successor;

  for (i = 0; i < 5/*map_points*/; i++) { // find closest road.
    dist = distance(i, &node_successor, start->east_mm, start->north_mm, &perCent); //next node to visit
    Serial.println("Start : Latitude " + String(start->latitude) + "\t Longitude " + String(start->longitude) + "\t Dist "
                   + String(dist));

    if (abs(dist) < abs(closest_mm))  {
      close_index = node_successor;
      closest_mm = dist;
      done = 1;// perCent; //Not really true amount of nodes done?
      road->index = i;
      road->sigma_mm = node_successor;
    }
  }
  if (closest_mm < MAX_DISTANCE)  {
    i = road->index; //0
    node_successor = close_index; //0
    road->east_mm =  Nodes[i].east_mm  +  done * (Nodes[node_successor].east_mm  - Nodes[i].east_mm) / 100;
    road->north_mm = Nodes[i].north_mm + done * (Nodes[node_successor].north_mm - Nodes[i].north_mm) / 100;
  }
  else  {
    for (i = 0; i < 5/*map_points*/; i++) { // find closest node
      //Serial.println("I got here");
      dist = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);

      if (dist < closest_mm)  {
        close_index = i;
        closest_mm = dist;
      }
    }
    road->index = road->sigma_mm = close_index;
    road->east_mm =  Nodes[close_index].east_mm;
    road->north_mm = Nodes[close_index].north_mm;
  }

  road->Evector_x1000 = 1000;
  road->Nvector_x1000 = 0;
  road->time_ms = 0;
  road->speed_mmPs = DESIRED_SPEED_mmPs;

  //Test FindClosest Road:
  Serial.println("Distance " + String(dist));
  Serial.println("Road :  East_mm " + String(road->east_mm) + "\t North_mm " + String(road->north_mm));

}

//Test ClosestRoad:
void test_closestRoad() {

  waypoint roadorigin;
  waypoint roadDestination;

  Serial.println("First " );
  FindClosestRoad(&mission[0], &roadorigin);

  Serial.println();

  Serial.println("Second ");
  FindClosestRoad(&mission[3], &roadDestination);
  for (int last = 0; last < 4; last++) {
    Serial.println(" mission " + String(mission[last].east_mm ) + "\t roadorigin " + String(roadorigin.east_mm));
    Serial.println(" mission " + String(mission[last].longitude ) + "\t roadorigin " + String(roadorigin.east_mm));
  }
  for (int last = 0; last < 4; last++) {
    Serial.println(" mission " + String(mission[last].east_mm ) + "\t roadorigin " + String(roadDestination.east_mm));
    Serial.println(" mission " + String(mission[last].longitude ) + "\t roadorigin " + String(roadDestination.north_mm));
  }
}
/*---------------------------------------------------------------------------------------*/
// start and destination are on the road network given in Nodes.
// start is in Path[1].
// Place other junction waypoints into Path.
// Returned value is next index into Path.
// start->index identifies the closest node.
// sigma_mm holds the index to the other node.
// A* is traditionally done with pushing and popping node from an Open and Closed list.
// Since we have a small number of nodes, we instead reserve a slot on Open and Closed
// for each node.

int BuildPath (int j, waypoint* start, waypoint* destination) { // Construct path backward to start.
  Serial.println("To break");
  int last = 1;
  int route[map_points];
  int i, k, node;
  long dist_mm;

  k = map_points - 1;
  route[k] = j;

  while (Open[j].ParentID != currentlocation) {
    j = route[--k] = Open[j].ParentID;
  }

  path[last] = start;
  for ( ; k < map_points; k++)  {
    node = route[k];
    path[++last].east_mm = Nodes[node].east_mm;
    path[last].north_mm  = Nodes[node].north_mm;
  }
  path[++last] = destination;
  for (k = 0; k <= last; k++) {
    if (k > 0) path[k].sigma_mm = 10; // map should be good to a cm.
    path[k].index = k;
    path[k].speed_mmPs = DESIRED_SPEED_mmPs;
    path[k].Compute_LatLon(origin);  // this is never used
  }
  last++;
  for (j = 0; j < last - 1; j++)  {
    path[j].vectors(&path[j + 1]);
  }

  return last;
}
/*---------------------------------------------------------------------------------------*/

void test_buildPath() {

  //  BuildPath(0, Path, destination);
}
//Usa A star
int FindPath(waypoint *start, waypoint *destination)  { //While OpenSet is not empty

  Serial.println("Start East_mm " + String(start->east_mm) + "\t North " + String(start->north_mm));
  Serial.println("Start East_mm " + String(destination->east_mm) + "\t North " + String(destination->north_mm));
  long ClosedCost[map_points];
  int  i, neighbor, k;
  long NewCost, NewStartCost, NewCostToGoal;
  long NewIndex;
  long BestCost, BestID;
  bool Processed = false;

  for (i = 0; i < map_points; i++)  { // mark all nodes as empty
    Open[i].TotalCost = MAX_DISTANCE;
    ClosedCost[i] = MAX_DISTANCE;
  }

  i = start->index; // get successor nodes of start
  Open[i].CostFromStart = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
  Open[i].CostToGoal = destination->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);

  Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;
  Open[i].ParentID = currentlocation;

  i = start->sigma_mm;
  Open[i].CostFromStart = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
  Open[i].CostToGoal = destination->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
  Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;

  Open[i].ParentID = currentlocation;
  while (BestCost < MAX_DISTANCE) { //While OpenSet is not empty
    BestCost = MAX_DISTANCE;
    BestID = -1;
    // pop lowest cost node from Open; i.e. find index of lowest cost item
    for (i = 0; i < 6; i++) {
      if (Open[i].TotalCost < BestCost) {
        BestID = i;
        //            Serial.println("BESTID " + String(BestID));
        BestCost = Open[i].TotalCost;
        //            Serial.println("BestCost " + String(BestCost));
      }

    }
    if (BestID < 0) {
      return INVALID;
    }
    Serial.println("BESTID " + String(BestID));
    Serial.println("DestinationINdex " + String(destination->index));
    Open[BestID].TotalCost = MAX_DISTANCE;  // Remove node from "stack".
    if (BestID == destination->index || BestID == destination->sigma_mm)  { // Done:: reached the goal!!

      return BuildPath(BestID, start, destination);   // Construct path backward to start.
    }

    i = BestID;  // get successor nodes from map

    for (neighbor = 0; neighbor < 5; neighbor++)  {
      NewIndex = Nodes[i].destination[neighbor];

      if (NewIndex == END)continue; // No success in this slot

      NewStartCost =  Open[i].CostFromStart + Nodes[i].Distance[neighbor];
      NewCostToGoal = destination->distance_mm(Nodes[NewIndex].east_mm, Nodes[NewIndex].north_mm);
      NewCost = NewStartCost + NewCostToGoal;

      if (NewCost >= ClosedCost[NewIndex]) // check if this node is already on Open or Closed.
        continue;  // Have already looked at this node

      else if (ClosedCost[NewIndex] != MAX_DISTANCE) { // looked at this node before, but at a higher cost
        ClosedCost[NewIndex] = MAX_DISTANCE;  // remove node from Closed
      }
      if (NewCost >= Open[NewIndex].TotalCost)
        continue;   // This node is a less efficient way of getting to a node on the list
      // Push successor node onto stack.

      Open[NewIndex].CostFromStart = NewStartCost;
      Open[NewIndex].CostToGoal = NewCostToGoal;
      Open[NewIndex].TotalCost = NewCost;
      Open[NewIndex].ParentID = i;
    }  // end of successor nodes


    ClosedCost[BestID] =  BestCost; // Push node onto Closed
  }

  Serial.println("Destination East_mm " + String(destination->east_mm) + "\t North " + String(destination->north_mm));

  return 0;  // failure
}

/*---------------------------------------------------------------------------------------*/
// Low level path is a straight line from start to detination.
// PathPlan makes an intermediate level path that uses as many roads as possible.
//start = currentlocation: destination = heading to;
//Find the cloeset road and call Findpath to do the A star
int PlanPath (waypoint *start, waypoint *destination) {

  //Serial.println("Start : East_mm = " + String(start->east_mm) + "\t North_mm =  " + String(start->north_mm));
  waypoint roadorigin, roadDestination;

  int last = 0;
  path[0] = start;
  path[0].index = 0;

  FindClosestRoad( start, &roadorigin );
  FindClosestRoad( destination, &roadDestination );

  int w = abs(start->east_mm  - roadorigin.east_mm) + abs(start->north_mm - roadorigin.north_mm);
  int x = abs(destination->east_mm  - roadDestination.east_mm)  + abs(destination->north_mm - roadDestination.north_mm);

  int straight_dist = 190 * abs(start->east_mm  - destination->east_mm) +  abs(start->north_mm - destination->north_mm);
  if (w + x >= straight_dist) { // don't use roads; go direct
    last = 1;
    Serial.println("In Straight");
  }
  else {  // use A* with the road network
    Serial.println("In Else");
    path[1] = roadorigin;
    path[1].index = 1;
    //why index = 7?
    destination -> index = 7;
    last = FindPath(&roadorigin, &roadDestination);
  }

  path[last] = destination;
  path[last - 1].vectors(&path[last]);
  path[last].Evector_x1000 = path[last - 1].Evector_x1000;
  path[last].Nvector_x1000 = path[last - 1].Nvector_x1000;
  path[last].index = last | END;

  //    Serial.println("Destination : East_mm = " + String(destination->east_mm) + "\t North_mm =  " + String(destination->north_mm));
  Serial.println();

  return last;

}
/*---------------------------------------------------------------------------------------*/
// LoadMap
// Loads the map nodes from a file.
// Takes in the name of the file to load and loads the appropriate map.
// Returns true if the map was loaded.
// Returns false if the load failed.
boolean LoadMap(char* fileName) {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open(fileName, FILE_READ);

  // if the file opened okay, read from it:
  if (myFile) {
    // Initialize a string buffer to read lines from the file into
    // Allocate an extra char at the end to add null terminator
    char* buffer = (char*)malloc(myFile.size() + 1);

    // index for the next character to read into buffer
    char* ptr = buffer;

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      *ptr = myFile.read();
      ++ptr;
    }
    // Null terminate the buffer string
    *ptr = '\0';

    // Set up tokenizer for the file buffer string
    char * delimiter = " ,\n";
    char* token;
    int col = 0;
    int row = 0;

    // get the first token
    token = strtok(buffer, delimiter);

    // walk through other tokens
    while ( token != NULL ) {
      switch (col % 10) {
        case 0:  // latitude
          Nodes[row].east_mm = atol(token);

          col++;
          break;

        case 1:  // longitude
          Nodes[row].north_mm = atol(token);
          col++;
          break;

        case 2:  // filename
          if (token == "END") {
            Nodes[row].destination[0] = END;
          }
          else  {
            Nodes[row].destination[0] = atoi(token);
          }
          col++;
          break;

        case 3:  // filename
          if (token == "END") {
            Nodes[row].destination[1] = END;
          }
          else  {
            Nodes[row].destination[1] = atoi(token);
          }
          col++;
          break;

        case 4:  // filename
          if (token == "END") {
            Nodes[row].destination[2] = END;
          }
          else  {
            Nodes[row].destination[2] = atoi(token);
          }
          col++;
          break;

        case 5:  // filename
          if (token == "END") {
            Nodes[row].destination[3] = END;
          }
          else  {
            Nodes[row].destination[3] = atoi(token);
          }
          col++;
          break;

        case 6:  // filename
          Nodes[row].Distance[0] = atol(token);
          col++;
          break;

        case 7:  // filename
          Nodes[row].Distance[1] = atol(token);
          col++;
          break;

        case 8:  // filename
          Nodes[row].Distance[2] = atol(token);
          col++;
          break;

        case 9:  // filename
          Nodes[row].Distance[3] = atol(token);

          //this method below needs to be looked at in Common.cpp
          //convertLatLonToMM(Nodes[row].east_mm, Nodes[row].north_mm);
          col++;
          row++;
          break;

        default:  // unexpected condition; print error
          Serial.print("Unexpected error happened while reading map description file.");
          Serial.print("Please verify the file is in the correct format.");
          Serial.println("Planner may not work correctly if this message appears.");
          break;
      }

      token = strtok(NULL, delimiter);
      Serial.println(token);
    }
    map_points = row;
    Serial.println("Test map");
    Serial.println(map_points);
    //To test LoadMap:
    for (int i = 0; i < map_points; i++)  {
      Serial.print("inside the loop: ");
      Serial.println(i);
      Serial.print(Nodes[i].east_mm);
      Serial.print(",");
      Serial.print(Nodes[i].north_mm);
      Serial.print(",");
      Serial.print(Nodes[i].destination[0]);
      Serial.print(",");
      Serial.print(Nodes[i].destination[1]);
      Serial.print(",");
      Serial.print(Nodes[i].destination[2]);
      Serial.print(",");
      Serial.print(Nodes[i].destination[3]);
      Serial.print(",");
      Serial.print(Nodes[i].Distance[0]);
      Serial.print(",");
      Serial.print(Nodes[i].Distance[1]);
      Serial.print(",");
      Serial.print(Nodes[i].Distance[2]);
      Serial.print(",");
      Serial.println(Nodes[i].Distance[3]);
    }

    // If file loaded, read data into Nodes[]
    free(buffer);
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    myFile.close();
    Serial.println("error opening");
    Serial.println(fileName);
    return false;
  }
  return true;
}

/*---------------------------------------------------------------------------------------*/
// SelectMap:  Return the file name of the closest origin
// Determines which map to load.
// Takes in the current location as a waypoint and a string with the name of the file that
//   contains the origins and file names of the maps.
// Determines which origin is closest to the waypoint and returns it as a junction.
// Assumes the file is in the correct format according to the description above.
void SelectMap(waypoint currentLocation, char* fileName, char* nearestMap)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open(fileName, FILE_READ);

  // if the file opened okay, read from it:
  if (myFile) {
    // Initialize a string buffer to read lines from the file into
    // Allocate an extra char at the end to add null terminator
    char* buffer = (char*)malloc(myFile.size() + 1);

    //Serial.println("Printing file info");
    // index for the next character to read into buffer
    char* ptr = buffer;

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      *ptr = myFile.read();
      ++ptr;
    }

    // Null terminate the buffer string
    *ptr = '\0';

    // Set up storage for coordinates and file names
    // Note: we malloc here because the stack is too small
    float* map_latitudes = (float*)malloc(MAX_MAPS * 4);
    float* map_longitudes = (float*)malloc(MAX_MAPS * 4);
    char** map_file_names = (char**)malloc(MAX_MAPS * sizeof(char*));

    for (int i = 0; i < MAX_MAPS; i++)  {
      // initialize using invalid values so that we can ensure valid data in allocated memory
      map_latitudes[i] = 91;
      map_longitudes[i] = 181;
      map_file_names[i] = "";
    }

    // Set up tokenizer for the file buffer string
    char *delimiter = " ,\n";
    char* token;
    int col = 0;
    int row = 0;
    char* t;


    // get the first token
    token = strtok(buffer, delimiter);

    // fill the map_latitude, map_longitude, & map_file with tokens
    while ( token != NULL ) {
      switch (col % 3)  {
        case 0:  // latitude
          map_latitudes[row] = atof(token);

          Serial.print("index: ");
          Serial.println(row);
          Serial.println(map_latitudes[row]);
          col++;
          break;

        case 1:  // longitude
          map_longitudes[row] = atof(token);
          col++;
          Serial.print("index: ");
          Serial.println(row);
          Serial.println(map_longitudes[row]);
          break;

        case 2:  // filename
          map_file_names[row] = token;
          col++;
          Serial.print("index: ");
          Serial.println(row);
          Serial.println(map_file_names[row]);
          row++;
          break;

        default:  // unexpected condition; print error
          Serial.println("Unexpected error happened while reading map description file. Please verify the file is in the correct format. Planner may not work correctly if this message appears.");
          break;
      }
      token = strtok(NULL, delimiter);
    }
    int closestIndex = -1;
    long closestDistance = MAX_DISTANCE;
    for (int i = 0; i < MAX_MAPS; i++)  {
      int dist = fabs((map_latitudes[i] - currentLocation.latitude)) +
                 abs(map_longitudes[i] - currentLocation.longitude);
      if (dist < closestDistance) {
        closestIndex = i;
        closestDistance = dist;
      }
    }
    if (closestIndex >= 0)  {
      // Determine closest map to current location
      // Update origin global variable
      origin.latitude = map_latitudes[closestIndex];
      origin.longitude = map_longitudes[closestIndex];
      for (int i = 0; i < 13; i++)
      {
        nearestMap[i] = map_file_names[closestIndex][i];
      }
    }
    else  {
      Serial.println("error determining closest map.");
    }
    // Free the memory allocated for the buffer
    free(buffer);

    free(map_latitudes);
    free(map_longitudes);
    free(map_file_names);

    // close the file:
    myFile.close();
    Serial.println("Map definitions loaded.");
  } else {
    // if the file didn't open, print an error:
    myFile.close();
    Serial.println("error opening MAP_DEFS.txt");
  }

}
/*---------------------------------------------------------------------------------------*/
/**
   All the Methods for C4 starts here
*/
/*---------------------------------------------------------------------------------------*/
void initialize_C4() {
  //Store the initial GPS latitude and longtitude to select the correct map
  Start.latitude = estimated_position.latitude;
  Start.longitude = estimated_position.longitude;

  Serial.println("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial3.println("initialization failed!");
  }
  Serial.println("initialization done.");
  char nearestMap[13] = "";

  SelectMap(Start, "MAP_DEFS.txt", nearestMap); //populates info from map_def to nearestMap;

  //Serial.println(nearestMap);

  //populate nearest map in junction Nodes structure
  LoadMap(nearestMap);

  //takes in the Nodes that contains all of the map
  ConstructNetwork(Nodes, map_points); //To fill out the rest of the nodes info

  GetGoals(Nodes, CONES);
}

//Test mission::a list of waypoints to cover
void test_mission() {

  for (int i = 0; i < map_points; i++)  {
    Serial.println("mission " + String(i) + " = " + String(mission[i].latitude) + "\t north_mm " + String(mission[i].north_mm));
  }
}

///MOCK_MAP.TXTT
void test_path() {

  for (int i = 0; i < 4; i++) {
    Serial.println("Path " + String(path[i].east_mm));
  }
}
void test_distance()  {
  int percent ;
  int destination;
  int dist = distance(0, &destination , 0 , 0, &percent);

  Serial.println("dist " + String(dist));
}
void setup_C4() {
  initialize_C4(); //Start selecting/load map and start planning path

  //set the Start to the first Node
  Start.east_mm = Nodes[0].east_mm;
  Start.north_mm = Nodes[0].north_mm;

  Serial.println("Start planning path");
  last_index_of_path = PlanPath (&Start, &mission[0]);

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
  /*ps3.dt = &SendData;
  ps3.input = &Serial2; //connection to read from
  ps3.output = &Serial2; //connection to write to
  ps3.capture = MsgType::drive;
  SendData.clear();*/
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
  Serial.println("Entered C3 setup");
  state = STRAIGHT;
  //the path is set to approach the first intersection at index 1
  next = 1;

  //Setting up for sending data from C3 to C2
  C3_communication_with_C2();
  Serial.println("Passed C3 communication to C2");
}

void loop_C3() {
  Serial.println("Entered the C3 loop");
  //Determining the state of the Trike
  find_state(turn_radius_mm, next);

  //Determining the turn direction for the trike "left, right or straight"
  turn_direction = get_turn_direction_angle(next);

  //Send speed and angle to C2 to diplay the Led on the test stance
  //only send data to C2 if we get new data. Avoid sending the same data
 
  speed_mmPs = speeds[speedIndex];
  turn_direction = angg[speedIndex];
  if(speedIndex == 5)
    speedIndex = 0;
  else
    speedIndex++;
  if (pre_desired_speed != speed_mmPs || pre_turn_angle != turn_direction) {
    Serial.println("Sending C3 to C2");
    CAN_FRAME output; //intialize can frame to carry message to C2
    output.length = MAX_CAN_FRAME_DATA_LEN_16;
    output.id = HiDrive_CANID; //Drive instructions from hilevel board
    //output.data.low = sendData(speed_mmPs);
    output.data.low = speed_mmPs;
    output.data.high = turn_direction;
    
    //Serial.println("###" + String(speed_mmPs) + ", becomes: " + String(output.data.low));
    
    Serial.println("**Sending: Speed: " + String(output.data.low) + " Angl: " + (output.data.high));
    
    CAN.sendFrame(output); //send the message
    
    /*SendData.clear();
    SendData.kind = MsgType::drive;
    //chheck this
    SendData.speed_mmPs = sendData(speed_mmPs);
    SendData.angle_mDeg = scaleDownAngle(turn_direction);
    SendData.write(&Serial2);*/

    pre_desired_speed = speed_mmPs;
    pre_turn_angle = scaleDownAngle(turn_direction);
  }

}
void setup() {
  //for the micro SD
  
  pinMode(chipSelect, OUTPUT);

  Serial.begin(9600);
  Serial2.begin(9600);
  if (CAN.begin(CAN_BPS_500K)) { // initalize CAN with 500kbps baud rate 
    Serial.println("init success");
  }

  Serial.println("Starting C6");
  //populate_path(); //hard code path in replacement of C4
  setup_C6();
  //setup_C4();
  Serial.println("Starting C3");
  setup_C3();
}

void loop() {
  Serial.println("Starting C6 loop");
  loop_C6();
  //RE-compute path if too far off track (future development) for C4
  Serial.println("Starting C3 loop");
  loop_C3();
  delay(6000);
}
