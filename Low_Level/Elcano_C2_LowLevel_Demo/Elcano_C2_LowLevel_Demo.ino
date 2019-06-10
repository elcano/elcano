#include "Settings.h"
#include "Vehicle.h"


Vehicle myTrike  = Vehicle();

//Timing stuff
#define LOOP_TIME_MS 100
unsigned long nextTime;
unsigned long endTime;
unsigned long delayTime;
#define ULONG_MAX 0x7FFFFFFF

//Code for demo
unsigned long SectionStart;
int section = 0;

//Do we want to print to the serial port?
//Is a define so that it is constant in all code
#define SerialPrint false

void setup() {

Serial.begin(baud);
myTrike.initialize();

//for demo
SectionStart = floor(millis()/1000); //seconds
section = 0;  

if (SerialPrint)
	Serial.println("Setup complete");
}

void loop() {
//Timing code
  nextTime = nextTime + LOOP_TIME_MS;
  unsigned long timeStart_ms = millis();

//Checks if the brakes are on too long
//If enabled computes the current angle and speed
  myTrike.update();



/***********START OF DEMO SECTION**********************************/
  //local vars 
  static long int desired_speed_cmPs, desired_angle;
  int secs = floor(timeStart_ms / 1000) - SectionStart;
    
//Each section is 5 seconds
  if (secs >= 5){
	  if (section < 2) {
// wait for 10 sec before starting the routine
	}
    else if(section < 9){
//dancing section
    desired_angle = ChooseAngle(section-2);
    desired_speed_cmPs = ChooseSpeed(section-2);
    myTrike.noPID(desired_angle, desired_speed_cmPs);  
    }
    else if (section == 9)
//reduces speed, argument is by how quickly large numbers stop quicker
    myTrike.stop(0.25);
    
    section++;
    SectionStart= floor(timeStart_ms / 1000);
  }
  //once we run the routine and have probably stopped, exit
  if (section > 15)
	  exit(1);
  /***********END OF DEMO SECTION**********************************/



//Timing code
   endTime = millis();
 delayTime = 0UL;
  if((nextTime >= endTime) && (((endTime < LOOP_TIME_MS)&&(nextTime < LOOP_TIME_MS)) || ((endTime >= LOOP_TIME_MS) && (nextTime >= LOOP_TIME_MS)))){
    delayTime = nextTime - endTime;
    if(nextTime < LOOP_TIME_MS)
      delayTime = ULONG_MAX - endTime + nextTime;
    else{
      nextTime = endTime;
      delayTime = 0UL;
    }
  }
  if (delayTime > 0UL)
    delay(delayTime);

}

/*
for demo: 
speeds up and then default is 8
*/
int ChooseSpeed(int section){
    int s = 0;
  switch(section){
    case 0:
      s = 2;
      break;
     case 1:
      s = 4;
      break;
     case 2:
      s = 6;
      break;
     default:
      s = 8;
  }

  s = s* KmPh_mmPs;
  return s;
}

/*
for demo: goes 
straight, left, left,
straight, right, right
default is straight
*/
int ChooseAngle(int section){
  int angle = 0;
  switch(section){
    case 0:
      angle = CENTER_TURN;
      break;
     case 1:
     case 2:
      angle = CENTER_TURN - 12;
      break;
     case 3:
      angle = CENTER_TURN;
      break;
     case 4:
     case 5:
      angle = CENTER_TURN +12;
      break;
     default:
      angle = CENTER_TURN;
  }
  return angle;
  
}

