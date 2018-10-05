/*
 Move Actuator: Reduced version of Elcano_C2_Base for testing actuators
 
 Elcano Contol Module C2 Basic: Bare bones low level control of vehicle.
 
 Outputs are
 1) Analog 0-4 V signal for traction motor speed
 2) Four relay signals for brakes.
 3) PWM signal for steering.
  */

// Input/Output (IO) pin names for the MegaShieldDB printed circuit board (PCB)
// #include <IOPCB.h>
/*==========================================================================*/
// @ToDo: Fix this fix. If there are variant systems, they should be selected
// in Settings.h, and then IO.h can be included *after* that, and test the
// selector, or use values defined in Settings.h.
// Temporary fix 10/7/15:   IOPCB.h is here
/*==========================================================================*/
/* IO_PCB.h:  I/O pin assignments for Arduino Mega 2560 on MegaShieldDB
*/

//#include <Settings.h>

// Values (0-255) represent digital values that are PWM signals used by analogWrite.

// MIN and MAX ACC set the minimum signal to get the motor going, and maximum allowable acceleration for the motor
#define MIN_ACC_OUT 50
#define MAX_ACC_OUT 235

// RIGHT, STRAIGHT, and LEFT TURN_OUT set values to be sent to the steer actuator that changes the direction of the front wheels
#define RIGHT_TURN_OUT 229
#define LEFT_TURN_OUT 127
#define STRAIGHT_TURN_OUT 178

// RIGHT, STRAIGHT, and LEFT TURN_MS are pulse widths in msec received from the RC controller
#define RIGHT_TURN_MS 1000
#define LEFT_TURN_MS 2000
#define STRAIGHT_TURN_MS 1500

// Turn sensors are believed if they are in this range while wheels are straight
// These numbers vary considerably, depending on which sensor angle is set to straight.
#define RIGHT_MIN_COUNT 80
#define RIGHT_MAX_COUNT 284
#define LEFT_MIN_COUNT  80
#define LEFT_MAX_COUNT  284

// Trike specific pins/channels
// Output to motor actuator
#define DAC_CHANNEL 0
// Output to steer actuator
#define STEER_OUT_PIN 7
// Output to brake actuator
//#define BRAKE_OUT_PIN 6
// DISK_BRAKE is deprecated, use BRAKE_OUT_PIN

// Trike-specific physical parameters
#define WHEEL_DIAMETER_MM 482
// Wheel Circumference
#define WHEEL_CIRCUM_MM (long) (WHEEL_DIAMETER_MM * PI)
//    Turning radius in cm.
#define TURN_RADIUS_CM 209
//    Turning speed in degrees per ms.
#define TURN_SPEED_DPMS 29700
//    Smallest change in turning angle, in millidegrees
#define TURN_RESOLUTION_MDEG 60
//    Maximum turning angle, in degrees
#define TURN_MAX_DEG 30
//  Motor
#define MOTOR_POLE_PAIRS 23
// maximum allowed speed
#define MAX_SPEED_KPH 15

// Use DAC A
#define THROTTLE_CHANNEL 0

// ====== End Settings.h ======================

// Define the tests to do.
#define BRAKE_RAMP
#define STEER_RAMP
#define MOTOR_RAMP
// If operating with the MegaShieldDB, we can use the Digital Analog Converter to move the vehicle
#define DAC

/*
The Mega is designed to be used with a data-logging shield.
The nonMega shield uses A4 and A5 for RTC and D10,11,12,and 13 for MOSI data logging.
*/

// @ToDo: There are declarations here that are per-trike, and some that are common.
// Parameters have been added to Settings.h for the per-trike values.
// Should the common parameters also be defined in Settings.h?

// DIGITAL I/O ----------------------------------------------

// D0-7 Connector -------------------------------
// On the Mega, any of D0 to D13 can be PWM.
/* D0 is (Rx0) Read Serial Data. */
const int Rx0 = 0;      // external input
/* [out] Digital Signal 1: (Tx0). Transmit Serial Data.  */
const int Tx0 = 1;      // external output

// This is the 5V supply produced by the E-bike controller.
//  If it goes away, 36V power has been turned off (presumably by the rider's key switch).
//const int EStop =   2;         // external input
const int WheelClick = 2;      //  was 3; interrupt; Reed switch generates one pulse per rotation.

// D8-13 Connector ----------------------
// The shield does not provide a socket for D8-13
// D8 is spare on X4-2;  D9 is spare on X5-2.
// Signals D10-13 are not brought out on connectors, and may be used by a shield.
const int SDchipSelect = 10;  // for some shields
const int LED = 13;

