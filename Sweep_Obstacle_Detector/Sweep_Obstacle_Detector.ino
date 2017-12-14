/*
 * Sweep Obstacle Detector
 * by B. Johnson
 *
 * A simple obstacle detection sketch for use with the Scanse Sweep lidar
 * sensor.
 *
 * Searches for obstacles within the arc formed by the left and right
 * boundaries. If a reading within this arc has a distance which is
 * less than the calculated range, an obstacle has been found.
 *
 * The left and right boundaries are determined by the width of the vehicle. The
 * range is determined by the vehicle speed and the time it takes for the vehicle
 * to fully stop after braking.
 */
//#define DEBUG // debug flag

#include <math.h>
#include <Sweep.h>
#include <SoftwareSerial.h>

#include <ElcanoSerial.h>
#include "DistanceLookupTable.h"

const uint8_t POWER_PIN = 2; // Power Enable pin
const uint8_t SYNC_PIN = 3; // Sync / Device Ready pin
const uint8_t BRAKE_PIN = 4; // Brake pin
const uint8_t SOFTSERIAL_TX_PIN = 9; // SoftwareSerial output
const uint8_t SOFTSERIAL_RX_PIN = 8; // SoftwareSerial input

const uint32_t SERIAL1_BAUD_RATE = 115200; // Baud rate for Serial1
const uint32_t SERIAL_BAUD_RATE = 9600; // Baud rate for Serial
const uint8_t SERIAL_TIMEOUT_MS = 5; // Timeout for serial communication
const uint16_t READING_DELAY_US = 500; // Delay between readings

const uint16_t vehicleSpeed_cmPs = 100; // Speed of vehicle (centimeters per second)
const uint16_t brakeTime_ms = 1000; // Time needed to fully stop after applying brake
const uint16_t RANGE_CM = ((brakeTime_ms / 1000) * vehicleSpeed_cmPs) * 2; // Range within which to scan for obstacles
const uint16_t VEHICLE_WIDTH_CM = 76; // Width of vehicle

const uint8_t MOTOR_SPEED_HZ = 8;   // Number of rotations per second
const uint8_t (&MOTOR_SPEED_CODE)[2] = MOTOR_SPEED_CODE_8_HZ;   // Number of rotations per second
const uint16_t SAMPLE_RATE_HZ = 500; // Number of samples per second
const uint8_t (&SAMPLE_RATE_CODE)[2] = SAMPLE_RATE_CODE_500_HZ; // Number of samples per second

const uint16_t MAX_ANGLE = 359; // Maximum whole angle value
const uint16_t RIGHT_ANGLE_BOUNDARY = 90; // Right boundary angle of scan range
const uint16_t LEFT_ANGLE_BOUNDARY = 270; // Left boundary angle of scan range
const uint8_t BRAKE_PIN_TIME_MS = 100; // Length of time to hold brake pin high
const uint16_t MIN_DISTANCE_CM = 2; // Minimum distance for reading to be valid
const uint16_t MAX_ANGLE_DIFF_DEGREES = 30; // Maximum angle difference between readings to be considered valid reading
const uint8_t MIN_SIG_STRENGTH = 100; // Minimum signal strength for obstacle reading to be valid

const uint8_t NUM_FRACTIONAL_BITS = 4; // Number of fractional bits in raw angle

// SoftwareSerial port
SoftwareSerial softSerial(SOFTSERIAL_RX_PIN, SOFTSERIAL_TX_PIN);

// Sweep sensor
Sweep sweep(Serial1);

// Distance lookup table
DistanceLookupTable table(VEHICLE_WIDTH_CM, vehicleSpeed_cmPs, brakeTime_ms);

// ElcanoSerial data
elcano::SerialData data;

void setup()
{
  #ifdef DEBUG
  // Serial
  Serial.setTimeout(SERIAL_TIMEOUT_MS);
  Serial.begin(SERIAL_BAUD_RATE);
  #endif
  // Serial1
  Serial1.begin(SERIAL1_BAUD_RATE);
  // SoftwareSerial
  softSerial.begin(elcano::baudrate);
  
  // Power Enable pin
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  // Sync / Device Ready pin
  pinMode(SYNC_PIN, INPUT);
  
  // ElcanoSerial data
  data.clear();
  data.kind = elcano::MsgType::sensor;

  // Wait until motor speed has stabilized
  if (!sweep.getMotorReady())
  {
    sweep.waitUntilMotorReady();
  }
  // Set motor speed
  if (sweep.getMotorSpeed() != MOTOR_SPEED_HZ)
  {
    sweep.setMotorSpeed(MOTOR_SPEED_CODE);
  }
  // Wait until motor speed has stabilized
  if (!sweep.getMotorReady())
  {
    sweep.waitUntilMotorReady();
  }
  // Set sample rate
  if (sweep.getSampleRate() != SAMPLE_RATE_HZ)
  {
    sweep.setSampleRate(SAMPLE_RATE_CODE);
  }
  // Begin taking readings
  if (!sweep.isScanning())
  {
    sweep.startScanning();
  } 
}

void loop()
{
  // Get reading if available
  bool hadReading = false;
  ScanPacket reading = sweep.getReading(&hadReading);
  if (hadReading)
  {
    // Get integer part of angle
    uint16_t angle = reading.getAngleRaw() >> NUM_FRACTIONAL_BITS;
    bool withinRange = ((angle <= MAX_ANGLE) &&
                         (angle >= LEFT_ANGLE_BOUNDARY ||
                          angle <= RIGHT_ANGLE_BOUNDARY));

    // Get distance from obstacle
    uint16_t distance = reading.getDistanceCentimeters();
    bool withinDistance = (distance >= MIN_DISTANCE_CM) &&
                          (distance <= table.getDistance(angle));

    // Get signal strength
    uint8_t signalStrength = reading.getSignalStrength();
    bool withinSignalStrength = (signalStrength >= MIN_SIG_STRENGTH);
    
    // Set ElcanoSerial data and write if obstacle detected
    if (withinRange && withinDistance && withinSignalStrength)
    {
      data.obstacle_mm = distance;
      data.angle_mDeg = angle;
      data.write((HardwareSerial*)&softSerial);
      #ifdef DEBUG
      data.write((HardwareSerial*)&Serial);
      #endif
    }
    // Delay between readings
    delayMicroseconds(READING_DELAY_US);
  }
}
