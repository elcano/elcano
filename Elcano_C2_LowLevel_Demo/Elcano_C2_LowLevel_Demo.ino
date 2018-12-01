#include "Settings.h"
#include "Vehicle.h"


Vehicle myTrike  = Vehicle();
#define LOOP_TIME_MS 100
unsigned long SectionStart;
int section = 0;
unsigned long nextTime;
unsigned long endTime;
unsigned long delayTime;
#define ULONG_MAX 0x7FFFFFFF


void setup() {
  
Serial.begin(115200);
SectionStart = floor(millis()/1000); //seconds
section = 0;  
myTrike.initialize();
Serial.println("Setup complete");

}


void loop() {
  nextTime = nextTime + LOOP_TIME_MS;
  unsigned long timeStart_ms = millis();
  static long int desired_speed_cmPs, desired_angle;
  static bool e_stop = 0;
  myTrike.update();
  unsigned long secs = SectionStart - floor(timeStart_ms / 1000);


  
  if (secs >= 20){
    desired_angle = ChooseAngle(section);
    desired_speed_cmPs = ChooseSpeed(section);
    myTrike.noPID(desired_angle, desired_speed_cmPs);
    
    section++;
    Serial.println(section);
    SectionStart= floor(timeStart_ms / 1000);
  }
  if(section > 5)
  section=0;




  
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


int ChooseSpeed(int section){
    int s = 0;
  switch(section){
    case 0:
      s = 20 * KmPh_mmPs;
      break;
     case 1:
     case 2:
      s = 10 * KmPh_mmPs;
      break;
     case 3:
      s = 20 * KmPh_mmPs;
      break;
     case 4:
     case 5:
      s = 10 * KmPh_mmPs;
      break;
     default:
      s = 20 * KmPh_mmPs;
  }
  return s;
}
