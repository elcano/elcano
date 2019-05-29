#include "Common.h"
#include <IODue.h>
#include <Arduino.h>
#include <SD.h>
#include "Planner.h"


namespace elcano {
Planner::Planner(Origin &org, Waypoint &estimated_pos){//default constructor
    
    map_points = 0; //will be filled in during map loading
    mission_index = 0; //index number of mission/cone currently attempting to reach
    last_index_of_path = CONES -1; //hardcode path of the last index/dest to 3 [cur,loc1,goal]
    //Store the initial GPS latitude and longtitude to select the correct map
    Start.latitude = estimated_pos.latitude; //set to the initial position of the trike 
    Start.longitude = estimated_pos.longitude;
    //if(DEBUG)Serial.println();
    initialize_Planner(org, estimated_pos); //Start selecting/load map and start planning path
    
    Start.east_mm = Nodes[0].east_mm; //should start be firt place you wanna go or where you are right now?
    Start.north_mm = Nodes[0].north_mm;

    if (DEBUG) Serial.println("Start planning path");
    last_index_of_path = PlanPath(org, Start, mission[mission_index]); //start with first cone
  }
	/*---------------------------------------------------------------------------------------*/
// SelectMap:  Return the file name of the closest origin
// Determines which map to load.
// Takes in the current location as a Waypoint and a string with the name of the file that
//   contains the origins and file names of the maps.
// Determines which origin is closest to the Waypoint and returns it as a Junction.
// Assumes the file is in the correct format according to the description above.
void Planner::SelectMap(Origin &orgin, Waypoint &startLocation, char* fileName, char* nearestMap)
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

			//MAX_MAPS is set to 10 in Common.h
			// Set up storage for coordinates and file names
			// Note: we malloc here because the stack is too small
			float* map_latitudes = (float*)malloc(MAX_MAPS * 4);
			float* map_longitudes = (float*)malloc(MAX_MAPS * 4);
			char** map_file_names = (char**)malloc(MAX_MAPS * sizeof(char*));

			for (int i = 0; i < MAX_MAPS; i++) {
				// initialize using invalid values so that we can ensure valid data in allocated memory
				map_latitudes[i] = 91;
				map_longitudes[i] = 181;
				map_file_names[i] = "";
			}
			if (DEBUG) {
				Serial.println("");
				Serial.println("Loaded " + String(fileName) + " now loading and printing maps array");
			}
			// Set up tokenizer for the file buffer string
			char *delimiter = " ,\n";
			char* token;
			int col = 0;
			int row = 0;
			char* t;

			// get the first token
			token = strtok(buffer, delimiter);

			//The first token has junk at the front and won't atof() need to use below work around
			//for first number before switch 
			//need to make into a string, manipulate and turn back to null terminated char array. 
			String temps = String(token);
			temps.trim();
			const int len = temps.length();
			char* tempchar = new char[len + 1];
			int numGood = 0;
			for (int i = 0; i < temps.length(); i++) {
				if (temps[i] == '-') {
					tempchar[numGood] = temps[i];
					numGood++;
				}
				if (temps[i] == '.') {
					tempchar[numGood] = temps[i];
					numGood++;
				}
				if (temps[i] >= '0' && temps[i] <= '9') {
					tempchar[numGood] = temps[i];
					numGood++;
				}
			}
			// temps.toCharArray(tempchar, temps.length()+1);
			 //need to null terminate string or can't convert
			tempchar[numGood + 1] = '\0';
			float temp;
			temp = atof(tempchar);
			map_latitudes[row] = temp;
			if (DEBUG) {
				Serial.println("index: " + String(row));
				Serial.println(map_latitudes[row], 6);
			}
			col++;
			token = strtok(NULL, delimiter);

