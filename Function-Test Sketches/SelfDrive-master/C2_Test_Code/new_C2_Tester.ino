#include <ElcanoSerial.h>
#include <Serial_Communication.h> 
#include <LEDTester.h>

using namespace elcano;

//   Data normally in <Setting.h>
// Trike-specific physical parameters
#define WHEEL_DIAMETER_MM 482
// Wheel Circumference
#define WHEEL_CIRCUM_MM (long) (WHEEL_DIAMETER_MM * PI)
//Interrupt from the speedometer
#define IRPT_WHEEL 3
#define MMPS2KMPH(speed_mmPs) (speed_mmPs* 0.0036)
//#define MMPS2KMPH(speed_mmPs) (speed_mmPs* MEG/3600)

long startTime;

static struct hist {
  long olderSpeed_mmPs;  // older data
  unsigned long olderTime_ms;   // time stamp of older speed
  long oldSpeed_mmPs;  // last data from the interrupt
  unsigned long oldTime_ms;  // time stamp of old speed
  long tickMillis;
  long oldTickMillis;
  unsigned long nowTime_ms;
  unsigned long TickTime_ms;  // Tick times are used to compute speeds
  unsigned long OldTick_ms;   // Tick times may not match time stamps if we don't process
  // results of every interrupt
} history;

// 10 milliseconds -- adjust to accomodate the fastest needed response or
// sensor data capture.
#define LOOP_TIME_MS 100

static long distance_mm = 0;

float Odometer_m = 0;

#define MEG 1000000
#define MAX_SPEED_KPH 50  //re-defined value from Setting.h
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
unsigned long MinTickTime_ms;
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 3000
// A speed of less than 0.3 KPH is zero.
unsigned long MaxTickTime_ms;
// MinTickTime_ms = 9239 ms = 9 sec

// Speed in revolutions per second is independent of wheel size.
float SpeedCyclometer_revPs = 0.0;//revolutions per sec

#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_SECOND 2
#define IRQ_RUNNING 3
#define NO_DATA 0x7FFFFFFF
volatile byte InterruptState = IRQ_NONE;  // Tells us if we have initialized.         
volatile unsigned long TickTime = 0;  // Time from one wheel rotation to the next gives speed.
volatile unsigned long OldTick = 0;


ParseState parseState2, parseState;
SerialData SendData, ReceiveData;

// sending the actual speed 
long SpeedCyclometer_mmPs = 0;
//long actual_speed = 0; 
long wheel_angle = 6;

// receiving data
long desired_speed = 0; 
long turn_angle = 0; 
long pre_desired_speed = 0;
long pre_angle = 0;

void setup() {
    SpeedCyclometer_mmPs = 0;
    startTime = millis();

    pixels.begin();
    clearLights();

    pinMode(IRPT_WHEEL, INPUT);
    setupWheelRev();
    
    // Setting up data for sending 
    Serial.begin(9600); 
    Serial3.begin(9600);
    parseState2.dt  = &SendData;
    parseState2.input = &Serial3;
    parseState2.output = &Serial3;
    parseState2.capture = MsgType::drive;
    SendData.clear();
  
    //setup for receiving data 
    parseState.dt  = &ReceiveData;
    parseState.input = &Serial3;
    parseState.output = &Serial3;
    parseState.capture = MsgType::drive;
    ReceiveData.clear();
}

void Receive_Data() {

  
    Serial.println(desired_speed);
    Serial.println(turn_angle);   
}

void Send_Data() {
  
    // Sending data 
      SendData.clear(); 
      SendData.kind = MsgType::drive;
      SendData.speed_mmPs = sendData(SpeedCyclometer_mmPs);
      SendData.angle_mDeg = wheel_angle;
      SendData.write(&Serial3);   
}

