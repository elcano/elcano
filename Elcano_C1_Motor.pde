/*
Elcano Contol Module C1: Motor and brake control.
This also incorporates control module C2: Steering.

INPUTS:

Digital Signal 0: J1 pin 1 (RxD) Serial signal from C3 Pilot
  Serial input is a Gamebot command. Use of this syntax provides compatibility with the simulator.

  DRIVE {Speed ComandedSpinSpeed} {FrontSteer ComandedSteerAngle} 

  Where: 
  {Speed ComandedSpinSpeed} 	ComandedSpinSpeed is a ‘float’ giving the spin speed for the rear wheel. The value is the absolute spin speed, in radians per second. 
  {FrontSteer ComandedSteerAngle} 	ComandedSteerAngle is a ‘float’ that specifies the steer angle of Elcano’s front wheels. The value is the absolute steer angle, in radians. 

Digital Signal 2: J1 pin 3 to Atmega8 pin 4 PD2 (INT0): Input from cyclometer. 
  The signal comes from a reed switch that opens on each revolution of the wheel. 
  The signal is used to compute ActualSpinSpeed (radians/sec).

Digital Signal 3: J1 pin 4  (INT1) Reserved

Analog input 0: J2 pin 1 (ADC0) ActualSteerAngle.


OUTPUTS:
Digital Signal 1: J1 pin 2 (TxD). Reserved.
Digital Signal 8: J3 pin 1 (T1) PWM. A3: Steering Motor. 
  Turns left or right. If no signal, wheels are locked to a straight ahead position.
Digital Signal 9: J3 pin 2 (OC1) PWM. A1: Drive Motor. 
  Controls motor on rear wheel.  Three wires for throttle.
Digital Signal 10: J3 pin 3 (SS) PWM. A2: Brake Motor. 
  Controls disk brakes on left and right front wheels. Brakes are mechanically linked to operate in tandem. 
Digital Signal 11: J3 pin 4 (MOSI) PWM. Recombinant braking.
Digital Signal 13: J3 pin 5 (SCK)  LED on Arduino board. Used for testing.


UNASSIGNED
Digital Signal 4, 5, 6, 7, 12
Analog input 1,2,3,4,5

*/

const int RxD = 0;
const int TxD = 1;
const int Tachometer = 2;
const int Int1 = 3;
const int Steer = 8;
const int Drive = 9;
const int DiskBrake = 10;
const int Recombinant = 11;
const int LED = 13; 
const int SteerFeedback = 1
volatile unsigned long WheelRevMicros = 0;
void setup() 
{ 
        pinMode(RxD, INPUT);
        pinMode(TxD, OUTPUT);
        pinMode(Tachometer, INPUT);
        pinMode(Int1, INPUT);
        pinMode(LED, OUTPUT); 
	attachInterrupt (0, WheelRev, RISING);
	Serial.begin(9600); 

}	
void loop() 
{
  static float ComandedSpinSpeed = 0;
  static float ComandedSteerAngle = 0;
  static float CurrentSpinSpeed = 0;
  static float CurrentSteerAngle = 0;
 // TO DO: Read Gamebot command on RxD and put in ComandedSpinSpeed and ComandedSteerAngle
 // TO DO: If no new Gamebot command received in 2 sec, stop.
 // TO DO: Use WheelRevMicros to compute CurrentSpinSpeed.
 // TO DO: Use differences in commanded and actual spin speed to control Drive, DiskBrake and Recombinant.
 // TO DO: Read CurrentSteerAngle from SteerFeedback.
 // TO DO: Use differences in commanded and actual steer angle to control Steer.
} 
void WheelRev()
{
	static unsigned long OldTick;
	static unsigned long TickTime;
	OldTick = TickTime;
	TickTime = micros();
	if (OldTick <= TickTime)
		WheelRevMicros = TickTime - OldTick;
	else // overflow
		WheelRevMicros = TickTime + ~OldTick;
}