// D14 - 21 Connector -------------------------------------
const int RxD2 = 14;      // reserved external input on X2-19
// Message in Gamebots format giving wheel spin rate and steering angle.
const int TxD2 = 15;      // Cruise Drive Command
const int RxD3 = 16;      // available on X2-22
const int TxD3 = 17;      // available on X2-20
/*  
The Stop signal is a momentary button push from either the console or remote.
A rising edge produces an interrupt.
The Cruise button works the same way.
The ~Estop signal is the 5V supply produced by the motor controller.
This supply goes away when either the key switch is turned off or RC4 is pressed.
Lack of 5V from the motor controller is an emergency stop.
Interrupts are on 2,3,18,19,20 and 21.
*/
const int SelectCD     = 49;  // Select IC 3 DAC (channels C and D)
const int ThrottleMISO = 50;
const int ThrottleMOSI = 51;
const int ThrottleSCK  = 52;
const int SelectAB     = 53;  // Select IC 2 DAC (channels A and B)

/*==========================================================================*/
// End of IOPCB.h
/*==========================================================================*/

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


// Values to send over DAC
const int FullThrottle =  MAX_ACC_OUT;   // 3.63 V
const int MinimumThrottle = MIN_ACC_OUT;  // Throttle has no effect until 1.2 V
// Values to send on PWM to get response of actuators
// Steering
const int HardLeft = LEFT_TURN_OUT;
const int Straight = STRAIGHT_TURN_OUT;
const int HardRight = RIGHT_TURN_OUT;

// set the initial positions
int ThrottlePosition = MinimumThrottle;
int SteerPosition = Straight;

int SteerIncrement = 1;
int ThrottleIncrement = 1;


/*  Elcano #1 Servo range is 100 mm for steering.

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

/* Solenoid controlled Brakes.
 *  
 *  Solenoid (Johnson Electic model 150 174432-024) 
 *  can be kept at lower voltage (12V) indefinitely. 
 *  It has a holding force of 14.5 lb.
 *  At the higher voltage, data sheet expects it to be high (24V) for
 *  25% of the time and low for 75%. However, it can stay high for 100 sec.
 *  The solenoid typically reacts in less than a second.
 *  For 0.25 inch throw and 24V it can pull 7 lb.
 *  We are using the part with 0.3 inch throw, 12V in low state and 36V in high
 *  state, but keeping voltage high for only a second or two.
 *  This appears to be within the part's capabilities.
 *  
 *  The solenoid brake on Elcano replaces the earlier linear actuator.
 *  Solenoid advantages:
 *      Faster: Typically 0.2 seconds vs 2 in/sec for no load. 
 *      Slightly Lighter: 482 grams each vs. 1.16 kg
 *      Available; Linear actuator is no longer available for 2 or 4 inch throw.
 *      Less expensive: 2 * $70 vs. $250
 *      More durable; 5 of the linear actutors have failed.
 *  Linear actuator advantages:
 *      Single unit can pull both brakes (25 lb. thrust); solenoid requires a unit for each brake.
 *      No restriction on throw length; solenoid will not work with > 0.3 inch throw.
 *      Maintains last position without power.   
 *      Do not need to worry about heat dissipation.
 *      
 *  The linear actuator was controlled by pulse width on one digital line.    
 *  The solenoids are controlled by relays on two digital lines.
 *      
 *    Tyler Folsom   April 2018  
 *    
     *  Expected behavior
     *  When Green LED is on, NO is connected to COM; NC is not
     *  Writing HIGH to a relay will turn LED on, and connect NO to COM
     *  When green LED is off, NC is connected to COM; NO is not.
     *  Writing LOW to a relay will turn LED off, and connect NC to COM.
     *  You shoud hear a click when the relay operates.
     *  If there is a change in LED, but no click, the relay does not have enough power.
     */
   
class Brakes
{
 public:
  Brakes();
  void Stop();
  void Release();
  void Check();
 private:
  enum brake_state {BR_OFF, BR_HI_VOLTS, BR_LO_VOLTS} state;
  unsigned long clock_hi_ms;
  const int LeftBrakeOnPin = 10;
  const int RightBrakeOnPin = 4;
  const int LeftBrakeVoltPin = 8;
  const int RightBrakeVoltPin = 7;
  const unsigned long MaxHi_ms = 800;
 } ;

// For normal operation
const long int loop_time_ms = 100;  // Limits time in the loop.

 Brakes brake = Brakes();
 

