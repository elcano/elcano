/*
Program to test Elcano_Serial.cpp
Tyler Folsom         Sept 7, 2015
This test was run an an Arduino Mega, with pin D18 (TX1) connected to pin D0 (RX0).
* Aisha. Jan 3, 2017.
* Paul Curry. Jan 3, 2017
*/


#include <Elcano_Serial.h>
using namespace elcano;
#define LOOP_TIME_MS 5000
ParseState state;
#define hardware Serial1
#define to_computer Serial
SerialData serial_data;

/* Setup serialData object */
void setup() {
    to_computer.begin(9600);
    to_computer.print("\n\n\n");
    hardware.begin(9600);
    state.dev = &hardware;
    state.dt = &serial_data;

}

/* Begin testing */
void loop() {
  to_computer.print("\n\nBeginning Tests\n"); 
  msg_drive0();
  msg_drive1();
  msg_drive2();

  msg_sensor0();
  msg_sensor1();
  msg_sensor2();
  
  msg_seg0();
  msg_seg1();
  
  msg_goal0();
  msg_goal1();
  msg_goal2();
  
  delay(LOOP_TIME_MS); 
 
}
void msg_drive0(){
  to_computer.println("in msg_drive0");
  serial_data.clear();
  
  serial_data.kind = MsgType::drive;
  serial_data.angle_deg = 25;    // OK
  serial_data.speed_cmPs = 50;
  serial_data.write(&hardware);
  test();
}
void msg_sensor0(){
  to_computer.println("in msg_sensor0");
  serial_data.clear();
  serial_data.kind = MsgType::sensor;
  serial_data.angle_deg = -12;   
  serial_data.bearing_deg = 432;
  serial_data.posN_cm = 90210;
  serial_data.posE_cm = 420;
  serial_data.speed_cmPs = 69;
  serial_data.write(&hardware);
  test();
}
void msg_sensor1(){
  to_computer.println("in msg_sensor1");
  serial_data.clear();
  serial_data.kind = MsgType::sensor;
  serial_data.angle_deg = -100;
  serial_data.bearing_deg = -30;
  serial_data.posN_cm = 30;
  serial_data.posE_cm = -91; 
  serial_data.angle_deg = 165;   
  serial_data.speed_cmPs = 1691;      
  serial_data.write(&hardware);
  test();
}
void msg_sensor2(){
  to_computer.println("in msg_sensor2");
  serial_data.clear();
  serial_data.kind = MsgType::sensor;
  serial_data.angle_deg = 45837;
  serial_data.bearing_deg = 30095;
  serial_data.posN_cm = 650;
  serial_data.posE_cm = 109;  
  serial_data.angle_deg = 72;   
  serial_data.speed_cmPs = 95;      
  serial_data.write(&hardware);
  test();
}
void msg_goal2(){
  to_computer.println("in msg_goal2");
  serial_data.clear();
  serial_data.kind = MsgType::goal;
  serial_data.number = 2;
  serial_data.posE_cm = 0;
  serial_data.probability = 59;
  serial_data.posN_cm = 0;
  serial_data.bearing_deg = 679;
  serial_data.write(&hardware);  // OK
  test();

}
void msg_goal0(){
  to_computer.println("in msg_goal0");
  serial_data.clear();
  serial_data.kind = MsgType::goal;
  serial_data.number = 3;
  serial_data.posE_cm = -23456;
  serial_data.posN_cm = 123450;
  serial_data.bearing_deg = 679;
  serial_data.write(&hardware);  // OK
  serial_data.bearing_deg = 45;
  test();
  
}
void msg_goal1(){
  to_computer.println("in msg_goal1");
  serial_data.clear();
  serial_data.kind = MsgType::goal;
  serial_data.number = 3;
  serial_data.posE_cm = -23456;
  serial_data.posN_cm = 123450;
  serial_data.bearing_deg = 679;
  serial_data.write(&hardware);  // OK
  test();
}
void msg_drive1(){
  to_computer.println("in msg_drive1");
  serial_data.clear();
  serial_data.kind = MsgType::drive;
  serial_data.speed_cmPs = 1500;    // OK
  serial_data.angle_deg = 60;
  serial_data.write(&hardware);
  test();
}
void msg_drive2(){
  to_computer.println("in msg_drive2");
  serial_data.clear();
  serial_data.kind = MsgType::drive;
  serial_data.angle_deg = 20;
  serial_data.speed_cmPs = 1000;  // OK
  serial_data.write(&hardware);
  test();
}
void msg_seg0(){
  to_computer.println("in msg_seq0");
  serial_data.clear();
  serial_data.kind = MsgType::seg;
  serial_data.number = 1;
  serial_data.posE_cm = 14263;
  //serial_data.probability = 59;  // not used
  serial_data.speed_cmPs = 1599; // test
  serial_data.bearing_deg = 270;
  serial_data.posN_cm = 123456;
  serial_data.write(&hardware);
  test();
  
}
void msg_seg1(){
  to_computer.println("in msg_seq1");
  serial_data.clear();
  serial_data.kind = MsgType::seg;
  serial_data.number = 2;
  serial_data.posE_cm = 4263;
  serial_data.speed_cmPs = 1599; 
  serial_data.bearing_deg = 227;
  serial_data.posN_cm = 789012;
  serial_data.write(&hardware);
  test();
}

