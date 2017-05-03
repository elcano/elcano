<<<<<<< HEAD
/*******************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ******************************************************************************/
#include <Wire.h>
// #include <limits.h>

#include "Arduino.h"
#include "Adafruit_LSM303_U.h"

/* enabling this #define will enable the debug print blocks
#define LSM303_DEBUG
*/ 

static float _lsm303Accel_MG_LSB     = 0.001F;   // 1, 2, 4 or 12 mg per lsb
static float _lsm303Mag_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _lsm303Mag_Gauss_LSB_Z  = 980.0F;   // Varies with gain

const float x_offset = 10.41F;
const float y_offset = 0.955F;
const float z_offset = -44.54F;

/*******************************************************************************
 ACCELEROMETER
 ******************************************************************************/
/*******************************************************************************
 PRIVATE FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
void Adafruit_LSM303_Accel::write8(byte address, byte reg, byte value)
{
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
byte Adafruit_LSM303_Accel::read8(byte address, byte reg)
{
    byte value;

    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();

    Wire.requestFrom(address, (byte)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

/******************************************************************************/
/*! @brief  Reads the raw data from the sensor                                */
/******************************************************************************/
void Adafruit_LSM303_Accel::read()
{
    // Read the accelerometer
    Wire.beginTransmission((byte)LSM303_ADDRESS_ACCEL);
    Wire.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);

    Wire.endTransmission();
    Wire.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);

    // Wait around until enough data is available
    while (Wire.available() < 6);

    uint8_t xlo = Wire.read(); uint8_t xhi = Wire.read();
    uint8_t ylo = Wire.read(); uint8_t yhi = Wire.read();
    uint8_t zlo = Wire.read(); uint8_t zhi = Wire.read();
 
    // Shift values to create properly formed integer (low byte first)
    _accelData.x = (int16_t)(xlo | (xhi << 8)) >> 4;
    _accelData.y = (int16_t)(ylo | (yhi << 8)) >> 4;
    _accelData.z = (int16_t)(zlo | (zhi << 8)) >> 4;
}


/*******************************************************************************
 CONSTRUCTOR
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Instantiates a new Adafruit_LSM303 class                          */
/******************************************************************************/
Adafruit_LSM303_Accel::Adafruit_LSM303_Accel(int32_t sensorID)
{
    _sensorID = sensorID;
}

/*******************************************************************************
 PUBLIC FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Setups the hardward                                               */
/******************************************************************************/
bool Adafruit_LSM303_Accel::begin()
{
    // Enable I2C
    Wire.begin();

    // Enable the accelerometer (100Hz)
    write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x57);

    // LSM303DLHC has no WHOAMI register so read CTRL_REG1_A back to check
    // if we are connected or not
    uint8_t reg1_a = read8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A);

    if (reg1_a != 0x57) { return false; }  

    return true;
}

/******************************************************************************/
/*! @brief  Gets the most recent sensor event                                 */
/******************************************************************************/
bool Adafruit_LSM303_Accel::getEvent(sensors_event_t *event)
{
    memset(event, 0, sizeof(sensors_event_t)); /* Clear the event */
    read(); /* Read new data */

    event->version = sizeof(sensors_event_t);
    event->sensor_id = _sensorID;
    event->type = SENSOR_TYPE_ACCELEROMETER;
    event->timestamp = millis();
    event->acceleration.x = _accelData.x * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
    event->acceleration.y = _accelData.y * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
    event->acceleration.z = _accelData.z * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;

    return true;
}

/******************************************************************************/
/*! @brief  Gets the sensor_t data                                            */
/******************************************************************************/
void Adafruit_LSM303_Accel::getSensor(sensor_t *sensor)
{
    memset(sensor, 0, sizeof(sensor_t)); /* Clear the sensor_t object */

    /* Insert the sensor name in the fixed length char array */
    strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = 0;
    sensor->version     = 1;
    sensor->sensor_id   = _sensorID;
    sensor->type        = SENSOR_TYPE_ACCELEROMETER;
    sensor->min_delay   = 0;
    sensor->max_value   = 0.0F; // TBD
    sensor->min_value   = 0.0F; // TBD
    sensor->resolution  = 0.0F; // TBD
}


