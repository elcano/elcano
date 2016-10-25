#include "Elcano_Serial.h"

/*
** Elcano_Serial.cpp
** By Dylan Katz
**
** Manages our protocal for robust communication of SerialData over serial connections
*/

namespace elcano {

ParseStateError ParseState::update(void) {
	int c = dev->read();
	if (c == -1) return ParseStateError::unavailable;
	if (c == ' ' || c == '\t' || c == '\0' || c == '\r') return ParseStateError::incomplete;
	switch(state) {
	case 0:
		// During this state, we begin the processing of a new SerialData
		// packet and we must receive {D,S,G,X} to state which type of packet
		// we are working with
		dt->clear();
		switch(c) {
		case 'D': dt->kind = MsgType::drive;  break;
		case 'S': dt->kind = MsgType::sensor; break;
		case 'G': dt->kind = MsgType::goal;   break;
		case 'X': dt->kind = MsgType::seg;    break;
		default : return ParseStateError::bad_type;
		}
		state = 1;
		return ParseStateError::incomplete;
	case 1:
		// During this state, we need to find '{' if we are reading the value
		// for an attribute, or '\n' if we are done with the packet
		switch(c) {
		case '\n': state = 0; return dt->verify() ? ParseStateError::success : ParseStateError::inval_comb;
		case '{' : state = 2; return ParseStateError::incomplete;
		default  : return ParseStateError::bad_lcurly;
		}
	case 2:
		// During this state, we begin reading an attribute of the SerialData
		// and we must recieve {n,s,a,b,r,p} to state _which_ attribute
		switch(c) {
		case 'n': state = 3; dt->number      = 0; return ParseStateError::incomplete;
		case 's': state = 4; dt->speed_cmPs  = 0; return ParseStateError::incomplete;
		case 'a': state = 5; dt->angle_deg   = 0; return ParseStateError::incomplete;
		case 'b': state = 6; dt->bearing_deg = 0; return ParseStateError::incomplete;
		case 'r': state = 7; dt->probability = 0; return ParseStateError::incomplete;
		case 'p': state = 8; dt->posE_cm = 0; dt->posN_cm = 0; return ParseStateError::incomplete;
		default : return ParseStateError::bad_attrib;
		}
#define STATES(SS, PS, NS, TERM, HOME, VAR)   \
    case SS:                                   \
        if (c == '-') {                         \
            state = NS;                          \
            return ParseStateError::incomplete; } \
        state = PS;                               \
    case PS:                                      \
        if (c >= '0' && c <= '9') {               \
            dt->VAR *= 10;                        \
            dt->VAR += c - '0';                   \
            return ParseStateError::incomplete; } \
        else if (c == TERM) {                     \
            state = HOME;                         \
            return ParseStateError::incomplete; } \
        else                                      \
            return ParseStateError::bad_number;   \
    case NS:                                      \
        if (c >= '0' && c <= '9') {               \
            dt->VAR *= 10;                        \
            dt->VAR -= c - '0';                   \
            return ParseStateError::incomplete; } \
        else if (c == TERM) {                     \
            state = HOME;                         \
            return ParseStateError::incomplete; } \
        else                                      \
            return ParseStateError::bad_number;
STATES(3, 13, 23, '}', 1, number)
STATES(4, 14, 24, '}', 1, speed_cmPs)
STATES(5, 15, 25, '}', 1, angle_deg)
STATES(6, 16, 26, '}', 1, bearing_deg)
STATES(7, 17, 27, '}', 1, probability)
STATES(8, 18, 28, ',', 9, posE_cm)
STATES(9, 19, 29, '}', 1, posN_cm)
#undef STATES
	}
}

bool SerialData::write(HardwareSerial *dev) {
	switch (kind) {
	case MsgType::drive:  dev->print("D"); break;
	case MsgType::sensor: dev->print("S"); break;
	case MsgType::goal:   dev->print("G"); break;
	case MsgType::seg:    dev->print("X"); break;
	default:         return false;
	}
	if (number != NaN && (kind == MsgType::goal || kind == MsgType::seg)) {
		dev->print("{n ");
		dev->print(number);
		dev->print("}");
	}
	if (speed_cmPs != NaN && kind != MsgType::goal) {
		dev->print("{s ");
		dev->print(speed_cmPs);
		dev->print("}");
	}
	if (angle_deg != NaN && (kind == MsgType::drive || kind == MsgType::sensor)) {
		dev->print("{a ");
		dev->print(angle_deg);
		dev->print("}");
	}
	if (bearing_deg != NaN && kind != MsgType::drive) {
		dev->print("{b ");
		dev->print(bearing_deg);
		dev->print("}");
	}
	if (posE_cm != NaN && posN_cm != NaN && kind != MsgType::drive) {
		dev->print("{p ");
		dev->print(posE_cm);
		dev->print(",");
		dev->print(posN_cm);
		dev->print("}");
	}
	if (probability != NaN && kind == MsgType::goal) {
		dev->print("{r ");
		dev->print(probability);
		dev->print("}");
	}
	dev->print("\n");
	return true;
}

void SerialData::clear(void) {
    kind        = MsgType::none;
    number      = NaN;
    speed_cmPs  = NaN;
    angle_deg   = NaN;
    bearing_deg = NaN;
    posE_cm     = NaN;
    posN_cm     = NaN;
    probability = NaN;
}

bool SerialData::verify(void) {
	switch (kind) {
	case MsgType::drive:
		if (speed_cmPs  == NaN) return false;
		if (angle_deg   == NaN) return false;
		break;
	case MsgType::sensor:
		if (speed_cmPs  == NaN) return false;
		if (posE_cm     == NaN) return false;
		if (posN_cm     == NaN) return false;
		if (bearing_deg == NaN) return false;
		if (angle_deg   == NaN) return false;
		break;
	case MsgType::goal:
		if (number      == NaN) return false;
		if (posE_cm     == NaN) return false;
		if (posN_cm     == NaN) return false;
		if (bearing_deg == NaN) return false;
		break;
	case MsgType::seg:
		if (number      == NaN) return false;
		if (posE_cm     == NaN) return false;
		if (posN_cm     == NaN) return false;
		if (bearing_deg == NaN) return false;
		if (speed_cmPs  == NaN) return false;
		break;
	default:
		return false;
	}
	return true;
}

} // namespace elcano
