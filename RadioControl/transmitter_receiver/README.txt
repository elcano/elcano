	* File name: transmitter-receiver.ino
	* Created by: Joe Breithaupt for the Elcano Project
	* Date: June 2018
	* Required libraries: 
    ElcanoSerial (authors: Elcano project)
    RadioHead (author: Mike McCauley)
	* Summary
	This unified transmitter and receiver file uses the RadioHead ASK protocol to communicate 
remote control signals over radio from an arduino in the transmitter unit to a receiver arduino. The
receiver arduino converts these inputs into an ElcanoSerial drive packet which it sends to C2 
(the vehicle driver processor) over an arduino hardware serial line. In effect, this allows
radio control using ElcanoSerial.
	Receiver loop timing is carefully controlled so that the processor is always checking for new packets
from the transmitter but only sends a packet over ElcanoSerial every 100 milliseconds(ms). 
There is also a watchdog routine that activates the ebrake if so many cycles have gone by without a packet.
This is accomplished using "if" control structures that only execute if a certain amount of milliseconds have
elapsed since some event. This avoids the blocking behaviour and inconsistency (like drift) of using 
the delay() function. 
	* Known issues: ElcanoSerial relies on HardwareSerial which conflicts with the way an arduino Micro
uses Serial and Serial1. This means receiver code will not run on a micro because the sketch 
will not compile