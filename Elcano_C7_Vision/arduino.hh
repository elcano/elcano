#pragma once

#include <serial/serial.h>

/*!
 * \file arduino.hh
 * \brief Send/receive information to an external device
 */

namespace elcano
{
	//! The Null value set when a value is undefined in SerialData
	const int32_t NaN = 0x7FFFFFF;
	
	/*!
	 * \brief The different types of messages for use in SerialData
	 * To read more, check `Documnetaion/SerialCmd.html`
	 */
	enum class MsgType : int16_t
	{
		none   = 0, /*!< A blank message */
		drive  = 1, /*!< A message to drive a certain distance */
		sensor = 2, /*!< A sensory message */
		goal   = 3, /*!< Location of a cone */
		seg    = 4  /*!< Next desired position from current */
	};
	
	/*!
	 * \brief More useful print statement for MsgType
	 * \param os The output stream being writen to
	 * \param mt The mesage being writen out
	 * \return A reference to the output stream
	 */
	std::ostream&
	operator<<(
		std::ostream& os,
		MsgType       mt
	);
	
	//! Contains info that gets sent to/recieved from a serial port
	struct SerialData
	{
		MsgType kind;        /*!< The type of message present */
		int32_t number;      /*!< The cone number */
		int32_t speed;       /*!< The vehicle speed in cm/s */
		int32_t angle;       /*!< The cart angle in degrees */
		int32_t bearing;     /*!< The vehicle bearing in degrees */
		int32_t posE;        /*!< Position (W-E axis) in centimeters */
		int32_t posN;        /*!< Position (N-S axis) in centimeters */
		int32_t probability; /*!< Probability (INT_MIN - INT-MAX) */
		int32_t traveled;    /*!< Distance traveled in centimeters */
	};

	/*!
	 * \brief Dump data from a SerialData struct to an output stream
	 * \param os The output stream being writen to
	 * \param sd The data being writen out
	 * \return A reference to the output stream
	 */
	std::ostream&
	operator<<(
		std::ostream&     os,
		const SerialData& sd
	);

	/*!
	 * \brief Clear/reset the info present within a SerialData
	 * \param sd The serial data to be cleared 
	 */
	void clear(
		SerialData& sd
	);

	/*!
	 * \brief Parse the info in a string in the Elcano_Serial format
	 * \param in The string of data in the Elcano_Serial format
	 * \param sd Set to the data parsed from the in
	 * Uses parser in `arduino.y`. This function is used internally, and
	 * may also be useful for debugging purposes, as is the case in
	 * `test-arduino.cc`
	 */
	void
	read(
		const std::string& in,
		SerialData&        sd
	);

	/*!
	 * \brief Read info from a serial port, and parse it
	 * \param in The serial port to read from
	 * \param sd The SerialData struct to store the result
	 */
	void
	read(
		serial::Serial& in,
		SerialData&     sd
	);
	
	/*!
	 * \brief Write info from a SerialData in the Elcano_Serial format
	 * \param os The output stream to write to
	 * \param sd The SerialData to write out
	 * This function is used internally, and may also be useful for
	 * debugging purposes, as is the case in `test-arduino.cc`
	 */
	void
	write(
		std::ostream&     os,
		const SerialData& sd
	);
	
	/*!
	 * \brief Write info from a SerialData to a serial port
	 * \param os The output serial to write to
	 * \param sd The SerialData to write out
	 */
	void
	write(
		serial::Serial&   os,
		const SerialData& sd
	);
}
