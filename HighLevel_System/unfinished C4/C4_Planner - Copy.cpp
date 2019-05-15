#include "C4_Planner.h"

#define CONES 1
//pre-defined goal/destination to get to
long goal_lat[CONES] = {47760934};
long goal_lon[CONES] = { -122189963};
//long goal_lat[CONES] = {  47621881,   47621825,   47623144,   47620616,   47621881};
//long goal_lon[CONES] = {-122349894, -122352120, -122351987, -122351087, -122349894};

Junction Nodes[MAX_WAYPOINTS]; //Storing the loaded map

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

// Fill the distances of the Junctions in the MAP
void C4_Planner::ConstructNetwork(Junction *Map, int MapPoints) {
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
void C4_Planner::GetGoals(Junction *nodes , int Goals)  {
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
     // Distance = sqrt(deltaX * deltaX + deltaY * deltaY);
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
long C4_Planner::distance(int cur_node, int *k,  long cur_east_mm, long cur_north_mm, int* perCent) {
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

    //      if(DEBUG){
    //        Serial.println("RoadX_mm " + String(RoadDX_mm));
    //        Serial.println("Destination " + String(destination));
    //        Serial.println("Nodes[destination].east_mm " + String(Nodes[destination].east_mm));
    //        Serial.println("-Nodes[cur_loc].east_mm " + String(-Nodes[cur_node].east_mm));
    //      }
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
   // Road_distance = sqrt( (RoadDX_mm * RoadDX_mm) + (RoadDY_mm * RoadDY_mm));
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
void C4_Planner::FindClosestRoad(waypoint *start, waypoint *road) {  //populate road with best road from start
  long closest_mm = MAX_DISTANCE;
  long dist;
  int close_index;
  int perCent;
  long done = 1;//2000;
  int i, node_successor;

  for (i = 0; i < 5/*map_points*/; i++) { // find closest road.
    dist = distance(i, &node_successor, start->east_mm, start->north_mm, &perCent); //next node to visit
    if(DEBUG)  Serial.println("Start : Latitude " + String(start->latitude) + "\t Longitude " + String(start->longitude) + "\t Dist "
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
  if(DEBUG)  Serial.println("Distance " + String(dist));
  if(DEBUG)  Serial.println("Road :  East_mm " + String(road->east_mm) + "\t North_mm " + String(road->north_mm));

}

//Test ClosestRoad:
void C4_Planner::test_closestRoad() {

  waypoint roadorigin;
  waypoint roadDestination;

  if(DEBUG)  Serial.println("First " );
  FindClosestRoad(&mission[0], &roadorigin);

  if(DEBUG)  Serial.println();

  if(DEBUG)  Serial.println("Second ");
  FindClosestRoad(&mission[3], &roadDestination);
  for (int last = 0; last < 4; last++) {
    if(DEBUG)  Serial.println(" mission " + String(mission[last].east_mm ) + "\t roadorigin " + String(roadorigin.east_mm));
    if(DEBUG)  Serial.println(" mission " + String(mission[last].longitude ) + "\t roadorigin " + String(roadorigin.east_mm));
  }
  for (int last = 0; last < 4; last++) {
    if(DEBUG)  Serial.println(" mission " + String(mission[last].east_mm ) + "\t roadorigin " + String(roadDestination.east_mm));
    if(DEBUG)  Serial.println(" mission " + String(mission[last].longitude ) + "\t roadorigin " + String(roadDestination.north_mm));
  }
}
/*---------------------------------------------------------------------------------------*/
// start and destination are on the road network given in Nodes.
// start is in Path[1].
// Place other Junction waypoints into Path.
// Returned value is next index into Path.
// start->index identifies the closest node.
// sigma_mm holds the index to the other node.
// A* is traditionally done with pushing and popping node from an Open and Closed list.
// Since we have a small number of nodes, we instead reserve a slot on Open and Closed
// for each node.

int C4_Planner::BuildPath (int j, waypoint* start, waypoint* destination) { // Construct path backward to start.
  if(DEBUG)  Serial.println("To break");
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

void C4_Planner::test_buildPath() {

  //  BuildPath(0, Path, destination);
}
//Usa A star
int C4_Planner::FindPath(waypoint *start, waypoint *destination)  { //While OpenSet is not empty

  if(DEBUG)  Serial.println("Start East_mm " + String(start->east_mm) + "\t North " + String(start->north_mm));
  if(DEBUG)  Serial.println("Start East_mm " + String(destination->east_mm) + "\t North " + String(destination->north_mm));
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
    if(DEBUG) Serial.println("BESTID " + String(BestID));
    if(DEBUG)  Serial.println("DestinationINdex " + String(destination->index));
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

  if(DEBUG)  Serial.println("Destination East_mm " + String(destination->east_mm) + "\t North " + String(destination->north_mm));

  return 0;  // failure
}

/*---------------------------------------------------------------------------------------*/
// Low level path is a straight line from start to detination.
// PathPlan makes an intermediate level path that uses as many roads as possible.
//start = currentlocation: destination = heading to;
//Find the cloeset road and call Findpath to do the A star
int C4_Planner::PlanPath (waypoint *start, waypoint *destination) {

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
    if(DEBUG)  Serial.println("In Straight");
  }
  else {  // use A* with the road network
    if(DEBUG)  Serial.println("In Else");
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
  if(DEBUG)  Serial.println();

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
          if(DEBUG)  Serial.print("Unexpected error happened while reading map description file.");
          if(DEBUG)  Serial.print("Please verify the file is in the correct format.");
          if(DEBUG)  Serial.println("Planner may not work correctly if this message appears.");
          break;
      }

      token = strtok(NULL, delimiter);
      if(DEBUG)  Serial.println(token);
    }
    map_points = row;
    if(DEBUG)  Serial.println("Test map");
    if(DEBUG)  Serial.println(map_points);
    //To test LoadMap:
    for (int i = 0; i < map_points; i++)  {
      if(DEBUG)  Serial.print("inside the loop: ");
      if(DEBUG)  Serial.println(i);
      if(DEBUG)  Serial.print(Nodes[i].east_mm);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].north_mm);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].destination[0]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].destination[1]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].destination[2]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].destination[3]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].Distance[0]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].Distance[1]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.print(Nodes[i].Distance[2]);
      if(DEBUG)  Serial.print(",");
      if(DEBUG)  Serial.println(Nodes[i].Distance[3]);
    }

    // If file loaded, read data into Nodes[]
    free(buffer);
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    myFile.close();
    if(DEBUG)  Serial.println("error opening");
    if(DEBUG)  Serial.println(fileName);
    return false;
  }
  return true;
}

