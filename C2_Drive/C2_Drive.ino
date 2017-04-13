    /*
 Elcano Contol Module C2 Basic: Bare bones low level control of vehicle.
 This code is the first step in moving the vehicle under high level control from the C3 Pilot. 
 
 Throttle, brake and steering inputs can come from three sources: 
 1) On board joystick
     e.g. 
 2) Remotely controlled (RC) joystick and buttons
     e.g: HiTec Optic 5 RCS: http://hitecrcd.com/images/products/pdf/22_OPTIC_5_MANUAL_P.pdf
 3) An external high level computer, such as Arduino C3 Pilot.
 
 Normally the vehicle will be configured to use either (1) or (2) for manual control. 
 Automatic control (3) is always an option.

 
 The inputs consist of analog or Pulse wave Modulated (PWM) values.  Inputs are:
 1) Throttle
 2) Brake
 3) Steering
 4) a digital emergency stop (E-stop) that kills 36V power to the traction motor.
    The E-stop is electronic only, and will function if the Elcano_C2_Basic firmware is unresponsive.
    If E-stop is detected by software, it will apply brakes. Software will attempt to turn off the motor.
 5) a digital stop command.
    If the vehicle is under computer control, it will enter manual mode and begin to stop.
    Any manual activation of throttle will abort the stop.
 6) A digital start command.
    This turns control over to a high level computer and begins motion to the next waypoint.
    Waypoints are loaded into the higher level computer and are not directly settable for the C2 module.
    Once under computer control, manual control will be unresponsive.
   
   Future enhancements can make transfer between manual and automatic control similar to automotive 
   cruise control. 
 
 Outputs are
 1) Analog 0-4 V signal for traction motor speed
 2) Pulse wave Modulated (PWM) signal for brakes.
 3) PWM signal for steering.
 
 7/1/15  TCF Added ThrottleChannel
 */

// Input/Output (IO) pin names for the MegaShieldDB printed circuit board (PCB)
#include <IOPCB.h>

// When setting up the project, select
//   Sketch  |  Import Library ... |  SPI
// include the Serial Periferal Interface (SPI) library:
#include <SPI.h>
// The MegaShieldDB has a four channel Digital to Analog Converter (DAC).
// Basic Arduino cannot write a true analog signal, but only PWM.
// Many servos take PWM.
// An electric bicycle (E-bike) throttle expects an analog signal.
// We have found that feeding a pwm signal to an e-bike controller makes the motor chug at low speed.

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#ifndef NaN
#define NaN 0xFFFFFFFF
#endif

#define LOOP_TIME_MS 100
#define WHEEL_DIAMETER_MM 397
#define BUFFER_SIZE 80
#define ERROR_HISTORY 20

// Values to send over DAC
const int FullThrottle =  227;   // 3.63 V
const int MinimumThrottle = 70;  // Throttle has no effect until 1.2 V
// Values to send on PWM to get response of actuators
const int FullBrake = 167;  // start with a conservative value; could go as high as 255;  
const int NoBrake = 207; // start with a conservative value; could go as low as 127;
// Steering
const int HardLeft = 250; //  could go as high as 255;
const int Straight = 187;
const int HardRight = 126;

// globals
long sensor_speed_mmPs = 0;
long drive_speed_mmPs = 0;
char IncomingMessage[BUFFER_SIZE];
int  InIndex=0;
int  throttle_control = MinimumThrottle;
int  brake_control = FullBrake;
int  steer_control = Straight;
long speed_errors[ERROR_HISTORY];


