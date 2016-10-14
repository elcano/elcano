#include <PID_v1.h>
#include <Settings.h>
#include <SPI.h>
#include <Servo.h>
Servo STEER_SERVO;



#define MEG 1000000
#define MAX_SPEED_KPH 50
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)

// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
unsigned long MinTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MAX_SPEED_mmPs)
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 3000
// A speed of less than 0.3 KPH is zero.
unsigned long MaxTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MIN_SPEED_mmPs)
// MinTickTime_ms = 9239 ms = 9 sec

const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)

double SpeedCyclometer_mmPs = 0; //Note: doubles on Arduinos are the same thing as floats, 4bytes, single precision
// Speed in revolutions per second is independent of wheel size.
//float SpeedCyclometer_revPs = 0.0;//revolutions per sec

#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_SECOND 2
#define IRQ_RUNNING 3
#define NO_DATA 0x7FFFFFFF
volatile byte InterruptState = IRQ_NONE;  // Tells us if we have initialized.
volatile byte ClickNumber = 0;         // Used to distinguish old data from new.
volatile unsigned long TickTime = 0;  // Time from one wheel rotation to the next gives speed.
volatile unsigned long OldTick = 0;

static struct hist {
  long olderSpeed_mmPs;  // older data
  unsigned long olderTime_ms;   // time stamp of older speed

  long oldSpeed_mmPs;  // last data from the interrupt
  byte oldClickNumber;
  unsigned long oldTime_ms;  // time stamp of old speed

  byte nowClickNumber;
  unsigned long nowTime_ms;
  unsigned long TickTime_ms;  // Tick times are used to compute speeds
  unsigned long OldTick_ms;   // Tick times may not match time stamps if we don't process
  // results of every interrupt
} history;


double PIDThrottleOutput; //used to tell Throttle and Brake what to do as far as acceleration
double desiredSpeed = 2800.0; //aprox 10kph
#define PID_CALCULATE_TIME 50

double proportionalConstant = .0175;
double integralConstant = .0141;
double derivativeConstant = .00001;

// PID setup block
PID speedPID(&SpeedCyclometer_mmPs, &PIDThrottleOutput, &desiredSpeed, proportionalConstant, integralConstant, derivativeConstant, DIRECT);



//...................................................................................

void setup(){
  Serial.begin(9600);
  SPI.begin(); 
  speedPID.SetOutputLimits(MIN_ACC_OUT, MAX_ACC_OUT); //useful if we want to change the limits on what values the output can be set to
  speedPID.SetSampleTime(PID_CALCULATE_TIME); //useful if we want to change the compute period
  setupWheelRev();
  //attachInterrupt(digitalPinToInterrupt(IRPT_RDR), ISR_RDR_rise, RISING);
  STEER_SERVO.attach(STEER_OUT_PIN);
  STEER_SERVO.writeMicroseconds(STRAIGHT_TURN_OUT);
}

void loop(){
  computeSpeed(&history);
  PrintSpeed(history);
  ThrottlePID();
//  Serial.print("Int State ");
//  Serial.println(InterruptState);
//  Serial.print("Click ");
//  Serial.println(ClickNumber);
  //delay(50); 
}

void ThrottlePID(){
  
  speedPID.Compute();

  Serial.print("Throttle out value ");
  Serial.println(PIDThrottleOutput);
  int throttleControl = (int)PIDThrottleOutput;
  moveVehicle(throttleControl);

  if(PIDThrottleOutput == MIN_ACC_OUT){
    //apply brakes
    //brake(MAX_BRAKE_OUT);
  }
  else{
    //brake(MIN_BRAKE_OUT);
  }
  
  return;
}


void PrintSpeed(struct hist data)
{
  Serial.print(SpeedCyclometer_mmPs); Serial.print("\t");
  Serial.println();
}

