#include <SPI.h>

/*
 * Sonar_SPI.h
 * By Jesse Llona
 *
 * Manages Serial Communications between the C5 sonar computer
 * and the C3 pilot computer.
 * Based off of Elcano_Serial by Dylan Katz
 */


namespace elcano
{

class dataSPI
{
	String input;			// The info to recieve or send
	bool verifyCommand(); 	// verifies that info was recieved correctly
};


}
