#include "H3Setting.h"

/*=======================================================================================
    All C6 (Navigator) Methods start here
  =======================================================================================*/
void setup_C6() {
  mySerial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  mag.enableAutoRange(true); //Enable auto-gain
  if (!mag.begin()) {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  }
  mag.begin();

  initial_position();    // get initial position by GPS
  newPos.speed_mmPs = 0; // initial speed of trike is 0
}

void loop_C6() {
  oldPos.time_ms = millis();
  delay(1);

  //try to get a new GPS position
  got_GPS = AcquireGPS(GPS_reading); 
   
  if (got_GPS) {
    // get north and east coordinates from originl
    GPS_reading.Compute_mm(origin);   
    //Serial.println("GPS compute_mm");
    //Serial.println(GPS_reading.east_mm); 
    //Serial.println(GPS_reading.north_mm);
    //Serial.println();
  }

  // TODO: Receive actual speed from low level and update the newPos.speed_mmps
  newPos.speed_mmPs = 10; // hardcode for now
  newPos.time_ms = millis();

  //get heading coordinates from the compass
  newPos.bearing_deg = getHeading();

  if (got_GPS) { 
    //estimated_position is updated to the current position inside this method
    FindFuzzyCrossPointXY(GPS_reading, newPos, estimated_position);
    //calculating the E and N vector by constantly updating everything you move
    oldPos.vectors(&estimated_position);
  }
  else {
    //calculate position using Dead Reckoning
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

/*=======================================================================================
  All the Methods for C3 (pilot) starts here
=======================================================================================*/
/** The method find and set the state (speed) of the trike
  param : turning radius
  param : n = next -> index of the intersection you're approaching */
void find_state(long turn_radius_mm, int n) {
//  Serial.print("State is: ");
//  Serial.println(state);
//  Serial.print("n is: ");
//  Serial.println(n);
//  Serial.println();

  switch (state) {
    
    case STRAIGHT:
      speed_mmPs = DESIRED_SPEED_mmPs;
      if (enter_Intersection_From_Staight_To_Turn(turn_radius_mm, n)) {
        if (n == 2) {   //last index of path
          Serial.println("We arrived destination");
          state = APPROACH_GOAL;
        }
        else {
          Serial.println("We are entering a turn!!!!!!");
          state = ENTER_TURN;
        }
      }
      break;

    case STOP:
      speed_mmPs = 0;
      break;

    case ENTER_TURN:
      speed_mmPs = 835; //TODO： setting to turning speed
      if (leave_Intersection_From_Turn_To_Staight(turn_radius_mm, n)) {
        state = STRAIGHT;
        next++;
      }
      break;

    case APPROACH_GOAL:
      if (do_We_Pass_The_NextGoal(n)) {
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
}

void loop_C3() {
  //Determining the state of the Trike
  find_state(turn_radius_mm, next);
  //Determining the turn direction for the trike "left, right or straight"
  turn_direction = turn_Right_Or_Left_When_Enter_Turn(next);

  if (pre_desired_speed != speed_mmPs || pre_turn_angle != scaleDownAngle(turn_direction)) {
    //    Serial.print("Speed: ");
    //    Serial.print(speed_mmPs);
    //    Serial.print("  |   Angle: ");  // problem wih this value
    //    Serial.println(turn_direction);
    
    // TODO: send speed and angle to low level

    pre_desired_speed = speed_mmPs;
    pre_turn_angle = scaleDownAngle(turn_direction); 
  }
}

void setup(){
  Serial.begin(9600);
  Serial2.begin(9600);
  populate_path();
  setup_C6();
  setup_C3();
}

void loop() {
  loop_C6();
  loop_C3();
}