/*  Elcano #1 Servo range is 50 mm for brake, 100 mm for steering.

    Elcano servo has a hardware controller that moves to a
    particular position based on an input PWM signal from Arduino.
    The Arduino PWM signal is a square wave at a base frequency of 490 Hz or 2.04 ms.
    PWM changes the duty cycle to encode   
    0 is always off; 255 always on. One step is 7.92 us.
    
    Elcano servo is fully retracted on a pulse width of 2 ms;
    fully extended at 1 ms and centered at 1.5 ms.
    There is a deadband of 8 us.
    At 12v, servo operating speed is 56mm/s with no load or
    35 mm/s at maximum load.
    
    Output from hardware servo controller to either servo has five wires, with observed bahavior of:
    White: 0V
    Yellow: 5V
    Blue: 0-5V depending on position of servo.
    Black: 12V while servo extends; 0V at rest or retracting.
    Red:   12V while retracting; 0V at rest or extending.
    The reading on the Blue line has hysteresis when Elcano sends a PWM signal; 
    there appear to be different (PWM, position) pairs when retracting or extending.
    Motor speed is probably controlled by the current on the red or black line.   
*/
/*---------------------------------------------------------------------------------------*/
void initialize()
{
  for (int i = 0; i < ERROR_HISTORY; i++)
  {
      speed_errors[i] = 0;
  }
 
}
/*---------------------------------------------------------------------------------------*/
void setup()
{
    //Set up pin modes and interrupts, call serial.begin and call initialize.
    Serial.begin(115200); // monitor
    Serial3.begin(115200); // C3 to C2;  C2 to C6
	/* A typical message is 20 ASCII characters of 20 bits each
	   (2 8-bit bytes / character + start bit + stop bit)
	   At 115,200 bits/s, a typical message takes 3.5 ms.
	   Thus there is about a 10 ms delay (best case) in passing 
	   information from C6 to C2.
	*/
    
    // SPI: set the slaveSelectPin as an output:
    pinMode (SelectAB, OUTPUT);
    pinMode (SelectCD, OUTPUT);
    pinMode (10, OUTPUT);
    SPI.setDataMode( SPI_MODE0);
    SPI.setBitOrder( MSBFIRST);
    // initialize SPI:
    SPI.begin(); 
    for (int channel = 0; channel < 4; channel++)
        DAC_Write (channel, 0);   // reset did not clear previous states
 
    pinMode(Throttle, OUTPUT);
    pinMode(DiskBrake, OUTPUT);
    pinMode(Steer, OUTPUT);
    pinMode(AccelerateJoystick, INPUT);
    pinMode(SteerJoystick, INPUT);
    pinMode(JoystickCenter, INPUT);

    Serial.println("Start initialization");        
    initialize();
     
    moveBrake(NoBrake);   // release brake
    Serial.println("Initialized");
     
}
/*---------------------------------------------------------------------------------------*/
char * GetWord(char * major, char * str)
{
	char * CSp1;

	CSp1 = strstr(str, major);
	if (CSp1!=NULL)
	CSp1 += strlen(major);
	return CSp1;
}
float GetNumber(char *minor, char*Args)
{
  float data = NaN;
  if (Args == NULL) return data;
  // SENSOR, so grab the new sensor_speed.
  char * Number = GetWord(minor, Args);
  if (Number==NULL) return data;
   // change } to 0
   char* end = strchr(Number, '}');
   if (end == NULL) return NaN;
   *end = '\0';
   data = atof(Number);
   // change back to }
   *end = '}';
   // convert speed from km/h to mm/s
//   sensor_speed_mmPs = (long)(data * 1000000.0 / 3600.0);
   return data;
}
/*---------------------------------------------------------------------------------------*/
#define MSG_NONE 0
#define MSG_SENSOR 1
#define MSG_DRIVE 2
int ProcessMessage ()
{
    int kind = MSG_NONE;
    float data;
	// Determine if message is "SENSOR {Speed xxx.xx}"	
	char * Args = GetWord ("SENSOR", IncomingMessage);
	if (Args != NULL)
	{	
            data = GetNumber("Speed", Args);
    	    // convert speed from km/h to mm/s
    	    if (data != NaN) 
            {
                sensor_speed_mmPs = (long)(data * 1000000.0 / 3600.0);
                kind = MSG_SENSOR;
            }
	}
	// Determine if message is "DRIVE {Speed xxx.xx}"	
        Args = GetWord ("DRIVE", IncomingMessage);
	if (Args != NULL)
	{	
            data = GetNumber("Speed", Args);
    	    // convert speed from rev/s to mm/s
    	    if (data != NaN) 
            {
                drive_speed_mmPs = (long)(data * PI * WHEEL_DIAMETER_MM);
                kind = MSG_DRIVE;
            }
	}
    return kind;
}
/*---------------------------------------------------------------------------------------*/
void Throttle_PID(long error_speed_mmPs)

