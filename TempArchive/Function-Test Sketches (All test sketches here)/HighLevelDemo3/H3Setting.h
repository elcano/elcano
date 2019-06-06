#include <Common.h>
#include <SPI.h>
#include <SD.h>
#include <IODue.h>
#include <Wire.h>
extern TwoWire Wire1;
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <FusionData.h>
#include <Adafruit_GPS.h>
#include <ElcanoSerial.h>
#include <Serial_Communication.h>
#define DESIRED_SPEED_mmPs 1000         //TODO: need to be changed
#define currentlocation  -1             //currentLocation
#define mySerial Serial3
using namespace elcano;

bool got_GPS = false;
long speed_mmPs = DESIRED_SPEED_mmPs;
int  next = 1;                          //index to path in a list
long current_heading = -1;
long pre_desired_speed = 0;
long turn_direction = 0;
long pre_turn_angle = 0;
long extractSpeed = 0;                  //alternative to checksum since it's not implemented ie check for bad incoming data through serial
long turn_radius_mm = 2000;             //TODO: need to be calibrate

Origin origin(47.759830, -122.190254);  //origin is set to infront of UW1 
waypoint path[2];                       //course route to goal/mission
waypoint path0, path1;
waypoint GPS_reading, estimated_position, oldPos, newPos, Start;

Adafruit_GPS GPS(&mySerial);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

enum States { STOP, STRAIGHT, ENTER_TURN, LEAVE_TURN, APPROACH_GOAL, LEAVE_GOAL};
States state;


// populate the intersection positions in the path
void populate_path() {

  path0.east_mm = -9432;
  path0.north_mm = -66564;

  path1.east_mm = -15136;
  path1.north_mm = -65716;

  path[0] = path0;
  path[1] = path1;

  for (int i = 0; i < 1; i++) {
    path[i].vectors(&path[i + 1]);
  }
}

// get gps coordinates
bool AcquireGPS(waypoint &gps_position) {
  float latitude, longitude;
  char c;
  for (int i = 0; i < 25; i++) {
    c = GPS.read();
  }
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return false;
    if (GPS.fix) {
      gps_position.latitude = GPS.latitudeDegrees;
      gps_position.longitude = GPS.longitudeDegrees;
       Serial.println("From GPS coordinates: ");   // 47.78823090 | -122.22163391
       Serial.print(GPS.latitudeDegrees,8);    
       Serial.print("  |  ");
       Serial.println(GPS.longitudeDegrees,8);
      return true;
    }
    return false;
  }
}

// return where the trike is heading to
long getHeading(void) {
  sensors_event_t event;
  mag.getEvent(&event);
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / PIf;
  if (heading < 0)  {
    heading = 360 + heading;
  }
  return heading;
}

void initial_position() {
  bool GPS_available = AcquireGPS(estimated_position);
  Serial.println("Initializing GPS...");
  while (!GPS_available) {
    GPS_available = AcquireGPS(estimated_position);
  }
  Serial.println("GPS initialize success!");
  Serial.println("====================================================");
  estimated_position.time_ms = millis();
  estimated_position.Compute_EandN_Vectors(getHeading()); //get position E and N vector 
  estimated_position.Compute_mm(origin);  //initialize north and east mm based on original

  Serial.print("Estimate E: "); Serial.println(estimated_position.east_mm);
  Serial.print("Estimate N: "); Serial.println(estimated_position.north_mm);
  oldPos = estimated_position;
}


/*=======================================================================================
   All the Methods for C3 (pilot) starts here
=======================================================================================*/
/**
   This method checks to see if we have passed the destination and missed the turn.

   param : n = next -> index of the intersection you're approaching
   return true = past the destination , false = Have not past the destination
*/
bool do_We_Pass_The_NextGoal(int n) {
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
bool enter_Intersection_From_Staight_To_Turn(long turn_radius_mm, int n) {
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

/** leave_Intersection_From_Turn_To_Staight
   This method test for intersection from leaving turn to straight

   param : turning radius of the vehicle
   param : n = next -> index of the intersection you're approaching
   return : true -> if approached intersection to leave turn, false -> otherwise  */
bool leave_Intersection_From_Turn_To_Staight(long turning_radius_mm, int n) {
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

/** turn_Right_Or_Left_When_Enter_Turn
   This method determines the direction of turns of the vehicle of either left 
   or right once approached a turn intersection. Angle returned is either 90 for right 
   and -90 for left.  

   param : n = next -> index of the intersection you're approaching
   return : pos = right, neg = left, 0 = straight */
int turn_Right_Or_Left_When_Enter_Turn(int n) {

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
