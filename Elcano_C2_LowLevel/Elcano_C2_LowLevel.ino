
#include <SPI.h>
#include "Settings.h"
#include "Vehicle.h"
#include "mcp_can.h"

Vehicle myTrike = Vehicle();
//myTrike.move(angle, speed) uses PIDS
//myTrike.noPID(angle,speed) does not use PIDS

//Timing stuff
#define LOOP_TIME_MS 100
uint32_t nextTime;
uint32_t endTime;
uint32_t delayTime;
#define ULONG_MAX 0x7FFFFFFF

#define Drive_CANID 0X05

void setup() {
  myTrike.initialize();
  if (DEBUG) {
    Serial.println("Setup complete");
  }
  
}

void loop()
{
  //Timing code
  nextTime = nextTime + LOOP_TIME_MS;
  uint32_t timeStart_ms = millis();

  myTrike.update();

  //Timing code
  endTime = millis();
  delayTime = 0UL;
  if ((nextTime >= endTime) && (((endTime < LOOP_TIME_MS) && (nextTime < LOOP_TIME_MS)) || ((endTime >= LOOP_TIME_MS) && (nextTime >= LOOP_TIME_MS)))) {
    delayTime = nextTime - endTime;
    if (nextTime < LOOP_TIME_MS)
      delayTime = ULONG_MAX - endTime + nextTime;
    else {
      nextTime = endTime;
      delayTime = 0UL;
    }
  }
  if (delayTime > 0UL)
    delay(delayTime);
}
