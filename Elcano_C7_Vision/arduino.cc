#include "arduino.hh"
#include "arduino.tab.hh"

/* Send/receive information to an external device */

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *);
extern void yy_delete_buffer(YY_BUFFER_STATE);

namespace elcano
{
	std::ostream&
	operator<<(
		std::ostream& os,
		MsgType in
	) {
		switch (in)
		{
		case MsgType::none:
			os << "none";
			break;
		case MsgType::drive:
			os << "drive";
			break;
		case MsgType::sensor:
			os << "sensor";
			break;
		case MsgType::goal:
			os << "goal";
			break;
		case MsgType::seg:
			os << "seg";
			break;
		default:
			os << "?";
		}
		return os;
	}
	
	std::ostream&
	operator<<(
		std::ostream& os,
		const SerialData& in
	) {
		os << "[SerialData]" << std::endl;
		os << "kind = \"" << in.kind << "\"" << std::endl;
		os << "number = " << in.number << std::endl;
		os << "speed = " << in.speed << std::endl;
		os << "angle = " << in.angle << std::endl;
		os << "bearing = " << in.bearing << std::endl;
		os << "pos = [ " << in.posE << ", " << in.posN << " ]" << std::endl;
		os << "probability = " << in.probability << std::endl;
		return os;
	}
	
	void clear(
		SerialData& info
	) {
		info.kind = MsgType::none;
		info.number = NaN;
		info.speed = NaN;
		info.angle = NaN;
		info.bearing = NaN;
		info.posE = NaN;
		info.posN = NaN;
		info.probability = NaN;
	}
	
	void
	read(
		const std::string &in,
		SerialData &info
	) {
		clear(info);
		YY_BUFFER_STATE buffer = yy_scan_string(in.c_str());
		int r = yyparse(&info);
		yy_delete_buffer(buffer);
		if (r) std::cerr << in << std::endl;
	}
	
	void
	read(
		serial::Serial& device,
		SerialData& info
	) {
		read(device.readline(), info);
	}
	
	void
	write(
		std::ostream &ss,
		const SerialData& info
	) {
		switch (info.kind)
		{
		case MsgType::drive:
			ss << "D";
			if (info.speed != NaN)
				ss << " {s " << info.speed << "}";
			if (info.angle != NaN)
				ss << " {a " << info.angle << "}";
			break;
		case MsgType::sensor:
			ss << "S";
			if (info.speed != NaN)
				ss << " {s " << info.speed << "}";
			if (info.angle != NaN)
				ss << " {a " << info.angle << "}";
			if (info.posE != NaN && info.posN != NaN)
				ss << " {p " << info.posE << "," << info.posN << "}";
			break;
		case MsgType::goal:
			ss << "G";
			if (info.number != NaN)
				ss << " {n " << info.number << "}";
			if (info.posE != NaN && info.posN != NaN)
				ss << " {p " << info.posE << "," << info.posN << "}";
			if (info.bearing != NaN)
				ss << " {b " << info.bearing << " }";
			if (info.probability != NaN)
				ss << " {r " << info.probability << "}";
			break;
		case MsgType::seg:
			ss << "X";
			if (info.number != NaN)
				ss << " {n " << info.number << "}";
			if (info.posE != NaN && info.posN != NaN)
				ss << " {p " << info.posE << "," << info.posN << "}";
			if (info.bearing != NaN)
				ss << " {b " << info.bearing << "}";
			if (info.speed != NaN)
				ss << " {s " << info.speed << "}";
			break;
		case MsgType::none: [[fallthrough]]
		default:
			break;
		}
	}
	
	void
	write(
		serial::Serial& device,
		const SerialData& info
	) {
		std::ostringstream ss;
		write(ss, info);
		ss << "\0\n";
		device.write(ss.str());
	}
}