void test(){

  /*
   * We have to wait until the state update status is that of success, otherwise we are 
   * probably getting an unavailable or incomplete status
   * 
   */
  ParseStateError r = state.update();
  bool flag = true;
  while(flag && r != ParseStateError::success) {
    r = state.update();
    switch(r) {
      case ParseStateError::inval_comb: //!< Complete package, failed validation
        to_computer.println("status = inval_comb\n");
        flag = false;
        break;
      case ParseStateError::success: //!< Complete package
        to_computer.println("status = success");
        flag = false;
        break;
      case ParseStateError::bad_type: //!< Syntax error: types should be [DSGX]
        to_computer.println("status = bad type\n");
        flag = false;
        break;
      case ParseStateError::bad_lcurly: //!< Syntax error: expected '{' or '\n' but got neither
        to_computer.println("status = bad_lcurly\n");
        flag = false;
        break;
      case ParseStateError::bad_attrib: //!< Syntax error: attributes should be [nsabpr]
        to_computer.println("status = bad_attrib\n");
        flag = false;
        break;
      case ParseStateError::bad_number: //!< Syntax error: number had a bad symbol
        to_computer.println("status = bad_number\n");
        flag = false;
        break;
    }
  }
     
  if(r==ParseStateError::success){
    //  to_computer.println(String (serial_data.kind)+"msg_kind");
    if(serial_data.kind == MsgType::drive){
      to_computer.println("I am in drive");
      to_computer.println(String (serial_data.speed_cmPs) + " speed");
      to_computer.println(String (serial_data.angle_deg) + " angle_deg");
      to_computer.println();
    }
    if(serial_data.kind == MsgType::seg||
      serial_data.kind == MsgType::goal){
      to_computer.println("I am in seg|goal");
      to_computer.println(String (serial_data.bearing_deg) + " bearing_deg");
      to_computer.println(String (serial_data.posE_cm) + " posE_cm");
      to_computer.println(String (serial_data.posN_cm) + " posN_cm");
      to_computer.println();
    
    }
    if(serial_data.kind == MsgType::sensor){
      to_computer.println("I am in sensor");
      to_computer.println(String (serial_data.number)+ " number");
      to_computer.println(String (serial_data.bearing_deg) + " bearing_deg");
      to_computer.println(String (serial_data.posE_cm) + " posE_cm");
      to_computer.println(String (serial_data.posN_cm) + " posN_cm");
      to_computer.println();
    }
    if(serial_data.kind == MsgType::none){
      to_computer.println("I am in none"); 
    }
      //to_computer.println("I donot know where I am in");
  } else { // There was an error somewhere 
    to_computer.println(errorToString(r));
  }
  
}

/* debug method. Takes in the ParseStateError and outputs a string correpsonding to  
 *  the name of the value in the enum. 
 *  
 *  Ex. SerialData::errorToString(ParseStateError::bad_lcurly) = "bad_lcurly"
 */
String errorToString(ParseStateError enumVal) {
  
  switch(enumVal) {
    case ParseStateError::inval_comb: //!< Complete package, failed validation
      return "inval_comb"; 
    case ParseStateError::success: //!< Complete package
      return "success";
    case ParseStateError::incomplete: //!< Successful character read, not ready for usage
      return "incomplete";
    case ParseStateError::unavailable: //!< Couldn't read a character from the device at this time
      return "unavailable";
    case ParseStateError::bad_type: //!< Syntax error: types should be [DSGX]
      return "bad_type";
    case ParseStateError::bad_lcurly: //!< Syntax error: expected '{' or '\n' but got neither
      return "bad_lcurly";
    case ParseStateError::bad_attrib: //!< Syntax error: attributes should be [nsabpr]
      return "bad_attrib";
    case ParseStateError::bad_number: //!< Syntax error: number had a bad symbol
      return "bad number";
    default:
      return "not recognized";
  }
  
}


 

