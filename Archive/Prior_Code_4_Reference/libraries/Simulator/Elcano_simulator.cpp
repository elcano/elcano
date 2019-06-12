/* Elcano_simulator.cpp
   This enables running the Arduino Modules C3, C4, C6 and C7
   on a PC, under either Linux or Windows.
   The Elcano_simulator interfaces to the USARSIM simulator
   using DRIVE{} and SEN{} gamebot commands.
   The USARSIM simple UI can serve as a model for the Elcano_simulator.
   https://sourceforge.net/projects/usarsim/

   TODO: constuct a makefile that allows compiling under Linux or Windows.
	The makefile must copy Serial.cpp from a central location to each
	Arduino directory.  
	Code must compile both for Arduinos and Simulator.
   If compiling under Visual Studio 2008:
   Tools | Options | VC++ Project Settings | C/C++ File Extensions
     >>>> add *.pde
   Project | Properties | Configuration Properties | C/C++ | Pecompiled Headers
     >>>> select Not using Precompiled Headers
   Project | Properties | Configuration Properties | C/C++ | Pecompiled Definitions
     >>>> add SIMULATOR
*/


#include "Elcano_simulator.h"
void pinMode(int a1, int a2) {} // stub
void digitalWrite(int a1, int a2) {}

/* Here we include files common to Arduino boards. 
   Each Arduino has a copy of these files.
   In the simulator, there is only one instance.
   Common files should be reentrant.
   Common files should not use static or global variables.
*/
#include "Serial.cpp"

using namespace C3_Pilot;
using namespace C4_Planner;
using namespace C6_Navigator;
using namespace C7_Vision;

/*
TODO: Include Arduino libraries
*/
void Elcano_simulator()
{
	C6_Navigator_setup();
	C4_Planner_setup();
	C7_Vision_setup();
	C3_Pilot_setup();

	while (true)
	{
		C6_Navigator_loop();
		C4_Planner_loop();
		C7_Vision_loop();
		C3_Pilot_loop();
	}

}