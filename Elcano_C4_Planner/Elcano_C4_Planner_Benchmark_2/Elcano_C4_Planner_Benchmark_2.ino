#include <Common.h>
#include <IO.h>

#include <SPI.h>
#include <SD.h>

/*  
Elcano Module C4: Path Planner.

Output to C6: Desired route and speed curves and whether it exits a road or changes lane.
Input: RNDF, MDF and initial position files from memory stick.
Input from C5: Speed and position of obstacles.
Input from C6: Position, orientation, velocity and acceleration.

/*---------------------------------------------------------------------------------------

Files:

Out of Scope for 3/13/2015 due date: 
{
  RNDF (Route Network Definition File). Format is defined on 
  http://archive.darpa.mil/grandchallenge/docs/RNDF_MDF_Formats_031407.pdf. 
  This is a digital map of all roads in the area where the vehicle will be operating. 
  The camera will locate road edges or lane markers and the vehicle will follow them. 
  Thus location can be determined primarily from odometry.
  
  MDF (Mission Definition File). These are latitudes and longitudes that the vehicle 
  is required to visit.
}
We attempted to research the above file system; however, it has been archived and the
examples are no longer available. Rather than spending extra time reading through the
documentation to implement it, we decided to focus on implementing a file system with
comma delimited values for the data we need to build junction objects.

-----------------------
|     MAP_DEFS.txt    |
-----------------------

This file provides the latitude and longitude coordinates for each of the maps, followed
by the file name for that map. Commas also separate each map.

The format should be as follows:

latitude_0,longitude_0,filename_0.txt,
...,
latitude_n,longitude_n,filename_n.txt


A practical example would look like this:

47.758949,-122.190746,MAP001.txt,
47.6213,-122.3509,MAP002.txt


-----------------------
|      map files      |
-----------------------

These files provide the junction data. The junction struct has the following variables:

long east_mm
long north_mm
int destination[4]
long Distance[4]

east_mm is the position East of the origin in millimeters
north_mm is the position North of the origin in millimeters
destination is an array of indeces into the Nodes[] array that connect to this node
Distance is an array of longs holding the distances from this node to each of the destinations
  in millimeters.

The file format is a comma delimited list of the values in the struct, with a comma after each junction.
It should be formated as follows:

east_mm_0,north_mm_0,destination_0[0],destination_0[1],destination_0[2],destination_0[3],Distance_0[0],Distance_0[1],Distance_0[2],Distance_0[3],
...,
east_mm_n,north_mm_n,destination_n[0],destination_n[1],destination_n[2],destination_n[3],Distance_n[0],Distance_n[1],Distance_n[2],Distance_n[3],

A practical example would look like this:

-183969,380865,1,2,END,END,1,1,1,1,
-73039,380865,0,3,7,END,1,1,1,1,
-182101,338388,0,3,4,5,1,1,1,1

-----------------------
|     file names      |
-----------------------

The Arduino SD Card library uses a FAT file system. FAT file systems have a limitation when it comes to 
naming conventions. You must use the 8.3 format, so that file names look like “NAME001.EXT”, where 
“NAME001” is an 8 character or fewer string, and “EXT” is a 3 character extension. People commonly use 
the extensions .TXT and .LOG. It is possible to have a shorter file name (for example, mydata.txt, or 
time.log), but you cannot use longer file names. 

/*---------------------------------------------------------------------------------------


Initial position. Specifies the starting location and orientation. Velocity is zero. 
If this is a file, it is read by C4 (Path Planner) and passed to C6 (Navigator). 
If it is user input, it is read by C6 (Navigator).

The present C4 module is the only micro-controller that has a file system.

USB: All Arduinos contain a USB port that lets them download code from a PC.
Since a USB connection is not a network, the Arduinos cannot talk to each other over USB.
To enable USB, one of the Arduinos must include a USB server. If there is a USB server, it might be
C4, which may have an OS, or
C6, which needs to talk to lots of instruments, or
C7, which may have a USB link to a camera, or
we could have another processor whose sole function is communication.


 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header
 ** Note: SD card functions tested with a stand-alone SD card shield on the Mega 2560. Some
    adjustments may need to be made if using a different configuration.*/

