#include <due_can.h>
#include "variant.h"
#include <Wire.h>
extern TwoWire Wire1;
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>

#define mySerial Serial3
Adafruit_GPS GPS(&mySerial);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


void AcquireGPS() {
  char c;
  for (int i = 0; i < 25; i++) {
    c = GPS.read();
  }
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
    if (GPS.fix) {
        Serial.print(GPS.latitudeDegrees, 8);
        Serial.print("  ||  ");
        Serial.println(GPS.longitudeDegrees, 8); 
    }
  }
}


void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  mag.enableAutoRange(true); //Enable auto-gain
  Serial.println("start!");
}

void loop() {
  // put your main code here, to run repeatedly:
  AcquireGPS();
  delay(1000);
}
