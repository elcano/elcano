#include "DistanceLookupTable.h"

inline float DistanceLookupTable::toRadians(int n)
{
  return n * (M_PI / 180.0);
}

DistanceLookupTable::DistanceLookupTable(const uint16_t vehicleWidth_cm,
                                         const uint16_t vehicleSpeed_cmPs,
                                         const uint16_t brakeTime_ms)
{
  // Range: (Seconds needed to fully stop)
  // * (Vehicle speed in centimeters per second) * 2
  float f_range_cm = (brakeTime_ms / 1000.0) * vehicleSpeed_cmPs * 2;
  uint16_t range_cm = static_cast<uint16_t>(f_range_cm);
  // Horizontal Width: Half of vehicle width
  uint16_t horizontalWidth_cm = vehicleWidth_cm / 2;
  // Right Angle Boundary: Angle (in degrees)
  // of right vertex of rectangular area
  float f_rightBound_degrees = asin((horizontalWidth_cm) / (range_cm * 1.0))
                       * (180.0 / M_PI);
  uint16_t rightBound_degrees = static_cast<uint16_t>(f_rightBound_degrees);
  // Left Angle Boundary: Angle (in degrees) 
  // of left vertex of rectangular area
  uint16_t leftBound_degrees = 360 - rightBound_degrees;

  // Case 1: 0 -- rightBound_degrees
  for (int i = 0; i <= rightBound_degrees; i++)
  {
    float f_distance = range_cm / cos(toRadians(i));
    distances[i] = static_cast<uint16_t>(f_distance);
  }
  // Case 2: (rightBound_degrees + 1) -- 90
  for (int i = rightBound_degrees + 1; i <= 90; i++)
  {
    float f_distance = (horizontalWidth_cm) / cos(toRadians(90 - i));
    distances[i] = static_cast<uint16_t>(f_distance);
  }
  // Case 3: 270 -- (leftBound_degrees - 1)
  for (int i = 270; i <= (leftBound_degrees - 1); i++)
  {
    float f_distance = (horizontalWidth_cm) / cos(toRadians(i - 270));
    distances[i - LEFT_RIGHT_DIFF] = static_cast<uint16_t>(f_distance);
  }
  // Case 4: leftBound_degrees -- 359
  for (int i = leftBound_degrees; i <= 359; i++)
  {
    float f_distance = range_cm / cos(toRadians(i));
    distances[i - LEFT_RIGHT_DIFF] = static_cast<uint16_t>(f_distance);
  }
}

uint16_t DistanceLookupTable::getDistance(const uint16_t angle_degrees) const
{
  // Reduce angle to less than 360 degrees
  uint16_t reduced_angle_degrees = angle_degrees % 360;
  // Case 1: 0 - 90 degrees
  if (reduced_angle_degrees <= 90) // && reduced_angle_degrees >= 0 
  {
    return distances[reduced_angle_degrees];
  }
  // Case 2: 270 - 359 degrees
  else if (reduced_angle_degrees >= 270)
  {
    return distances[reduced_angle_degrees - 179];
  }
  else // (reduced_angle_degrees > 90 && reduced_angle_degrees < 270)
  {
    return 0;
  }
}