/*---------------------------------------------------------------------------------------*/ 

#define PI ((float) 3.1415925)
#ifndef NULL
#define NULL 0 
#endif
#define START  -1
#define EMPTY  -2

void DataReady();
extern bool DataAvailable;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
//const int chipSelect = 4;

/*---------------------------------------------------------------------------------------*/ 
// EDIT for route
// CONES includes start and stop
#define CONES 5
long goal_lat[CONES] = {  47621881,   47621825,   47623144,   47620616,   47621881};
long goal_lon[CONES] = {-122349894, -122352120, -122351987, -122351087, -122349894};
/*  mph   mm/s
     3    1341
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
#define DESIRED_SPEED_mmPs 2235
/*---------------------------------------------------------------------------------------*/ 



#define MAX_MAPS 10         // The maximum number of map files stored to SD card.
#define MAX_WAYPOINTS 64    // The maximum number of waypoints in each map file.
/*   There are two coordinate systems.
     MDF and RNDF use latitude and longitude.
     C3 and C6 assume that the earth is flat at the scale that they deal with
*/

/*  Nodes and Links define the road network.
    The path for the robot is an array of pointers to Nodes.
    LATITUDE_ORIGIN and LONGITUDE_ORIGIN define  (0,0) on the mm coordinate system. 
    Start is the position of the robot as it begins a new leg of the journey.  
    Destination is the next goal position in mission.
    Path joins Start and Destination. Finding it is the major task 
    of this Path Planner module.
    The Distance numbers are initially multipliers giving path roughness.
    They are replaced by their product with the actual distance.
    Route is a finer scale list of waypoints from present or recent position.
    Exit is the route from Path[last]-> location to Destination.
*/
int map_points = 64;
// struct curve Links[20];
struct junction Nodes[MAX_WAYPOINTS];
struct AStar
{
    int ParentID;
    long CostFromStart;
    long CostToGoal;
    long TotalCost;
} Open[MAX_WAYPOINTS];

class waypoint Origin, Start;
waypoint Path[MAX_WAYPOINTS];  // course route to goal
//waypoint FinePath[MAX_WAYPOINTS];  // a low level part of path that smoothes the corners.
  
waypoint mission[CONES];  // aka MDF
int waypoints = CONES;

