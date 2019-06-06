#include <math.h>
#include <stdint.h>

/* 
 * DistanceLookupTable
 * by B. Johnson
 *
 * This class is a lookup table for distances corresponding to angle values
 * (in degrees). The purpose of this class is to provide a fast means for
 * the Sweep obstacle detection Arduino to determine if a given reading lies
 * within the scope of an obstacle.
 *
 * Given the width of the vehicle (in centimeters), its speed (in centimeters
 * per second), and the time to fully stop after applying the brakes (in
 * milliseconds), it calculates the rectangular boundaries in front of the
 * vehicle, between angle degrees 270 and 90.
 *
 * The range (distance away from front of vehicle) of this rectangular area is
 * twice the distance necessary to stop the vehicle at the current speed. Its
 * width is the width of the vehicle.
 */

class DistanceLookupTable
{
  private:
    // Number of entries in lookup table
    static const uint8_t  TABLE_SIZE       = 181;
    // Angle difference between Quadrant II (270-359 degrees)
    // and Quadrant I (0-90 degrees)
    static const uint16_t LEFT_RIGHT_DIFF  = 179;

    // The set of distances corresponding to each angle in the range
    // (according to degrees)
    int distances[TABLE_SIZE];

    // Converts a value in degrees to radians
    inline static float toRadians(int n);

  public:
    // This constructor uses the given vehicle width, speed, and brake time
    // (time needed to fully activate brakes) to construct the lookup table.
    DistanceLookupTable(const uint16_t vehicleWidth_cm,
                        const uint16_t vehicleSpeed_cmPs,
                        const uint16_t brakeTime_ms);

    // Returns the distance corresponding to the given angle value in degrees.
    // A valid angle is within the range of 270 - 90 degrees. If given an
    // invalid angle it will return 0.
    uint16_t getDistance(uint16_t angle_degrees) const;
};