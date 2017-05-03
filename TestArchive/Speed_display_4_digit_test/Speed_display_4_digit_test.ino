/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

Modified for use with SparkFun Serial7Segment LED digital display by Jeremy Bobotek

Notes for display usage: 
Special commands to display (such as clear display) must be sent in 2 byte packets.
Writes to display must be sent in 4 byte packets.

 */

#define s7s Serial3

void testsetup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  s7s.begin(9600);
  //char resety[2] = {0x7F, '2'}; //set baud rate to 9600 on device
  //char dataMode[2] = {0x82, '2'};//set mode to data in EEPROM
  
  //Serial1.print(resety); //clear display
  s7s.write("v");
  s7s.write(0x79); // Send the Move Cursor Command
  s7s.write(0x00); // Move Cursor to left-most digit
  //Serial1.print(dataMode); //change mode to data mode
  randomSeed(0);
}

uint16_t count = random(0,100);

void testloop() {
  char temp[4];
  sprintf(temp, "%4d", count);
  String temp3 = (String)temp;
  //temp3 = "1234";
  if(Serial.available()){
    s7s.write(Serial.read());
  }
  s7s.print(temp3);
  Serial.print(temp3);
  count = random(0,100);
  //count++;
//  if(count > 9999){
//    count = 0;
//  }
  

  Serial.println(count);
  delay(2000);
}

