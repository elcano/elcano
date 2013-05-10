/*
 Elcano Contol Module C2: Instrument panel controls and LEDs.
 This routine handles simple drive-by-wire, as well as handing control over to the
 computer based on the state of the enable switches.
 */


#include <IO_PCB.h>


// inslude the SPI library:
#include <SPI.h>
/*---------------------------------------------------------------------------------------*/
void setup()
{
  //Set up pin modes and interrupts, call serial.begin and call initialize.
  Serial.begin(9600);
  
  // SPI: set the slaveSelectPin as an output:
  pinMode (SelectAB, OUTPUT);
  pinMode (SelectCD, OUTPUT);
  pinMode (10, OUTPUT);
  SPI.setDataMode( SPI_MODE0);
  SPI.setBitOrder( MSBFIRST);
  // initialize SPI:
  PRR0 &= ~4;  // turn off PRR0.PRSPI bit so power isn't off
  SPI.begin(); 
  for (int i = 0; i < 4; i++)
      DAC_Write (i, 0);   // reset did not clear previous states
  
//  Serial.println(SS);
//  Serial.println(MOSI);
//  Serial.println(MISO);
//  Serial.println(SCK); 
//  Serial.println(SPSR);
//  Serial.println(SPCR);
//  Serial.println(DDRB);
// Serial.println("SPSR=%x SPCR=%x\n", SPSR, SPCR); 
}

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/ 
// WheelRev is called by an interrupt.
/*
void WheelRev()
{
    static unsigned long OldTick = 0;
    unsigned long TickTime;
    unsigned long WheelRevMicros;
    TickTime = micros();
    if (OldTick == TickTime)
        return;
    if (OldTick <= TickTime)
      	WheelRevMicros = TickTime - OldTick;
    else // overflow
      	WheelRevMicros = TickTime + ~OldTick;
    SpeedCyclometer_degPs = (360 * MEG) / WheelRevMicros;
    OldTick = TickTime;
}
*/
/*---------------------------------------------------------------------------------------*/
void loop()
{

/*  To do:
Use SPI interface to output signal to motor.

Initial test will look at the voltage on this pin.  
May 10, 2013 TCF: Observed ramp on all four DAC output pins.
On PCB #1
Ramp on DAC channel A (IC2 pin 8) is seen on DB15 X3-12
Channel A is the output to motor, and is the only DAC channel currently used.
Ramp on DAC channel B (IC2 pin 6) is seen on DB15 X3-13
Ramp on DAC channel D (IC3 pin 6) is seen on DB9M X5-3

No ramp is seen on DB9F X4-3.  There is no continuity between IC3 pin 8 and X4-3.

Only after it looks good, do we attach the DB15 cable to drive the motor.
   
*/
 // go through the four channels of the digital analog converter:
  for (int channel = 0; channel < 4; channel++)
  { 
      // change the voltage on this channel from min to max:
      for (int level = 0; level < 255; level++) 
      {
        DAC_Write(channel, level);
        delay(100);
      }
      // wait a second at the top:
      delay(1000);
      // change the voltage on this channel from max to min:
      for (int level = 0; level < 255; level++) 
      {
        DAC_Write(channel, 255 - level);
        delay(100);
      }
 }    

}
/*---------------------------------------------------------------------------------------*/


void DAC_Write(int address, int value)
// address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
// value: digital value converted to analog voltage
// Output goes to mcp 4802 DAC
// No input
/*
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

{
  int byte1 = ((value & 0xF0)>>4) | 0x10; // acitve mode, bits D7-D4
  int byte2 = (value & 0x0F)<<4;           // D3-D0
  if (address < 2)
  {
      // take the SS pin low to select the chip:
      digitalWrite(SelectAB,LOW);
      if (address >= 0)
      { 
        if (address == 1)
          byte1 |= 0x80;  // second channnel
        SPI.transfer(byte1);
        SPI.transfer(byte2);
 //       Serial.print(byte1);  Serial.print(", ");
 //       Serial.println(byte2);
       }
      // take the SS pin high to de-select the chip:
      digitalWrite(SelectAB,HIGH);
  }
  else
  {
      // take the SS pin low to select the chip:
      digitalWrite(SelectCD,LOW);
      if (address <= 3)
      {
        if (address == 3)
          byte1 |= 0x80;  // second channnel
        SPI.transfer(byte1);
        SPI.transfer(byte2);
      }
       // take the SS pin high to de-select the chip:
      digitalWrite(SelectCD,HIGH);
  }
}
/*---------------------------------------------------------------------------------------*/