/*---------------------------------------------------------------------------------------*/ 
// Fill in the rest of Nodes
void ConstructNetwork(junction *Map, int MapPoints)
{
  double deltaX, deltaY;
  int jj;
  for (int i = 0; i < MapPoints; i++)
  {
      if( Map[i].east_mm == INVALID)  continue;
      for (int j = 0;  j< 4; j++)
      {
        jj = Map[i].destination[j];
        if (jj == END) continue;
        deltaX = Map[i].east_mm;
        deltaX -= Map[jj].east_mm;
        deltaY = Map[i].north_mm;
        deltaY -= Map[jj].north_mm;
        // Distance[] is initially a roughness scale.
        Map[i].Distance[j] *= sqrt(deltaX*deltaX + deltaY*deltaY);
      }
  }
}
/*---------------------------------------------------------------------------------------*/ 
// Set up mission structure from cone latitude and longitude list.
void GetGoals(waypoint *Waypoint, int Goals)
{
  double deltaX, deltaY, Distance;
  for (int i = 0; i < CONES; i++)
  {
    mission[i].latitude  = goal_lat[i];
    mission[i].longitude = goal_lon[i];
    mission[i].Compute_mm();
 /*   Serial.print(mission[i].latitude);   Serial.print(",");
    Serial.print(mission[i].longitude);   Serial.print(",");
    Serial.print(mission[i].east_mm);   Serial.print(",");
    Serial.println(mission[i].north_mm);   */
    mission[i].speed_mmPs = DESIRED_SPEED_mmPs;
    mission[i].index = 1 | GOAL;
    mission[i].sigma_mm = 1000;
    mission[i].time_ms = 0;
    if (i == 0)
    {    // only needed if CONES == 1
      mission[i].Evector_x1000 = 1000;
      mission[i].Nvector_x1000 = 0;
    }
    else
    {
      deltaX = mission[i].east_mm - mission[i-1].east_mm;
      deltaY = mission[i].north_mm - mission[i-1].north_mm;
      Distance = sqrt(deltaX*deltaX + deltaY*deltaY);
      mission[i-1].Evector_x1000 = (deltaX * 1000.) / Distance;
      mission[i-1].Nvector_x1000 = (deltaY * 1000.) / Distance;
    }
    if (i == CONES-1)
    {
      mission[i].Evector_x1000 = mission[i-1].Evector_x1000;
      mission[i].Nvector_x1000 = mission[i-1].Nvector_x1000;
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
long distance(int i, int *k,  // index into Nodes[]
     long east_mm, long north_mm,
    int* perCent)  // per cent of segment that has been completed
{
  float deltaX, deltaY, dist_mm;
  int j, jj;
  long Eunit_x1000, Nunit_x1000;
  long closest_mm = MAX_DISTANCE; 
  long Road_distance, RoadDX_mm, RoadDY_mm;
//  int pathCompletion;
  long pc;
  
  *perCent = 0;
  *k = 0;
  closest_mm = MAX_DISTANCE;
  for (j=0; j < 4; j++)
  {   // Don't make computations twice.
      jj = Nodes[i].destination[j];
      if (jj == END || jj < i) continue;    
      // compute road unit vectors from i to j
      RoadDX_mm = -Nodes[i].east_mm + Nodes[jj].east_mm;
      int Eunit_x1000 = RoadDX_mm  * 1000 / Nodes[i].Distance[j];
      RoadDY_mm = -Nodes[i].north_mm + Nodes[jj].north_mm;
      int Nunit_x1000 = RoadDY_mm * 1000 / Nodes[i].Distance[j];
//      if (abs(RoadDX_mm) >= abs(RoadDY_mm))
//      {
//          pathCompletion = 100 * (east_mm - Nodes[i].east_mm) / RoadDX_mm;
//      }
//      else
//      {
//          pathCompletion = 100 * (north_mm - Nodes[i].north_mm) / RoadDY_mm;
//      }
      // normal vector is (Nunit, -Eunit)
      deltaX = -Nodes[i].east_mm + east_mm ;
      deltaY = -Nodes[i].north_mm + north_mm;
      // sign of return value gives which side of road it is on.
      Road_distance = (-deltaY * Eunit_x1000 + deltaX * Nunit_x1000) / 1000;
      pc =             (deltaX * Eunit_x1000 + deltaY * Nunit_x1000) / (Nodes[i].Distance[j] * 10);
  /*    
      Serial.print("(");   Serial.print(east_mm);  Serial.print(", ");
      Serial.print(north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(Nodes[i].east_mm);  Serial.print(", ");
      Serial.print(Nodes[i].north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(Nodes[jj].east_mm);  Serial.print(", ");
      Serial.print(Nodes[jj].north_mm);  Serial.print(") ");
      Serial.print(Nodes[i].Distance[j]); Serial.print("(");   Serial.print(Eunit_x1000);  Serial.print(", ");
      Serial.print(Nunit_x1000);  Serial.print(") ");
      Serial.print("(");   Serial.print(Road_distance);  Serial.print(", ");
//      Serial.print(pathCompletion);   Serial.print(", "); 
      Serial.print(pc); Serial.println(") ");
 */
      if (abs(Road_distance) < abs(closest_mm) &&
          pc >= 0 && pc <= 100)
      {
          closest_mm = Road_distance;
          *k = jj;
          *perCent = pc;
      }
  }
  return closest_mm;
}
/*---------------------------------------------------------------------------------------*/

void FindClosestRoad(waypoint *start, 
                    waypoint *road)  // information returned
{
  long closest_mm = MAX_DISTANCE;
  long dist;
  int close_index;
  int perCent;
  long done = 2000;
  int i, j;
  // find closest road.
  for (i = 0; i < map_points; i++)
  {
    dist = distance(i, &j, start->east_mm, start->north_mm, &perCent);
    if (abs(dist) < abs(closest_mm))
    {
      close_index = j;
      closest_mm = dist;
      done = perCent;
      road->index = i;
      road->sigma_mm = j;
    }
  }
  if (closest_mm < MAX_DISTANCE)
  {
//    Serial.println(done);
    i = road->index;
    j = close_index;
    road->east_mm =  Nodes[i].east_mm  + 
      done *(Nodes[j].east_mm  - Nodes[i].east_mm) / 100;
    road->north_mm = Nodes[i].north_mm + 
      done *(Nodes[j].north_mm - Nodes[i].north_mm) / 100;
 /*   Serial.print("(");   Serial.print(Nodes[i].east_mm);  Serial.print(", ");
      Serial.print(Nodes[i].north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(Nodes[j].east_mm);  Serial.print(", ");
      Serial.print(Nodes[j].north_mm);  Serial.print(") ");
      Serial.print("(");   Serial.print(road->east_mm);  Serial.print(", ");
      Serial.print(road->north_mm);  Serial.print(") "); Serial.println(j); */
 }
  else
  { // find closest node
     for (i = 0; i < map_points; i++)
    {
      dist = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
      if (dist < closest_mm)
      {
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

int BuildPath (int j, waypoint* start, waypoint* destination)
{
   // Done; we have reached the node that leads to goal turn-off
   // Construct path backward to start.
  int last = 1;
  int route[map_points];
  int i, k, node;
  long dist_mm;

   k = map_points-1;
   route[k] = j;
//   Serial.println(k);
   while(Open[j].ParentID != START)
   {
      j = route[--k] = Open[j].ParentID;
 //     Serial.println(k);
   }
   Path[last] = start;
   for ( ; k < map_points; k++)
   {
     node = route[k];
     Path[++last].east_mm = Nodes[node].east_mm;
     Path[last].north_mm  = Nodes[node].north_mm;
 //    Serial.print(last); Serial.print(", "); 
 //    Serial.print(Path[last].east_mm); Serial.print(", ");
 //    Serial.println(Path[last].north_mm); 
  }
  Path[++last] = destination;
  for (k = 0; k <= last; k++)
  {
     if(k > 0) Path[k].sigma_mm = 10;  // map should be good to a cm.
     Path[k].index = k;
     Path[k].speed_mmPs = DESIRED_SPEED_mmPs;
     Path[k].Compute_LatLon();  // this is never used
  }
  last++;
  for (j = 0; j < last-1; j++)
  {
      Path[j].vectors(&Path[j+1]);
  }

  return last;
}  // end of BuildPath
/*---------------------------------------------------------------------------------------*/

int FindPath(waypoint *start, waypoint *destination)
{

  long ClosedCost[map_points];
//  int OpenIndex = 0;
//  int ClosedIndex = 1;
  int  i, j, k;
  long NewCost, NewStartCost, NewCostToGoal;
  long NewIndex;
  long BestCost, BestID;
  bool Processed = false;
  
  for (i = 0; i < map_points; i++)
  { // mark all nodes as empty
    Open[i].TotalCost = MAX_DISTANCE;
    ClosedCost[i] = MAX_DISTANCE;
  }

   // get successor nodes of start
   i = start->index;
   Open[i].CostFromStart = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].CostToGoal = destination->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;
   Open[i].ParentID = START;
//   SendPath(start, 1);
//   SendPath(destination, 1);
//   Serial.println(i);
   i = start->sigma_mm;
   Open[i].CostFromStart = start->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].CostToGoal = destination->distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
   Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;
   Open[i].ParentID = START;  
//   Serial.println(i);
//   Serial.println("Start A*");
//  Serial.flush();
  BestCost = 0;
  while (BestCost < MAX_DISTANCE)
  { 
     BestCost = MAX_DISTANCE;
     BestID = -1;
//     Serial.println(BestCost);
     // pop lowest cost node from Open; i.e. find index of lowest cost item
     for (i = 0; i < map_points; i++)
     {
//       Serial.print(i); Serial.print(", "); Serial.print(Open[i].TotalCost); 
//       Serial.print(", "); Serial.println(ClosedCost[i]);
        if (Open[i].TotalCost < BestCost)
        {
          BestID = i;
          BestCost = Open[i].TotalCost;
//     Serial.print("Best: "); Serial.print(i); Serial.print(", "); Serial.println(BestCost);
       }
     }
     if (BestID < 0)
     {
//       Serial.println("No path found.");
//       Serial.flush();
       return INVALID;
     }
     Open[BestID].TotalCost = MAX_DISTANCE;  // Remove node from "stack".    
     if (BestID == destination->index || BestID == destination->sigma_mm)
        {  // Done; we have reached the node that leads to goal turn-off
           // Construct path backward to start.
//          Serial.println("Found a path.");
//          Serial.flush();
          return BuildPath(BestID, start, destination);
        }
    // get successor nodes from map
    i = BestID;
//    Serial.print(i); Serial.print(", "); Serial.println(BestCost);
    for (j = 0; j < 4; j++)
    {
        NewIndex = Nodes[i].destination[j];  
        if (NewIndex == END)
            continue;  // no successor in this slot
        NewStartCost =  Open[i].CostFromStart + Nodes[i].Distance[j]; 
        NewCostToGoal = destination->distance_mm
         (Nodes[NewIndex].east_mm, Nodes[NewIndex].north_mm);
        NewCost = NewStartCost + NewCostToGoal;
        // check if this node is already on Open or Closed.
        if (NewCost >= ClosedCost[NewIndex]) 
            continue;  // Have already looked at this node
        else if (ClosedCost[NewIndex] != MAX_DISTANCE)
        {  // looked at this node before, but at a higher cost
            ClosedCost[NewIndex] = MAX_DISTANCE;  // remove node from Closed
        }
        if (NewCost >= Open[NewIndex].TotalCost)
            continue;   // This node is a less efficient way of getting to a node on the list.
        // Push successor node onto stack.
        Open[NewIndex].CostFromStart = NewStartCost;
        Open[NewIndex].CostToGoal = NewCostToGoal;
        Open[NewIndex].TotalCost = NewCost;
        Open[NewIndex].ParentID = i;
    }  // end of successor nodes
    // Push node onto Closed
    ClosedCost[BestID] =  BestCost; 
  }   // end of while Open is not empty
  
  return 0;  // failure
}
/*---------------------------------------------------------------------------------------*/
// Low level path is a straight line from start to detination.
// PathPlan makes an intermediate level path that uses as many roads as possible.
int PlanPath (waypoint *start, waypoint *destination)
 {
   waypoint roadOrigin, roadDestination;
   int last = 0;
   Path[0] = start;
   Path[0].index = 0;

//   SendPath( start, 1);
//   SendPath( destination, 1);

   FindClosestRoad( start, &roadOrigin );
   FindClosestRoad( destination, &roadDestination ); 
//   SendPath( start, 1);
//   SendPath( destination, 1);
//   SendPath( &roadOrigin, 1);
//   SendPath( &roadDestination, 1);
   if  (abs(start->east_mm  - roadOrigin.east_mm)
      + abs(start->north_mm - roadOrigin.north_mm)
      + abs(destination->east_mm  - roadDestination.east_mm)
      + abs(destination->north_mm - roadDestination.north_mm) >=
        abs(start->east_mm  - destination->east_mm)
      + abs(start->north_mm - destination->north_mm))
      {  // don't use roads; go direct
        last = 1;
//        Serial.println("Direct route");
      }
      else
      {  // use A* with the road network
        Path[1] = roadOrigin;
        Path[1].index = 1;
//        Serial.println("Use A*");
//        SendPath(Path, 2);
        last = FindPath(&roadOrigin, &roadDestination);
//        Serial.print("A* finds last =  "); Serial.println(last);
      }
    Path[last] = destination;
    Path[last-1].vectors(&Path[last]);
    Path[last].Evector_x1000 = Path[last-1].Evector_x1000;
    Path[last].Nvector_x1000 = Path[last-1].Nvector_x1000;
    Path[last].index = last | END;
    return last;
 }
/*---------------------------------------------------------------------------------------*/
// Transmit the path to C3 Pilot over a serial line.
void SendPath(waypoint *course, int count)
{
  char *dataString;
  for( int i = 0; i < count; i++)
  {
    dataString = course[i].formPointString();
    checksum(dataString);
    Serial.println(dataString);
    if (course[i].index & END)
        break;
  }
}
/*---------------------------------------------------------------------------------------*/
// LoadMap
// Loads the map nodes from a file.
// Takes in the name of the file to load and loads the appropriate map. 
// Returns true if the map was loaded.
// Returns false if the load failed.
boolean LoadMap(char* fileName)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open(fileName, FILE_READ);
  
  // if the file opened okay, read from it:
  if (myFile) {
    // Initialize a string buffer to read lines from the file into
    // Allocate an extra char at the end to add null terminator
    char* buffer = (char*)malloc(myFile.size()+1);  

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
    char delimiter[2] = ",";
    char* token;
    int col = 0;
    int row = 0;
    
    // get the first token 
    token = strtok(buffer, delimiter);
    
    // walk through other tokens
    while( token != NULL && row < MAX_WAYPOINTS ) 
    {
      switch (col % 10)
      {
        case 0:  // latitude
        Nodes[row].east_mm = atol(token);
        col++;
        break;
        
        case 1:  // longitude
        Nodes[row].north_mm = atol(token);
        col++;
        break;
        
        case 2:  // filename
        if (token == "END")
        {
          Nodes[row].destination[0] = END;
        }
        else
        {
          Nodes[row].destination[0] = atoi(token);
        }
        col++;
        break;
        
        case 3:  // filename
        if (token == "END")
        {
          Nodes[row].destination[1] = END;
        }
        else
        {
          Nodes[row].destination[1] = atoi(token);
        }
        col++;
        break;

        case 4:  // filename
        if (token == "END")
        {
          Nodes[row].destination[2] = END;
        }
        else
        {
          Nodes[row].destination[2] = atoi(token);
        }
        col++;
        break;

        case 5:  // filename
        if (token == "END")
        {
          Nodes[row].destination[3] = END;
        }
        else
        {
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
        col++;
        row++;
        break;

        default:  // unexpected condition; print error
        Serial.println("Unexpected error happened while reading map description file. Please verify the file is in the correct format. Planner may not work correctly if this message appears.");
        break;
      }
      token = strtok(NULL, delimiter);
    }
    map_points = row;
    
    // If file loaded, read data into Nodes[]
    free(buffer);
    myFile.close();
  } else {
    
    // if the file didn't open, print an error:
    myFile.close();
    Serial.print("error opening ");
    Serial.print(fileName);
    return false;
  }  
  return true;
}

/*---------------------------------------------------------------------------------------*/
// SelectMap
// Determines which map to load.
// Takes in the current location as a waypoint, a string with the name of the file that
//   contains the origins and file names of the maps, and a string reference. 
// Determines which origin is closest to the waypoint updates the string reference to the
//   nearest waypoint's file name.
// Assumes the file is in the correct format according to the description in the 
//   documentation.
void SelectMap(waypoint currentLocation, char* fileName, char* nearestMap)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open(fileName, FILE_READ);
  
  // if the file opened okay, read from it:
  if (myFile) {
    
    // Initialize a string buffer to read lines from the file into
    // Allocate an extra char at the end to add null terminator
    char* buffer = (char*)malloc(myFile.size()+1);  

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

    for(int i = 0; i < MAX_MAPS; i++)
    {
      // initialize using invalid values so that we can ensure valid data in allocated memory
      map_latitudes[i] = 91;  
      map_longitudes[i] = 181;
      map_file_names[i] = "";
    }
    
    // Set up tokenizer for the file buffer string
    char delimiter[2] = ",";
    char* token;
    int col = 0;
    int row = 0;
    
    // get the first token 
    token = strtok(buffer, delimiter);
    
    // walk through other tokens
    while( token != NULL && row < MAX_MAPS ) 
    {
      switch (col % 3)
      {
        case 0:  // latitude
        map_latitudes[row] = atof(token);
        col++;
        break;
        
        case 1:  // longitude
        map_longitudes[row] = atof(token);
        col++;
        break;
        
        case 2:  // filename
        map_file_names[row] = token;
        col++;
        row++;
        break;
        
        default:  // unexpected condition; print error
        Serial.println("Unexpected error happened while reading map description file. Please verify the file is in the correct format. Planner may not work correctly if this message appears.");
        break;
      }
      token = strtok(NULL, delimiter);
    }
/*    Serial.println("Latitudes: ");
    for (int i = 0; i < MAX_MAPS; i++)
    {
      if (map_latitudes[i] >= -90 && map_latitudes[i] <= 90)
      {
        Serial.println(map_latitudes[i],8);
      }
    }

    Serial.println("Longitudes: ");
    for (int i = 0; i < MAX_MAPS; i++)
    {
      if (map_longitudes[i] >= -180 && map_longitudes[i] <= 180)
      {
        Serial.println(map_longitudes[i],8);
      }
    }
    
    Serial.println("File names: ");
    for (int i = 0; i < MAX_MAPS; i++)
    {
      if (map_file_names[i] != "")
      {
        Serial.println(map_file_names[i]);
      }
    }*/
    int closestIndex = -1;
    long closestDistance = MAX_DISTANCE;
    for (int i = 0; i < MAX_MAPS; i++)
    {
      int dist = sqrt((map_latitudes[i] - currentLocation.latitude)*(map_latitudes[i] - currentLocation.latitude) + (map_longitudes[i] - currentLocation.longitude)*(map_longitudes[i] - currentLocation.longitude));
      if (dist < closestDistance) 
      {
        closestIndex = i;
        closestDistance = dist;
      }
    }
    if (closestIndex >= 0)
    {
      Origin.latitude = map_latitudes[closestIndex];
      Origin.longitude = map_longitudes[closestIndex];
      for (int i = 0; i < 13; i++)
      {
        nearestMap[i] = map_file_names[closestIndex][i];
      }
      Serial.print("Map ");
      Serial.print(nearestMap);
      Serial.println(" found.");
    }
    else
    {
      Serial.println("error determining closest map.");
    }
    // Free the memory allocated for the buffer    
    free(buffer);
    
    // Determine closest map to current location
    // Update Origin global variable
    
    free(map_latitudes);
    free(map_longitudes);
    free(map_file_names);

    // close the file:
    myFile.close();
    Serial.println("Map definitions loaded.");
  } else {
    
    // if the file didn't open, print an error:
    myFile.close();
    Serial.println("error opening map_defs.txt");
  }
  
  // Return the file name of the closest origin 
}

/*---------------------------------------------------------------------------------------*/ 
void initialize()
{
  int last; 
  Origin.Evector_x1000 = INVALID; 
  Origin.Nvector_x1000 = INVALID; 
  Origin.east_mm = 0;
  Origin.north_mm = 0;  
  Origin.latitude = INVALID;
  Origin.longitude = INVALID; 
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(SS, OUTPUT);
   
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done.");
  char nearestMap[13] = "";
  SelectMap(Start,"map_defs.txt",nearestMap);
  Serial.print("nearestMap: ");
  Serial.println(nearestMap);
  LoadMap(nearestMap);
  
     ConstructNetwork(Nodes, map_points);
     
  /* If (initial position is provided)  // used when GPS is unavailable or inaccurate
     {
        ReadPosition();  // latitude and longitude
        // The origin in the (east_mm, north_mm) coordinate system is defined to be the
        // latitude and longitude at which the robot starts. 
        // (Evector, Nvector) is initial robot attitude.
        SendInitialPosition(C6);  // latitude and longitude
     }
     */
     
  /* Convert latitude and longitude positions to flat earth coordinates.
     Fill in waypoint structure  */
       GetGoals(mission, CONES);
       
       // Send mission to C3.
       SendPath(mission, CONES);
      Serial.println();     
     /* Plan a Path (using map) between each Node. 
     Use the A* algorithm as given in
     Bryan Stout "The Basics of A* for Path Planning" in
     Mark DeLoura (ed) Game Programming Gems, Charles River Media, 2000. */
     
      last = PlanPath (&mission[0], &mission[1]);
      Path[last].index |= GOAL;
     
     /*
     PlanPath does not look at obstacles; The Pilot (C3) does that.  As the vehicle 
     position diverges from expectations due to obstacles, C4 keeps updating the plan.
     The output from PlanPath is a sequence of waypoints Path[] where each waypoint
     is associated with a junction.  We must then supplement the junction
     points with all the intermediate non-junction points in the RNDF so that
     the vehicle can follow curves in the road. The first section of this is Route.
     
     We now have a sequence of segments that defines the mission from
     the origin to the destination.
     
     We will need finer grain that the Route waypoints from the map. */
     
     SendPath(Path, map_points);
      Serial.println();     
    
     /* Read vehicle position, attitude and velocity.
     ReceiveState(C6); 
     fill in Origin.latitude, Origin.longitude, Origin.bearing 
     set Start to Origin; 
     
     GetPosition supplements each pair of waypoints with an Hermite curve that 
     links them.
        Foley et al., Introduction to Computer Graphics.
           */
       
     /* Construct the speed profiles over the Route.
        Consider how much time we want to come up to speed, time to decelerate
        and reduced speed around turns. 
        ConstructSpeeds();  */
        
     /* Send the Route and initial speed profile to C6.
        C6 will add current vehicle position, attitude, velocity and acceleration 
        to create the vehicle state.  The state information is sent on to C3 and 
        fed back to C4.
        SendState(C6); */
}
/*---------------------------------------------------------------------------------------*/ 
void setup() 
{ 
        pinMode(Rx0, INPUT);
        pinMode(Tx0, OUTPUT);
        pinMode(LED, OUTPUT); 
     	Serial.begin(9600); 
        Serial.flush();
        Serial.println();
        Serial.println();
        pinMode(DATA_READY, INPUT);
        DataAvailable = false;
        attachInterrupt(0, DataReady, FALLING);
        
        long startTime = 0;
        long endTime = 0;
        long averageTime = 0;
        for (int i = 0; i < 10; i++)
        {
          startTime = millis();
          initialize();
          endTime = millis();
          averageTime += endTime - startTime;
        }
        averageTime /= 10;
        Serial.print("Average time with ");
        Serial.print(map_points);
        Serial.print(" nodes: ");
        Serial.println(averageTime);
       

}
/*---------------------------------------------------------------------------------------*/ 
void loop() 
{
/*  static int Goal = 1;
  int last;
  /*
    Maintain a list of waypoints from current position to destination.
    Eliminate any waypoints that have already been visited.
    
    ReceiveState(C6);  // This may be at the interrupt level
    PlanPath();  // only from current position to next waypoint
    UpdateRoute();
    ConstructSpeeds();
    SendState(C6);
 
    About once a second, write the current position and remaining mission, so that if we
    get a reset, we can start from where we left off.   
    */
 //   Serial.println("Loop");
/*    if (DataAvailable)
    {
        // read vehicle position from C6
        readline(0);
        // send data to C3
        writeline(0);
        digitalWrite(C4_DATA_SENT, HIGH);  // transition interrupts the processor
        delay(1);
        digitalWrite(C4_DATA_SENT, LOW);
        Start.readPointString(1000, 0);  
        last = PlanPath (&Start, &mission[Goal]);
        Path[last].index |= GOAL;
        // TO DO: Make a fine path, providing proper curve path
        if (last < MAX_WAYPOINTS/2 && Goal < CONES)
        {
          last = PlanPath (&Start, &mission[++Goal]);
          Path[last].index |= GOAL;
        }
        SendPath(Path, MAX_WAYPOINTS);
        Serial.println();     
    }
    else
    {  // delay for 1 sec, waiting for data
        long end_time = millis() + 1000;
        while (millis() < end_time)
        {
            if (DataAvailable) break;
        }
    }*/
}