/* Use throttle and brakes to keep vehicle at a desired speed.
   A PID controller uses the error in the set point to increase or decrease the juice.
   P = proportional; change based on present error
   I = integra;  change based on recent sum of errors
   D = derivative: change based on how error is changing.
   The controller needs to avoid problems of being too sluggish or too skittery.
   A sluggish control (overdamped) takes too long to reach the set-point. 
   A skitterish control (underdamped) can overshoot, then undershoot, producing 
   oscillations and jerky motion.
   Getting the right control is a matter of tuning right parts of P, I, and D,
   which is something of a black art.
   For more information, search for:
   VanDoren Proportional Integral Derivative Control
*/
{ 
  static long speed_errors[ERROR_HISTORY];
  static int error_index = 0;
  int i;
  static long error_sum = 0;
  long mean_speed_error = 0;
  long extrapolated_error = 0;
  long PID_error;
  const float P_tune = 0.4;
  const float I_tune = 0.5;
  const float D_tune = 0.1;
  const long speed_tolerance_mmPs = 75;  // about 0.2 mph
  // setting the max_error affacts control: anything bigger gets maximum response
  const long max_error_mmPs = 2500; // about 5.6 mph

  error_sum -= speed_errors[error_index];
  speed_errors[error_index] = error_speed_mmPs;
  error_sum += error_speed_mmPs;
  mean_speed_error = error_sum / ERROR_HISTORY;
  i = (error_index-1) % ERROR_HISTORY;
  extrapolated_error = 2 * error_speed_mmPs - speed_errors[i];
  
  PID_error = P_tune * error_speed_mmPs 
            + I_tune * mean_speed_error
            + D_tune * extrapolated_error;
  if (PID_error > speed_tolerance_mmPs)
  {  // too fast
    long throttle_decrease = (FullThrottle - MinimumThrottle) * PID_error / max_error_mmPs;
    throttle_control -= throttle_decrease;
    if (throttle_control < MinimumThrottle)
        throttle_control = MinimumThrottle;
    moveVehicle(throttle_control);
    
    long brake_increase = (NoBrake - FullBrake) * PID_error / max_error_mmPs;
    // NoBrake = 207; FullBrake = 167; 
    brake_control -= brake_increase;
    if (brake_control < FullBrake)
        brake_control = FullBrake;
    moveBrake(brake_control);
  }
  else if (PID_error < speed_tolerance_mmPs)
  {  // too slow
    long throttle_increase = (FullThrottle - MinimumThrottle) * PID_error / max_error_mmPs;
    throttle_control += throttle_increase;
    if (throttle_control > FullThrottle)
        throttle_control = FullThrottle;
    moveVehicle(throttle_control);
    
    // release brakes
    long brake_decrease = (NoBrake - FullBrake) * PID_error / max_error_mmPs;
    // NoBrake = 207; FullBrake = 167; 
    brake_control += brake_decrease;
    if (brake_control > NoBrake)
        brake_control = NoBrake;
    moveBrake(brake_control);
  }
  // else maintain current speed
  //Serial.print("Thottle Brake,");  // csv for spreadsheet
  Serial.print(throttle_control);
  Serial.print(",");
  Serial.print(brake_control);
  Serial.print(",");
  Serial.print(drive_speed_mmPs);  Serial.print(",");
  Serial.println(sensor_speed_mmPs); 
}

