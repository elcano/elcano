#include "ElcanoSerial.h"
#include <FastCRC.h>
FastCRC8 CRC8;

namespace elcano
{	
	bool SerialData::write(HardwareSerial * out)
	{
		if(!verify()) return false;
		if(kind == MsgType::drive)
		{
			//create the drive string
			int position = 0;
			char buffer[64];
			buffer[position++] = 'D';
			buffer[position++] = '{';
			buffer[position++] = 's';
			buffer[position++] = ' ';
			
			String num = String(speed_cmPs);
			for(int i = 0; i < num.length(); i++)
			{
				buffer[position++] = num.charAt(i);
			}
			buffer[position++] = '}';
			buffer[position++] = '{';
			buffer[position++] = 'a';
			buffer[position++] = ' ';
			num = String(angle_mDeg);
			for(int i = 0; i < num.length(); i++)
			{
				buffer[position++] = num.charAt(i);
			}
			buffer[position++] = '}';
			out->print(buffer);
			out->print(CRC8.smbus(buffer, position));
			out->print('\n');
			
		}
	}
	
	void SerialData::clear()
	{
		kind = MsgType::none;
		number = NaN;
		speed_cmPs = NaN;
		angle_mDeg = NaN;
		bearing_deg = NaN;
		posE_cm = NaN;
		posN_cm = NaN;
		probability = NaN;
		outSize = 0;
		for(int i = 0; i < 60; i++) outBuffer[i] = 0;
	}
	bool SerialData::verify()
	{
		switch (kind) {
	    case MsgType::drive: //"D{s 123}{a 123}CRC\n"
	      if (speed_cmPs  == NaN) return false;
	      if (angle_mDeg   == NaN) return false;
	      break;
	    case MsgType::sensor: //S{p 123,123}
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
}
