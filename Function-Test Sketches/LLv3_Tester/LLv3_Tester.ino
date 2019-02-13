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

#include <SPI.h>

// Using the Seeed Studio CAN sheild library, from https://github.com/Seeed-Studio/CAN_BUS_Shield
// available in the library manager, search "CAN_BUS" to get it in the top three results.
#include <mcp_can_dfs.h>
#include <mcp_can.h>

// The LLBv3 pinout is different from previous LLB versions, this file defines keywords and maps features to pins
#include "pinout.h"



 void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Interactive System Test Start!");

  pinMode(BRAKE_ON, OUTPUT);
  pinMode(VOLTAGE_SWITCH, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.println("Start Testing?");
  if( !interactive_confirm()){
    Serial.println("...");
    return;
  }

  bool testStatus[5] = {0};
  uint8_t testIndex = 0;

  testStatus[testIndex++] = CAN_test();
  
  testStatus[testIndex++] = relay_test();
  
  testStatus[testIndex++] = buzzer_test();

  
  Serial.println("All tests done! Summary:");
  for(uint8_t index = 0; index < 5; index++){
    Serial.println(testStatus[index]);
  }
 
  
  Serial.println("Entering REPL mode.");
}

// variables for loop scope
bool relay_state[2] = {false, false};

// In the loop routine we allow the user to enter commands and run specific tests and functions
void loop() {
  while(Serial.available() > 0) Serial.read(); // clear input buffer
  Serial.println("Enter command:");
  
  while(Serial.available() <= 0) {
    delay(10);
  }

  uint8_t input = (Serial.read() - 48); // ASCII text offset
  switch(input){
    case 1:
      relay_state[0] = !relay_state[0];
      Serial.print("1: Toggle Brake");
      Serial.println(relay_state[0]?"ON":"OFF");
      delay(100);
      digitalWrite(BRAKE_ON, relay_state[0]);
      break;
    case 2:
      relay_state[1] = !relay_state[1];
      Serial.print ("2: Toggle Voltage ");
      Serial.println(relay_state[1]?"ON":"OFF");
      delay(100);
      digitalWrite(VOLTAGE_SWITCH, relay_state[1]);
      break;
    case 3:
      Serial.println("3: Buzzer");
      delay(100);
      buzz();
      break;
    case 221: // just hit return
      break;
    default:
      Serial.print("unknown command #");
      Serial.println(input);
      break;
  }
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
  buzz();
  Serial.println("Buzzer Test Done.");
  Serial.println("Did you hear a series of tones?");
  return interactive_confirm();
}

void buzz(){
  for(uint8_t count = 0; count < 50; count++){
    for (uint8_t t = 0; t < 128; t++){
      tone(BUZZER, 1000 + t * 200 ,200);
    }
  }
}

bool relay_test(){
  Serial.print("Starting relay testing...");
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
  pinMode(CAN_SS, OUTPUT);
  
  MCP_CAN CAN(CAN_SS);
  if(CAN_OK != CAN.begin(CAN_500KBPS)){
    Serial.println("Failed to Init!");
    return false;
  }
  Serial.println("CAN Init OK");

  
  
  Serial.print("Testing Send,\nSending 10 messages...");
  uint8_t msgbuf[8] = {1,2,3,4,5,6,7,8};
  //uint8_t msgbuf[8] = {'a','b','c','d','e','f','g','h'};

  for(uint8_t count = 0; count < 8; count++){
    CAN.sendMsgBuf(0x70, 0, 8, msgbuf);
    delay(500);
  }
  Serial.println("Done");
  if(CAN.checkError()) Serial.println("Error Detected: " + CAN.checkError());
  Serial.println("CAN send OK?");
  
  bool transmit = interactive_confirm();


  Serial.println("Testing CAN receive");

  // Receive messages for 10 seconds
  uint16_t startTime = millis();
  while(millis() - startTime < 10000){
    unsigned char len = 0;
    unsigned char buf[8];

    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned int canId = CAN.getCanId();
        
        Serial.println("-----------------------------");
        Serial.print("Get data from ID: ");
        Serial.println(canId, HEX);

        Serial.print("DATA length: ");
        Serial.println(len);

        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }

         Serial.println();
    }
  }
  
  bool receive = interactive_confirm();

  
  return transmit && receive;
}

bool odo_test(){
  
}
