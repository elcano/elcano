#pragma once

#include <serial/serial.h>

/* Send/receive information to an external device */

namespace elcano
{
	const int32_t NaN = 0x7FFFFFF;
	
	enum class MsgType : int16_t
	{
		none   = 0,
		drive  = 1,
		sensor = 2,
		goal   = 3,
		seg    = 4
	};
	
	std::ostream&
	operator<<(
		std::ostream&,
		MsgType
	);
	
	struct SerialData
	{
		MsgType kind;
		int32_t number;
		int32_t speed;    /* cmPs */
		int32_t angle;    /* degrees */
		int32_t bearing;  /* degrees */
		int32_t posE;     /* centimeters */
		int32_t posN;     /* centimeters */
		int32_t probability;
		int32_t traveled; /* centimeters */
	};

	std::ostream&
	operator<<(
		std::ostream& os,
		const SerialData& in
	);

	void clear(
		SerialData&       /* Info */
	);

	void
	read(
		serial::Serial&,  /* Device */
		SerialData&       /* Info */
	);
	
	void
	write(
		std::ostream&,    /* Output */
		const SerialData& /* Info */
	);
	
	void
	write(
		serial::Serial&,  /* Device */
		const SerialData& /* Info */
	);
}
