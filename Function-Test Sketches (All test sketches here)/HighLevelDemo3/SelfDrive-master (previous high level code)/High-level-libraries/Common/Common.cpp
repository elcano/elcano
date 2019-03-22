#include <IODue.h>
#include <Arduino.h>
#include <Common.h>
#include <math.h>
// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
char dataString[BUFFSIZ];
volatile bool DataAvailable;

char GPSfile[BUFFSIZ] = "mmddhhmm.csv";
char ObstacleString[BUFFSIZ];
char StartTime[BUFFSIZ] = "yy,mm,dd,hh,mm,ss,xxx";
const char TimeHeader[] = "year,month,day,hour,minute,second,msec";
const char* RawKF = "Raw GPS data,,,,,,Kalman Filtered data";
const char* Header = "Latitude,Longitude,East_m,North_m,SigmaE_m,SigmaN_m,Time_s,";
const char* ObstHeader = "Left,Front,Right,Busy";

namespace common {
/*---------------------------------------------------------------------------------------*/
	bool checksum(char *msg) {
		int sum = 0;
		if (msg[0] != '$')
			return false;
		for (int i = 1; i < BUFFSIZ - 4; i++) {
			if (msg[i] == '*') {
				int msb = (sum >> 4);
				msg[i + 1] = msb > 9 ? 'A' + msb - 10 : '0' + msb;
				int lsb = (sum & 0x0F);
				msg[i + 2] = lsb > 9 ? 'A' + lsb - 10 : '0' + lsb;
				//      msg[i+3] = '\n';  // rely on Serial.println()
				return true;
			}
			sum ^= msg[i];
		}
		return false;
	}
}
namespace elcano {

/*---------------------------------------------------------------------------------------*/
//---------------------------------------------------------
	long int parsedecimal(char *str) {
		long int d = 0;

		while (str[0] != 0) {
			if ((str[0] > '9') || (str[0] < '0'))
				return d;
			d *= 10;
			d += str[0] - '0';
			str++;
		}
		return d;
	}
//---------------------------------------------------------
//  read a number of form  123.456,
// On entry, str points to the 1 or -
// On return, it points to the next charater beyond the comma.
// whole number can have any number of digits.
// fraction must be three digits.
// If there is no decimal point, returned value is an integer.
// If there is a decimal point, returned value is scaled.

	long int ReadDecimal(char *str) {
		long whole;
		bool negative = false;
		if (str[0] == ',') {
			str++;   // a blank field means no data
			return INVALID;
		}
		if (str[0] == '-') {
			negative = true;
			str++;
		}
		whole = parsedecimal(str);
		if (str[0] == '.') {
			str++;  // skip decimal point
			whole *= 1000;
			whole += parsedecimal(str);
		}
		if (str[0] = ',')
			str++;
		return negative ? -whole : whole;

	}