/*******************************************************************************
 MAGNETOMETER
 ******************************************************************************/
/*******************************************************************************
 PRIVATE FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
void Adafruit_LSM303_Mag::write8(byte address, byte reg, byte value)
{
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
byte Adafruit_LSM303_Mag::read8(byte address, byte reg)
{
    byte value;

    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();

    Wire.requestFrom(address, (byte)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

/******************************************************************************/
/*! @brief  Reads the raw data from the sensor                                */
/******************************************************************************/
void Adafruit_LSM303_Mag::read()
{
    // Read the magnetometer
    Wire.beginTransmission((byte)LSM303_ADDRESS_MAG);
    Wire.write(LSM303_REGISTER_MAG_OUT_X_H_M);
    Wire.endTransmission();

    Wire.requestFrom((byte)LSM303_ADDRESS_MAG, (byte)6);
  
    // Wait around until enough data is available
    while (Wire.available() < 6);

    // Note high before low (different than accel)  
    uint8_t xhi = Wire.read(); uint8_t xlo = Wire.read();
    uint8_t zhi = Wire.read(); uint8_t zlo = Wire.read();
    uint8_t yhi = Wire.read(); uint8_t ylo = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    _magData.x = (int16_t)(xlo | ((int16_t)xhi << 8));
    _magData.y = (int16_t)(ylo | ((int16_t)yhi << 8));
    _magData.z = (int16_t)(zlo | ((int16_t)zhi << 8));

    // ToDo: Calculate orientation
    // _magData.orientation = 0.0;

    // _magData.x += x_offset;
    // _magData.y += y_offset;
    // _magData.z += z_offset;
}

/*******************************************************************************
 CONSTRUCTOR
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Instantiates a new Adafruit_LSM303 class                          */
/******************************************************************************/
Adafruit_LSM303_Mag::Adafruit_LSM303_Mag(int32_t sensorID)
{
    _sensorID = sensorID;
    _autoRangeEnabled = false;
}

/*******************************************************************************
 PUBLIC FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Setups the HW                                                     */
/******************************************************************************/
bool Adafruit_LSM303_Mag::begin()
{
    // Enable I2C
    Wire.begin();

    // Enable the magnetometer
    write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00);

    // LSM303DLHC has no WHOAMI register so read CRA_REG_M to check
    // the default value (0b00010000/0x10)
    uint8_t reg1_a = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRA_REG_M);
    
    if (reg1_a != 0x10) { return false; }

    setMagGain(LSM303_MAGGAIN_1_3); // Set the gain to a known level

    return true;
}

/******************************************************************************/
/*! @brief  Enables or disables auto-ranging                                  */
/******************************************************************************/
void Adafruit_LSM303_Mag::enableAutoRange(bool enabled)
{
  _autoRangeEnabled = enabled;
}

/******************************************************************************/
/*! @brief  Sets the magnetometer's gain                                      */
/******************************************************************************/
void Adafruit_LSM303_Mag::setMagGain(lsm303MagGain gain)
{
    write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, (byte)gain);
  
    _magGain = gain;
 
    switch(gain)
    {
        case LSM303_MAGGAIN_1_3:
            _lsm303Mag_Gauss_LSB_XY = 1100;
            _lsm303Mag_Gauss_LSB_Z  = 980;
            break;
        case LSM303_MAGGAIN_1_9:
            _lsm303Mag_Gauss_LSB_XY = 855;
            _lsm303Mag_Gauss_LSB_Z  = 760;
            break;
        case LSM303_MAGGAIN_2_5:
            _lsm303Mag_Gauss_LSB_XY = 670;
            _lsm303Mag_Gauss_LSB_Z  = 600;
            break;
        case LSM303_MAGGAIN_4_0:
            _lsm303Mag_Gauss_LSB_XY = 450;
            _lsm303Mag_Gauss_LSB_Z  = 400;
            break;
        case LSM303_MAGGAIN_4_7:
            _lsm303Mag_Gauss_LSB_XY = 400;
            _lsm303Mag_Gauss_LSB_Z  = 355;
            break;
        case LSM303_MAGGAIN_5_6:
            _lsm303Mag_Gauss_LSB_XY = 330;
            _lsm303Mag_Gauss_LSB_Z  = 295;
            break;
        case LSM303_MAGGAIN_8_1:
            _lsm303Mag_Gauss_LSB_XY = 230;
            _lsm303Mag_Gauss_LSB_Z  = 205;
            break;
    } 
}

