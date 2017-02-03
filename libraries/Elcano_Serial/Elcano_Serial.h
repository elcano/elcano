#pragma once

/*
** Elcano_Serial.h
** By Dylan Katz
**
** Manages our protocal for robust communication of SerialData over serial connections
*/

#include <HardwareSerial.h>

namespace elcano {

//! The number representing when no data is sent
const int32_t NaN = 0x7FFFFFFF;

//! The different possible types of SerialData packets
enum class MsgType : int8_t {
	none   = 0, //!< Empty type
	drive  = 1, //!< Drive to a position
	/* speed_cmPs
	** angle_deg
	*/

	sensor = 2, //!< Info from the sensor
	/* speed_cmPs
	** angle_deg
	** posE_cm
	** posN_cm
	** bearing_deg
	*/

	goal   = 3, //!< Position of a goal
	/* number
	** posE_cm
	** posN_cm
	** bearing_deg
	** probability (optional)
	*/

	seg    = 4  //!< Part of the navigation path
	/* number
	** posE_cm
	** posN_cm
	** bearing_deg
	** speed_cmPs
	*/
};

//! Contains information to send/recieve over a serial connection.
struct SerialData {
	MsgType kind;   //!< The type of message being received [0-4]
	int32_t number; //!< The number of the unit
	int32_t speed_cmPs;  //!< The speed the bike is moving at (cm/s)
	int32_t angle_deg;   //!< Angle (deg) of the bike
	int32_t bearing_deg; //!< Bearing (deg) of the camera
	int32_t posE_cm;     //!< Position (cm) on the E-W axis
	int32_t posN_cm;     //!< Position (cm) on the N-S axis
	int32_t probability; //!< Probability that the value is a cone
	
	void clear(void); //!< Set the values to the defaults
	bool write(HardwareSerial * /**< Connection to write to */); //!< Write to a serial connection
	bool verify(void); //!< Check that the types match the values
};

//! The different possible results of the ParseState::update method
enum class ParseStateError : int8_t {
	inval_comb  =  2, //!< Complete package, failed validation
	success     =  1, //!< Complete package
	incomplete  =  0, //!< Successful character read, not ready for usage
	unavailable = -1, //!< Couldn't read a character from the device at this time
	bad_type    = -2, //!< Syntax error: types should be [DSGX]
	bad_lcurly  = -3, //!< Syntax error: expected '{' or '\n' but got neither
	bad_attrib  = -4, //!< Syntax error: attributes should be [nsabpr]
	bad_number  = -5  //!< Syntax error: number had a bad symbol
};

//! Contains internal state for the SerialData parser.
struct ParseState {
	HardwareSerial *dev; //!< Connection to read from
	SerialData *dt; //!< SerialData to write to
	
	ParseStateError update(void); //!< Update the state of the parser based on a single character
private:
	uint8_t state = 0; //!< Internal state variable
};

} // namespace elcano

/*

When structuring a component, follow the following skeleton:

	elcano::ParseState ps;
	elcano::SerialData dt;
	
	void setup() {
		Serial1.begin(9600);
		Serial2.begin(9600);
		
		ps.dt  = &dt;
		ps.dev = &Serial1;
		dt.clear();
		
		// Any other initialization code goes here
	}
	
	void loop() {
		elcano::ParseStateError r = ps.update();
		if (r == elcano::ParseStateError::success) {
			if (dt.kind == elcano::MsgType::DESIRED_TYPE) {
				// Update code here that depends on having recieved a data set
			} else {
				dt.write(&Serial2);
			}
		}
		
		// Update code here that does not depend on having received a data set
	}

Replace `DESIRED_TYPE` with {`drive`, `sensor`, `goal`, `seg`} depending on your component.

This ensures that we are sending data around the vehicle in a complete loop.
	
*/
