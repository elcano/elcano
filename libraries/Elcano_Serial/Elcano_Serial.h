#pragma once

/*
** Elcano_Serial.h
** By Dylan Katz
**
** Manages our protocal for robust communication of SerialData over serial connections
*/

#include <HardwareSerial.h>

#define NaN 0x7FFFFFFF

#define MSG_NONE   0
#define MSG_DRIVE  1
#define MSG_SENSOR 2
#define MSG_GOAL   3
#define MSG_SEG    4

#define PSE_SUCCESS      1
#define PSE_INCOMPLETE   0
#define PSE_UNAVAILABLE -1
#define PSE_BAD_TYPE    -2
#define PSE_BAD_LCURLY  -3
#define PSE_BAD_ATTRIB  -4
#define PSE_BAD_NUMBER  -5

//! Contains information to send/recieve over a serial connection.
struct SerialData {
	uint8_t kind; //!< The type of message being received [0-4]
	int32_t number; //!< The cone number
	int32_t speed_cmPs; //!< The speed the bike is moving at (cm/s)
	int32_t angle_deg; //!< Angle (deg) of the bike
	int32_t bearing_deg; //!< Bearing (deg) of the camera
	int32_t posE_cm; //!< Position (cm) on the E-W axis
	int32_t posN_cm; //!< Position (cm) on the N-S axis
	int32_t probability; //!< Probability that the value is a cone
	
	void clear(void); //!< Set the values to the defaults
	bool write(HardwareSerial * /**< Connection to write to */); //!< Write to a serial connection
};

//! Contains internal state for the SerialData parser.
struct ParseState {
	HardwareSerial *dev; //!< Connection to read from
	SerialData *dt; //!< SerialData to write to
	
	int8_t update(void); //!< Update the state of the parser based on a single character
private:
	uint8_t state = 0; //!< Internal state variable
};

//! Helper function to avoid API breakage (TODO: update the rest of the codebase to the new API and remove this)
inline void readSerial(HardwareSerial *hs /**< The connection to read from */,
                       SerialData *dt /**< The SerialData to read from */) {
	ParseState ps;
	ps.dev = hs;
	ps.dt = dt;
	while (ps.update() != PSE_SUCCESS);
}
