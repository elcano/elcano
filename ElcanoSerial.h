#pragma once

#include <HardwareSerial.h>

// Elcano_Serial.h
// By Dylan Katz
//
// Manages our protocal for robust communication of SerialData over serial
// connections. Complete documentation and usage is on the wiki.
namespace elcano {

//! The number representing when no data is sent
const int32_t NaN = 0x7FFFFFFF;

//! The baudrate to be used universally across the system
const int32_t baudrate = 74800;

//! The maximum message size
const int32_t MAX_MSG_SIZE = 64;

//! The min continuous occurrence of the unavailable byte (255) that is
//  interprted as the absence of any message on the wire.
const int32_t MIN_UNAVAILABLE_CT = 0;

//! The different possible types of SerialData packets
enum class MsgType : int8_t {
  none   = 1 << 0, //!< Empty type
  drive  = 1 << 1, //!< Drive to a position
  sensor = 1 << 2, //!< Info from the sensor
  goal   = 1 << 3, //!< Position of a goal
  seg    = 1 << 4  //!< Part of the navigation path
};

//! Allow bitfield operators for MsgType
inline constexpr bool operator&(MsgType a, MsgType b) {
  return static_cast<bool>(static_cast<int8_t>(a) & static_cast<int8_t>(b));
}

inline constexpr MsgType operator|(MsgType a, MsgType b) {
  return static_cast<MsgType>(static_cast<int8_t>(a) | static_cast<int8_t>(b));
}

//! Contains information to send/recieve over a serial connection.
struct SerialData {
  MsgType kind;        //!< The type of message being received [0-4]
  int32_t number;      //!< The number of the unit
  int32_t speed_cmPs;  //!< The speed the bike is moving at (cm/s)
  int32_t angle_mDeg;  //!< Angle (deg) of the bike
  int32_t obstacle_mm; //!< Distance to obstacle in mm
  int32_t probability; //!< Probability that the value is a cone
  int32_t bearing_deg; //!< Bearing (deg) of the camera
  int32_t posE_cm;     //!< Position (cm) on the E-W axis
  int32_t posN_cm;     //!< Position (cm) on the N-S axis

  void clear(void);  //!< Set the values to the defaults
  bool write(HardwareSerial * /**< Connection to write to */); //!< Write to a serial connection
  bool verify(void); //!< Check that the types match the values
};

//! The different possible results of the ParseState::update method
enum class ParseStateError : int8_t {
  success      = 0, //!< Complete package
  inval_comb   = 1, //!< Complete package, failed validation
  unavailable  = 2, //!< Couldn't read a complete packet from the device at this time
  bad_attrib   = 5, //!< Syntax error: attributes should be [nsaobrp] and only used once
  bad_number   = 6, //!< Syntax error: number had a bad symbol
  passthru     = 7, //!< An uncaptured message has been passed through
  bad_checksum = 8, //!< Error in checksum
  long_msg     = 9 //!< Message is too long
};

//! Contains internal state for the SerialData parser.
struct ParseState {
  SerialData *dt;         //!< SerialData to store content in
  HardwareSerial *input;  //!< Connection to read from
  HardwareSerial *output; //!< Connection to write to
  MsgType capture;        //!< MsgType(s) to capture

  //!< Update the state of the parser based on a single character
  ParseStateError update(unsigned int max_passthru_ct = 50);
};

} // namespace elcano
