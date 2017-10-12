#include "DistanceLookupTable.h"

const uint16_t VEHICLE_WIDTH_CM   = 65535;
const uint16_t VEHICLE_SPEED_CMPS = 1;
const uint16_t BRAKE_TIME_MS      = 1000;
DistanceLookupTable table(VEHICLE_WIDTH_CM,
                          VEHICLE_SPEED_CMPS,
                          BRAKE_TIME_MS);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.read() == 's')
  {
    for (int i = 0; i <= 90; i++)
    {
      Serial.print("Angle: ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print("Distance: ");
      Serial.print(table.getDistance(i));
      Serial.print(" ");
      Serial.print("Angle: ");
      Serial.print(360 - i);
      Serial.print(" ");
      Serial.print("Distance: ");
      Serial.print(table.getDistance(360 - i));
      Serial.println(" ");
    }
  }
}

