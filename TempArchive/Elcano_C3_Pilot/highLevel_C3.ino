#define DESIRED_SPEED_mmPs 2235 //5mph
#define SLOW_SPEED_mmPs 1000 //1-2mph

const long turn_speed = 500;
const long MIN_TURNING_RADIUS = 2400;
long speed_mmPs = DESIRED_SPEED_mmPs;

int next = 1; //index to path in a list
int last_index_of_path; //last is the the last index of the Path/goal

junction Nodes[MAX_WAYPOINTS]; //Storing the loaded map

waypoint path[MAX_WAYPOINTS];  // course route to goal/mission
waypoint mission[CONES]; //The taget node to reach/goal
waypoint GPS_reading, estimated_position, Start;

enum States { STOP, STRAIGHT, ENTER_TURN, LEAVE_TURN, APPROACH_GOAL, LEAVE_GOAL};
States state;

/*---------------------------------------------------------------------------------------*/
/**
   All the Methods for C3 starts here
*/
/*---------------------------------------------------------------------------------------*/
/**
   This method computes the turning radius for the trike
   by taking in the speed
*/
long turning_radius_mm(long speed_mmPs) {
  return 2 * MIN_TURNING_RADIUS;
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
    }
    else if (path[n - 1].east_mm < path[n].east_mm && estimated_position.east_mm < path[n].east_mm) {
      return true;
    }
  }
  else {
    if (path[n].north_mm > path[n - 1].north_mm && estimated_position.north_mm > path[n].north_mm) {
      return true;
    }
    else if (path[n - 1].north_mm < path[n].north_mm && estimated_position.north_mm < path[n].north_mm) {
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
    if (path[n].east_mm > path[n - 1].east_mm && estimated_position.east_mm > path[n].east_mm + turn_radius_mm) {
      return true;
    }
    else if (path[n - 1].east_mm < path[n].east_mm && estimated_position.east_mm < path[n].east_mm - turn_radius_mm) {
      return true;
    }
  }
  else {
    if (path[n].north_mm > path[n - 1].north_mm && estimated_position.north_mm > path[n].north_mm + turn_radius_mm) {
      return true;
    }
    else if (path[n - 1].north_mm < path[n].north_mm && estimated_position.north_mm < path[n].north_mm - turn_radius_mm) {
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
  if (abs(path[n - 1].east_mm - path[n].east_mm) > abs(path[n - 1].north_mm - path[n].north_mm)) {
    if (path[n].east_mm > path[n - 1].east_mm && estimated_position.east_mm > path[n - 1].east_mm + turning_radius_mm) {
      return true;
    }
    else if (path[n - 1].east_mm < path[n].east_mm && estimated_position.east_mm < path[n - 1].east_mm - turning_radius_mm) {
      return true;
    }
  }
  else {
    if (path[n].north_mm > path[n - 1].north_mm && estimated_position.north_mm > path[n - 1].north_mm + turning_radius_mm) {
      return true;
    }
    else if (path[n - 1].north_mm < path[n].north_mm && estimated_position.north_mm < path[n - 1].north_mm - turning_radius_mm) {
      return true;
    }
  }
  return false;
}

/**
   The method find and set the state of the trike and
   also set the speed of the trike depending on the state it's in

   param : turning radius
   param : n = next -> index of the intersection you're approaching
*/
void find_state(long turn_radius_mm, int n) {

  //setting the speed to default
  speed_mmPs = DESIRED_SPEED_mmPs;

  switch (state) {
    case STRAIGHT:
      if (test_approach_intersection(turn_radius_mm, n)) {
        //Needs to find the last index of path
        if (n == last_index_of_path) {
          state = APPROACH_GOAL;
        }
        else {
          state = ENTER_TURN;
        }
        break;

      case STOP:
        break;

      case ENTER_TURN:
        //setting to turning speed
        speed_mmPs = turn_speed;

        if (test_past_destination(n)) {
          state = LEAVE_TURN;
          next++;

        }
        break;

      case LEAVE_TURN:
        //setting to turning speed
        speed_mmPs = turn_speed;
        if (test_leave_intersection(turn_radius_mm, n)) {
          state = STRAIGHT;
        }
        break;

      case APPROACH_GOAL:
        if (test_past_destination(n)) {
          state = STOP;
        }
        else if (test_approach_intersection(turn_radius_mm, n)) {
          //changing to a slower speed
          speed_mmPs = SLOW_SPEED_mmPs;
        }
        break;

      case LEAVE_GOAL:
        break;
      }
  }

}

/**
   This method determines the direction of turns of the vehicle
   in degrees

   param : n = next -> index of the intersection you're approaching
   return : pos = right, neg = left, 0 = straight
*/
int get_turn_direction_angle(int n) {
  if (state == ENTER_TURN || state == LEAVE_TURN) {
    float turn_direction = (estimated_position.Evector_x1000 * path[n].Evector_x1000) + (estimated_position.Nvector_x1000 * path[n].Nvector_x1000);
    int turn_direction_angle = 0;
    turn_direction /= 1000000.0;
    turn_direction = acos(turn_direction); // angle in radians
    float cross = (estimated_position.Evector_x1000 * path[n].Nvector_x1000) - (estimated_position.Nvector_x1000 * path[n].Evector_x1000);
    turn_direction_angle = turn_direction * 180 / PI; //angle is degrees
    if (cross < 0)
      turn_direction_angle = -turn_direction_angle;

    return turn_direction_angle;
  }
}