/*---------------------------------------------------------------------------------------*/
// SelectMap:  Return the file name of the closest origin
// Determines which map to load.
// Takes in the current location as a waypoint and a string with the name of the file that
//   contains the origins and file names of the maps.
// Determines which origin is closest to the waypoint and returns it as a Junction.
// Assumes the file is in the correct format according to the description above.
void C4_Planner::SelectMap(waypoint currentLocation, char* fileName, char* nearestMap)
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

          if(DEBUG) {
            Serial.print("index: ");
            Serial.println(row);
            Serial.println(map_latitudes[row]);
          }
          col++;
          break;

        case 1:  // longitude
          map_longitudes[row] = atof(token);
          col++;
          if(DEBUG) {
            Serial.print("index: ");
            Serial.println(row);
            Serial.println(map_longitudes[row]);
          }
          break;

        case 2:  // filename
          map_file_names[row] = token;
          col++;
          if (DEBUG) {
            Serial.print("index: ");
            Serial.println(row);
            Serial.println(map_file_names[row]);
          }
          row++;
          break;

        default:  // unexpected condition; print error
          if (DEBUG) Serial.println("Unexpected error happened while reading map description file. Please verify the file is in the correct format. Planner may not work correctly if this message appears.");
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
      if(DEBUG)  Serial.println("error determining closest map.");
    }
    // Free the memory allocated for the buffer
    free(buffer);

    free(map_latitudes);
    free(map_longitudes);
    free(map_file_names);

    // close the file:
    myFile.close();
    if(DEBUG)  Serial.println("Map definitions loaded.");
  } else {
    // if the file didn't open, print an error:
    myFile.close();
    if(DEBUG)  Serial.println("error opening MAP_DEFS.txt");
  }

}
/*---------------------------------------------------------------------------------------*/
/**
   All the Methods for C4 starts here
*/
/*---------------------------------------------------------------------------------------*/
void C4_Planner::initialize_C4() {
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

  //populate nearest map in Junction Nodes structure
  LoadMap(nearestMap);

  //takes in the Nodes that contains all of the map
  ConstructNetwork(Nodes, map_points); //To fill out the rest of the nodes info

  GetGoals(Nodes, CONES);
}

//Test mission::a list of waypoints to cover
void C4_Planner::test_mission() {

  for (int i = 0; i < map_points; i++)  {
    Serial.println("mission " + String(i) + " = " + String(mission[i].latitude) + "\t north_mm " + String(mission[i].north_mm));
  }
}

///MOCK_MAP.TXTT
void C4_Planner::test_path() {

  for (int i = 0; i < 4; i++) {
   // Serial.println("Path " + String(path[i].east_mm));
  }
}
void C4_Planner::test_distance()  {
  int percent ;
  int destination;
  int dist = distance(0, &destination , 0 , 0, &percent);

  Serial.println("dist " + String(dist));
}
C4_Planner::C4_Planner::() {
  initialize_C4(); //Start selecting/load map and start planning path

  //set the Start to the first Node
  Start.east_mm = Nodes[0].east_mm;
  Start.north_mm = Nodes[0].north_mm;

  Serial.println("Start planning path");
  last_index_of_path = PlanPath (&Start, &mission[0]);

}
