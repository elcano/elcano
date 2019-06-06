//Documentation Shifted to Word Document

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
/*  
Elcano Module C4: Path Planner.

Output to C6: Desired route and speed curves and whether it exits a road or changes lane.
Input: RNDF, MDF and initial position files from memory stick.
Input from C5: Speed and position of obstacles.
Input from C6: Position, orientation, velocity and acceleration.

/*-------------------------------------------
 * --------------------------------------------

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

This file provides the latitude and longitude coordinates for eachf of the maps, followed
by the file name for that map. Commas also separate each map.

The format should be as follows:

latitude_0,longitude_0,filename_0.txt,
...,
latitude_n,longitude_n,filename_n.txt


A practical example would look like this:

47.758949,-122.190746,UWB_MAP.txt,
47.6213,-122.3509,SEATLCEN.txt


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

-183969,380865,1,2f,END,END,1,1,1,1,
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





//To test LoadMap:
//    for (int i = 0; i < map_points; i++)
//    {
//      Serial.print(Nodes[i].east_mm);
//      Serial.print(",");
//      Serial.print(Nodes[i].north_mm);
//      Serial.print(",");
//      Serial.print(Nodes[i].destination[0]);
//      Serial.print(",");
//      Serial.print(Nodes[i].destination[1]);
//      Serial.print(",");
//      Serial.print(Nodes[i].destination[2]);
//      Serial.print(",");
//      Serial.print(Nodes[i].destination[3]);
//      Serial.print(",");
//      Serial.print(Nodes[i].Distance[0]);
//      Serial.print(",");
//      Serial.print(Nodes[i].Distance[1]);
//      Serial.print(",");
//      Serial.print(Nodes[i].Distance[2]);
//      Serial.print(",");
//      Serial.println(Nodes[i].Distance[3]);
//    }
////  
// 


   //For Testing SelectMap::
//    for (int i = 0; i < MAX_MAPS; i++)
//    {
//      if (map_latitudes[i] >= -90 && map_latitudes[i] <= 90)
//      {
//        //Serial.println(map_latitudes[i],8);
//        //Serial.print(map_latitudes[i]);
//      }
//    }
//
//    //Serial.println("Longitudes: ");
//    for (int i = 0; i < MAX_MAPS; i++)
//    {
//      if (map_longitudes[i] >= -180 && map_longitudes[i] <= 180)
//      {
////        Serial.println(map_longitudes[i],8);
//      }
//    }
//    
//    ///Serial.println("File names: ");
//    for (int i = 0; i < MAX_MAPS; i++)
//    {
//      if (map_file_names[i] != "")
//      {
//       // Serial.println(map_file_names[i]);
//      }
//    }
//    