			// fill the map_latitude, map_longitude, & map_file with tokens
			while (token != NULL) {
				switch (col % 3) {
				case 0:  // latitude //skipped first time as handled above due to error handling
					map_latitudes[row] = atof(token);
					if (DEBUG) {
						Serial.println("index: " + String(row));
						Serial.println(map_latitudes[row], 6);
					}
					col++;
					break;

				case 1:  // longitude
					map_longitudes[row] = atof(token);
					col++;
					if (DEBUG) {Serial.println(map_longitudes[row], 6);}
					break;

				case 2:  // filename
					map_file_names[row] = token;
					col++;
					if (DEBUG) {Serial.println(map_file_names[row]);}
					row++;
					break;

				default:  // unexpected condition; print error
					if (DEBUG) {
						Serial.println("Unexpected error happened while reading map description file. Please verify the file is in the correct format. Planner may not work correctly if this message appears.");
					}
					break;
				}
				token = strtok(NULL, delimiter);
			}
			int closestIndex = -1;
			long closestDistance = MAX_DISTANCE;
      if(DEBUG)Serial.print("startLocation = ");
      if(DEBUG)Serial.print(startLocation.latitude, 6);
      if(DEBUG)Serial.print(", ");
      if(DEBUG)Serial.println(startLocation.longitude, 6);
			for (int i = 0; i < MAX_MAPS; i++) {
				int dist = fabs((map_latitudes[i] - startLocation.latitude)) +
					abs(map_longitudes[i] - startLocation.longitude);
				if (dist < closestDistance) {
					closestIndex = i;
					closestDistance = dist;
				}
			}
			if (closestIndex >= 0) {
         if(DEBUG)Serial.println("origin is now set to :" + String(orgin.latitude) + " " + String(orgin.longitude));
				// Determine closest map to current location
				// Update origin global variable
        if(DEBUG)Serial.println("map chosen index = " + String(closestIndex));
        //have to set origin with constructor so orgin.cos_lat will be set
        Origin newOrg(map_latitudes[closestIndex],map_longitudes[closestIndex]);
        orgin = newOrg;
			  //orgin.latitude = map_latitudes[closestIndex];
				//orgin.longitude = map_longitudes[closestIndex];
        if(DEBUG)Serial.println("closest index = " + String(closestIndex));
        if(DEBUG)Serial.print("origin is now set to : ");
        if(DEBUG)Serial.print(orgin.latitude, 6);
        if(DEBUG)Serial.print(", "); 
        if(DEBUG)Serial.println(orgin.longitude, 6);

				//make all file names 12 in length so this works 12 char plus \0
				for (int i = 0; i < 12; i++)
				{
					nearestMap[i] = map_file_names[closestIndex][i];
				}
				//null terminate the string as it is C
				//map names should be 13 char and the 14th will by the \0
				nearestMap[12] = '\0';
        if(DEBUG)Serial.println("nearestmap = " + String(nearestMap));
			}
			else {
				if (DEBUG) {
					Serial.println("error determining closest map.");
					Serial.println("");
				}
			}
			// Free the memory allocated for the buffer
			free(buffer);
			free(map_latitudes);
			free(map_longitudes);
			free(map_file_names);

