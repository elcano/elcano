#include "Pilot.h"


namespace elcano {
/******************************************************************************************************
 * constructor
 *****************************************************************************************************/
Pilot::Pilot(Origin &org, Waypoint &estimated_pos, Waypoint &old_position) {
  
  if(DEBUG) Serial.println("Starting Planning");
  myPlanner = new Planner(org, estimated_pos);
  
  //Trike state starts Straight
  if(DEBUG) Serial.println("Entered Pilot setup");
  state = STARTING;
  //the path is set to approach the first intersection at index 1
  next = 1;
  speed_mmPs = DESIRED_SPEED_mmPs;
  turn_direction = 0;
  
  //CAN FRAME initialization
  output.length = MAX_CAN_FRAME_DATA_LEN_16;
  output.id = HiDrive_CANID; //Drive instructions from hilevel board
  
  for(int i = 0; i < myPlanner->last_index_of_path; i++){
    if(DEBUG3) Serial.println("Path [" + String(i) + "] East is " + String(myPlanner->path[i].east_mm));
  }
}
/******************************************************************************************************
 * tuning_radius_mm(long) 
 *  This method computes the turning radius for the trike
 *  Not using the speed at the moment. Needs improvment for futher development
 *  by taking in the speed
 *  param :
 *  return :
 *****************************************************************************************************/
long Pilot::turning_radius_mm(long speed_mmPs) {
  return  MIN_TURNING_RADIUS_MM; 
}

/******************************************************************************************************
 * test_past_destination(int)  
 *   This method checks to see if we have passed the destination and missed the turn.
 *   Also takes in consider of the type of path shape such as rectagle, skinny, square from current location
 *   to the intersection when determing if we passed the destination.
 *   param : n = next -> index of the intersection you're approaching
 *  return true = past the destination , false = Have not past the destination
 *****************************************************************************************************/
bool Pilot::test_past_destination(int n, Waypoint &estPos) {
  if (abs(myPlanner->path[n - 1].east_mm - myPlanner->path[n].east_mm) > abs(myPlanner->path[n - 1].north_mm - myPlanner->path[n].north_mm)) {
    if (myPlanner->path[n].east_mm > myPlanner->path[n - 1].east_mm && estPos.east_mm > myPlanner->path[n].east_mm) {
      return true;
    } else if (myPlanner->path[n - 1].east_mm > myPlanner->path[n].east_mm && estPos.east_mm < myPlanner->path[n].east_mm) {
      return true;
    }
  } else {
    if (myPlanner->path[n].north_mm > myPlanner->path[n - 1].north_mm && estPos.north_mm > myPlanner->path[n].north_mm) {
      return true;
    } else if (myPlanner->path[n - 1].north_mm > myPlanner->path[n].north_mm && estPos.north_mm < myPlanner->path[n].north_mm) {
      return true;
    }
  }
  return false;
}

/******************************************************************************************************
 * test_approach_intersection(long, int)  
 *  This method test for intersection from straight to entering turn
 *  param : turning radius of the vehicle
 *  param : n = next -> index of the intersection you're approaching
 *  return : true -> if approached intersection to entering turn, false -> otherwise
 *****************************************************************************************************/
bool Pilot::test_approach_intersection(long turn_radius_mm, int n, Waypoint &estPos) {
  //if the distance from E to W is greater than  N to S
  if (abs(myPlanner->path[n - 1].east_mm - myPlanner->path[n].east_mm) > abs(myPlanner->path[n - 1].north_mm - myPlanner->path[n].north_mm)) {
    //if destination is further east than current location AND current location is further East than destination - turn radius
    if (myPlanner->path[n].east_mm > myPlanner->path[n - 1].east_mm && estPos.east_mm >= myPlanner->path[n].east_mm - turn_radius_mm) {
      return true;
    }  //if dest is further west than current AND current location is further west than destination + turn radius 
    else if (myPlanner->path[n - 1].east_mm > myPlanner->path[n].east_mm &&
               estPos.east_mm <= myPlanner->path[n].east_mm + turn_radius_mm) {
        return true;
    }
  } else { //Bigger difference between current and destination N/S than E/W
     //if destination is North AND Destination - turn radius is less mm than current location
    if (myPlanner->path[n].north_mm > myPlanner->path[n - 1].north_mm &&
        estPos.north_mm >= myPlanner->path[n].north_mm - turn_radius_mm) {
      return true;
      // destination is south AND Destination + turn radius is greater than current location
    } else if (myPlanner->path[n - 1].north_mm > myPlanner->path[n].north_mm &&
               estPos.north_mm <= myPlanner->path[n].north_mm + turn_radius_mm) {
      return true;
    }
  }
  return false;
}

/******************************************************************************************************
 * test_leave_intersection(long, int) 
 *  This method test for intersection from leaving turn to straight
 *  param : turning radius of the vehicle
 *  param : n = next -> index of the intersection you're approaching
 *  return : true -> if approached intersection to leave turn, false -> otherwise
 *****************************************************************************************************/
bool Pilot::test_leave_intersection(long turning_radius_mm, int n, Waypoint &estPos) {
    //more change in east
    if (abs(myPlanner->path[n - 1].east_mm - myPlanner->path[n].east_mm) > abs(myPlanner->path[n - 1].north_mm - myPlanner->path[n].north_mm)) {
        if (myPlanner->path[n].east_mm > myPlanner->path[n - 1].east_mm) {
            if(abs((abs(myPlanner->path[n].north_mm) + turning_radius_mm) - (abs(estPos.north_mm) + turning_radius_mm)) >= turning_radius_mm/2){
            return true;
            }
        }
        else if(myPlanner->path[n - 1].east_mm > myPlanner->path[n].east_mm) {
            if(abs((abs(myPlanner->path[n].north_mm) + turning_radius_mm) - (abs(estPos.north_mm) + turning_radius_mm)) >= turning_radius_mm/2) {
            return true;
            }
        }
    }
    else {
        //more change in north
        if(myPlanner->path[n].north_mm > myPlanner->path[n - 1].north_mm){
            if(abs((abs(myPlanner->path[n].east_mm) + turning_radius_mm) - (abs(estPos.east_mm) + turning_radius_mm)) >= turning_radius_mm/2){
                return true;
            }
        }
        else if(myPlanner->path[n - 1].north_mm > myPlanner->path[n].north_mm){
            if(abs((abs(myPlanner->path[n].east_mm) + turning_radius_mm) - (abs(estPos.east_mm) + turning_radius_mm)) >= turning_radius_mm/2) {
                return true;
            }
        }
    }
    return false;
}

/******************************************************************************************************
 * get_turn_direction_angle(int)  
 *  This method determines the direction of turns of the vehicle of either left 
 *  or right once approached a turn intersection. Angle returned is either 90 for right 
 *  or -90 for left.  
 *  param : n = next -> index of the intersection you're approaching
 *  return : positive = right, negative = left, 0 = straight
 *****************************************************************************************************/
int Pilot::get_turn_direction_angle(int n, Waypoint &estPos) {

  if (state == ENTER_TURN) {
    int turn_direction_angle = 0;
    double turn_dir = (estPos.Evector_x1000 * myPlanner->path[n].Evector_x1000) + (estPos.Nvector_x1000 * myPlanner->path[n].Nvector_x1000);
    double x_magnitude = sqrt((estPos.Evector_x1000 * estPos.Evector_x1000) + (estPos.Nvector_x1000 * estPos.Nvector_x1000));
    double y_magnitude = sqrt ((myPlanner->path[n].Evector_x1000 * myPlanner->path[n].Evector_x1000) + (myPlanner->path[n].Nvector_x1000 * myPlanner->path[n].Nvector_x1000));
    double dot_product = (turn_dir / (x_magnitude * y_magnitude));
    dot_product /= 1000000.0;
    turn_dir = acos(dot_product); // angle in radians
    double cross = (estPos.Evector_x1000 * myPlanner->path[n].Nvector_x1000) - (estPos.Nvector_x1000 * myPlanner->path[n].Evector_x1000);
    turn_direction_angle = turn_dir * 180 / PIf; //angle is degrees
    if (cross > 0)
      turn_direction_angle = -turn_direction_angle;
      
    return turn_direction_angle;
  }
  return 0;
}

/******************************************************************************************************
 * proper_heading()
 * Method is called on the STARTING state, it is used to find the proper
 * direction the trike should be aiming via the compass heading
 * it determines the arctangent from current position to the next position in the list
 * once the heading is within 5 degrees of what is calculated it moves on to STRAIGHT state
 * 5 degrees could be altered to make more precise later if needed
 * returns true if the heading is in the required "close enough" range of the waypoint or cone it is heading
 * for, otherwise returns false and keeps attempting to aim correctly at the cone
 * param
 *****************************************************************************************************/
bool Pilot::proper_heading(Waypoint &estimated_pos, int n) {
  long northDiff = abs(estimated_pos.north_mm - myPlanner->path[n].north_mm);
  if(estimated_pos.north_mm > myPlanner->path[n].north_mm) 
    northDiff = northDiff * -1; //heading south
  
  long eastDiff = abs(estimated_pos.east_mm - myPlanner->path[n].east_mm);
  if(estimated_pos.east_mm > myPlanner->path[n].east_mm)
    eastDiff = eastDiff * -1; //heading west
  
  
  double properHeading = atan2(northDiff, eastDiff) * 180.0 / PI;
  if(DEBUG4) Serial.println("Heading needed: " + String(properHeading) + ", Current Heading: " + String(estimated_pos.bearing_deg));

  //TODO fix 360 bug
  if(estimated_pos.bearing_deg >= (properHeading - 5) && estimated_pos.bearing_deg <= (properHeading + 5)) {
    if(DEBUG4) Serial.println("Within 5 Degrees");
    return true; //within 5 degree variance on heading, proceed to STRAIGHT state
  }
 
  //not within 5 degrees of proper heading, need to turn bike
  if(properHeading > estimated_pos.bearing_deg) {
    if(estimated_pos.bearing_deg + 180 >= 360 || estimated_pos.bearing_deg >= properHeading) { //right turn
      //set turn angle right
      turn_direction = 90;
      if(DEBUG4) Serial.println("Turning Right");
      return false;
    }
  }
  if(properHeading < estimated_pos.bearing_deg) {
    if(estimated_pos.bearing_deg + 180 >= 360 && estimated_pos.bearing_deg - 180 > properHeading) { //right turn
      //set turn angle right
      turn_direction = 90;
      if(DEBUG4) Serial.println("Turning Right");
      return false;
    }
  }
  else {
    //set turn angle left
    turn_direction = -90;
    if(DEBUG4) Serial.println("Turning Left");
    return false;
  }
}

/******************************************************************************************************
 * find_state(long, int)  
 *  The method find and set the state of the trike and
 *  also set the speed of the trike depending on the state it's in
 *  param : turning radius_mm
 *  param : n = next -> index of the intersection you're approaching
 *****************************************************************************************************/
void Pilot::find_state(long turn_radius_mm, int n, Waypoint & estimated_pos) {
  switch (state) {
    case STARTING:
      if(proper_heading(estimated_pos, n)) {
        state = STRAIGHT;
      }
      break;
      
    case STRAIGHT:
      if (test_approach_intersection(turn_radius_mm, n, estimated_pos)) {
        //last index of myPlanner->path/goal
        if (n == myPlanner->last_index_of_path) {
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

      if (test_leave_intersection(turn_radius_mm, n, estimated_pos)) {
        state = STRAIGHT;
        next++;

      }
      break;

    case APPROACH_GOAL:
      if (test_past_destination(n, estimated_pos)) {
        if(DEBUG3) Serial.println("Reached Goal!");
        state = STOP;
      }
      break;
  }

}

/******************************************************************************************************
 * hardCoded_Pilot_Test()
 *   Tests the code by cycling through a hard coded speed and angle array to send to lowlevel
 *   this can be used to observe the bike repsonding to different speeds and angles to 
 *   ensure proper CAN communication and response.
 *   Currently contains two tests, A & B. 
 *   Comment out one based on if using delays in your code for debugging
 *****************************************************************************************************/
void Pilot::hardCoded_Pilot_Test() {
  speed_mmPs = speeds[speedIndex];
  turn_direction = angg[speedIndex];

//test section A. to be used if no delays are in place so speed and angle not changed constantly
 /* if(insaneCounter < 6000) {
      insaneCounter++;
  }
  else {
    if(speedIndex == 5)
      speedIndex = 0;
    else
      speedIndex++;
    insaneCounter = 0;  
    Serial.println("Speed: " + String(speeds[speedIndex]));
  }
 // Serial.println("insaneCounter = " + String(insaneCounter) + ", Index = " + String(speedIndex));
 */

 //test section B. to be used if delays are in place so angle will speed/angle will change occasionally
  if(speedIndex == 5)
    speedIndex = 0;
  else
    speedIndex++; 
}

/******************************************************************************************************
 * Pilot_communicate_LowLevel()
 * Transmits the data over CAN BUS from Pilot to Lowlevel
 * Sends a speed in mm/s and a turn angle in degrees
 *****************************************************************************************************/
void Pilot::Pilot_communicate_LowLevel() {
  //if (pre_desired_speed != speed_mmPs || pre_turn_angle != turn_direction) {  send every time, lowlevel only reacts when not close
    
    output.data.low = speed_mmPs;
    output.data.high = turn_direction;
        
    if(DEBUG2)
      Serial.println("**Sending: Speed: " + String(output.data.low) + " Angl: " + (output.data.high));
    
    CAN.sendFrame(output); //send the message
    //delay(1000); //usually only needed if using a Serial Print, comment out when running real test

    //keep track of previous data to compare next loop
    pre_desired_speed = speed_mmPs;
    pre_turn_angle = turn_direction;
}

/*****************************************************************************************************
 * update()
 * Determines state of trike, uses this to get a disered speed and angle
 * and transmits this data to the C2_Lowlevel board
 * param: estimated position & oldPosition Waypoints
 *****************************************************************************************************/
void Pilot::update(Waypoint &estimated_pos, Waypoint &old_pos) {
  if(first) initializePosition(estimated_pos, old_pos);
  //Determining the state of the Trike
  find_state(TURN_RADIUS_MM, next, estimated_pos);
  if(DEBUG3) Serial.println("State is: " + String(wrdState[state]) + ", path # " + String(next));
  //comment out and uncomment lines below when not in testing
  //hardCoded_Pilot_Test();
  
  //Determining the turn direction for the trike "left, right or straight" comment out if using test line above
  turn_direction = get_turn_direction_angle(next, estimated_pos);
  if(DEBUG3) Serial.println("setting turn angle to: " + String(turn_direction));

  Pilot_communicate_LowLevel(); //send data to lowlevel through CAN
}
 
 /********************************************************************************************************
 * populate_path
 *   Hard coded path for testing
 *******************************************************************************************************/
 //void Pilot::populate_path() {

 //   pathz0.east_mm =  6925;
 //   pathz0.north_mm = 5893;

 //   pathz1.east_mm = 11735;
 //   pathz1.north_mm = 5913;

 //   pathz2.east_mm = 17416;
 //   pathz2.north_mm = 18791;
//
 //   pathz[0] = pathz0;
 //   pathz[1] = pathz1;
 //   pathz[2] = pathz2;

 //   for (int i = 0; i < 3; i++) {
 //       pathz[i].vectors(&pathz[i+1]);
 //   }
//}

/********************************************************************************************************
 * intializePosition()
 *   Hard code assigning estimated position to the first element in path
 *******************************************************************************************************/
  void Pilot::initializePosition(Waypoint &estPos, Waypoint &oldPos) {
  estPos.east_mm = myPlanner->path[0].east_mm;
  estPos.north_mm = myPlanner->path[0].north_mm;
  estPos.Evector_x1000 = myPlanner->path[0].Evector_x1000;
  estPos.Nvector_x1000 = myPlanner->path[0].Nvector_x1000;
  oldPos = estPos;
  first = false; //one time through this routine
} 

} // namespace elcano