void ISR_RDR_rise(){
  unsigned long tick;
  noInterrupts();
  tick = millis();
  if (InterruptState != IRQ_RUNNING){
    // Need to process 1st two interrupts before results are meaningful.
    InterruptState++;
  }
  //checks to see if the time between cycles isn't impossibly short.
  if (tick - TickTime > MinTickTime_ms){
    OldTick = TickTime;
    TickTime = tick;
    ++ClickNumber;
  }
  interrupts();
}

void setupWheelRev()
{
 // pinMode(IRPT_WHEEL, INPUT);//pulls input HIGH
  float MinTick = WHEEL_CIRCUM_MM;
  //    SerialMonitor.print (" MinTick = ");
  //    SerialMonitor.println (MinTick);
  MinTick *= 1000.0;
  MinTick /= MAX_SPEED_mmPs;
  //    SerialMonitor.print (MinTick);
  MinTickTime_ms = MinTick;
  //    SerialMonitor.print (" MinTickTime_ms = ");
  //    SerialMonitor.println (MinTickTime_ms);

  //    SerialMonitor.print (" MIN_SPEED_mPh = ");
  //    SerialMonitor.print (MIN_SPEED_mPh);
  float MIN_SPEED_mmPs =  ((MIN_SPEED_mPh * 1000.0) / 3600.0);
  //    MIN_SPEED_mmPs = 135 mm/s
  //    SerialMonitor.print (" MIN_SPEED_mmPs = ");
  //    SerialMonitor.print (MIN_SPEED_mmPs);
  float MaxTick = (WHEEL_DIAMETER_MM * PI * 1000.0) / MIN_SPEED_mmPs;
  //    SerialMonitor.print (" MaxTick = ");
  //    SerialMonitor.print (MaxTick);
  MaxTickTime_ms = MaxTick;
  //    SerialMonitor.print (" MaxTickTime = ");
  //    SerialMonitor.println (MaxTickTime_ms);
  TickTime = millis();
  // OldTick will normally be less than TickTime.
  // When it is greater, TickTime - OldTick is a large positive number,
  // indicating that we have not moved.
  // TickTime would overflow after days of continuous operation, causing a glitch of
  // a display of zero speed.  It is unlikely that we have enough battery power to ever see this.
  OldTick = TickTime;
  InterruptState = IRQ_NONE;
  ClickNumber = 0;
  history.oldSpeed_mmPs = history.olderSpeed_mmPs = NO_DATA;

  speedPID.SetMode(AUTOMATIC); //initializes PID controller and allows it to run Compute
  
  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), ISR_RDR_rise, RISING);//pin 3 on Mega
  //    SerialMonitor.print("TickTime: ");
  //    SerialMonitor.print(TickTime);
  //    SerialMonitor.print(" OldTick: ");
  //    SerialMonitor.println(OldTick);

  //    SerialMonitor.println("WheelRev setup complete");
  
}

