////#include <GPS.h>
//#include <ElcanoSerial.h>
//using namespace elcano;
//
///*
//  Elcano Module C6: Navigator.
//  Includes C5 Obstacle detection.
//
//  Documentation:
//  NavigationSystem (TO DO: Write document, based on these comments).
//  Wiring_C6Mega.xls
//  Navigation sheet of Elcano_BOM  (TO DO: Reorganize Elcano BOM).
//
//  The Navigator has the job of making the best estimate of current vehicle
//  position, attitude, velocity and acceleration.
//
//  It uses a variety of sensors, some of which may not be present or reliable.
//
//  Yes: S1: Hall Odometer.  This unit gives wheel spin speed feedback.
//  TO DO: Connect hardware and integrate into Kalman Filter
//
//  Future: Visual Odometry from S4 (Smart camera) as passed though C7 (Visual Data
//  Management). Visual Odometry could also come from an optical mouse.
//
//  Future: S3: Digital Compass
//
//  Future: S9: Inertial Navigation Unit.
//
//  Yes, from KF: Dead reckoning based on prior state estimate.
//  TO DO: Integrate Kalman Filter with GPS and Odometry.
//
//  Yes: Distance and bearing to landmarks / obstacles.
//
//  Yes: S7: GPS.  GPS should not the primary navigation sensor and the robot should be
//  able to operate indoors, in tunnels or other areas where GPS is not available
//  or accurate.
//
//  Sensed navigation information is passed to C3/C4, which use prexisting or SLAM
//  digital maps, intended path to refine the estimated position.
//  C3/C4 may use a particle filter.
//  C3/C4 may also receive lateral lane deviation from a camera (C7).
//
//  The odometry sensor is one dimensional and gives the position along the
//  intended path. Lane deviation is also one dimensional and gives position
//  normal to the intended path. Odometry, lane following and a digital map
//  should be sufficient for localization.  An odometer will drift. This drift
//  can be nulled out by landmark recognition or GPS.
//
//  An alternative to the Kalman Filter (KF) is fuzzy numbers.
//  The position estimate from most sensors is a pair of fuzzy numbers.
//  [Milan Mares, Computation Over Fuzzy Quantities, CRC Press.]
//  A fuzzy number can be thought of as a triangle whose center point
//  is the crisp position and whose limits are the tolerances to which
//  the position is known.
//
//  GPS provides a pyramid aligned north-south and east-west, in contrast to
//  odometry / lane deviation, which is a pyramid oriented in the direction of
//  vehicle motion. The intersection of two fuzzy sets is their minimum.
//  By taking the minima of all position estimate pyramids, we get a surface
//  describing the possible positions of the vehicle, whichcan be used by a
//  particle filter.
//
//  Serial lines:
//  0: Monitor
//  1: INU
//  2: Tx: Estimated state;
//      $C6EST,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms><positionStndDev_mm>*CKSUM
//      $C6OBS,<number>,<obstacle1_range_mm>,<obstacle1_bearing>, ...*CKSUM
//   Rx: Desired course
//      $C4XPC,<east_mm>,<north_mm>,<speed_mmPs>,<bearing>,<time_ms>*CKSUM
//      // at leat 18 characters
//  3: GPS
//  Rx: $GPRMC,...  typically 68 characters
//      $GPGSA,...
//      $GPGGA,... typically 68 characters
//  Tx: $PSRF103,...
//*/
//
///*---------------------------------------------------------------------------------------*/
////wheel speed code added by shailja
//
//#define CLICK_IN 1
//#define LOOP_TIME_MS 1000
//#define CLICK_TIME_MS 1000
//
//#define WHEEL_DIAMETER_MM 397
//#define MEG 1000000
//#ifndef PI
//#define PI ((float) 3.1415925)
//#endif
//#define MAX_SPEED_KPH 50  
//#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
//// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
//unsigned long MinTickTime_ms;
//// ((WHEEL_DIAMETER_MM * 3142) / MAX_SPEED_mmPs)
//// MinTickTime_ms = 89 ms
//#define MIN_SPEED_mPh 500
//// A speed of less than 0.5 KPH is zero.
//unsigned long MaxTickTime_ms;
//// ((WHEEL_DIAMETER_MM * 3142) / MIN_SPEED_mmPs)
//// MinTickTime_ms = 9239 ms = 9 sec
//
//float Odometer_m = 0;
////long SpeedCyclometer_mmPs = 0; by shailja
//// Speed in revolutions per second is independent of wheel size.
//float SpeedCyclometer_revPs = 0.0;//revolutions per sec
//volatile unsigned long TickTime = 0;
//long WheelRev_ms = 0;
//unsigned long OldTick = 0;
//#define IRQ_NONE 0
//#define IRQ_FIRST 1
//#define IRQ_RUNNING 2
//volatile int InterruptState = IRQ_NONE;
//unsigned long ShowTime_ms;
//
////wheel speed code end 
//
///*---------------------------------------------------------------------------------------*/ 
//// WheelRev is called by an interrupt.
//void WheelRev()
//{
//    static int flip = 0;
//    unsigned long tick;   
//    noInterrupts();
//    tick = millis();
//    if (InterruptState != IRQ_RUNNING)
//    // Need to process 1st two interrupts before results are meaningful.
//        InterruptState++;
//
//    if (tick - TickTime > MinTickTime_ms)
//    {
//        OldTick = TickTime;
//        TickTime = tick;
//    }
//    if (flip)
//        digitalWrite(13, LOW);
//    else
//        digitalWrite(13, HIGH);
//    flip =!flip;  
//    
//    interrupts();
//}
//
//
//
//#include <Adafruit_GPS.h>
////#define DEBUG 1
//#define MEGA
//#include <Common.h>
//#include <SPI.h>
//#include <SD.h>
//#include <IO.h>
//#include <Matrix.h>
//
//#include <Wire.h>
////#include <Adafruit_LSM303.h>
//#include <Adafruit_LSM303_U.h>
//#include <Elcano_Serial.h>
//#include <FusionData.h>
//HardwareSerial mySerial = Serial3;
//Adafruit_GPS GPS(&Serial3);
//#define GPSECHO  false
//uint32_t timer = millis();
//boolean usingInterrupt = false;
///* The global parse state */
//ParseState ps;
//
///* Assign a unique ID to this sensor at the same time */
////Adafruit_LSM303_Mag mag = Adafruit_LSM303_Mag(1366123);
//
////long CurrentHeading = -1;
//
//// On the Ethernet Shield, CS is pin 4. Note that even if it's not
//// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
//// 53 on the Mega) must be left as an output or the SD library
//// functions will not work.
//
//#ifdef SIMULATOR
//#include <Elcano_simulator.h>
//#endif
//
//
//namespace C6_Navigator
//{
//void setup();
//void loop();
//}
//
//// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
//// but some are 38400 or other. Check the datasheet!
//#define GPSRATE 9600
//
//#define MAX_WAYPOINTS 10
///*   There are two coordinate systems.
//     MDF and RNDF use latitude and longitude.
//     C3 and C6 assume that the earth is flat at the scale that they deal with.
//     All calculations with C6 are performed in the (east_mm, north_mm)
//     coordinate system. A raw GPS reading is in latitude and longitude, but it
//     is transformed to the flat earth system using the distance from the origin,
//     as set by (LATITUDE_ORIGIN, LONGITUDE_ORIGIN).
//     A third coordinate system has its origin at the vehicle center and is
//     aligned with the vehicle. This is usually transformed into the flat
//     world coordinate system.
//*/
//
//
//#ifdef SIMULATOR
//namespace C6_Navigator {
//#endif
//// limited to 8+3 characters
//#define FILE_NAME "GPSLog.csv"
//
////File dataFile;
////char GPSfile[BUFFSIZ] = "mmddhhmm.csv";
//char ObstacleString[BUFFSIZ];
//char StartTime[BUFFSIZ] = "yy,mm,dd,hh,mm,ss,xxx";
//const char TimeHeader[] = "year,month,day,hour,minute,second,msec";
//const char* RawKF = "Raw GPS data,,,,,,Kalman Filtered data";
//const char* Header = "Latitude,Longitude,East_m,North_m,SigmaE_m,SigmaN_m,Time_s,";
//const char* ObstHeader = "Left,Front,Right,Busy";
//
//// Added by Varsha
//SerialData C2_Results;
//SerialData C4_Results;
//File logFile;
//
////#define WHEEL_DIAMETER_MM 397
//
///* time (micro seconds) for a wheel revolution */
//volatile long Odometer_mm = 0;
//volatile long SpeedCyclometer_mmPs;
//// Speed in degrees per second is independent of wheel size.
////volatile long SpeedCyclometer_degPs;
//
//// waypoint mission[MAX_WAYPOINTS];
//waypoint GPS_reading;
//waypoint estimated_position;
////instrument IMU;
//const unsigned long LoopPeriod = 100;  // msec//???shailja
//
//// Added by Varsha for localization
//PositionData oldPos, newPos;
//
///* This procedure accepts SerialData structure and prints all the members of the struct to a serial
//   output device. This will be only used for debugging purposes
//
//   Added by Varsha
//*/
////void displayResults(SerialData &Results)
////{
////  Serial.print("SerialData::Kind :" );
////  Serial.println(Results.kind);
////  Serial.print("SerialData::Number:");
////  Serial.println(Results.number);
////  Serial.print("SerialData::speed_cmPs:");
////  Serial.println(Results.speed_cmPs);
////  Serial.print("SerialData::angle_deg:");
////  Serial.println(Results.angle_deg);    // front wheels
////  Serial.print("SerialData::bearing_deg:");
////  Serial.println(Results.bearing_deg);  // compass direction
////  Serial.print("SerialData::posE_cm:");
////  Serial.println(Results.posE_cm);
////  Serial.print("SerialData::posN_cm:");
////  Serial.println(Results.posN_cm);
////  Serial.print("SerialData::probability:");
////  Serial.println(Results.probability);
////}
//
////---------------------------------------------------------------------------
//char* obstacleDetect()
//  {
//  // Calibration shows that readings are 5 cm low.
//  #define OFFSET 5
//    int LeftRange =  analogRead(LEFT) + OFFSET;
//    int Range =      analogRead(FRONT) + OFFSET;
//    int RightRange = analogRead(RIGHT) + OFFSET;
//
//  sprintf(ObstacleString,
//  "%d.%0.2d,%d.%0.2d,%d.%0.2d,",
//  LeftRange/100, LeftRange%100, Range/100, Range%100, RightRange/100, RightRange%100);
//
//  return ObstacleString;
//}
///*---------------------------------------------------------------------------------------*/
///* Feb 23, 2016  TCF.  This instance of WheelRev is depricated.
//   Current code for WheelRev is in Elcano_C2_LowLevel.
//   The odometer processing is part of low level control.
//   C2 will send odometer information to C6 over a serial line. */
//
///*---------------------------------------------------------------------------------------*/
///*  Jun 20, 2016. Varsha - Get heading value from LSM303 sensor
//    To convert the microTesla readings into a 0-360 degree compass heading,
//    we can use the atan2() function to compute the angle of the vector
//    defined by the Y and X axis readings. The result will be in radians,
//    so we multiply by 180 degrees and divide by Pi to convert that to degrees.
//*/
//long GetHeading(void)
//{
//  //Get a new sensor event from the magnetometer
//  sensors_event_t event;
//  mag.getEvent(&event);
//
//  //Calculate the current heading (angle of the vector y,x)
//  //Normalize the heading
//  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / M_PI;
//
//  if (heading < 0)
//  {
//    heading = 360 + heading;
//  }
//  // Converting heading to x1000
//  return ((long)(heading * HEADING_PRECISION ));
//}
//
//
///*---------------------------------------------------------------------------------------*/
//
//void initialize()
//{
//  pinMode(GPS_POWER, OUTPUT);
//  char* GPSString;
//  char* protocol =  "$PSRF100,1,4800,8,1,0*0E"; // NMEA
//  char* disable =   "$PSRF103,02,00,00,01*26\r\n";
//  char* querryGGA = "$PSRF103,00,01,00,01*25";
//  bool GPS_available = false;
//  GPS.begin(GPSRATE);
//  digitalWrite(GPS_POWER, LOW);         // pull low to turn on!
//  Serial.flush();
//  Serial3.flush();
//  delay(5000);
//  // prints title with ending line break
// // Serial.println(" GPS parser");
//  //  Serial.print("Acquiring GPS RMC...");
//  //checksum(protocol);
//  ///Serial3.println(protocol);
//  //disable[10] = '2';
//  //checksum(disable);
// // Serial3.println(disable);   // no GSA
//
//  GPS_available = estimated_position.AcquireGPRMC(70000);
//
//  //Serial.println(TimeHeader);
// // Serial.println(StartTime);
// // Serial.println(RawKF);
//  //Serial.print(Header);
//  //Serial.print(Header);
// // Serial.println(ObstHeader);
//  if (GPS_available)
//  {
//    estimated_position.sigma_mm = 1.0E4; // 10 m standard deviation
//        //Serial.println("OK");
//  }
//  else
//  {
//    estimated_position.latitude = 47.62130;  // set by hand here
//    estimated_position.longitude = -122.35090;
//    estimated_position.Compute_mm();
//    estimated_position.sigma_mm = 1.0E5; // 100 m standard deviation
//    estimated_position.time_ms = millis();
//        //Serial.println("Failed");
//  }
//  // Set velocity and acceleration to zero.
//  estimated_position.speed_mmPs = 0;
//  // Set attitude.
//  estimated_position.Evector_x1000 = 1000;  // to be taken from path or set by hand
//  estimated_position.Nvector_x1000 = 0;
//  GPS_reading = estimated_position;
//  GPSString = estimated_position.formPointString();
//  //Serial.println(GPSString);
//
//  // Set Odometer to 0.
//  // Set lateral deviation to 0.
//  // Read compass.
//  // Added by Varsha - To get heading data
//  CurrentHeading = GetHeading();
//
//  // ReadINU.
//  // SendState(C4);
//  // Wait to get path from C4
//  //    while (mission[1].latitude > 90)
//  {
//    /* If (message from C4)
//      {
//      ReadState(C4);  // get initial route and speed
//      }
//      Read GPS, compass and IMU and update their estimates.
//    */
//  }
//  /* disable GPS messages;
//    for (char i='0'; i < '6'; i++)
//    {
//    disable[9] = i;
//    checksum(disable);
//    Serial3.println(disable);
//    }
//    Serial3.println(querryGGA);
//  */
//  // GPS_available = GPS_reading.AcquireGPGGA(300);
//  // ready to roll
//  // Fuse all position estimates.
//  // Send vehicle state to C3 and C4.
//
//
//}
//
//
///*---------------------------------------------------------------------------------------*/
//
//void setup()
//{
//
//  pinMode(Rx0, INPUT);
//  pinMode(Tx0, OUTPUT);
//  pinMode(GPS_RX, INPUT);
//  pinMode(GPS_TX, OUTPUT);
//  pinMode(C4_RX, INPUT);
//  pinMode(C4_TX, OUTPUT);
//  pinMode(INU_RX, INPUT);
//  pinMode(INU_TX, OUTPUT);
//  pinMode(GPS_POWER, OUTPUT);
// GPS.begin(GPSRATE); // GPS
//   GPS.begin(GPSRATE); // GPS
//  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
//  // Set the update rate
//  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
//  // For the parsing code to work nicely and have time to sort thru the data, and
//  // print it out we don't suggest using anything higher than 1 Hz
//
//  // Request updates on antenna status, comment out to keep quiet
//  GPS.sendCommand(PGCMD_ANTENNA);
//
//  // the nice thing about this code is you can have a timer0 interrupt go off
//  // every 1 millisecond, and read data from the GPS for you. that makes the
//  // loop code a heck of a lot easier!
//  useInterrupt(true);
//
//  delay(1000);
//  // Ask for firmware version
//  mySerial.println(PMTK_Q_RELEASE);
//  Serial.begin(9600);
//  Serial2.begin(9600);
//  digitalWrite(GPS_POWER, LOW);         // pull low to turn on!
//  // make sure that the default chip select pin is set to
//  // output, even if you don't use it:
//  pinMode(chipSelect, OUTPUT);
//  pinMode(53, OUTPUT);  // Unused CS on Mega
//  pinMode(GPS_RED_LED, OUTPUT);
//  pinMode(GPS_GREEN_LED, OUTPUT);
//  digitalWrite(GPS_GREEN_LED, LOW);
//  digitalWrite(GPS_RED_LED, LOW);
//
////speed code added by shailja
//Serial.begin(9600);//serial monitor
//
//    pinMode(13, OUTPUT); //led
//    digitalWrite(13, HIGH);//turn LED on
//    
//    pinMode(2, INPUT);//pulls input HIGH
//    float MinTick = WHEEL_DIAMETER_MM * PI;
////    Serial.print (" MinTick = ");
////    Serial.println (MinTick);
//    MinTick *= 1000.0;
//    MinTick /= MAX_SPEED_mmPs;
////    Serial.print (MinTick);
//    MinTickTime_ms = MinTick;
////    Serial.print (" MinTickTime_ms = ");
////    Serial.println (MinTickTime_ms);
//
////    Serial.print (" MIN_SPEED_mPh = ");
////    Serial.print (MIN_SPEED_mPh);
//    float MIN_SPEED_mmPs =  ((MIN_SPEED_mPh * 1000.0) / 3600.0);
//    // MIN_SPEED_mmPs = 135 mm/s
////    Serial.print (" MIN_SPEED_mmPs = ");
////    Serial.print (MIN_SPEED_mmPs);
//    float MaxTick = (WHEEL_DIAMETER_MM * PI * 1000.0) / MIN_SPEED_mmPs;
////    Serial.print (" MaxTick = ");
////    Serial.print (MaxTick);
//    MaxTickTime_ms = MaxTick;
////    Serial.print (" MaxTickTime = ");
////    Serial.println (MaxTickTime_ms);
//    TickTime = millis();
//    // OldTick will normally be less than TickTime.
//    // When it is greater, TickTime - OldTick is a large positive number,
//    // indicating that we have not moved.
//    // TickTime would overflow after days of continuous operation, causing a glitch of
//    // a display of zero speed.  It is unlikely that we have enough battery power to ever see this.
//    OldTick = TickTime;
//    ShowTime_ms = TickTime;
//    WheelRev_ms = 0;
//    InterruptState = IRQ_NONE;
//#ifdef CLICK_IN
//    attachInterrupt (0, WheelRev, RISING);//pin 2 on Mega
//#endif
//    Serial.println("setup complete");
//    //speed code end by shailja
//
//
//
//  
//
//  // Assigning SerialData and HardwareSerial device to ParseState
//  // for read later in the code
//  ps.dt = &C2_Results;
//  ps.dev = &Serial2;
//  C2_Results.clear();
//
//  /* Initializing old PositionData struct to default values
//      Added by Varsha
//  */
//  oldPos.Clear();
//  oldPos.time_ms = millis();
//
//  //Enable auto-gain
//  mag.enableAutoRange(true);
//
//  //Initialise the sensor
//  if (!mag.begin())
//  {
//    //There was a problem detecting the LSM303 ... check your connections
//  //  Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
//    while (1);
//  }
//  CurrentHeading = GetHeading();
//  //initialize();
//  //Serial.print("Initializing GPS SD card...");
//
//  // see if the card is present and can be initialized:
//  if (!SD.begin(chipSelect))
//  {
//    //Serial.println("Card failed, or not present");
//    digitalWrite(GPS_RED_LED, HIGH);
//  }
//  else
//  {
//    //Serial.println("card initialized.\n");
//    //dataFile = SD.open(GPSfile, FILE_WRITE);
//    // if the file is available, write date and time to it:
////    if (dataFile)
////    {
////      dataFile.println(TimeHeader);
////      dataFile.println(StartTime);
////      //dataFile.println(RawKF);
////      dataFile.print(Header);
////      dataFile.print(Header);
////      //dataFile.println(ObstHeader);
////      dataFile.close();
////    }
//
//    //Sending output to csv file.
//    //logFile = SD.open("LOG.csv", FILE_WRITE);
//    //if (logFile)
//    //{
//    //  String header = "Time,gps_x_pos,gps_y_pos,compass_x_pos,compass_y_pos,Fuzzy_x_pos,Fuzzy_y_pos ";
//    //  logFile.println(header);
//    //  logFile.close();
//    //}
//    //else
//    //{
//    //  Serial.println("Couldn't open log file");
//    //}
//
//  }
//
//  // Commented by Varsha as low-level routine will be used from C2
//  //pinMode(CYCLOMETER, INPUT);
//  //attachInterrupt (5, WheelRev, RISING);
//
//}
///*---------------------------------------------------------------------------------------*/
//void waypoint::SetTime(char *pTime, char * pDate)
//{
//  //GPSfile = "mmddhhmm.CSV";
//  strncpy(GPSfile,   pDate + 2, 2); // month
//  strncpy(GPSfile + 2, pDate, 2);  // day
//  strncpy(GPSfile + 4, pTime, 2);  // GMT hour
//  strncpy(GPSfile + 6, pTime + 2, 2); // minute
//  Serial.println(GPSfile);
//
//  strncpy(StartTime,     pDate + 4, 2); // year
//  strncpy(StartTime + 3,   pDate + 2, 2); // month
//  strncpy(StartTime + 6,   pDate, 2);  // day
//  strncpy(StartTime + 9,   pTime, 2);  // GMT hour
//  strncpy(StartTime + 12,  pTime + 2, 2); // minute
//  strncpy(StartTime + 15,  pTime + 4, 2); // second
//  strncpy(StartTime + 18,  pTime + 7, 3); // millisecond
//}
//// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
//SIGNAL(TIMER0_COMPA_vect) {
//  char c = GPS.read();
//  // if you want to debug, this is a good time to do it!
//#ifdef UDR0
//  if (GPSECHO)
//    if (c) UDR0 = c; 
//    // writing direct to UDR0 is much much faster than Serial.print
//    // but only one character can be written at a time.
//#endif
//}
//
//void useInterrupt(boolean v) {
//  if (v) {
//    // Timer0 is already used for millis() - we'll just interrupt somewhere
//    // in the middle and call the "Compare A" function above
//    OCR0A = 0xAF;
//    TIMSK0 |= _BV(OCIE0A);
//    usingInterrupt = true;
//  } else {
//    // do not call the interrupt function COMPA anymore
//    TIMSK0 &= ~_BV(OCIE0A);
//    usingInterrupt = false;
//  }
//}
//
//void show_speed()
//{
//    ShowTime_ms = millis();  
//   if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)  // no OR 1 interrupts
//   {
//       SpeedCyclometer_mmPs = 0;
//       SpeedCyclometer_revPs = 0;
//   } 
//  // check if velocity has gone to zero
////  Serial.print (" Times: Show: ");
////  Serial.print (ShowTime_ms);
////  Serial.print (" OldTick: ");
////  Serial.print (OldTick);
////  Serial.print (", Tick: ");
////  Serial.println (TickTime);
//
//    if(ShowTime_ms - TickTime > MaxTickTime_ms)
//    {  // stopped
//        SpeedCyclometer_mmPs = 0;
//        SpeedCyclometer_revPs = 0;
//    }
//    else
//    {  // moving
//        WheelRev_ms = max(TickTime - OldTick, ShowTime_ms - TickTime);
//        if (InterruptState == IRQ_RUNNING)
//        {  // have new data
//      
//            float Circum_mm = (WHEEL_DIAMETER_MM * PI);
//            if (WheelRev_ms > 0)
//            {
//                SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
//                SpeedCyclometer_mmPs  = Circum_mm * SpeedCyclometer_revPs;
//            }            else
//            {
//                SpeedCyclometer_mmPs = 0;
//                SpeedCyclometer_revPs = 0;
//            }
//        }
////      Serial.print (" revolutions = ");
////      Serial.print (revolutions);
////      Serial.print (" state = ");
////      Serial.print (InterruptState);
//    }
//  
//    unsigned long WheelRev_s, WheelRevFraction_s;  
////  Serial.print("; WheelRev_ms = ");
////  Serial.println (WheelRev_ms);
////  Serial.print(" ms ");
////  Serial.print("speed: ");
//    Serial.print(SpeedCyclometer_mmPs/1000);
//    Serial.print(".");    
//    Serial.print(SpeedCyclometer_mmPs%1000);
//    Serial.print(" m/s; ");
//    Serial.print(SpeedCyclometer_revPs);
//    Serial.print(" Rev/s");
////  Serial.print(" last interval at: ");
////  unsigned long Interval_ms = ShowTime_ms - TickTime;
////  WheelRev_s = Interval_ms / 1000;
////  WheelRevFraction_s = Interval_ms % 1000;
////  Serial.print (WheelRev_s);
////  Serial.print(".");
////  Serial.print (WheelRevFraction_s);
////  Serial.print(" s ");
//    Serial.print("; ");
//    Serial.print (SpeedCyclometer_mmPs*3600.0/MEG);
//    Serial.print(" km/h, ");
//    Odometer_m += (float)(LOOP_TIME_MS * SpeedCyclometer_mmPs) / MEG;
//    Serial.print(" distance traveled: ");
//    Serial.print (Odometer_m);
//    Serial.println(" m "); 
//}
//
//void computeSpeed() 
//{
//    int i, cycles;
//    unsigned long time, endTime;
//      time = millis();
//    if (LOOP_TIME_MS > CLICK_TIME_MS)
//    {  // high speed
//         cycles = LOOP_TIME_MS / CLICK_TIME_MS;
//         // delay until endTime 
//         // keeps a constant rate of loop calls, but don't count time in loop
//        for (i=0; i<cycles; i++)
//        {
//            endTime = time + CLICK_TIME_MS ;//loop at 0.25 sec
//            while (time < endTime)
//            {
//                time = millis();
//            }
//#ifndef CLICK_IN      
//           WheelRev();
//#endif
//        }
//        show_speed();
//    }
//    else  // low speed
//    {
//        cycles = CLICK_TIME_MS / LOOP_TIME_MS;
//        for (i=0; i<cycles; i++)
//        {
//            endTime = time + LOOP_TIME_MS ;
//            while (time < endTime)
//            {
//                time = millis();
//            }
//            show_speed();
//        }
//#ifndef CLICK_IN      
//        WheelRev();
//        show_speed();
//#endif
//    }
//}
///*---------------------------------------------------------------------------------------*/
//void loop()
//{
//  //Serial.println("Inside C6 loop");
// // unsigned long deltaT_ms;
//  unsigned long time = millis();
//  unsigned long endTime = time + LoopPeriod;
//  unsigned long work_time = time;
//  int PerCentBusy;
//  char* pData;
//  char* pGPS;
//  char* pObstacles;
//    
//  // in case you are not using the interrupt above, you'll
//  // need to 'hand query' the GPS, not suggested :(
//  if (! usingInterrupt) {
//    // read data from the GPS in the 'main loop'
//    char c = GPS.read();
//    // if you want to debug, this is a good time to do it!
//    if (GPSECHO)
//      if (c) Serial.print(c);
//  }
// 
//  // if a sentence is received, we can check the checksum, parse it...
//  if (GPS.newNMEAreceived()) {
//    // a tricky thing here is if we print the NMEA sentence, or data
//    // we end up not listening and catching other sentences!
//    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
//    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
// 
//    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
//      return;  // we can fail to parse a sentence in which case we should just wait for another
//  }
//
//  // if millis() or timer wraps around, we'll just reset it
//  if (timer > millis())  timer = millis();
//
//  // approximately every 2 seconds or so, print out the current stats
//  if (millis() - timer > 2000) {
//    timer = millis(); // reset the timer
//   
//    Serial.print("\nTime: ");
//    Serial.print(GPS.hour, DEC); Serial.print(':');
//    Serial.print(GPS.minute, DEC); Serial.print(':');
//    Serial.print(GPS.seconds, DEC); Serial.print('.');
//    Serial.println(GPS.milliseconds);
//    Serial.print("Date: ");
//    Serial.print(GPS.day, DEC); Serial.print('/');
//    Serial.print(GPS.month, DEC); Serial.print("/20");
//    Serial.println(GPS.year, DEC);
//    Serial.print("Fix: "); Serial.print((int)GPS.fix);
//    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
//  }
//
//      bool GPS_available = GPS.fix;
//      double HDOP = double(GPS.HDOP); 
//      long error_m = sqrt((3.04*HDOP)*(3.04*HDOP)*10000 + 3.57*3.57*10000);
//      long error_mm = 10 * error_m;
//      GPS_reading.sigma_mm = error_mm;
//      
//  Serial.print("AcquireGPGGA:");
//  GPS_reading.latitude =GPS.latitude*(10000);
//  //Serial.println(GPS_available);
//  Serial.print("GPS reading::latitude:");  
//  Serial.print(GPS_reading.latitude);
//  GPS_reading.longitude = GPS.longitude*(-10000);
//  Serial.print(",longitude:");
//  Serial.println(GPS_reading.longitude);
// GPS_reading.Compute_mm();
// //Serial.println(GPS_reading.east_mm);
// 
// 
//    
//  /* Perform dead reckoning from clock and previous state
//    Read compass.
//    ReadINU.
//    Set attitude.
//    Read Hall Odometer;  */
//
//  //IMU.Read(GPS_reading);
//
//  // Added by Varsha - to get heading
//  CurrentHeading = GetHeading();
//  Serial.print("CurrentHeading:");
//  Serial.println(CurrentHeading);
//  // End of changes
//
//  /*    }
//      if (landmarks availabe)
//      {  // get the position based on bearing and angle to a known location.
//        ReadLandmarks(C4);
//      }
//  // Fuse all position estimates with a Kalman Filter */
////  deltaT_ms = GPS_reading.time_ms - estimated_position.time_ms;
////  estimated_position.fuse(GPS_reading, deltaT_ms);
//  estimated_position = GPS_reading;
//  
//  //Populating GPS position from C6 to C2 Results
//  C2_Results.clear();
//  C2_Results.posE_cm = estimated_position.east_mm / 10;
//  C2_Results.posN_cm = estimated_position.north_mm / 10;
////  C2_Results.kind = MSG_SENSOR;
//  
//  // Read data from C2 using Elcano_Serial
//  int r = ps.update();
//  /* Test Code for simulation - Need to be removed
//     Speed is set to constant value at 5 kmPs
//     which is 139 cmPs ( approx )
//  */
//  if (r == PSE_SUCCESS)
//  {
//    // C2_Results is a complete data packet from C2
//  }
//  
//  C2_Results.speed_cmPs = 139;
////  C2_Results.kind = MSG_SENSOR;
//
//  /* Remove till this point */
//  // Uncomment below line
//  //if ( C2_Results.kind == MSG_SENSOR && r == PSE_SUCCESS )
////  if ( C2_Results.kind == MSG_SENSOR )
//  {
//    computeSpeed();
//    newPos.speed_cmPs = SpeedCyclometer_mmPs/10.;
//    newPos.bearing_deg = CurrentHeading;
//    newPos.time_ms = time;
//
//    //position data from compass
//    ComputePositionWithDR(oldPos, newPos);
//
//    // Populate PositionData struct
//    PositionData gps, fuzzy_out;
//
//    gps.x_Pos = estimated_position.east_mm;
//    gps.y_Pos = estimated_position.north_mm;
//    gps.sigma_mm = estimated_position.sigma_mm;
//
//    //#ifdef DEBUG
////      printing the GPS output
//     Serial.print("estimated_position::east_mm:");
//     Serial.print(estimated_position.east_mm);
//     Serial.print(",north_mm:");
//    Serial.print(estimated_position.north_mm);
//     Serial.print(",Sigma:");
//     Serial.println(estimated_position.sigma_mm);
//      Serial.print("estimated_position::latitude_east_mm:");
//      Serial.print(estimated_position.latitude);
//      Serial.print(",longitude_north_mm:");
//      Serial.println(estimated_position.longitude);
//    GPS_reading.Compute_mm();
//    
//      Serial.print("Shailja GPS_reading::east_mm:");
//      Serial.print(GPS_reading.east_mm);
//     Serial.print(",north_mm:");
//     Serial.print(GPS_reading.north_mm);
//     Serial.print(",Sigma:");
//      Serial.println(GPS_reading.sigma_mm);
//   Serial.print("Shailja GPS reading::latitude:");
//     Serial.print(GPS_reading.latitude);
//    Serial.print(",longitude:");
//     Serial.println(GPS_reading.longitude);
//    
//    TranslateCoordinates(newPos, gps, SUBTRACT_FROM_ORIGIN);
//    RotateCoordinates(gps, newPos.bearing_deg, ROTATE_CLOCKWISE);
//    FindFuzzyCrossPointXY(gps, newPos.distance_mm, newPos.bearing_deg, fuzzy_out);
//    RotateCoordinates(fuzzy_out, newPos.bearing_deg, ROTATE_COUNTER_CLOCKWISE);
//    TranslateCoordinates(oldPos, fuzzy_out, ADD_TO_ORIGIN);
//    newPos.x_Pos = fuzzy_out.x_Pos/100 + oldPos.x_Pos;
//    newPos.y_Pos = fuzzy_out.y_Pos/100 + oldPos.y_Pos; 
//    Serial.print("Shailja fussy:");
//    Serial.println(fuzzy_out.x_Pos);
//    Serial.println(fuzzy_out.y_Pos);
//    if (newPos.speed_cmPs !=0){
//    // Swap data to be used
//    CopyData(oldPos, newPos);
//    }
//    
//    Serial.print("oldPos_x:");
//    Serial.println(oldPos.x_Pos);
//    Serial.print("newPos_x:");
//    Serial.println(newPos.x_Pos); 
//
//    Serial.print("oldPos_y:");
//    Serial.println(oldPos.y_Pos);
//    Serial.print("newPos_y:");
//    Serial.println(newPos.y_Pos); 
//
//    // Preparing Result struct to send data to C4
//    C4_Results.clear();
////    C4_Results.kind = MSG_GOAL;
//    C4_Results.bearing_deg = CurrentHeading;
//    
//    // Updating the C4_Results with the
//    // odometer details from C2
//    C4_Results.speed_cmPs = SpeedCyclometer_mmPs/10;
//    // Copy GPS fuzzy output to C4
//    C4_Results.posE_cm = fuzzy_out.x_Pos / 10;
//    C4_Results.posN_cm = fuzzy_out.y_Pos / 10;
//
//    //#ifdef DEBUG
//      Serial.print("fuzzy_out::x_Pos=");
//      Serial.print(fuzzy_out.x_Pos);
//      Serial.print(",y_Pos=");
//      Serial.println(fuzzy_out.y_Pos);
//    //#endif
//
//    //Populating output to csv.
//    //String dataString = String(newPos.time_ms, DEC) + "," + String(gps.x_Pos, DEC) +
//                        "," + String(gps.y_Pos, DEC) + "," + String(newPos.x_Pos, DEC) + "," + String(newPos.y_Pos, DEC) +
//                        "," + String(fuzzy_out.x_Pos, DEC) + "," + String(fuzzy_out.y_Pos, DEC);
//
//    C4_Results.write(&Serial2);
//    
//    //Open a file to write
//    //Only one file can be open at one time
//
//    // if (SD.begin(chipSelect))
//    // {
//    //
//    //     logFile = SD.open("LOG.csv", FILE_WRITE);
//    //     if (logFile)
//    //     {
//    //         logFile.println(dataString);
//    //         logFile.close();
//    //         Serial.println(dataString);
//    //     }
//    //     else
//    //       {
////              Serial.print("Couldn't open the log file - ");
////              Serial.println(dataString);
//     //      }
//    //}
//    //else
//    //{
//    //  Serial.print("Card not initialized - ");
//    //  Serial.println(dataString);
//    //}
//  }
//
//  C2_Results.write(&Serial2);
////
////  Serial.print("time, gps, dt_ms = ");
////  Serial.print(time, DEC); Serial.print(", ");
////  Serial.print(GPS_reading.time_ms, DEC); Serial.print(", ");
////  Serial.println(deltaT_ms, DEC);
//
//  // Send vehicle state to C3 and C4.
//
//  // open the file. note that only one file can be open at a time,
//  // so you have to close this one before opening another.
//  dataFile = SD.open(GPSfile, FILE_WRITE);
//
//  // if the file is available, write to it:
//  if (0)
//  {
//    digitalWrite(GPS_GREEN_LED, HIGH);
//    pGPS = GPS_reading.formPointString();
//    if (dataFile) dataFile.print(pGPS);
//    // print to the serial port too:
//    //Serial.print("pGPS::");    
//    //Serial.println(pGPS);
//    pData = estimated_position.formPointString();
//    if (dataFile) dataFile.print(pData);
////    Serial.print(pData);
////    Serial3.print(pData);  // send data to C4 path planner
//    pObstacles = obstacleDetect();
//    if (dataFile)
//    {
//      dataFile.print(pObstacles);
//    }
////      Serial.print(pObstacles);
//    }
//  else
//  {
//     digitalWrite(GPS_GREEN_LED, LOW);
////     Serial.print("GPS not available");
//  }
//
//  work_time = millis() - time;
//  PerCentBusy = (100 * work_time) / LoopPeriod;
//  if (dataFile)
//  {
//    dataFile.println(PerCentBusy);
//  }
////  Serial.println(PerCentBusy);
//  // if the file didn't open, pop up an error:
//  if (dataFile)
//  {
//    digitalWrite(GPS_RED_LED, LOW);
//    dataFile.close();
//  }
//  else
//  {
//    digitalWrite(GPS_RED_LED, HIGH);
//    //     Serial.println("error opening file");
//  }
//
//  // delay, but don't count time in loop
//  while (time < endTime)
//  {
//    time = millis();
//  }
//
//}
//
//
//#ifdef SIMULATOR
//} // end namespace
//#else
//void Show(char* x)
//{
////  Serial.print(x);
//}
//void Show(REAL x)
//{
////  Serial.print(x);
////  Serial.print(", ");
//}
//#endif
//
///* Entry point for the simulator.
//   This could be done with namespace, but the Arduino IDE does not handle preprocessor statements
//   wrapping a namespace in the standard way.
//*/
//void C6_Navigator_setup() { C6_Navigator::setup(); }
//
//void C6_Navigator_loop() { C6_Navigator::loop(); }
