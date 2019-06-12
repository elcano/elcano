#include "ElcanoSerial.h"
#include <FastCRC.h>

#define TERMINAL_CHAR '\n'

// Elcano_Serial.h
// By Dylan Katz
//
// Manages our protocol for robust communication of SerialData over serial
// connections. Complete documentation and usage is on the wiki.
namespace elcano {

static FastCRC8 CRC8;

static bool is_unavailable_byte(const uint8_t & c) {
  return c == 255;
}

static MsgType parse_msg_type(const uint8_t & c) {
  if (c == 'D') return MsgType::drive;
  if (c == 'S') return MsgType::sensor;
  if (c == 'G') return MsgType::goal;
  if (c == 'X') return MsgType::seg;
  return MsgType::none;
}

static int32_t* get_datum_ptr(const SerialData & dt, const uint8_t & datum_type) {
  if (datum_type == 'n') return &(dt.number);
  if (datum_type == 's') return &(dt.speed_cmPs);
  if (datum_type == 'a') return &(dt.angle_mDeg);
  if (datum_type == 'o') return &(dt.obstacle_mm);
  if (datum_type == 'b') return &(dt.bearing_deg);
  if (datum_type == 'r') return &(dt.probability);
  if (datum_type == 'p') return &(dt.posE_cm);
  if (datum_type == 'q') return &(dt.posN_cm);
  return nullptr;
}

ParseStateError ParseState::update(unsigned int max_passthru_ct) {
  dt->clear();

  uint8_t c; // The temp var for the newest byte.

  uint8_t buffer[MAX_MSG_SIZE];
  size_t buffer_idx = 0;

  // 1) Discard all bytes until receiving the head of a new packet.
  //    Do this b/c, in general, the node has just finished writing a new packet,
  //    and during this time, any incoming packet would already have been
  //    partially discarded.
  // 2) For all new packets, pass through all non-captured packets.
  // 1&2) While waiting for a new packet, detect lack of packets on the wire,
  //      so that this method can exit and give the single thread a chance to
  //      send a new packet.
  // Note, it is possible for a correctly formatted packet without a recipient
  // to circulate on the wire indefinitely.
  unsigned int passthru_ct = 0;
  while(true) {
    c = input->read();

    if (is_unavailable_byte(c)) {
      // Ideally we should use a threshold to recognize a few contiguous
      // `unavailable` bytes, but an attempt to do so resulted in either hanging
      // or perpetual `unavailable` result; further investigation is welcome.
      // If we use a threshold, it may be good for every node to add a jitter,
      // so that every node waits a different amount to verify the `unavailable`
      // condition, in order to prevent nodes from continuously sending new
      // packets at the same frequency, only to have them all discarded.
      return ParseStateError::unavailable;
    } else {
      if (c == TERMINAL_CHAR && passthru_ct > max_passthru_ct) {
        output->print(c);
        return ParseStateError::passthru;
      }

      MsgType msg_type = parse_msg_type(c);

      if (msg_type == MsgType::none) {
        // Not the head of a new packet.
        // Either discard or pass thru.
        if (passthru_ct > 0) {
          output->print(c);
        }
      } else if (msg_type & capture) {
        // Received the head of a captured packet.
        buffer[buffer_idx++] = c;
        dt->kind = msg_type;
        // Now go parse the rest of the packet.
        break; 
      } else {
        // Received the head of a non-captured packet.
        output->print(c);
        passthru_ct++;
      }
    }
  }

  auto get_next_byte = [this, &buffer, &buffer_idx](uint8_t & c) {
    do {
      c = this->input->read();
    } while (is_unavailable_byte(c));
    buffer[buffer_idx++] = c;
  };

  auto abort = [&get_next_byte, &buffer_idx, &MAX_MSG_SIZE](uint8_t & c) {
    do {
      get_next_byte(c);
    } while (c != TERMINAL_CHAR && buffer_idx < MAX_MSG_SIZE);
  };

  // Unlike Serial.parseInt():
  // - allows an initial digit to be already read
  // - does not handle negative sign
  // - parses an int, not a long
  auto parseInt = [&get_next_byte, &c, &buffer_idx, &MAX_MSG_SIZE](int32_t* datum_ptr, const uint8_t & stop_char) {
    do {
      get_next_byte(c);
      if (c >= '0' && c <= '9') {
        *datum_ptr = *datum_ptr * 10 + c - '0';
      } else {
        return c == stop_char;
      }
    } while (buffer_idx < MAX_MSG_SIZE);
    return false;
  };

  // 3) Either parse a datum or validate the checksum, as determined by the next byte. Repeat.
  while (buffer_idx < MAX_MSG_SIZE) {
    get_next_byte(c);
    if (c == '{') {
      // Then, read until '}' and parse a datum.

      get_next_byte(c);
      int32_t* datum_ptr = get_datum_ptr(*dt, c);
      if (datum_ptr == nullptr) {
        abort(c);
        return ParseStateError::bad_attrib;
      }

      get_next_byte(c);
      bool is_negative;
      if (c == '-') {
        *datum_ptr = 0;
        is_negative = true;
      } else {
        *datum_ptr = c - '0';
        is_negative = false;
      }

      bool parse_success = parseInt(datum_ptr, '}');
      if (! parse_success) {
        abort(c);
        return ParseStateError::bad_number;
      }

      if (is_negative) {
        *datum_ptr = *datum_ptr * -1;
      }

    } else if (c >= '0' && c <= '9') {
      // Then, read until TERMINAL_CHAR and validate the checksum.

      size_t checksum_begin_idx = buffer_idx - 1;

      int32_t parsed_checksum = c - '0';
      bool parse_success = parseInt(&parsed_checksum, TERMINAL_CHAR);
      if (! parse_success) {
        abort(c);
        return ParseStateError::bad_number;
      }

      uint8_t calculated_checksum = CRC8.smbus(buffer, checksum_begin_idx);

      if (calculated_checksum != static_cast<uint8_t>(parsed_checksum)) {
        return ParseStateError::bad_checksum;
      } else if (! dt->verify()) {
        return ParseStateError::inval_comb;
      } else {
        return ParseStateError::success;
      }
    } else {
      // Then, the packet did not follow the format of e.g.
      // `A{b1234}{c5678}999`
      return ParseStateError::bad_number;
    }
  }

  return ParseStateError::long_msg;
}

bool SerialData::write(HardwareSerial *dev) {
  char buffer[MAX_MSG_SIZE];
  size_t i = 0;
  bool success;
  
  switch (kind) {
    case MsgType::drive:  buffer[i++] = 'D'; break;
    case MsgType::sensor: buffer[i++] = 'S'; break;
    case MsgType::goal:   buffer[i++] = 'G'; break;
    case MsgType::seg:    buffer[i++] = 'X'; break;
    default:              return false;
  }

  auto append_datum = [&buffer, &i](const char & datum_type, const int32_t & datum) {
    String datum_str = String(datum);
    size_t strLen = datum_str.length();

    // 3 for '{', datum_type, '}'.
    if (i >= MAX_MSG_SIZE - 3 - strLen) {
      return false;
    }

    buffer[i++] = '{';
    buffer[i++] = datum_type;
    for (size_t j = 0; j < strLen; j++) {
      buffer[i++] = datum_str.charAt(j);
    }
    buffer[i++] = '}';

    return true;
  };

  // At this point the value of i will be 1. No bounds check necessary
  if (number != NaN && (kind == MsgType::goal || kind == MsgType::seg || kind == MsgType::drive)) {
    success = append_datum('n', number);
    if (! success) return false;
  }
  // At this point the max value of i is 16. No bounds check necessary
  if (speed_cmPs != NaN && kind != MsgType::goal) {
    success = append_datum('s', speed_cmPs);
    if (! success) return false;
  }
  // At this point the max value of i is 31. No bounds check necessary
  if (angle_mDeg != NaN && (kind == MsgType::drive || kind == MsgType::sensor)) {
    success = append_datum('a', angle_mDeg);
    if (! success) return false;
  }
  // At this point the max value of i is 46. No bounds check necessary
  if (obstacle_mm != NaN && kind == MsgType::sensor) {
    success = append_datum('o', obstacle_mm);
    if (! success) return false;
  }
  // At this point the max value of i is 61. No bounds check necessary
  if (bearing_deg != NaN && kind != MsgType::drive) {
    success = append_datum('b', bearing_deg);
    if (! success) return false;
  }
  // At this point the max value of i is 91. A bounds check is necessary
  // if (i >= MAX_MSG_SIZE) return false;
  if (probability != NaN && kind == MsgType::goal) {
    success = append_datum('r', probability);
    if (! success) return false;
  }
  // At this point the max value of i is 76. A bounds check is necessary
  // if (i >= MAX_MSG_SIZE) return false;
  if (posE_cm != NaN && posN_cm != NaN && kind != MsgType::drive) {
    success = append_datum('p', posE_cm);
    if (! success) return false;
    success = append_datum('q', posN_cm);
    if (! success) return false;
  }

  // At this point the max value of i is 108. A bounds check is necessary
  // if (i >= MAX_MSG_SIZE) return false;

  FastCRC8 CRC8;
  int checksum = CRC8.smbus((const uint8_t*)buffer, i);
  String chksumStr = String(checksum);
  int chksumLen = chksumStr.length();
  if (i == MAX_MSG_SIZE - chksumLen - 1) // 1 for TERMINAL_CHAR
    return false;
  for (int j = 0; j < chksumLen; j++)
    buffer[i++] = chksumStr.charAt(j);

  buffer[i++] = TERMINAL_CHAR;

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

} // namespace elcano
