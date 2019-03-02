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

#define HEADLESS false

#include <SPI.h>
#include <Servo.h>
#include <PinChangeInterrupt.h>

// Using the Seeed Studio CAN sheild library, from https://github.com/Seeed-Studio/CAN_BUS_Shield
// available in the library manager, search "CAN_BUS" to get it in the top three results.
#include <mcp_can_dfs.h>
#include <mcp_can.h>

// The LLBv3 pinout is different from previous LLB versions, this file defines keywords and maps features to pins
#include "pinout.h"

#define PWM_MAX 180
#define PWM_MIN 0

Servo steer_servo;
MCP_CAN CAN(CAN_SS);

volatile uint8_t odoCount = 0;
volatile bool odoClick = false;

void odo_ISR(){
  odoClick = true;
  odoCount++;
}

void setup() {
  pinMode(BRAKE_ON, OUTPUT);
  pinMode(VOLTAGE_SWITCH, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(DAC_SS, OUTPUT);
  pinMode(CAN_SS, OUTPUT);
  pinMode(ODO_INT, INPUT);
  pinMode(STEER_SERVO_PWM, OUTPUT);

  attachPCINT(16, odo_ISR, FALLING);
  
  while(HEADLESS){
    buzz(1);
    delay(100);
    buzz(1);
    delay(1000);
    
    odo_test();

    buzz(1);
    delay(100);
    buzz(1);
  }
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Interactive System Test Start!");

  
  steer_servo.attach(STEER_SERVO_PWM);
  
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  Serial.println("Start Testing?");
  if( !interactive_confirm()){
    Serial.println("...");
    testsDone();
    return;
  }

  bool testStatus[7] = {0};
  uint8_t testIndex = 0;

  testStatus[testIndex++] = odo_test();
  
  testStatus[testIndex++] = PWM_test();
  
//  testStatus[testIndex++] = ADC_test();

//  testStatus[testIndex++] = UART_test();

  testStatus[testIndex++] = DAC_test();
  
//  testStatus[testIndex++] = CAN_test();
  
  testStatus[testIndex++] = relay_test();
  
  testStatus[testIndex++] = buzzer_test();
  
  Serial.println("All tests done! Summary:");
  for(uint8_t index = 0; index < 5; index++){
    Serial.println(testStatus[index]);
  }
  testsDone();
}

void testsDone(){
  Serial.println("Entering REPL mode.");
  printTests();
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
  selectTest(input, false);
}

// print every defined test name
void printTests(){
  for (uint8_t i = 0; i<11; i++){
    selectTest(i, true);
  }
}
/*
 * Given the input value, run the appropriate test function.
 * If menuMode is asserted, do not change the state, just print the meny selector.
 */
void selectTest(uint8_t input, bool menuMode){
  switch(input){
    case 1:
      if (!menuMode){
        relay_state[0] = !relay_state[0];
      }
      Serial.print("1: Toggle Brake (");
      Serial.println(relay_state[0]?"ON)":"OFF)");
      if (!menuMode){
        delay(100);
        digitalWrite(BRAKE_ON, relay_state[0]);
      }
      break;
    case 2:
      if (!menuMode){
        relay_state[1] = !relay_state[1];
      }
      Serial.print ("2: Toggle Voltage (");
      Serial.println(relay_state[1]?"ON)":"OFF)");
      if (!menuMode){
        delay(100);
        digitalWrite(VOLTAGE_SWITCH, relay_state[1]);
      }
      break;
    case 3:
      Serial.println("3: Buzzer");
      if (!menuMode) buzz(50);
      break;
    case 4:
      Serial.println("4: DAC Sweep ");
      if (!menuMode){
        DAC_sweep();
      }
      break;
    case 5:
      Serial.println("5: PWM Sweep");
      if (!menuMode) PWM_sweep();
      break;
    case 6:
      Serial.println("6: ADC input values");
      if (!menuMode) Serial.println("Not implemented!"); //ADC_test();
      break;
    case 7:
      Serial.println("7: CAN send");
      if (!menuMode) Serial.println("Not implemented!");
      break;
    case 8:
      Serial.println("8: CAN receive");
      if (!menuMode) Serial.println("Not implemented!");
      break;
    case 9:
      Serial.println("9: Wheel click sensor");
      if (!menuMode) odo_test();
      break;
    case 10:
      Serial.println("10: UART test");
      if (!menuMode) Serial.println("Not implemented!"); //UART_test();
      break;
    case 221: // the user just hit return
      // print out the menu
      printTests();
      break;
    default:
      //Serial.print("unknown command #");
      //Serial.println(input);
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
  buzz(0);
  Serial.println("Buzzer Test Done.");
  Serial.println("Did you hear a series of tones?");
  return interactive_confirm();
}

void buzz(uint8_t max){
  for(uint8_t count = 0 ; count < max; count++){
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
  Serial.print("Starting DAC testing. Emitting voltage sweep...");
  DAC_sweep();
  Serial.println("Done.");
  Serial.println("Did you measure a sweep? Expected range (0.00v - 4.00v).");
  return interactive_confirm();
}

void DAC_sweep(){
  // sweep for 10 seconds
  uint16_t startTime = millis();
  uint8_t value = 0;
  int8_t stepval = 1;
  // loop for about 10 seconds
  while(millis() - startTime < 10000){
    DAC_Write(0, value);
    value += stepval;
    // if we have reached the max or min values, flip the direction of the step
    if(value == 255 || value == 0) {
      stepval = -stepval;
    }
    delay(1); // who knows if this is slow enough? hope so!
    //Serial.println("V: " + value);
  }
  DAC_Write(0, 0);
}

/*
 *  !! WARNING !!
 *  THE FOLLOWING METHOD DAC_Write HAS BEEN COPIED WITHOUT REVIEW
 */
void DAC_Write(int address, int value) {
  /*
  DAC_Write applies value to address, producing an analog voltage.
  // address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
  // value: digital value converted to analog voltage
  // Output goes to mcp 4802 Digital-Analog Converter Chip via SPI
  // There is no input back from the chip.
  REGISTER 5-3: WRITE COMMAND REGISTER FOR MCP4802 (8-BIT DAC)
  A/B  —  GA  SHDN  D7 D6 D5 D4 D3 D2 D1 D0 x x x x
  bit 15                                       bit 0

  bit 15   A/B: DACA or DACB Selection bit
         1 = Write to DACB
         0 = Write to DACA
  bit 14   — Don’t Care
  bit 13   GA: Output Gain Selection bit
         1 = 1x (VOUT = VREF * D/4096)
         0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
  bit 12   SHDN: Output Shutdown Control bit
         1 = Active mode operation. VOUT is available.
         0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
         VOUT pin is connected to 500 k (typical)
  bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.

  With 4.95 V on Vcc, observed output for 255 is 4.08V.
  This is as documented; with gain of 2, maximum output is 2 * Vref
*/
  /*Serial.print("DAC Write: A=");
  Serial.print(address);
  Serial.print(" V=");
  Serial.print(value);
  */
  int byte1 = ((value & 0xF0) >> 4) | 0x10; // acitve mode, bits D7-D4
  int byte2 = (value & 0x0F) << 4;         // D3-D0
  if (address < 2)
  {
    // take the SS pin low to select the chip:
    digitalWrite(48, LOW);
    if (address >= 0)
    {
      if (address == 1)
        byte1 |= 0x80;  // second channnel
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
    digitalWrite(48, HIGH);
  }
  else
  {
    // take the SS pin low to select the chip:
    digitalWrite(48, LOW);
    if (address <= 3)
    {
      if (address == 3)
        byte1 |= 0x80;  // second channnel
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
    digitalWrite(48, HIGH);
  }
  //Serial.println(" -- DAC Write Done");
}
 /*
  * END OF COPIED CODE
  */

bool PWM_test(){
  PWM_sweep();
  Serial.println("Did you measure a sweep? Servo should have moved back and forth over a full range.");
  return interactive_confirm();
}

void PWM_sweep(){
  Serial.print("Starting PWM testing. Emitting position sweep...");
  int8_t stepval = 1;
  uint8_t PWMvalue = 1;
  // loop for count/2 cycles
  for(uint8_t count = 0; count < 10; count++){
    for(; ;) {
      steer_servo.write(PWMvalue);
      delay(5); // who knows if this is slow enough? hope so!
      PWMvalue += stepval;
      if(PWMvalue >= PWM_MAX || PWMvalue <= PWM_MIN) {
        stepval = -stepval;
        break;
      }
    }
  }
  Serial.println("Done.");
}

// initializes the CAN tranceiver and sends messages without considering ACKs
bool CAN_test(){
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


bool UART_test(){
  Serial3.begin(9600);
  
  Serial3.print('T');
  char input = Serial3.read();
  
  return false;
}

bool ADC_test(){
  return false;
}

bool odo_test(){
  if(!HEADLESS) Serial.println("Watching for falling edges...");
  
  // Count the number of ticks per second for 10 seconds
  uint16_t startTime = millis();
  while(millis() - startTime < 10000){
    if(odoClick){
      uint8_t tempCount = odoCount;
      odoClick = false;

      buzz(5);
      if(!HEADLESS){
        Serial.print("Got");
        Serial.println(tempCount);
      }
      
      delay(1000);
    }
  }
  
  return interactive_confirm();
}
