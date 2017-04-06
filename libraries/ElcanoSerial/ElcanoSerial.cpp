#include "ElcanoSerial.h"
#include "Arduino.h"
#include <FastCRC.h>
FastCRC8 CRC8;


// Elcano_Serial.h
// By Dylan Katz
// 
// Manages our protocal for robust communication of SerialData over serial
// connections. Complete documentation and usage is on the wiki.
namespace elcano {

ParseStateError ParseState::update(void) {
  
  numStarted = false;
  int c;
start:
  input->read();
  if (c == -1) return ParseStateError::unavailable;
  if (c == ' ' || c == '\t' || c == '\0' || c == '\r') goto start;
  
  switch(state) {
  case 0:
    // During this state, we begin the processing of a new SerialData packet
    // and we must receive {D,S,G,X} to state which type of packet we are
    // working with. As an optimization, the user may set only a specific type
    // of messages to capture, with all other types result in the stream being
    // directly fed into the `output' serial port. Set the `capture' variable
    // to do this, and use `|' to let it capture more than one type at once.
    dt->clear();
    switch(c) {
#define STATE(CHR, TYPE)             \
    case CHR:                        \
      if (capture & MsgType::TYPE) { \
        dt->kind = MsgType::TYPE;    \
									 \
        state = 1;                   \
      } else {                       \
        output->print(CHR);          \
        state = 50;                  \
      }                              \
      break;
STATE('D', drive)
STATE('S', sensor)
STATE('G', goal)
STATE('X', seg)
#undef STATE
    default:
	  mostRecent = c;
      return ParseStateError::bad_type;
    }
    goto start;
  case 1:
    // During this state, we need to find '{' if we are reading the value for
    // an attribute, or '\n' if we are done with the packet
    switch(c) {
    case '\n': state = 0; return dt->verify() ? ParseStateError::success : ParseStateError::inval_comb;
    case '{' : state = 2; goto start;
    default  : state = 0; return ParseStateError::bad_lcurly;
    }
  case 2:
    // During this state, we begin reading an attribute of the SerialData and
    // we must recieve {n,s,a,b,r,p} to state _which_ attribute
    switch(c) {
    case 'n': state = 3; goto start;
    case 's': state = 4; goto start;
    case 'a': state = 5; goto start;
    case 'b': state = 6; goto start;
    case 'r': state = 7; goto start;
    case 'p': state = 8; goto start;
    default : state = 0; return ParseStateError::bad_attrib;
    }
#define STATES(SS, PS, NS, TERM, HOME, VAR) \	
  case SS:                                  \
    dt->VAR = 0;                            \
    if (c == '-') {                         \
      state = NS;                           \
      goto start;                           \
    }                                       \
    state = PS;                             \
  case PS:									\
    if (c >= '0' && c <= '9') {             \
	  if(c != '0' || numStarted){ 			\
	      dt->VAR *= 10;                    \ 
	      dt->VAR += c - '0';               \
	      goto start;                       \
	    } else if (c == TERM) {             \
	      state = HOME;                     \
	      goto start;                       \
	  } else {                              \
      	  state = 0;                        \
	      return ParseStateError::bad_number;\
	  }  									\
    }     							        \
	  else if(!numStarted && c != '0')      \
	  {										\
		numStarted = true;					\
		dt->VAR *= 10;                      \
		dt->VAR += c - '0';                 \
		goto start;                         \
	  } else if (c == TERM) {               \
		state = HOME;                       \
		goto start;                         \
	  } else {                              \
		state = 0;                          \
		return ParseStateError::bad_number; \
	  }										\
			                                \
  case NS:                                  \
  if (c >= '0' && c <= '9') {               \
	if(c != '0' || numStarted){ 		    \
		dt->VAR *= 10;                      \ 
		dt->VAR -= c - '0';                 \
		goto start;                         \
	  } else if (c == TERM) {               \
		state = HOME;                       \
		goto start;                         \
	  } else {                              \
		state = 0;                          \
		return ParseStateError::bad_number; \
	} 										\
    }      							    	\
	else if(!numStarted && c != '0')      	\
	{										\
	  numStarted = true;					\
	  dt->VAR *= 10;                        \
	  dt->VAR -= c - '0';                 	\
	  goto start;                         	\
	} else if (c == TERM) {               	\
	  state = HOME;                       	\
	  goto start;                         	\
	} else {                              	\
	  state = 0;                          	\
	  return ParseStateError::bad_number; 	\
	}											
STATES(3, 13, 23, '}', 1, number)
STATES(4, 14, 24, '}', 1, speed_cmPs)
STATES(5, 15, 25, '}', 1, angle_mDeg)
STATES(6, 16, 26, '}', 1, bearing_deg)
STATES(7, 17, 27, '}', 1, probability)
STATES(8, 18, 28, ',', 9, posE_cm)
STATES(9, 19, 29, '}', 1, posN_cm)
#undef STATES
  case 50:
    output->print((char)c);
    if (c == '\n') {
      state = 0;
      return ParseStateError::passthru;
    }
    goto start;
  }
}


bool SerialData::write(HardwareSerial *dev) {
  switch (kind) {
  case MsgType::drive:  
	dev->print("D");
	outBuffer[outSize++] = "D";
	break;
  case MsgType::sensor:
	dev->print("S"); 
	outBuffer[outSize++] = "S";
	break;
  case MsgType::goal:   
	dev->print("G"); 
	outBuffer[outSize++] = "G";
	break;
  case MsgType::seg:    
	dev->print("X"); 
	outBuffer[outSize++] = "X";
	break;
  default:              
	return false;
  }
  
  
  if (number != NaN && (kind == MsgType::goal || kind == MsgType::seg)) {
    dev->print("{n ");
	outBuffer[outSize++] = '{';
	outBuffer[outSize++] = 'n';
	outBuffer[outSize++] = ' ';
    
	dev->print(number);
	String num = String(number);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
    dev->print("}");
	outBuffer[outSize++] = '}';
  }
  if (speed_cmPs != NaN && kind != MsgType::goal) {
    dev->print("{s ");
	
	outBuffer[outSize++] = '{';
	outBuffer[outSize++] = 's';
	outBuffer[outSize++] = ' ';
	
    dev->print(speed_cmPs);
	String num = String(speed_cmPs);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
    dev->print("}");
	outBuffer[outSize++] = '}';
  }
  if (angle_mDeg != NaN && (kind == MsgType::drive || kind == MsgType::sensor)) {
    dev->print("{a ");
	
	outBuffer[outSize++] = '{';
	outBuffer[outSize++] = 'a';
	outBuffer[outSize++] = ' ';
	
    dev->print(angle_mDeg);
	String num = String(angle_mDeg);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
	
    dev->print("}");
	outBuffer[outSize++] = '}';
  }
  if (bearing_deg != NaN && kind != MsgType::drive) {
    dev->print("{b ");
	
	outBuffer[outSize++] = '{';
	outBuffer[outSize++] = 'b';
	outBuffer[outSize++] = ' ';
	
    dev->print(bearing_deg);
	String num = String(bearing_deg);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
	
	
    dev->print("}");
	outBuffer[outSize++] = '}';
	
  }
  if (posE_cm != NaN && posN_cm != NaN && kind != MsgType::drive) {
    dev->print("{p ");
	
	outBuffer[outSize++] = '{';
	outBuffer[outSize++] = 'p';
	outBuffer[outSize++] = ' ';
	
    dev->print(posE_cm);
	
	String num = String(posE_cm);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
	
    dev->print(",");
	outBuffer[outSize++] = ',';
	
    dev->print(posN_cm);
	
	num = String(posN_cm);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
	
    dev->print("}");
	outBuffer[outSize++] = '}';
  }
  
  if (probability != NaN && kind == MsgType::goal) {
    dev->print("{r ");
	
	outBuffer[outSize++] = '{';
	outBuffer[outSize++] = 'r';
	outBuffer[outSize++] = ' ';
	
    dev->print(probability);
	
	String num = String(probability);
	for(int i = 0; i < num.length(); i++)
	{
		outBuffer[outSize++] = num.charAt(i);
	}
	
    dev->print("}");
	outBuffer[outSize++] = '}';
	
  }
  dev->print(String(CRC8.smbus(outBuffer, outSize)));
  dev->print("\n");
  // Serial.println(String(CRC8.smbus(outBuffer, outSize)));
  return true;
}

void SerialData::clear(void) {
  kind        = MsgType::none;
  number      = NaN;
  speed_cmPs  = NaN;
  angle_mDeg   = NaN;
  bearing_deg = NaN;
  posE_cm     = NaN;
  posN_cm     = NaN;
  probability = NaN;
}

bool SerialData::verify() {
  switch (kind) {
  case MsgType::drive:
    if (speed_cmPs  == NaN) return false;
    if (angle_mDeg   == NaN) return false;
    break;
  case MsgType::sensor:
    if (speed_cmPs  == NaN) return false;
    if (posE_cm     == NaN) return false;
    if (posN_cm     == NaN) return false;
    if (bearing_deg == NaN) return false;
    if (angle_mDeg   == NaN) return false;
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