	void DataReady()  // called from an interrupt
	{
		DataAvailable = true;
	}

//---------------------------------------------------------
	void writeline(int channel) {
		switch (channel) {
			case 1:
				Serial1.println(buffer);
				break;
			case 2:
				Serial2.println(buffer);
				break;
			case 3:
				Serial3.println(buffer);
				break;
			default:
				Serial.println(buffer);
				break;
		}

	}

//---------------------------------------------------------
// return true if a line was read; false if not
	bool readline(int channel) {
		// buffer can hold 128 bytes; if not enough there yet, try later.
		const int MinimumMessage = 14;
		char c;
		static char buffidx = 0;                // an indexer into the buffer
		int Available;
		/* DataAvailable is a flag set in response to an interrpt.
		After the data has been sent, the sending processor toggles the DATA_READY line.
		The receiving computer gets interrupted by this signal and sets DataAvailable.
		*/
		//if (!DataAvailable)
		//return false;


		while (1) {
			switch (channel) {
				case 1:
					Available = Serial1.available();
					if (Available > 0)
						c = Serial1.read();
					break;
				case 2:
					Available = Serial2.available();
					if (Available > 0)
						c = Serial2.read();
					break;
				case 3:
					Available = Serial3.available();
					if (Available > 0)
						c = Serial3.read();
					break;
				default:
					Available = Serial.available();
					if (Available > 0)
						c = Serial.read();
					break;
			}

			if (c == -1)
				continue;
			if (c == '\n')
				continue;
			if ((buffidx == BUFFSIZ - 1) || (c == '\r')) {
				buffer[buffidx] = 0;
				DataAvailable = false;
				buffidx = 0;
				if (buffidx < MinimumMessage)
					return false;
				else
					return true;
			}
			buffer[buffidx++] = c;
		}
	}
	/* Function Determines crosspoint on X-axis */
	double CrossPointX(double x1, double y1, double x2, double y2,
										 double x3, double y3, double x4, double y4)
	{
	if (x1 == x2 && x3 == x4)
		return ((x1+x2)/2.0);
	if(x1 == x2)
		return x1;
	if(x3 == x4)
		return 3;
	
	/* yc = (y4-y3)/(x4-x3)*(xc-x3) + y3 = (y2-y1)/(x2-x1)*(xc-x1)+y1 */
	double slope1 = (y2-y1)/(double)(x2-x1);
	double slope3 = (y4-y3)/(double)(x4-x3);
	
	if(slope3 == slope1)
		return ((x2+x2+x3)/4.0);
	
	double xc = (slope3 * x3 - y3 - slope1 * x1 + y1) / (double)(slope3-slope1);
	return xc;
	}
	