/******************************************************************************/
/*! @brief  Sets the magnetometer's update rate                               */
/******************************************************************************/
void Adafruit_LSM303_Mag::setMagRate(lsm303MagRate rate)
{
	byte reg_m = ((byte)rate & 0x07) << 2;
    write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRA_REG_M, reg_m);
}

/******************************************************************************/
/*! @brief  Gets the most recent sensor event                                 */
/******************************************************************************/
bool Adafruit_LSM303_Mag::getEvent(sensors_event_t *event)
{
    bool readingValid = false;
  
    /* Clear the event */
    memset(event, 0, sizeof(sensors_event_t));
  
    while(!readingValid)
    {
        uint8_t reg_mg = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_SR_REG_Mg);
        
        //Is the Data Ready (DRDY)
        if (!(reg_mg & 0x1)) { return false; }
  
        read(); /* Read new data */
    
        /* Make sure the sensor isn't saturating if auto-ranging is enabled */
        if (!_autoRangeEnabled)
        {
            readingValid = true;
        }
        else
        {
#ifdef LSM303_DEBUG
            Serial.print(_magData.x); Serial.print(" ");
            Serial.print(_magData.y); Serial.print(" ");
            Serial.print(_magData.z); Serial.println(" ");
#endif	  
            /* Check if the sensor is saturating or not */
            if ( (_magData.x >= 2040) | (_magData.x <= -2040) |
                 (_magData.y >= 2040) | (_magData.y <= -2040) | 
                 (_magData.z >= 2040) | (_magData.z <= -2040) )
            {
                /* Saturating .... increase the range if we can */
                switch(_magGain)
                {
                    case LSM303_MAGGAIN_5_6:
                        setMagGain(LSM303_MAGGAIN_8_1);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 8.1");
#endif
                        break;
                    case LSM303_MAGGAIN_4_7:
                        setMagGain(LSM303_MAGGAIN_5_6);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 5.6");
#endif
                        break;
                    case LSM303_MAGGAIN_4_0:
                        setMagGain(LSM303_MAGGAIN_4_7);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 4.7");
#endif			
                        break;
                    case LSM303_MAGGAIN_2_5:
                        setMagGain(LSM303_MAGGAIN_4_0);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 4.0");
#endif			
                        break;
                    case LSM303_MAGGAIN_1_9:
                        setMagGain(LSM303_MAGGAIN_2_5);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 2.5");
#endif			
                        break;
                    case LSM303_MAGGAIN_1_3:
                        setMagGain(LSM303_MAGGAIN_1_9);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 1.9");
#endif			
                        break;
                    default:
                        readingValid = true;
                        break;
                }
            }
            else
            {
                /* All values are withing range */
                readingValid = true;
            }
        }
    }
  
    event->version   = sizeof(sensors_event_t);
    event->sensor_id = _sensorID;
    event->type      = SENSOR_TYPE_MAGNETIC_FIELD;
    event->timestamp = millis();

    event->magnetic.x = _magData.x / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event->magnetic.y = _magData.y / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event->magnetic.z = _magData.z / _lsm303Mag_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;
		
	return true;
}

/******************************************************************************/
/*! @brief  Gets the sensor_t data                                            */
/******************************************************************************/
void Adafruit_LSM303_Mag::getSensor(sensor_t *sensor)
{
    memset(sensor, 0, sizeof(sensor_t)); /* Clear the sensor_t object */

    /* Insert the sensor name in the fixed length char array */
    strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name)- 1] = 0;
    sensor->version     = 1;
    sensor->sensor_id   = _sensorID;
    sensor->type        = SENSOR_TYPE_MAGNETIC_FIELD;
    sensor->min_delay   = 0;
    sensor->max_value   = 0.0F; // TBD
    sensor->min_value   = 0.0F; // TBD
    sensor->resolution  = 0.0F; // TBD
}
=======
/*******************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ******************************************************************************/
#include <Wire.h>
// #include <limits.h>

#include "Arduino.h"
#include "Adafruit_LSM303_U.h"