void computeSpeed(struct hist *data){
  //cyclometer has only done 1 or 2 revolutions
  
  //normal procedures begin here
  unsigned long WheelRev_ms = TickTime - OldTick;
  float SpeedCyclometer_revPs = 0.0;//revolutions per sec

  if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)
  { // No data
    SpeedCyclometer_mmPs = 0;
    SpeedCyclometer_revPs = 0;
    Serial.print("No compute  ");
    //Serial.println(*speedCyclo);
    return;
  }
  
  if (InterruptState == IRQ_SECOND)
  { //  first computed speed
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  =
      data->oldSpeed_mmPs = data->olderSpeed_mmPs = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    data->oldTime_ms = OldTick;
    data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber = ClickNumber;
    Serial.print("First compute  ");
    Serial.println(SpeedCyclometer_mmPs);
    return;
  }

  if (InterruptState == IRQ_RUNNING)
  { //  new data for second computed speed
    
    if(TickTime == data->nowTime_ms)
    {//no new data
        //check to see if stopped first
      unsigned long timeStamp = millis();
      if (timeStamp - data->nowTime_ms > MaxTickTime_ms)
      { // too long without getting a tick
         SpeedCyclometer_mmPs = 0;
         SpeedCyclometer_revPs = 0;
         if (timeStamp - data->nowTime_ms > 2 * MaxTickTime_ms)
         {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
         }
         return;
       }
        
       if (data->oldSpeed_mmPs > SpeedCyclometer_mmPs)
       { // decelerrating, extrapolate new speed using a linear model
          float deceleration = (float) (data->oldSpeed_mmPs - SpeedCyclometer_mmPs) / (float) (timeStamp - data->nowTime_ms);

          SpeedCyclometer_mmPs = data->oldSpeed_mmPs - deceleration * (timeStamp - data->nowTime_ms);
          if (SpeedCyclometer_mmPs < 0)
            SpeedCyclometer_mmPs = 0;
          SpeedCyclometer_revPs = SpeedCyclometer_mmPs / WHEEL_CIRCUM_MM;
       }
       else
       { // accelerating; should get new data soon

       }
       return;
    }

    //update time block
    data->olderTime_ms = data->oldTime_ms;
    data->oldTime_ms = data->nowTime_ms;
    data->nowTime_ms = TickTime;
    data->oldClickNumber = data->nowClickNumber;
    data->nowClickNumber = ClickNumber;

    //update speed block
    data->olderSpeed_mmPs = data->oldSpeed_mmPs;
    data->oldSpeed_mmPs = SpeedCyclometer_mmPs;
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    
    Serial.print("Nominal compute  ");
    Serial.println(SpeedCyclometer_mmPs);
    return;
  }
  
}

//function updates what should always be updated in every loop of ComputeSpeed
void ComputeSpeedHelper(struct hist *data){
    data->oldTime_ms = data->nowTime_ms;
    data->nowTime_ms = TickTime;
    data->oldClickNumber = data->nowClickNumber;
    data->nowClickNumber = ClickNumber;
}

void moveVehicle(int acc)
{
  /* Observed behavior on ElCano #1 E-bike no load (May 10, 2013, TCF)
    0.831 V at rest 52 counts
    1.20 V: nothing 75
    1.27 V: just starting 79
    1.40 V: slow, steady 87
    1.50 V: brisker 94
    3.63 V: max 227 counts
    255 counts = 4.08 V
  */

  Serial.println("Inside moveVehicle");
  DAC_Write(DAC_CHANNEL, acc);
  //throttle_control = acc;    // post most recent throttle.
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
  A/B — GA SHDN D7 D6 D5 D4 D3 D2 D1 D0 x x x x
  bit 15 bit 0
  bit 15 A/B: DACA or DACB Selection bit
  1 = Write to DACB
  0 = Write to DACA
  bit 14 — Don’t Care
  bit 13 GA: Output Gain Selection bit
  1 = 1x (VOUT = VREF * D/4096)
  0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.
  bit 12 SHDN: Output Shutdown Control bit
  1 = Active mode operation. VOUT is available.
  0 = Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down.
  VOUT pin is connected to 500 k (typical)
  bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.
  With 4.95 V on Vcc, observed output for 255 is 4.08V.
  This is as documented; with gain of 2, maximum output is 2 * Vref
*/
{
  int byte1 = ((value & 0xF0) >> 4) | 0x10; // active mode, bits D7-D4
  int byte2 = (value & 0x0F) << 4; // D3-D0

  if (address < 2)
  {
    // take the SS pin low to select the chip:
    digitalWrite(SelectAB, LOW);
    if (address >= 0)
    {
      if (address == 1)
        byte1 |= 0x80; // second channnel
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
      Serial.println("Inside SI HIGH");
    digitalWrite(SelectAB, HIGH);
  }
  else
  {
    // take the SS pin low to select the chip:
    digitalWrite(SelectCD, LOW);
    if (address <= 3)
    {
      if (address == 3)
        byte1 |= 0x80; // second channnel
      SPI.transfer(byte1);
      SPI.transfer(byte2);
    }
    // take the SS pin high to de-select the chip:
    digitalWrite(SelectCD, HIGH);
  }
}