	/* The function calculates position using Dead Reckoning
	 */
	void ComputePositionWithDR(waypoint &oldData, waypoint &newData)
	{
	//Serial.println("millis(): " + String(millis()));
	//Serial.print("ComputePositionWithDR::NewTime:");
	//Serial.print(newData.time_ms);
	//Serial.print(",OldTime:");
	//Serial.println(oldData.time_ms);
	
	
	// To check if this is new reading or the same reading
		if ( newData.time_ms > oldData.time_ms) {
		
			// Calculate distance
			double distance_mm = (newData.time_ms - oldData.time_ms) * (newData.speed_mmPs);
		
			//Serial.print("ComputePositionWithDR::newSpeed_mmPs:");
			//Serial.print(newData.speed_mmPs);
			//Serial.print(",distance:");
			//Serial.println(newData.distance_mm);
		
			newData.east_mm = oldData.east_mm + cos((newData.bearing_deg) * TO_RADIANS) * distance_mm;
		
			newData.north_mm = oldData.north_mm + sin((newData.bearing_deg) * TO_RADIANS) * distance_mm;
		
		
			//Serial.print("ComputePositionWithDR::X_pos:");
			//Serial.print(newData.x_Pos);
			//Serial.print(",Y_pos:");
			//Serial.println(newData.y_Pos);
		}
	}
	/* This function finds fuzzy crosspoint between GPS & Dead reckoning data
	 waypoint &gps -> GPS input
	 waypoint &dr   -> Dead reckoning input
	 waypoint &estimated_position -> update estimated_position
	 */
	void FindFuzzyCrossPointXY(waypoint &gps, waypoint &dr, waypoint &estimated_position)
	{
	//if GPS and DR position is the same
	if (gps.east_mm == dr.east_mm && gps.north_mm == dr.north_mm)
		{
		estimated_position.north_mm = dr.north_mm;
		estimated_position.east_mm = dr.east_mm;
		return;
		}
	
	if (abs(gps.east_mm - dr.east_mm) > abs(gps.north_mm - dr.north_mm))
		{  // more change in east
			//Serial.println("more east");
			if (gps.east_mm >= dr.east_mm)
				{  // cross point is intersection of line down from DR and line up to GPS
					// DR down line is from (dr->east_mm, 1) to (dr->east_mm + DR_ERROR_mm, 0)
					// line up to GPS is from (gps->east_mm - gps->sigma_mm, 0) to (gps->east_mm, 1)
					estimated_position.east_mm = CrossPointX(dr.east_mm,1, dr.east_mm + DR_ERROR_mm, 0,
																										gps.east_mm - gps.sigma_mm, 0., gps.east_mm, 1);
				}
			else
				{  // cross point is intersection of line down from GPS and line up to DR
					estimated_position.east_mm = CrossPointX(dr.east_mm - DR_ERROR_mm,0., dr.east_mm, 1,
																										gps.east_mm, 1,gps.east_mm + gps.sigma_mm, 0.);
				}
			// north position is proportional to east position
			if (gps.north_mm >= dr.north_mm)
				{
				estimated_position.north_mm = dr.north_mm + (gps.north_mm - dr.north_mm) *
				abs((gps.east_mm-estimated_position.east_mm)/(double)(gps.east_mm - dr.east_mm));
				}
				else
					{
					estimated_position.north_mm = gps.north_mm + (dr.north_mm - gps.north_mm) *
					abs((gps.east_mm-estimated_position.east_mm)/(double)(gps.east_mm - dr.east_mm));
					}
		}
	else
		{  // more change in north
			// TO DO: similar to above, but swap east and north
			//Serial.println("more north");
			if(gps.north_mm >= dr.north_mm)
				{
				// cross point is intersection of line down from DR and line up to GPS
				// DR down line is from (1, dr->north_mm) to (0, dr->north_mm + DR_ERROR_mm)
				// line up to GPS is from (0, gps->north_mm - gps->sigma_mm) to (1, gps->north_mm)
				estimated_position.north_mm = CrossPointX(dr.north_mm,1, dr.north_mm + DR_ERROR_mm, 0,
																								 gps.north_mm - gps.sigma_mm, 0., gps.north_mm, 1);
				}
			else
				{	// cross point is intersection of line down from GPS and line up to DR
					estimated_position.north_mm = CrossPointX(dr.north_mm - DR_ERROR_mm,0., dr.north_mm, 1,
																									 gps.north_mm, 1,gps.north_mm + gps.sigma_mm, 0.);
				}
			// east position is proportional to north position
			if (gps.east_mm >= dr.east_mm)
				{
				estimated_position.east_mm = dr.east_mm + (gps.east_mm - dr.east_mm) *
				abs((gps.north_mm-estimated_position.north_mm)/(double)(gps.north_mm - dr.north_mm));
				}
				else
					{
					estimated_position.east_mm = gps.east_mm + (dr.east_mm - gps.east_mm) *
					abs((gps.north_mm-estimated_position.north_mm)/(double)(gps.north_mm - dr.north_mm));
					}
		}
	}

//Constructor for type Origin object
	Origin::Origin(double lat, double log) {
		latitude = lat;
		longitude = log;

		cos_lat = cos((latitude) * TO_RADIANS);
	}
	
	void waypoint::SetTime(char *pTime, char * pDate) {
		//GPSfile = "mmddhhmm.CSV";
		strncpy(GPSfile,   pDate + 2, 2); // month
		strncpy(GPSfile + 2, pDate, 2);  // day
		strncpy(GPSfile + 4, pTime, 2);  // GMT hour
		strncpy(GPSfile + 6, pTime + 2, 2); // minute
		Serial.println(GPSfile);
		
		strncpy(StartTime,     pDate + 4, 2); // year
		strncpy(StartTime + 3,   pDate + 2, 2); // month
		strncpy(StartTime + 6,   pDate, 2);  // day
		strncpy(StartTime + 9,   pTime, 2);  // GMT hour
		strncpy(StartTime + 12,  pTime + 2, 2); // minute
		strncpy(StartTime + 15,  pTime + 4, 2); // second
		strncpy(StartTime + 18,  pTime + 7, 3); // millisecond
	}

/* There are more accurate ways to compute distance between two latitude and longitude points.
   We use a simple approximation, since we are interesed in a flat projection over a small area.
   Curvature of the earth is not significant.
*/
	void waypoint::Compute_mm(Origin &origin) {
		// compute relative to origin, since Arduino double is limited to 6 digits.
		double diffWhole;
		double dist;

		diffWhole = (longitude - origin.longitude);
		dist = (diffWhole * TO_RADIANS * EARTH_RADIUS_MM);
	
		east_mm = (dist * origin.cos_lat);

		diffWhole = latitude - origin.latitude;
		dist = diffWhole * TO_RADIANS * EARTH_RADIUS_MM;

		north_mm = dist;
	}