/* enabling this #define will enable the debug print blocks
#define LSM303_DEBUG
*/ 

static float _lsm303Accel_MG_LSB     = 0.001F;   // 1, 2, 4 or 12 mg per lsb
static float _lsm303Mag_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _lsm303Mag_Gauss_LSB_Z  = 980.0F;   // Varies with gain

const float x_offset = 10.41F;
const float y_offset = 0.955F;
const float z_offset = -44.54F;

/*******************************************************************************
 ACCELEROMETER
 ******************************************************************************/
/*******************************************************************************
 PRIVATE FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
void Adafruit_LSM303_Accel::write8(byte address, byte reg, byte value)
{
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
byte Adafruit_LSM303_Accel::read8(byte address, byte reg)
{
    byte value;

    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();

    Wire.requestFrom(address, (byte)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

/******************************************************************************/
/*! @brief  Reads the raw data from the sensor                                */
/******************************************************************************/
void Adafruit_LSM303_Accel::read()
{
    // Read the accelerometer
    Wire.beginTransmission((byte)LSM303_ADDRESS_ACCEL);
    Wire.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);

    Wire.endTransmission();
    Wire.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);

    // Wait around until enough data is available
    while (Wire.available() < 6);

    uint8_t xlo = Wire.read(); uint8_t xhi = Wire.read();
    uint8_t ylo = Wire.read(); uint8_t yhi = Wire.read();
    uint8_t zlo = Wire.read(); uint8_t zhi = Wire.read();
 
    // Shift values to create properly formed integer (low byte first)
    _accelData.x = (int16_t)(xlo | (xhi << 8)) >> 4;
    _accelData.y = (int16_t)(ylo | (yhi << 8)) >> 4;
    _accelData.z = (int16_t)(zlo | (zhi << 8)) >> 4;
}


/*******************************************************************************
 CONSTRUCTOR
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Instantiates a new Adafruit_LSM303 class                          */
/******************************************************************************/
Adafruit_LSM303_Accel::Adafruit_LSM303_Accel(int32_t sensorID)
{
    _sensorID = sensorID;
}

/*******************************************************************************
 PUBLIC FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Setups the hardward                                               */
/******************************************************************************/
bool Adafruit_LSM303_Accel::begin()
{
    // Enable I2C
    Wire.begin();

    // Enable the accelerometer (100Hz)
    write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x57);

    // LSM303DLHC has no WHOAMI register so read CTRL_REG1_A back to check
    // if we are connected or not
    uint8_t reg1_a = read8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A);

    if (reg1_a != 0x57) { return false; }  

    return true;
}

/******************************************************************************/
/*! @brief  Gets the most recent sensor event                                 */
/******************************************************************************/
bool Adafruit_LSM303_Accel::getEvent(sensors_event_t *event)
{
    memset(event, 0, sizeof(sensors_event_t)); /* Clear the event */
    read(); /* Read new data */

    event->version = sizeof(sensors_event_t);
    event->sensor_id = _sensorID;
    event->type = SENSOR_TYPE_ACCELEROMETER;
    event->timestamp = millis();
    event->acceleration.x = _accelData.x * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
    event->acceleration.y = _accelData.y * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
    event->acceleration.z = _accelData.z * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;

    return true;
}

/******************************************************************************/
/*! @brief  Gets the sensor_t data                                            */
/******************************************************************************/
void Adafruit_LSM303_Accel::getSensor(sensor_t *sensor)
{
    memset(sensor, 0, sizeof(sensor_t)); /* Clear the sensor_t object */

    /* Insert the sensor name in the fixed length char array */
    strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = 0;
    sensor->version     = 1;
    sensor->sensor_id   = _sensorID;
    sensor->type        = SENSOR_TYPE_ACCELEROMETER;
    sensor->min_delay   = 0;
    sensor->max_value   = 0.0F; // TBD
    sensor->min_value   = 0.0F; // TBD
    sensor->resolution  = 0.0F; // TBD
}


/*******************************************************************************
 MAGNETOMETER
 ******************************************************************************/