/*---------------------------------------------------------------------------------------*/
void loop()
{
    int incomingByte = 0;   // for incoming serial data
    unsigned long time, endTime;
    time = millis();
    endTime = time + LOOP_TIME_MS ;
    while (time < endTime)
    {
	if ( Serial3.available() > 0) 
	{
	    // read the incoming byte from C4:
	    incomingByte =  Serial3.read();
	    {
		IncomingMessage[InIndex] = (char)(incomingByte);
		if (IncomingMessage[InIndex] == '\0'
		 || InIndex >= BUFFER_SIZE-1)
		{
		    int kind = ProcessMessage();  // see what we got
		    InIndex = 0;
                    if (kind != MSG_SENSOR)  // Sensor messages originate from C6
		        Serial3.print(IncomingMessage); // pass msg on to C6
                    Serial.println(IncomingMessage);  // for monitor
		}
		else
		{
		    ++InIndex;    	
		}
	    }
	}
	time = millis();
    }
 
  long error_speed_mmPs = drive_speed_mmPs - sensor_speed_mmPs;
  Throttle_PID(error_speed_mmPs);
  
 // apply steering
  steer_control =  Straight;
  moveSteer(steer_control); 
 
}
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/ 
void moveBrake(int i)
{
     analogWrite(DiskBrake, i);
}
/*---------------------------------------------------------------------------------------*/
void moveSteer(int i)
{
     analogWrite(Steer, i);
}
/*---------------------------------------------------------------------------------------*/
void moveVehicle(int counts)
{
    /* Observed behavior on ElCano #1 E-bike no load (May 10, 2013, TCF)
      0.831 V at rest       52 counts
      1.20 V: nothing       75
      1.27 V: just starting 79
      1.40 V: slow, steady  87
      1.50 V: brisker       94
      3.63 V: max          227 counts     
      255 counts = 4.08 V      
      */
   DAC_Write(ThrottleChannel, counts);
}
/*---------------------------------------------------------------------------------------*/
/* DAC_Write applies value to address, producing an analog voltage.
// address: 0 for chan A; 1 for chan B; 2 for chan C; 3 for chan D
// value: digital value converted to analog voltage
// Output goes to mcp 4802 Digital-Analog Converter Chip via SPI
// There is no input back from the chip.
*/
void DAC_Write(int address, int value)

/*
REGISTER 5-3: WRITE COMMAND REGISTER FOR MCP4802 (8-BIT DAC)
A/B  —  GA  SHDN  D7 D6 D5 D4 D3 D2 D1 D0 x x x x
bit 15                                       bit 0

bit 15   A/B: DACA or DACB Selection bit
         1 = Write to DACB
         0 = Write to DACA
bit 14   — Don’t Care
bit 13   GA: Output Gain Selection bit
         1 = 1x (VOUT = VREF * D/4096)
         0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
bit 12   SHDN: Output Shutdown Control bit
         1 = Active mode operation. VOUT is available. 
         0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
         VOUT pin is connected to 500 k (typical)
bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.


With 4.95 V on Vcc, observed output for 255 is 4.08V.
This is as documented; with gain of 2, maximum output is 2 * Vref

*/

{
  int byte1 = ((value & 0xF0)>>4) | 0x10; // acitve mode, bits D7-D4
  int byte2 = (value & 0x0F)<<4;           // D3-D0
  if (address < 2)
  {
      // take the SS pin low to select the chip:
      digitalWrite(SelectAB,LOW);
      if (address >= 0)
      { 
        if (address == 1)
          byte1 |= 0x80;  // second channnel
        SPI.transfer(byte1);
        SPI.transfer(byte2);
       }
      // take the SS pin high to de-select the chip:
      digitalWrite(SelectAB,HIGH);
  }
  else
  {
      // take the SS pin low to select the chip:
      digitalWrite(SelectCD,LOW);
      if (address <= 3)
      {
        if (address == 3)
          byte1 |= 0x80;  // second channnel
        SPI.transfer(byte1);
        SPI.transfer(byte2);
      }
       // take the SS pin high to de-select the chip:
      digitalWrite(SelectCD,HIGH);
  }
}
/*---------------------------------------------------------------------------------------*/ 