	void waypoint::Compute_LatLon(Origin &origin) {
		double theta;

		theta = asin((double)north_mm / EARTH_RADIUS_MM);
		latitude = origin.latitude + (theta / TO_RADIANS);

		theta = asin((double)(east_mm/origin.cos_lat) / EARTH_RADIUS_MM);
		longitude = origin.longitude + (theta / TO_RADIANS);
	}
//---------------------------------------------------------
/*
   Estimated state (index=-1)  or Waypoint, or list of next waypoints on route (index > 0);
	  $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<Evector_x1000>,<Nvector_x1000>,<index>*CKSUM
	  // at leat 18 characters
*/

	char *waypoint::formPointString()
// function uses a global dataString buffer; thus a second call to formDataString
// may overwrite results from the previous call.
	{
		// now log the information
		// make a string for assembling the data to log:
		long eastFraction, northFraction, speedFraction;
		eastFraction = east_mm >= 0 ? east_mm % 1000 : (-east_mm) % 1000;
		northFraction = north_mm >= 0 ? north_mm % 1000 : (-north_mm) % 1000;
		speedFraction = speed_mmPs >= 0 ? speed_mmPs % 1000 : (-speed_mmPs) % 1000;
		sprintf(dataString,
				"$POINT,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%ld.%0.3ld,%d,%d,%d*\r\n",
				east_mm / 1000, eastFraction, north_mm / 1000, northFraction,
				sigma_mm / 1000, sigma_mm % 1000, time_ms / 1000, time_ms % 1000,
				speed_mmPs / 1000, speed_mmPs % 1000, Evector_x1000, Nvector_x1000, index);

		return dataString;
	}

	bool waypoint::readPointString(unsigned long max_wait_ms, int channel) {
		char *parsePtr;
		unsigned long end_time = millis() + max_wait_ms;
		while (millis() < end_time) {
			if (readline(channel) && strncmp(buffer, "$POINT", 6) == 0) {
				//  $POINT,<east_m>,<north_m>,<sigma_m>,<time_s>,<speed_mPs>,<Evector_x1000>,<Nvector_x1000>,<index>*CKSUM
				parsePtr = buffer + 7;
				east_mm = ReadDecimal(parsePtr);
				north_mm = ReadDecimal(parsePtr);
				Serial.println("East_mm " + String(east_mm) + " \t  North_mm" + String(north_mm));
				sigma_mm = ReadDecimal(parsePtr);
				time_ms = ReadDecimal(parsePtr);
				speed_mmPs = ReadDecimal(parsePtr);
				Evector_x1000 = ReadDecimal(parsePtr);
				Nvector_x1000 = ReadDecimal(parsePtr);
				index = ReadDecimal(parsePtr);
				east_mm += 100;
				north_mm += 100;
				return true;
			}
		}
		return false;
	}

	void waypoint::operator=(waypoint &right) {
		latitude = right.latitude;
		longitude = right.longitude;
		east_mm = right.east_mm;
		north_mm = right.north_mm;
		sigma_mm = right.sigma_mm;
		time_ms = right.time_ms;
		Evector_x1000 = right.Evector_x1000;
		Nvector_x1000 = right.Nvector_x1000;
		speed_mmPs = right.speed_mmPs;
		index = right.index;
		return;
	}

	void waypoint::operator=(waypoint *right) {
		latitude = right->latitude;
		longitude = right->longitude;
		east_mm = right->east_mm;
		north_mm = right->north_mm;
		sigma_mm = right->sigma_mm;
		time_ms = right->time_ms;
		Evector_x1000 = right->Evector_x1000;
		Nvector_x1000 = right->Nvector_x1000;
		speed_mmPs = right->speed_mmPs;
		index = right->index;
		return;
	}