/*---------------------------------------------------------------------------------------*/
void setup()
{
    //Set up pin modes and interrupts, call serial.begin and call initialize.
    Serial.begin(9600);
    
    // SPI: set the slaveSelectPin as an output:
    pinMode (SelectAB, OUTPUT);
    pinMode (SelectCD, OUTPUT);
    pinMode (10, OUTPUT);
    SPI.setDataMode( SPI_MODE0);
    SPI.setBitOrder( MSBFIRST);
    // initialize SPI:
    // The following line should not be neccessary. It uses a system library.
//    PRR0 &= ~4;  // turn off PRR0.PRSPI bit so power isn't off
    SPI.begin(); 
    for (int channel = 0; channel < 4; channel++)
        DAC_Write (channel, 0);   // reset did not clear previous states
 
    pinMode(STEER_OUT_PIN, OUTPUT);

    brake.Release();   // release brake
    moveSteer(SteerPosition);
    moveVehicle(MinimumThrottle); 
    Serial.println("Initialized");
    Serial.print("Left\t");   
    Serial.print("Right\t");
    Serial.println("Time");   
}
/*---------------------------------------------------------------------------------------*/
void loop()
{
  static int BrakeStep;
  // Apply actuators no faster than MinTimeStep_msec
  const unsigned long MinTimeStep_msec = 100;
  unsigned long delay_ms = 0;
  unsigned long timeStart_ms = millis();
// Put no new code before here
  
  // apply throttle
#ifdef MOTOR_RAMP
    ThrottlePosition += ThrottleIncrement;
    if (ThrottlePosition > FullThrottle || ThrottlePosition < MinimumThrottle)
        ThrottleIncrement = -ThrottleIncrement; 
    moveVehicle(ThrottlePosition);
#endif  // MOTOR_RAMP  
  
  // apply brakes
#ifdef BRAKE_RAMP
 if (BrakeStep == 1)
   brake.Stop();
 if (BrakeStep == 40)
   brake.Release();
 brake.Check();
 if (++BrakeStep > 80)
    BrakeStep = 0;
 
#endif  // BRAKE_RAMP  
  
 // apply steering
#ifdef STEER_RAMP
    SteerPosition += SteerIncrement;
    if (SteerPosition > HardRight || SteerPosition < HardLeft)
        SteerIncrement = -SteerIncrement;
    moveSteer(SteerPosition);
#endif  // Steer_RAMP
  outputToSerial();

// Put no new code after here.
  unsigned long execution_ms = millis() - timeStart_ms;
  delay_ms = MinTimeStep_msec - execution_ms;
  if (delay_ms > 0) delay(delay_ms);
}
/*---------------------------------------------------------------------------------------*/
void moveSteer(int i)
{
     Serial.print ("Steer "); Serial.print(i);
     Serial.print (" on ");   Serial.println (STEER_OUT_PIN);
     analogWrite(STEER_OUT_PIN, i);
}
/*---------------------------------------------------------------------------------------*/
void outputToSerial()
{
#ifdef MOTOR_RAMP
  //put output data for motor here
#endif  // MOTOR_RAMP  
  
#ifdef BRAKE_RAMP
  //put output data for brake here
#endif  // BRAKE_RAMP

#ifdef STEER_RAMP 
     int left = analogRead(A2);               //Steer
     int right = analogRead(A3);
     Serial.print(left);   Serial.print("\t"); //Left turn sensor
     Serial.print(right);  Serial.print("\t"); //Right turn sensor
#endif //STEER_RAMP
  Serial.println(micros()); //Current time and end line
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
     Serial.print ("Motor "); Serial.print(counts);
     Serial.print (" on ");   Serial.println (THROTTLE_CHANNEL);
#ifdef DAC      
   DAC_Write(THROTTLE_CHANNEL, counts);
#endif
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
 
Brakes::Brakes()
{
  pinMode( LeftBrakeOnPin, OUTPUT);
  pinMode( RightBrakeOnPin, OUTPUT);
  pinMode( LeftBrakeVoltPin, OUTPUT);
  pinMode( RightBrakeVoltPin, OUTPUT);
  clock_hi_ms = millis();
  state = BR_OFF;
}
void Brakes::Release()
{
  /*  Expected behavior:
   * LEDs go off for relays 2 and 3;
   * Relay 2 has NO (connected to solenoids) open, and there is no power to solenoids.
   * Relay 3 connects COM (other end of solenoid) to NO (12V) 
   */
  digitalWrite(LeftBrakeOnPin, LOW);
  digitalWrite(LeftBrakeVoltPin, LOW);
  digitalWrite(RightBrakeOnPin, LOW);
  digitalWrite(RightBrakeVoltPin, LOW);
  state = BR_OFF;
}
void Brakes::Stop()
{
  /*  Expected behavior:
   *  Both LEDs come on for Relays 2 and 3
   *  Relay 2 connects NO (solenoids) to COM (ground)
   *  Relay 3 connects COM (other end of solenoids) to NC (36V)
   */
  digitalWrite(LeftBrakeVoltPin, HIGH);  // Need the higher voltage to activate the solenoid.
  digitalWrite(RightBrakeVoltPin, HIGH); 
  if (state != BR_HI_VOLTS)
  {
    clock_hi_ms = millis();  // keep track of when the higher voltage was applied.
  }
  digitalWrite(LeftBrakeOnPin, HIGH); // Activate solenoid to apply brakes.
  digitalWrite(RightBrakeOnPin, HIGH);
  state = BR_HI_VOLTS;
}
void Brakes::Check()
{
  /* Expected behavior
   *  If 36V has been on too long, relay 3 changes LED on to off, switching from 24 to 12V
   *  If the switch is high, brakes will be released, with both LEDs off.
   */
 
  unsigned long tick = millis();
  if (state == BR_HI_VOLTS && tick - clock_hi_ms > MaxHi_ms)
  {  // Have reached maximum time to keep voltage high
    digitalWrite(LeftBrakeVoltPin, LOW); // Set to lower voltage, which will keep brakes applied
    digitalWrite(RightBrakeVoltPin, LOW);
    state = BR_LO_VOLTS;
  }
}