			// close the file:
			myFile.close();
			if (DEBUG) {
				Serial.println("Map definitions loaded.");
				Serial.println("");
			}
		}
		else {
			// if the file didn't open, print an error:
			myFile.close();
			if (DEBUG) {
				Serial.println("error opening MAP_DEFS.TXT");
				Serial.println("");
			}
		}

	}
	/*---------------------------------------------------------------------------------------*/
	// nearestMap
	// Loads the map nodes from a file.
	// Takes in the name of the file to load and loads the appropriate map.
	// Returns true if the map was loaded.
	// Returns false if the load failed.
	bool Planner::LoadMap(char* fileName) {
		// open the file. note that only one file can be open at a time,
		// so you have to close this one before opening another.

	  //Try opening root then check each file name til find right one then do then close then rewindDirectory so can open new
		File root = SD.open("/");
		bool found = false;
		File useFile = root;
		while (!found) {
			useFile = root.openNextFile();
			if (DEBUG) {Serial.println(String(useFile.name()));}
			if (!String(useFile.name()).equals(String(fileName))) {
				useFile.close();
				if (DEBUG) {Serial.println("closed file: " + String(useFile.name()));}
			}
			else {
				found = true;
				if (DEBUG) {Serial.println("file's match: " + String(useFile.name()));}
			}
		}
		// if the file opened okay, read from it:
		if (useFile) {
			// Initialize a string buffer to read lines from the file into
			// Allocate an extra char at the end to add null terminator
			char* buffer = (char*)malloc(useFile.size() + 1);

			// index for the next character to read into buffer
			char* ptr = buffer;

			// read from the file until there's nothing else in it:
			while (useFile.available()) {
				*ptr = useFile.read();
				++ptr;
			}
			// Null terminate the buffer string
			*ptr = '\0';

			// Set up tokenizer for the file buffer string
			char * delimiter = " ,\n";
			char* token;
			int col = 0;
			int row = 0;

			// get the first token //it is junk so move on
			token = strtok(buffer, delimiter);
			//get the first good token
			token = strtok(NULL, delimiter);
			//Serial.println("The first token is: " + String(token));
			// walk through other tokens
			while (token != NULL) {
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
					else {
						Nodes[row].destination[0] = atoi(token);
					}
					col++;
					break;

				case 3:  // filename
					if (token == "END") {
						Nodes[row].destination[1] = END;
					}
					else {
						Nodes[row].destination[1] = atoi(token);
					}
					col++;
					break;

				case 4:  // filename
					if (token == "END") {
						Nodes[row].destination[2] = END;
					}
					else {
						Nodes[row].destination[2] = atoi(token);
					}
					col++;
					break;

				case 5:  // filename
					if (token == "END") {
						Nodes[row].destination[3] = END;
					}
					else {
						Nodes[row].destination[3] = atoi(token);
					}
					col++;
					break;
					//The distance array is the cost for taking that road. Temp holders is 1 
					//may be used later to help choose path. distance normally 1 if .5 may be half the speed if 2 may be twice the speed
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
					if (DEBUG) {
						Serial.print("Unexpected error happened while reading map description file.");
						Serial.print("Please verify the file is in the correct format.");
						Serial.println("Planner may not work correctly if this message appears.");
					}
					break;
				}

				token = strtok(NULL, delimiter);
			}
			map_points = row;
			if (DEBUG) {
				Serial.println("Test map");
				Serial.println(map_points);
			}
			//To test LoadMap:
			for (int i = 0; i < map_points; i++) {
				if (DEBUG) {
					Serial.println("inside the loop: " + String(i));
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
			}

			// If file loaded, read data into Nodes[]
			free(buffer);
			useFile.close();
			if (DEBUG) {Serial.println("Closed the file: " + String(useFile.name()));}
		}
		else {
			// if the file didn't open, print an error:
			useFile.close();
			if (DEBUG) {Serial.println("error opening: " + String(fileName));}
			return false;
		}
		return true;
	}

	/*---------------------------------------------------------------------------------------*/
	void Planner::initialize_Planner(Origin &orign, Waypoint &estimPos) {
		
		if(DEBUG)Serial.println("Initializing SD card...");
		//chipSelect = 35 located in IODUE.h library if change needed
   if (DEBUG)Serial.println("chipSelect = " + String(chipSelect));
		pinMode(chipSelect, OUTPUT);
		if (!SD.begin(chipSelect)) {
			Serial.println("initialization failed!");
		}
		else {
			Serial.println("initialization done.");
		}
		//13 to include null terminate - SD cannot have more than 12 char & \0 in filename caused error
		char nearestMap[13] = "";

		SelectMap(orign, Start, "MAP_DEFS.TXT", nearestMap); //populates info from map_def to nearestMap;
    //moved this 7 lines from nav initialize since no origin set there yet
    if(DEBUG)Serial.print("after map selection origin is now set to : ");
    if(DEBUG)Serial.print(orign.latitude, 6);
    if(DEBUG)Serial.print(", "); 
    if(DEBUG)Serial.println(orign.longitude, 6);
    if(DEBUG)Serial.print("current estimated_pos is: ");
    if(DEBUG)Serial.print(estimPos.latitude, 6);
    if(DEBUG)Serial.print(", "); 
    if(DEBUG)Serial.println(estimPos.longitude, 6);
    estimPos.Compute_mm(orign);  //initialize north and east coordinates for position
    if(DEBUG) {
      Serial.print("Estimate E: ");
      Serial.println(estimPos.east_mm);
      Serial.print("Estimate N: ");
      Serial.println(estimPos.north_mm);
    }
		Serial.println("Map selected was: " + String(nearestMap));

		//populate nearest map in Junction Nodes structure
		LoadMap(nearestMap);

		//takes in the Nodes that contains all of the map
		ConstructNetwork(Nodes); //To fill out the rest of the nodes info

		GetGoals(orign, Nodes);
	}

	/*---------------------------------------------------------------------------------------*/
	// Fill the distances of the Junctions in the MAP //Uses Nodes array
	void Planner::ConstructNetwork(Junction *Map) {
		double deltaX, deltaY;
		int destination;
		for (int i = 0; i < map_points; i++) {
			if (Map[i].east_mm == INVALID)  continue;
			for (int j = 0; j < 4; j++) {
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
	// Set up mission structure from goal_lat and goal_lat arrays (the goal cones to hit)
	void Planner::GetGoals(Origin &ori, Junction *nodes) {
		double deltaX, deltaY, Distance;
		for (int i = 0; i < CONES; i++) {
			mission[i].latitude = goal_lat[i];
			mission[i].longitude = goal_lon[i];
			mission[i].Compute_mm(ori);
			mission[i].speed_mmPs = DESIRED_SPEED_mmPs; //defined in Settings_HighLevel.h
			mission[i].index = 1 | GOAL;
			mission[i].sigma_mm = 1000;
			mission[i].time_ms = 0;

			if (i == 0) { //If CONE == 1
				mission[i].Evector_x1000 = 1000;  //didn't write this..why is E 100 and N 0
				mission[i].Nvector_x1000 = 0;
			}
			else {
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
	long Planner::distance(int &cur_node, int &k, long &cur_east_mm, long &cur_north_mm, int &perCent) {
		double deltaX, deltaY, dist_mm;
		int cur, destination;
		double Eunit_x1000, Nunit_x1000;
		double closest_mm = MAX_DISTANCE;
		double Road_distance, RoadDX_mm, RoadDY_mm;

		long pc; //per cent of completion from i to j.

		perCent = 0;
		k = 0;
	
		for (cur = 0; cur < 4; cur++) { // Don't make computations twice.
			destination = Nodes[cur_node].destination[cur];
     //if 0 or less already checked or is a dead end so don't check
			if (destination == 0 || destination < cur_node) continue;  //replace Destination with END
			
			// compute road unit vectors from i to cur
			RoadDX_mm = Nodes[destination].east_mm - Nodes[cur_node].east_mm;
			
			if(DEBUG)Serial.println("Destination " + String(destination));
      if(DEBUG)Serial.println("RoadDX_mm " + String(RoadDX_mm));
			if(DEBUG)Serial.println("Nodes[destination].east_mm " + String(Nodes[destination].east_mm));
			if(DEBUG)Serial.println("-Nodes[cur_loc].east_mm " + String(-Nodes[cur_node].east_mm));
			
			int Eunit_x1000 = RoadDX_mm * 1000 / Nodes[cur_node].Distance[cur];
      if(DEBUG)Serial.println("Eunit_x1000: " + String(Eunit_x1000));
      
			RoadDY_mm = Nodes[destination].north_mm - Nodes[cur_node].north_mm;
			if(DEBUG)Serial.println("RoadDY_mm " + String(RoadDY_mm));
			if(DEBUG)Serial.println("Nodes[destination].north_mm " + String(Nodes[destination].north_mm));
			if(DEBUG)Serial.println("-Nodes[cur_loc].north_mm " + String(-Nodes[cur_node].north_mm));
			
			int Nunit_x1000 = RoadDY_mm * 1000 / Nodes[cur_node].Distance[cur];
			if(DEBUG)Serial.println("Nunit_x1000: " + String(Nunit_x1000));
			//      // normal vector is (Nunit, -Eunit)
			//      //Answers: What would be the change in X/Y from my current Node.
			//      deltaX = cur_east_mm - Nodes[cur_node].east_mm;
			//      deltaY = cur_north_mm - Nodes[cur_node].north_mm;
			
			
			//      // sign of return value gives which side of road it is on.
			//      Road_distance = (-deltaY * Eunit_x1000 + deltaX * Nunit_x1000) / 1000;
      if(DEBUG)Serial.println("DX x DX: " + String(RoadDX_mm * RoadDX_mm));
      if(DEBUG)Serial.println("DY x DY: " + String(RoadDY_mm * RoadDY_mm));
      if(DEBUG)Serial.println("Added together: " + String((RoadDX_mm * RoadDX_mm) + (RoadDY_mm * RoadDY_mm)));
			Road_distance = sqrt((RoadDX_mm * RoadDX_mm) + (RoadDY_mm * RoadDY_mm));
			//Why do percentage computation like this?
			pc = (deltaX * Eunit_x1000 + deltaY * Nunit_x1000) / (Nodes[cur_node].Distance[cur] * 10);

		  if(DEBUG)Serial.println("Closest_mm " + String(closest_mm) + "\t Road_distance " + String(Road_distance));
		      
			if (abs(Road_distance) < abs(closest_mm) && pc >= 0 && pc <= 100) {
				closest_mm = Road_distance;
				k = destination;
				perCent = pc;

			}
		}
    if(DEBUG)Serial.println("In distance method returning closest_mm: " + String(closest_mm));
    if(DEBUG)Serial.println(" ");
		return long(closest_mm);
	}
	/*---------------------------------------------------------------------------------------*/
	//Figuring out a path to get the road network
	void Planner::FindClosestRoad(Waypoint &start, Waypoint &road) {  //populate road with best road from start
		long closest_mm = MAX_DISTANCE;
		long dist;
		int close_index;
		int perCent;
		long done = 1;//2000;
		int i, node_successor;

		for (i = 0; i < 5/*map_points*/; i++) { // find closest road.
			dist = distance(i, node_successor, start.east_mm, start.north_mm, perCent); //next node to visit
			//Serial.println("Start : Latitude " + String(start.latitude) + "\t Longitude " + String(start.longitude) + "\t Dist "
			//	+ String(dist));

			if (abs(dist) < abs(closest_mm)) {
				close_index = node_successor;
				closest_mm = dist;
				done = 1;// perCent; //Not really true amount of nodes done?
				road.index = i;
				road.sigma_mm = node_successor;
			}
		}
		if (closest_mm < MAX_DISTANCE) {
			i = road.index; //0
			node_successor = close_index; //0
			road.east_mm = Nodes[i].east_mm + done * (Nodes[node_successor].east_mm - Nodes[i].east_mm) / 100;
			road.north_mm = Nodes[i].north_mm + done * (Nodes[node_successor].north_mm - Nodes[i].north_mm) / 100;
		}
		else {
			for (i = 0; i < 5/*map_points*/; i++) { // find closest node
			  //Serial.println("I got here");
				dist = start.distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);

				if (dist < closest_mm) {
					close_index = i;
					closest_mm = dist;
				}
			}
			road.index = road.sigma_mm = close_index;
			road.east_mm = Nodes[close_index].east_mm;
			road.north_mm = Nodes[close_index].north_mm;
		}

		road.Evector_x1000 = 1000;
		road.Nvector_x1000 = 0;
		road.time_ms = 0;
		road.speed_mmPs = DESIRED_SPEED_mmPs;

		//Test FindClosest Road:
		if(DEBUG)Serial.println("Distance " + String(dist));
		if(DEBUG)Serial.println("Road :  East_mm " + String(road.east_mm) + "\t North_mm " + String(road.north_mm));

	}

	/*---------------------------------------------------------------------------------------*/
// start and destination are on the road network given in Nodes.
// start is in Path[1].
// Place other Junction Waypoints into Path.
// Returned value is next index into Path.
// start->index identifies the closest node.
// sigma_mm holds the index to the other node.
// A* is traditionally done with pushing and popping node from an Open and Closed list.
// Since we have a small number of nodes, we instead reserve a slot on Open and Closed
// for each node.

	int Planner::BuildPath(Origin &orgi, long &j, Waypoint &start, Waypoint &destination) { // Construct path backward to start.
		Serial.println("To break");
		int last = 1; //already did 0 in planPath
		int route[map_points];
		int i, k, node;
		long dist_mm;

		k = map_points - 1;
		route[k] = j;

		while (Open[j].ParentID != currentlocation) {
			j = route[--k] = Open[j].ParentID;
		}

		path[last] = start;
		for (; k < map_points; k++) {
			node = route[k];
			path[++last].east_mm = Nodes[node].east_mm;
			path[last].north_mm = Nodes[node].north_mm;
		}
		path[++last] = destination;
		for (k = 0; k <= last; k++) {
			if (k > 0) path[k].sigma_mm = 10; // map should be good to a cm.
			path[k].index = k;
			path[k].speed_mmPs = DESIRED_SPEED_mmPs;
			path[k].Compute_LatLon(orgi);  // this is never used
		}
		last++;
		for (j = 0; j < last - 1; j++) {
			path[j].vectors(&path[j + 1]);
		}

		return last; //The next index to use in planPath
    
	}

	/*---------------------------------------------------------------------------------------*/
	//Use Astar
	int Planner::FindPath(Origin &borigin, Waypoint &start, Waypoint &destination) { //While OpenSet is not empty

		Serial.println("Start East_mm " + String(start.east_mm) + "\t North " + String(start.north_mm));
		Serial.println("Start East_mm " + String(destination.east_mm) + "\t North " + String(destination.north_mm));
		long ClosedCost[map_points];
		int  i, neighbor, k;
		long NewCost, NewStartCost, NewCostToGoal;
		long NewIndex;
		long BestCost, BestID;
		bool Processed = false;

		for (i = 0; i < map_points; i++) { // mark all nodes as empty
			Open[i].TotalCost = MAX_DISTANCE;
			ClosedCost[i] = MAX_DISTANCE;
		}

		i = start.index; // get successor nodes of start
		Open[i].CostFromStart = start.distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
		Open[i].CostToGoal = destination.distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);

		Open[i].TotalCost = Open[i].CostFromStart + Open[i].CostToGoal;
		Open[i].ParentID = currentlocation;

		i = start.sigma_mm;
		Open[i].CostFromStart = start.distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
		Open[i].CostToGoal = destination.distance_mm(Nodes[i].east_mm, Nodes[i].north_mm);
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
			Serial.println("DestinationINdex " + String(destination.index));
			Open[BestID].TotalCost = MAX_DISTANCE;  // Remove node from "stack".
			if (BestID == destination.index || BestID == destination.sigma_mm) { // Done:: reached the goal!!

				return BuildPath(borigin, BestID, start, destination);   // Construct path backward to start.
			}

			i = BestID;  // get successor nodes from map

			for (neighbor = 0; neighbor < 4; neighbor++) {
				NewIndex = Nodes[i].destination[neighbor];

				if (NewIndex == END)continue; // No success in this slot

				NewStartCost = Open[i].CostFromStart + Nodes[i].Distance[neighbor];
				NewCostToGoal = destination.distance_mm(Nodes[NewIndex].east_mm, Nodes[NewIndex].north_mm);
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


			ClosedCost[BestID] = BestCost; // Push node onto Closed
		}

		Serial.println("Destination East_mm " + String(destination.east_mm) + "\t North " + String(destination.north_mm));

		return 0;  // failure
	}

	/*---------------------------------------------------------------------------------------*/
// Low level path is a straight line from start to detination.
// PathPlan makes an intermediate level path that uses as many roads as possible.
//start = currentlocation: destination = heading to;
//Find the cloeset road and call Findpath to do the A star
	int Planner::PlanPath(Origin &origin, Waypoint &start, Waypoint &destination) {

		//Serial.println("Start : East_mm = " + String(start->east_mm) + "\t North_mm =  " + String(start->north_mm));
		Waypoint roadOrigin, roadDestination;

		int last = 0;
		path[0] = start;
		path[0].index = 0;

		FindClosestRoad(start, roadOrigin);
		FindClosestRoad(destination, roadDestination);
    //if(DEBUG)Serial.println("In plan path - Selected road origin is: " + String(roadorigin.latitude));
    //if(DEBUG)Serial.println("In plan path - Selected road destination is: " + String(roadDestination.latitude));
    //if(DEBUG)Serial.println(" ");
		int w = abs(start.east_mm - roadOrigin.east_mm) + abs(start.north_mm - roadOrigin.north_mm);
		int x = abs(destination.east_mm - roadDestination.east_mm) + abs(destination.north_mm - roadDestination.north_mm);

		int straight_dist = 190 * abs(start.east_mm - destination.east_mm) + abs(start.north_mm - destination.north_mm);
		if (w + x >= straight_dist) { // don't use roads; go direct
			last = 1;
			Serial.println("In Straight");
		}
		else {  // use A* with the road network
			Serial.println("In Else");
			path[1] = roadOrigin;
			path[1].index = 1;
			//why index = 7?
			destination.index = 7;
			last = FindPath(origin, roadOrigin, roadDestination);
		}

		path[last] = destination;
		path[last - 1].vectors(&path[last]);
		path[last].Evector_x1000 = path[last - 1].Evector_x1000;
		path[last].Nvector_x1000 = path[last - 1].Nvector_x1000;
		path[last].index = last | END;

		Serial.println("Destination : East_mm = " + String(destination.east_mm) + "\t North_mm =  " + String(destination.north_mm));
		Serial.println(" ");

		return last;

	}

} // namespace elcano