	long waypoint::distance_mm(waypoint *other) {
		long deltaX, deltaY;
		deltaX = east_mm - other->east_mm;
		deltaY = north_mm - other->north_mm;
		return sqrt(deltaX * deltaX + deltaY * deltaY);
	}

	void waypoint::vectors(waypoint *other) {
		long deltaX, deltaY, dist;
		deltaX = -east_mm + other->east_mm;
		deltaY = -north_mm + other->north_mm;
		dist = sqrt(deltaX * deltaX + deltaY * deltaY);
		Evector_x1000 = (deltaX * 1000.) / dist;
		Nvector_x1000 = (deltaY * 1000.) / dist;
	}

	long waypoint::distance_mm(long East_mm, long North_mm) {
		long deltaX, deltaY;
		deltaX = East_mm - east_mm;
		deltaY = North_mm - north_mm;
		return sqrt(deltaX * deltaX + deltaY * deltaY);
	}

//========================= Items only for C6 Navigator =================================
//#ifdef MEGA

#define REAL float
// unsigned long millis() is time since program started running
// offset_ms is value of millis() at start_time
	unsigned long offset_ms = 0;

	void Filter(REAL *x, REAL *P, REAL *measure, REAL deltaT, REAL *variance) {} // needs to be implemented

/*---------------------------------------------------------------------------------------*/
//Method need futher implementation. Currently not used
	void waypoint::fuse(waypoint GPS_reading, int deltaT_ms, Origin &origin) {
		// Assume uncertainty standard deviation is 10 meters.
		// The numbers below are variances in m.
		// speed standard deviation is in m/sec;
		// assuming no time error it is same as position standard deviation
		static REAL uncertainty[] = {100., 0, 0, 0,
									 0, 100., 0, 0,
									 0, 0, 100., 0,
									 0, 0, 0, 100.};
		static REAL State[4] = {5000000, 0, 0, 0};
		REAL variance[] = {100., 0,
						   0, 100.};

		REAL deltaT_s = ((REAL) deltaT_ms) / 1000.0;
		REAL measurements[2];
		REAL speedX, speedY;
		if (State[0] > 2500000) {  // first time
			State[0] = GPS_reading.east_mm / 1000.;
			State[1] = GPS_reading.north_mm / 1000.;
		}
		speedX = ((REAL) (speed_mmPs) * Evector_x1000) / MEG;  // m/sec
		speedY = ((REAL) (speed_mmPs) * Nvector_x1000) / MEG;
		measurements[0] = GPS_reading.east_mm / 1000. + speedX * deltaT_s;
		measurements[1] = GPS_reading.north_mm / 1000. + speedY * deltaT_s;

		REAL GPS_sigma = ((REAL) GPS_reading.sigma_mm) / 1000.;
		variance[0] = variance[3] = GPS_sigma * GPS_sigma;

		Filter(State, uncertainty, measurements, deltaT_s, variance);

		east_mm = State[0] * 1000;
		north_mm = State[1] * 1000;
		speedX = State[2] * 1000;
		speedY = State[3] * 1000;
		sigma_mm = sqrt(uncertainty[0]) * 1000;

		Compute_LatLon(origin);
		speed_mmPs = 1000 * sqrt(speedX * speedX + speedY * speedY);
		if (speed_mmPs > 100 || speed_mmPs < -100) {
			Evector_x1000 = (MEG * speedX / speed_mmPs);
			Nvector_x1000 = (MEG * speedY / speed_mmPs);
		}
	}
	// Calculate and compute the E and N vector 
	void waypoint::Compute_EandN_Vectors(long heading_deg) {
		Evector_x1000 = cos((heading_deg) * TO_RADIANS) * 1000;
		Nvector_x1000 =	 sin((heading_deg) * TO_RADIANS) * 1000;
	}

//----------------------------------------------------------
	char *waypoint::GetLatLon(char *parseptr) {
		uint32_t latitd, longitd;
		char latdir, longdir;
		int latDegree, longDegree;
		float latFraction, longFraction;
		uint32_t degree, fraction;
		latitd = parsedecimal(parseptr);
		if (latitd != 0) {
			latitd *= 10000;
			parseptr = strchr(parseptr, '.') + 1;
			latitd += parsedecimal(parseptr);
		}
		parseptr = strchr(parseptr, ',') + 1;
		// read latitude N/S data
		if (parseptr[0] != ',') {
			latdir = parseptr[0];
		}
		// longitude
		parseptr = strchr(parseptr, ',') + 1;
		longitd = parsedecimal(parseptr);
		if (longitd != 0) {
			longitd *= 10000;
			parseptr = strchr(parseptr, '.') + 1;
			longitd += parsedecimal(parseptr);
		}
		parseptr = strchr(parseptr, ',') + 1;
		// read longitude E/W data
		if (parseptr[0] != ',') {
			longdir = parseptr[0];
		}
		// latitude in latDegree = dd, latMinutes = .mmmmmm
		latDegree = latitd / 1000000;
		latFraction = ((latitd % 1000000) / 10000) / 60;
		latitude = latDegree + latFraction;
		if (latdir == 'S')
			latitude = -latitude;
		//longitude in longDegree = ddd, latMinutes = .mmmmmm
		longDegree = longitd / 1000000;
		longFraction = ((longitd % 1000000) / 10000) / 60;
		longitude = longDegree + longFraction;
		if (longdir == 'W')
			longitude = -longitude;

		return parseptr;
	}

//----------------------------------------------------------
// Aquire a GPS GPRMC signal and fill in the waypoint data.
// return 1 if valid, zero if not.
// wait up to max_wait milliseconds to get a valid signal.
//origin to call compute_mm inside of GetLatLon
	bool waypoint::AcquireGPRMC(unsigned long max_wait_ms) {
		uint8_t groundspeed, trackangle;
		char status = 'V'; // V = data invalid
		char *parseptr;              // a character pointer for parsing
		// value of millis() for GPS data
		unsigned long AcquisitionTime_ms;
		char *pTime;
		char *pDate;
		unsigned long TimeOut = millis() + max_wait_ms;
// $GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10
		sigma_mm = 10000;
//  Serial.println("looking");
//  CosLatitude = cos(((double) LATITUDE_ORIGIN)/1000000. * TO_RADIANS);
		while (status != 'A') // A = data valid
		{
			if (!readline(3)) {  // nothing to read; how long have we waited?
				if (millis() > TimeOut) {
					//Serial.println("Timed out");
					return false;
				}
			}
			//Serial.println("Read line");
			Serial.println(buffer);
			// check if $GPRMC (global positioning fixed data)
			if (strncmp(buffer, "$GPRMC", 6) == 0) {
				Serial.println("parse data");
				Serial.println(buffer);
				AcquisitionTime_ms = millis();
				// hhmmss time data
				pTime =
				parseptr = buffer + 7;
				parseptr = strchr(parseptr, ',') + 1;
				status = parseptr[0];  // A = data valid; V = data not valid
				if (status != 'A')
					continue;
				parseptr += 2;
				// grab latitude & long data, take in origin to also compute east/north
				parseptr = GetLatLon(parseptr);
				// groundspeed
				parseptr = strchr(parseptr, ',') + 1;
//      Serial.println(parseptr);
				groundspeed = parsedecimal(parseptr);
				// track angle
				parseptr = strchr(parseptr, ',') + 1;
				trackangle = parsedecimal(parseptr);
				// date
				parseptr = strchr(parseptr, ',') + 1;
				pDate = parseptr;
				/*   Serial.print("\tGroundspeed: ");
					 Serial.print(groundspeed, DEC); Serial.print(",");
					 Serial.print("\tHeading: ");
					 Serial.print(trackangle, DEC); Serial.print(",");
					 Serial.print("\tDate: ");
					 Serial.println(pDate); */
				if (offset_ms == 0) {
					SetTime(pTime, pDate);
					offset_ms = AcquisitionTime_ms;
				}
				time_ms = AcquisitionTime_ms;
			}

		}
		if (status == 'A')
			return true;

    return false;
	}

//----------------------------------------------------------
// Aquire a GPS $GPGGA signal and fill in the waypoint data.
// return 1 if valid, zero if not.
// wait up to max_wait milliseconds to get a valid signal.
	bool waypoint::AcquireGPGGA(unsigned long max_wait_ms) {
		uint8_t satelites_used, hdop;
		REAL HDOP, error_m, error_mm;
		char FixIndicator = '0';
		char *parseptr;              // a character pointer for parsing
		// value of millis() for GPS data
		unsigned long AcquisitionTime_ms;

		char *pTime;
		unsigned long TimeOut = millis() + max_wait_ms;
		sigma_mm = 10000;
		// $GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M,,,,0000*18
		while (FixIndicator == '0') {
			// Readline reads the next line on the given channel
			// returning true means it read a full line
			// false means nothing to read
			// Readline clears the buffer and will read a new line when called again
			if (!readline(3)) {  // nothing to read; how long have we waited?
				if (millis() > TimeOut)
					//Serial.println("AcquireGPGGA timed out");
					return false;
				continue;
			}
			//Serial.println(buffer);
			if (strncmp(buffer, "$GPGGA", 6) == 0) {
				AcquisitionTime_ms = millis();
				pTime =
				parseptr = buffer + 7;
				parseptr = strchr(parseptr, ',') + 1;

				// grab latitude & long data
				parseptr = GetLatLon(parseptr);
				parseptr = strchr(parseptr, ',') + 1;
				FixIndicator = parseptr[0];  // A = data valid; V = data not valid
				if (FixIndicator == '0')
					continue;
				parseptr += 2;
				// satelites used
				parseptr = strchr(parseptr, ',') + 1;
				satelites_used = parsedecimal(parseptr);
				// HDOP
				parseptr = strchr(parseptr, ',') + 1;
				hdop = parsedecimal(parseptr);
				hdop *= 10;
				parseptr = strchr(parseptr, '.') + 1;
				hdop += parsedecimal(parseptr);
				// http://users.erols.com/dlwilson/gpshdop.htm uses the model
				// error = sqrt( (3.04*HDOP)^2 + (3.57)^2)
				// see http://en.wikipedia.org/wiki/Error_analysis_for_the_Global_Positioning_System
				// get Horizontal Dillution of Precision (HDOP).
				HDOP = (REAL) hdop / 10.;
				error_m = sqrt((3.04 * HDOP) * (3.04 * HDOP) + 3.57 * 3.57);
				error_mm = 1000 * error_m;
				sigma_mm = error_mm;

				if (offset_ms == 0) {
					char ch[] = "1205xx";
					SetTime(pTime, ch);
					offset_ms = AcquisitionTime_ms;
				}
				time_ms = AcquisitionTime_ms;
			}
		}
		if (FixIndicator == '0') {
			Serial.println("FixIndicator is 0");
			return false;
		}
		return true;
	}


}


//    bool convertLatLonToMM(long latitude, long longitude) {
//        long scaledOriginLat = LATITUDE_ORIGIN * 1000000;
//        long scaledOriginLon = LONGITUDE_ORIGIN * 1000000;
//
//        double dLat = (latitude) * (PI / 180.0) - (scaledOriginLat) * (PI / 180.0);
//        double dLon = latitude * (PI / 180.0) - scaledOriginLon * (PI / 180.0);
//
//        //Serial.println("dLat = " + String(dLat) + " dLon = " + String(dLon));
//        double soln = sin(dLat / 2) * cos(dLat / 2) + cos(scaledOriginLat * PI / 180) * cos(latitude * PI / 180) * sin(dLon / 2) * sin(dLon / 2);
//        double ans = 2 * atan2(sqrt(soln), sqrt(1 - soln));
//        double finalAns = EARTH_RADIUS_MM * ans;
//        return true;
//    }

//#endif  // MEGA
