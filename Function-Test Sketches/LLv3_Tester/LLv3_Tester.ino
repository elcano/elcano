/*
 * Test all the things!
 * Runs a simple series of interactive tests to verify correct operation of all of the LLBv3's features.
 * 
 * Buzzer
 * Relays
 * DAC output
 * Steering PWM output
 * CANbus initilization
 * Wheel rotation
 * 
 * Note that tests of external interfaces need loopback wiring harnesses attached.
 * E.G. connecting digital outputs to digital inputs and vice versa.
 * 
 */



// Using the Seeed Studio CAN sheild library, from https://github.com/Seeed-Studio/CAN_BUS_Shield
// available in the library manager, search "CAN_BUS" to get it in the top three results.
#include <mcp_can_dfs.h>
#include <mcp_can.h>

// The LLBv3 pinout is different from previous LLB versions, this file defines keywords and maps features to pins
#include "pinout.h"



 // Nothing happens in the loop routine, because the test only runs once!
void loop() { }

 void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Interactive System Test Start!");

  Serial.println("Start Testing?");
  if( !interactive_confirm()){
    Serial.println("??? ... ok");
    while(1);
  }

  bool testStatus[5] = {0};
  uint8_t testIndex = 0;
  
  testStatus[testIndex++] = relay_test();
  
  testStatus[testIndex++] = buzzer_test();


  Serial.println("All tests done!");
  for(uint8_t index = 0; index < 5; index++){
    Serial.println(testStatus[index]);
  }
 
  
  // TODO print summary of results
}

bool interactive_confirm(){
  // clear Serail input bufer (just removes newline characters...)
  while(Serial.available() > 0) Serial.read();
  Serial.println("y/N");
  while(Serial.available() <= 0) {
    delay(10);
  }
  char input = Serial.read();
  Serial.println(input);
  return (input == 'Y') || (input == 'y');
}

bool buzzer_test(){
  Serial.print("Buzzer Begin...");
  delay(1000);
  for(uint8_t count = 0; count < 50; count++){
    for (uint8_t t = 0; t < 128; t++){
      tone(BUZZER, 1000 + t * 200 ,200);
    }
  }
  Serial.println("Buzzer Test Done.");
  Serial.println("Did you hear a series of tones?");
  return interactive_confirm();
}

bool relay_test(){
  Serial.print("Starting relay testing...");
  pinMode(BRAKE_ON, OUTPUT);
  pinMode(VOLTAGE_SWITCH, OUTPUT);

  uint8_t count = 7;
  bool stateTable[count][2] = { {0,0},{1,1},{0,1},{1,1},{1,0},{1,1},{0,0} };
  
  for(uint8_t i = 0; i < count; i++){
    digitalWrite(BRAKE_ON, stateTable[i][0]);
    digitalWrite(VOLTAGE_SWITCH, stateTable[i][1]); 
    delay(500);
  }
  Serial.println("Done.");
  Serial.println("Did you hear the relays click, and the LEDs light?");
  return interactive_confirm();
}

bool DAC_test(){
  
}

// puts the CAN tranceiver into loopback mode and verifies that it initializes and "sends" a message
bool CAN_test(){
  
}

bool odo_test(){
  
}
