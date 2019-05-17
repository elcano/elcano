#include <IODue.h>
#include <Arduino.h>
#include "GPS_HighLevel.h"


namespace elcano {
  
	void GPS_HighLevel::setup_GPS() {
		//Serial 3 (mySerial) is used for GPS
		mySerial.begin(9600);
		GPS.begin(9600);

		// uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
		//GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
		// uncomment this line to turn on only the "minimum recommended" data
		GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
		// For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
		// the parser doesn't care about other sentences at this time

		// Set the update rate
		GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 5 Hz update rate
		// For the parsing code to work nicely and have time to sort thru the data, and
		// print it out we don't suggest using anything higher than 1 Hz

		// Request updates on antenna status, comment out to keep quiet
		GPS.sendCommand(PGCMD_ANTENNA);
	}

	/*---------------------------------------------------------------------------------------*/
	boolean GPS_HighLevel::AcquireGPS(Waypoint &gps_position) {
		if (DEBUG) {
			Serial.println("Acquire GPS");
		}
		float latitude, longitude;

		char c;
		//read atleast 25 characters every loop speed up update time for GPS
		for (int i = 0; i < 25; i++) {
			c = GPS.read();
			//test output 4-9-2019 --------- Mel
			if (DEBUG) {
				Serial.print(c);
				Serial.print(", ");
			}
		}
		if (DEBUG) {
			Serial.println("");
		}
		delay(1000);

		// if a sentence is received, we can check the checksum, parse it...
		if (GPS.newNMEAreceived()) {
			if (DEBUG) {
				Serial.println("newNMEArecieved");
			}
			// a tricky thing here is if we print the NMEA sentence, or data
			// we end up not listening and catching other sentences!
			// so be very wary if using OUTPUT_ALLDATA and trytng to print out data
			//Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

			if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return false;  // we can fail to parse a sentence in which case we should just wait for another

			if (GPS.fix) {
				if (DEBUG) {
					Serial.println("GPS fix");
				}
				gps_position.latitude = GPS.latitudeDegrees;
				gps_position.longitude = GPS.longitudeDegrees;
				if (DEBUG) {
					Serial.print("Latitude: ");
					Serial.println(gps_position.latitude, 6);
					Serial.print("Longitude: ");
					Serial.println(gps_position.longitude, 6);
				}
				return true;
			}
			return false;
		}
		return false;
	}

} // namespace elcano
