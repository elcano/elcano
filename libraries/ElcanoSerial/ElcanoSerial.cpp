#include "ElcanoSerial.h"

// Elcano_Serial.h
// By Dylan Katz
//
// Manages our protocol for robust communication of SerialData over serial
// connections. Complete documentation and usage is on the wiki.
namespace elcano {

ParseStateError ParseState::update(void) {
  int c;
  int i = 0;
  char buffer[MAX_MSG_SIZE];
  
  // Because no attribute type can be used for more than 1 flag, these
  // variables keep track of whether the given attribute type has been used
  // in a flag so far.
  bool numberWasUsed = false;
  bool speedWasUsed = false;
  bool angleWasUsed = false;
  bool obstacleWasUsed = false;
  bool bearingWasUsed = false;
  bool EposWasUsed = false;
  bool NposWasUsed = false;
  bool probabilityWasUsed = false;
  
  // Checksum variables
  uint8_t checksum = 0; // checksum computed from message string
  uint8_t msgChecksum = -1; // checksum included with the message
  
start:
  if (i == MAX_MSG_SIZE) {
    state = 0;
    return ParseStateError::long_msg;
  }
  c = input->read();
  buffer[i] = c;
  i++;
  if (c == -1) {
    state = 0;
    return ParseStateError::unavailable;
  }
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
      state = 0;
      return ParseStateError::bad_type;
    }
    goto start;
  case 1:
    // During this state, we need to find '{' if we are reading the value for
    // an attribute, or '\n' if we are done with the packet, or '-' or '1'-'9'
    // if we are reading the checksum
    switch(c) {
    //case '\n': state = 0; return dt->verify() ? ParseStateError::success : ParseStateError::inval_comb;
    case '\n': state =  0; return ParseStateError::success;
    case '{' : state =  2; goto start;
    case '1' :
    case '2' :
    case '3' :
    case '4' :
    case '5' :
    case '6' :
    case '7' :
    case '8' :
    case '9' : state = 40; checksum = CRC8.smbus(buffer, i - 1); msgChecksum = c - '0'; goto start;
    case '-' : state = 41; checksum = CRC8.smbus(buffer, i - 1); goto start;
    default  : state =  0; return ParseStateError::bad_lcurly;
    }
  case 2:
    // During this state, we begin reading an attribute of the SerialData and
    // we must recieve {n,s,a,o,b,r,p} to state _which_ attribute
    switch(c) {
    case 'n': state = 3; goto start;
    case 's': state = 4; goto start;
    case 'a': state = 5; goto start;
	case 'o': state = 6; goto start;
    case 'b': state = 7; goto start;
    case 'r': state = 8; goto start;
    case 'p': state = 9; goto start;
    default : state = 0; return ParseStateError::bad_attrib;
    }
#define STATES(SS, PS, NS, TERM, HOME, VAR, BOOL) \
  case SS:                                  \
    if (BOOL) {                             \
      state = 0;                            \
      return ParseStateError::bad_attrib;   \
    }                                       \
    dt->VAR = 0;                            \
    if (c == '-') {                         \
      state = NS;                           \
      goto start;                           \
    }                                       \
    state = PS;                             \
  case PS:                                  \
    if (c >= '0' && c <= '9') {             \
      dt->VAR *= 10;                        \
      dt->VAR += c - '0';                   \
      goto start;                           \
    } else if (c == TERM) {                 \
      BOOL = true;                          \
      state = HOME;                         \
      goto start;                           \
    } else {                                \
      state = 0;                            \
      return ParseStateError::bad_number;   \
    }                                       \
  case NS:                                  \
    if (c >= '0' && c <= '9') {             \
      dt->VAR *= 10;                        \
      dt->VAR -= c - '0';                   \
      goto start;                           \
    } else if (c == TERM) {                 \
      BOOL = true;                          \
      state = HOME;                         \
      goto start;                           \
    } else {                                \
      state = 0;                            \
      return ParseStateError::bad_number;   \
    }
STATES( 3, 13, 23, '}',  1, number, numberWasUsed)
STATES( 4, 14, 24, '}',  1, speed_cmPs, speedWasUsed)
STATES( 5, 15, 25, '}',  1, angle_mDeg, angleWasUsed)
STATES( 6, 16, 26, '}',  1, obstacle_mm, obstacleWasUsed)
STATES( 7, 17, 27, '}',  1, bearing_deg, bearingWasUsed)
STATES( 8, 18, 28, '}',  1, probability, probabilityWasUsed)
STATES( 9, 19, 29, ',', 10, posE_cm, EposWasUsed)
STATES(10, 20, 30, '}',  1, posN_cm, NposWasUsed)
#undef STATES
  case 40: // positive checksum
    if (c == '\n') {
      if (checksum != msgChecksum) {
        state = 0;
        return ParseStateError::bad_checksum;
        } else {
        state = 0;
        return ParseStateError::success;
        }
    } else if (c < '0' || c > '9') {
      return ParseStateError::bad_checksum;
    } else {
      msgChecksum *= 10;
      msgChecksum += c - '0';
      goto start;
    }
  case 41: // negative checksum
    if (c == '\n') {
      if (checksum != msgChecksum) {
        state = 0;
        return ParseStateError::bad_checksum;
      } else {
        state = 0;
        return ParseStateError::success;
      }
    } else if (c < '0' || c > '9') {
      return ParseStateError::bad_checksum;
    } else {
      msgChecksum *= 10;
      msgChecksum -= c - '0';
      goto start;
    }
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
  