/*******************************************************************************
 PRIVATE FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
void Adafruit_LSM303_Mag::write8(byte address, byte reg, byte value)
{
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

/******************************************************************************/
/*! @brief  Abstract away platform differences in Arduino wire library        */
/******************************************************************************/
byte Adafruit_LSM303_Mag::read8(byte address, byte reg)
{
    byte value;

    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();

    Wire.requestFrom(address, (byte)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

/******************************************************************************/
/*! @brief  Reads the raw data from the sensor                                */
/******************************************************************************/
void Adafruit_LSM303_Mag::read()
{
    // Read the magnetometer
    Wire.beginTransmission((byte)LSM303_ADDRESS_MAG);
    Wire.write(LSM303_REGISTER_MAG_OUT_X_H_M);
    Wire.endTransmission();

    Wire.requestFrom((byte)LSM303_ADDRESS_MAG, (byte)6);
  
    // Wait around until enough data is available
    while (Wire.available() < 6);

    // Note high before low (different than accel)  
    uint8_t xhi = Wire.read(); uint8_t xlo = Wire.read();
    uint8_t zhi = Wire.read(); uint8_t zlo = Wire.read();
    uint8_t yhi = Wire.read(); uint8_t ylo = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    _magData.x = (int16_t)(xlo | ((int16_t)xhi << 8));
    _magData.y = (int16_t)(ylo | ((int16_t)yhi << 8));
    _magData.z = (int16_t)(zlo | ((int16_t)zhi << 8));

    // ToDo: Calculate orientation
    // _magData.orientation = 0.0;

    // _magData.x += x_offset;
    // _magData.y += y_offset;
    // _magData.z += z_offset;
}

/*******************************************************************************
 CONSTRUCTOR
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Instantiates a new Adafruit_LSM303 class                          */
/******************************************************************************/
Adafruit_LSM303_Mag::Adafruit_LSM303_Mag(int32_t sensorID)
{
    _sensorID = sensorID;
    _autoRangeEnabled = false;
}

/*******************************************************************************
 PUBLIC FUNCTIONS
 ******************************************************************************/
/******************************************************************************/
/*! @brief  Setups the HW                                                     */
/******************************************************************************/
bool Adafruit_LSM303_Mag::begin()
{
    // Enable I2C
    Wire.begin();

    // Enable the magnetometer
    write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00);

    // LSM303DLHC has no WHOAMI register so read CRA_REG_M to check
    // the default value (0b00010000/0x10)
    uint8_t reg1_a = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRA_REG_M);
    
    if (reg1_a != 0x10) { return false; }

    setMagGain(LSM303_MAGGAIN_1_3); // Set the gain to a known level

    return true;
}

/******************************************************************************/
/*! @brief  Enables or disables auto-ranging                                  */
/******************************************************************************/
void Adafruit_LSM303_Mag::enableAutoRange(bool enabled)
{
  _autoRangeEnabled = enabled;
}

/******************************************************************************/
/*! @brief  Sets the magnetometer's gain                                      */
/******************************************************************************/
void Adafruit_LSM303_Mag::setMagGain(lsm303MagGain gain)
{
    write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, (byte)gain);
  
    _magGain = gain;
 
    switch(gain)
    {
        case LSM303_MAGGAIN_1_3:
            _lsm303Mag_Gauss_LSB_XY = 1100;
            _lsm303Mag_Gauss_LSB_Z  = 980;
            break;
        case LSM303_MAGGAIN_1_9:
            _lsm303Mag_Gauss_LSB_XY = 855;
            _lsm303Mag_Gauss_LSB_Z  = 760;
            break;
        case LSM303_MAGGAIN_2_5:
            _lsm303Mag_Gauss_LSB_XY = 670;
            _lsm303Mag_Gauss_LSB_Z  = 600;
            break;
        case LSM303_MAGGAIN_4_0:
            _lsm303Mag_Gauss_LSB_XY = 450;
            _lsm303Mag_Gauss_LSB_Z  = 400;
            break;
        case LSM303_MAGGAIN_4_7:
            _lsm303Mag_Gauss_LSB_XY = 400;
            _lsm303Mag_Gauss_LSB_Z  = 355;
            break;
        case LSM303_MAGGAIN_5_6:
            _lsm303Mag_Gauss_LSB_XY = 330;
            _lsm303Mag_Gauss_LSB_Z  = 295;
            break;
        case LSM303_MAGGAIN_8_1:
            _lsm303Mag_Gauss_LSB_XY = 230;
            _lsm303Mag_Gauss_LSB_Z  = 205;
            break;
    } 
}