void loop() {
    // Receiving data 
    ParseStateError r = parseState.update();
    if (r == ParseStateError::success) {
      desired_speed = receiveData(ReceiveData.speed_mmPs); 
      turn_angle = ReceiveData.angle_mDeg; 
       Receive_Data();
    } 

     Send_Data(); 

     // Time at which this loop pass should end in order to maintain a
    // loop period of LOOP_TIME_MS.
    unsigned long nextTime;
    // Get the next loop start time. Note this (and the millis() counter) will
    // roll over back to zero after they exceed the 32-bit size of unsigned long,
    // which happens after about 1.5 months of operation (
    nextTime = millis() + LOOP_TIME_MS;
 
    setLeftSpeed(MMPS2KMPH(SpeedCyclometer_mmPs));
    
    if(pre_desired_speed != desired_speed){
      setRightSpeed(MMPS2KMPH(desired_speed));
      pre_desired_speed = desired_speed; 
    }

    if(pre_angle != turn_angle){
      setAngles(turn_angle);
      pre_angle = turn_angle;
    }
    
    computeSpeed(&history);
    //while (millis() < nextTime);
}
/*----------------------------WheelRev---------------------------------------------------*/
// WheelRev is called by an interrupt.
void WheelRev() {
  //Serial.println("inside wheel rev");
  //static int flip = 0;
  unsigned long tick;
  noInterrupts();
  tick = millis();
  if (InterruptState != IRQ_RUNNING) {
    // Need to process 1st two interrupts before results are meaningful.
    InterruptState++;
  }

  if (tick - TickTime > MinTickTime_ms) {
    OldTick = TickTime;
    TickTime = tick;
  }
  
  interrupts();
}
/*----------------------------setupWheelRev----------------------------------------------*/
void setupWheelRev()  {
  float MinTick = WHEEL_CIRCUM_MM;
  MinTick *= 1000.0;
  MinTick /= MAX_SPEED_mmPs;
  MinTickTime_ms = MinTick;
  float MIN_SPEED_mmPs =  ((MIN_SPEED_mPh * 1000.0) / 3600.0);
  float MaxTick = (WHEEL_DIAMETER_MM * PI * 1000.0) / MIN_SPEED_mmPs;
  MaxTickTime_ms = MaxTick;
  TickTime = millis();
  // OldTick will normally be less than TickTime.
  // When it is greater, TickTime - OldTick is a large positive number,
  // indicating that we have not moved.
  // TickTime would overflow after days of continuous operation, causing a glitch of
  // a display of zero speed.  It is unlikely that we have enough battery power to ever see this.
  OldTick = TickTime;
  InterruptState = IRQ_NONE;

  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), WheelRev, RISING);//pin 2 on Mega
}

/*----------------------------computeSpeed-----------------------------------------------*/
void computeSpeed(struct hist *data) {
  //cyclometer has only done 1 or 2 revolutions
  //normal procedures begin here
  unsigned long WheelRev_ms = TickTime - OldTick;
  float SpeedCyclometer_revPs = 0.0;//revolutions per sec
  if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)  { // No data
    
    SpeedCyclometer_mmPs = 0;
    SpeedCyclometer_revPs = 0;
    
    return;
  }

  // computes the speed for the first time 
  if (InterruptState == IRQ_SECOND) { 
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  =
      data->oldSpeed_mmPs = data->olderSpeed_mmPs = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    data->oldTime_ms = OldTick;
    data->nowTime_ms = TickTime;  // time stamp for oldSpeed_mmPs
    return;
  }

  // check to see whether the wheel has stopped 
  if (InterruptState == IRQ_RUNNING)  { //  new data for second computed speed
    if (TickTime == data->nowTime_ms) { //no new data
      //check to see if stopped first
      unsigned long timeStamp = millis();
      if (timeStamp - data->nowTime_ms > MaxTickTime_ms)  { // too long without getting a tick
        SpeedCyclometer_mmPs = 0;
        SpeedCyclometer_revPs = 0;
        if (timeStamp - data->nowTime_ms > 2 * MaxTickTime_ms)  {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
        }
        return;
      }
      // moving but decelerating 
      if (data->oldSpeed_mmPs > SpeedCyclometer_mmPs) { // decelerrating, extrapolate new speed using a linear model
        float deceleration = (float) (data->oldSpeed_mmPs - SpeedCyclometer_mmPs) / (float) (timeStamp - data->nowTime_ms);
        SpeedCyclometer_mmPs = data->oldSpeed_mmPs - deceleration * (timeStamp - data->nowTime_ms);

        if (SpeedCyclometer_mmPs < 0)
          SpeedCyclometer_mmPs = 0;

        SpeedCyclometer_revPs = SpeedCyclometer_mmPs / WHEEL_CIRCUM_MM;   
      }
   
    }

    // moving and accelerating 
    //update time block
    data->olderTime_ms = data->oldTime_ms;
    data->oldTime_ms = data->nowTime_ms;
    data->nowTime_ms = TickTime;

    //update speed block
    data->olderSpeed_mmPs = data->oldSpeed_mmPs;
    data->oldSpeed_mmPs = SpeedCyclometer_mmPs;
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    SpeedCyclometer_mmPs  = WHEEL_CIRCUM_MM * SpeedCyclometer_revPs;
    

    data->oldTickMillis = data->tickMillis;
    data->tickMillis = millis();

    distance_mm += ((data -> oldTime_ms - data -> olderTime_ms) / 1000.0) * (data -> oldSpeed_mmPs);

    if (data->TickTime_ms - data->OldTick_ms > 1000)
      SpeedCyclometer_mmPs = 0;
      SpeedCyclometer_revPs = 0; 

    return;
  }
}