  char buffer[MAX_MSG_SIZE];
  int i = 0;
  String string;
  
  switch (kind) {
  case MsgType::drive:  buffer[i++] = 'D'; break;
  case MsgType::sensor: buffer[i++] = 'S'; break;
  case MsgType::goal:   buffer[i++] = 'G'; break;
  case MsgType::seg:    buffer[i++] = 'X'; break;
  default:              return false;
  }
  // At this point the value of i will be 1. No bounds check necessary
  if (number != NaN && (kind == MsgType::goal || kind == MsgType::seg)) {
    buffer[i++] = '{';
    buffer[i++] = 'n';
    buffer[i++] = ' ';
    string = String(number);
    int strLen = string.length();
    for (int j = 0; j < strLen; j++) {
      buffer[i++] = string.charAt(j);
    }
    buffer[i++] = '}';
  }
  // At this point the max value of i is 16. No bounds check necessary
  if (speed_cmPs != NaN && kind != MsgType::goal) {
    buffer[i++] = '{';
    buffer[i++] = 's';
    buffer[i++] = ' ';
    string = String(speed_cmPs);
    int strLen = string.length();
    for (int j = 0; j < strLen; j++) {
      buffer[i++] = string.charAt(j);
    }
    buffer[i++] = '}';
  }
  // At this point the max value of i is 31. No bounds check necessary
  if (angle_mDeg != NaN && (kind == MsgType::drive || kind == MsgType::sensor)) {
    buffer[i++] = '{';
    buffer[i++] = 'a';
    buffer[i++] = ' ';
    string = String(angle_mDeg);
    int strLen = string.length();
    for (int j = 0; j < strLen; j++) {
      buffer[i++] = string.charAt(j);
    }
    buffer[i++] = '}';
  }
  // At this point the max value of i is 46. No bounds check necessary
  if (obstacle_mm != NaN && kind == MsgType::sensor) {
    buffer[i++] = '{';
    buffer[i++] = 'o';
    buffer[i++] = ' ';
    string = String(obstacle_mm);
    int strLen = string.length();
    for (int j = 0; j < strLen; j++) {
      buffer[i++] = string.charAt(j);
    }
    buffer[i++] = '}';
  }
  // At this point the max value of i is 61. No bounds check necessary
  if (bearing_deg != NaN && kind != MsgType::drive) {
    buffer[i++] = '{';
    buffer[i++] = 'b';
    buffer[i++] = ' ';
    string = String(bearing_deg);
    int strLen = string.length();
    for (int j = 0; j < strLen; j++) {
      buffer[i++] = string.charAt(j);
    }
    buffer[i++] = '}';
  }
  // At this point the max value of i is 91. A bounds check is necessary
  if (probability != NaN && kind == MsgType::goal) {
    if (i >= MAX_MSG_SIZE - 6)
      return false;
    buffer[i++] = '{';
    buffer[i++] = 'r';
    buffer[i++] = ' ';
    string = String(probability);
    int strLen = string.length();
    if (i >= MAX_MSG_SIZE - strLen - 2)
      return false;
    for (int j = 0; j < strLen; j++)
      buffer[i++] = string.charAt(j);
    buffer[i++] = '}';
  }
  // At this point the max value of i is 76. A bounds check is necessary
  if (posE_cm != NaN && posN_cm != NaN && kind != MsgType::drive) {
    if (i >= MAX_MSG_SIZE - 8)
      return false;
    buffer[i++] = '{';
    buffer[i++] = 'p';
    buffer[i++] = ' ';
    string = String(posE_cm);
    int strLen = string.length();
    if (i >= MAX_MSG_SIZE - strLen - 4)
      return false;
    for (int j = 0; j < strLen; j++)
      buffer[i++] = string.charAt(j);
    buffer[i++] = ',';
    string = String(posN_cm);
    strLen = string.length();
    if (i >= MAX_MSG_SIZE - strLen - 2)
      return false;
    for (int j = 0; j < strLen; j++)
      buffer[i++] = string.charAt(j);
    buffer[i++] = '}';
  }
  // At this point the max value of i is 106. A bounds check is necessary
  FastCRC8 CRC8;
  int checksum = CRC8.smbus(buffer, i);
  String chksumStr = String(checksum);
  int chksumLen = chksumStr.length();
  if (i == MAX_MSG_SIZE - chksumLen - 1)
    return false;
  for (int j = 0; j < chksumLen; j++)
    buffer[i++] = chksumStr.charAt(j);
  buffer[i++] = '\n';
  
  // Write buffer to device
  for (int j = 0; j < i; j++) {
    dev->print(buffer[j]);
  }
  return true;
}

void SerialData::clear(void) {
  kind        = MsgType::none;
  number      = NaN;
  speed_cmPs  = NaN;
  angle_mDeg  = NaN;
  obstacle_mm = NaN;
  bearing_deg = NaN;
  posE_cm     = NaN;
  posN_cm     = NaN;
  probability = NaN;
}

/*
bool SerialData::verify(void) {
  return true;
  switch (kind) {
  case MsgType::drive:
    if (speed_cmPs  == NaN) return false;
    if (angle_mDeg  == NaN) return false;
    break;
  case MsgType::sensor:
    if (speed_cmPs  == NaN) return false;
    if (posE_cm     == NaN) return false;
    if (posN_cm     == NaN) return false;
	if (obstacle_mm == NaN) return false;
    if (bearing_deg == NaN) return false;
    if (angle_mDeg  == NaN) return false;
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
*/

} // namespace elcano