/******************************************************************************/
/*! @brief  Sets the magnetometer's update rate                               */
/******************************************************************************/
void Adafruit_LSM303_Mag::setMagRate(lsm303MagRate rate)
{
	byte reg_m = ((byte)rate & 0x07) << 2;
    write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRA_REG_M, reg_m);
}

/******************************************************************************/
/*! @brief  Gets the most recent sensor event                                 */
/******************************************************************************/
bool Adafruit_LSM303_Mag::getEvent(sensors_event_t *event)
{
    bool readingValid = false;
  
    /* Clear the event */
    memset(event, 0, sizeof(sensors_event_t));
  
    while(!readingValid)
    {
        uint8_t reg_mg = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_SR_REG_Mg);
        
        //Is the Data Ready (DRDY)
        if (!(reg_mg & 0x1)) { return false; }
  
        read(); /* Read new data */
    
        /* Make sure the sensor isn't saturating if auto-ranging is enabled */
        if (!_autoRangeEnabled)
        {
            readingValid = true;
        }
        else
        {
#ifdef LSM303_DEBUG
            Serial.print(_magData.x); Serial.print(" ");
            Serial.print(_magData.y); Serial.print(" ");
            Serial.print(_magData.z); Serial.println(" ");
#endif	  
            /* Check if the sensor is saturating or not */
            if ( (_magData.x >= 2040) | (_magData.x <= -2040) |
                 (_magData.y >= 2040) | (_magData.y <= -2040) | 
                 (_magData.z >= 2040) | (_magData.z <= -2040) )
            {
                /* Saturating .... increase the range if we can */
                switch(_magGain)
                {
                    case LSM303_MAGGAIN_5_6:
                        setMagGain(LSM303_MAGGAIN_8_1);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 8.1");
#endif
                        break;
                    case LSM303_MAGGAIN_4_7:
                        setMagGain(LSM303_MAGGAIN_5_6);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 5.6");
#endif
                        break;
                    case LSM303_MAGGAIN_4_0:
                        setMagGain(LSM303_MAGGAIN_4_7);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 4.7");
#endif			
                        break;
                    case LSM303_MAGGAIN_2_5:
                        setMagGain(LSM303_MAGGAIN_4_0);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 4.0");
#endif			
                        break;
                    case LSM303_MAGGAIN_1_9:
                        setMagGain(LSM303_MAGGAIN_2_5);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 2.5");
#endif			
                        break;
                    case LSM303_MAGGAIN_1_3:
                        setMagGain(LSM303_MAGGAIN_1_9);
                        readingValid = false;
#ifdef LSM303_DEBUG
                        Serial.println("Changing range to +/- 1.9");
#endif			
                        break;
                    default:
                        readingValid = true;
                        break;
                }
            }
            else
            {
                /* All values are withing range */
                readingValid = true;
            }
        }
    }
  
    event->version   = sizeof(sensors_event_t);
    event->sensor_id = _sensorID;
    event->type      = SENSOR_TYPE_MAGNETIC_FIELD;
    event->timestamp = millis();

    event->magnetic.x = _magData.x / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event->magnetic.y = _magData.y / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event->magnetic.z = _magData.z / _lsm303Mag_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;
		
	return true;
}

/******************************************************************************/
/*! @brief  Gets the sensor_t data                                            */
/******************************************************************************/
void Adafruit_LSM303_Mag::getSensor(sensor_t *sensor)
{
    memset(sensor, 0, sizeof(sensor_t)); /* Clear the sensor_t object */

    /* Insert the sensor name in the fixed length char array */
    strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name)- 1] = 0;
    sensor->version     = 1;
    sensor->sensor_id   = _sensorID;
    sensor->type        = SENSOR_TYPE_MAGNETIC_FIELD;
    sensor->min_delay   = 0;
    sensor->max_value   = 0.0F; // TBD
    sensor->min_value   = 0.0F; // TBD
    sensor->resolution  = 0.0F; // TBD
}
>>>>>>> aabdin01-develop
