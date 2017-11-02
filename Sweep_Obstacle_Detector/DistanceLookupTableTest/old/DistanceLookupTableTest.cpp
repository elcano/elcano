#include <iostream>
#include <stdlib.h>

#include "DistanceLookupTable.h"

/*
 * DistanceLookupTableTest
 * by B. Johnson
 *
 * A simple program which tests the functionality of the DistanceLookupTable
 * class.
 *
 * This program takes 3 command line arguments:
 * 1. Width of the vehicle (in centimeters)
 * 2. Speed of the vehicle (in centimeters per second)
 * 3. Time necessary to stop vehicle after applying brakes (in milliseconds)
 *
 * Constructs a DistanceLookupTable with the given values and prints each of
 * the entries.
 *
 */
int main(int argc, char** argv)
{
  if (argc == 4)
  {
    uint16_t vehicleWidth_cm = strtoul(argv[1], NULL, 10);
    uint16_t vehicleSpeed_cmPs = strtoul(argv[2], NULL, 10);
    uint16_t brakeTime_ms = strtoul(argv[3], NULL, 10);
    DistanceLookupTable table(vehicleWidth_cm,
                              vehicleSpeed_cmPs,
                              brakeTime_ms);
    for (int i = 0; i <= 90; i++)
    {
      std::cout << "Angle: " << i << " ";
      std::cout << "Distance: " << table.getDistance(i) << " ";
      std::cout << "Angle: " << 360 - i << " ";
      std::cout << "Distance: " << table.getDistance(360 - i) << std::endl;
    }
    return EXIT_SUCCESS;
  }
}
