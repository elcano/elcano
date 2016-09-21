#include <PID_v1.h>
#include <Settings.h>

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

long SpeedCyclometer_mmPs = 0;
// Speed in revolutions per second is independent of wheel size.
float SpeedCyclometer_revPs = 0.0;//revolutions per sec

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

  byte nowClickNumber;  // situation when we want to display the speed
  //byte InterruptState;
  unsigned long nowTime_ms;
  unsigned long TickTime_ms;  // Tick times are used to compute speeds
  unsigned long OldTick_ms;   // Tick times may not match time stamps if we don't process
  // results of every interrupt
} history;

void setup(){
  Serial.begin(9600);

  setupWheelRev();
  //attachInterrupt(digitalPinToInterrupt(IRPT_RDR), ISR_RDR_rise, RISING);
}

void loop(){
  ComputeSpeed(&history, &SpeedCyclometer_mmPs);
  PrintSpeed(history);
//  Serial.print("Int State ");
//  Serial.println(InterruptState);
  Serial.print("Click ");
  Serial.println(ClickNumber);
  delay(10); 
}

void PrintSpeed(struct hist data)
{
  Serial.print(SpeedCyclometer_mmPs); Serial.print("\t");
}

void ISR_RDR_rise() {
  unsigned long tick;
  noInterrupts();
  tick = millis();
  if (InterruptState != IRQ_RUNNING){
    // Need to process 1st two interrupts before results are meaningful.
    InterruptState++;
  }
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

  attachInterrupt (digitalPinToInterrupt(IRPT_WHEEL), ISR_RDR_rise, RISING);//pin 3 on Mega
  //    SerialMonitor.print("TickTime: ");
  //    SerialMonitor.print(TickTime);
  //    SerialMonitor.print(" OldTick: ");
  //    SerialMonitor.println(OldTick);

  //    SerialMonitor.println("WheelRev setup complete");

}

void ComputeSpeed(struct hist *data, long *speedCyclo)
{
  if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)
  { // No data
    *speedCyclo = 0;
    SpeedCyclometer_revPs = 0;
    Serial.print("No compute  ");
    Serial.println(*speedCyclo);
    return;
  }
  Serial.println("Stepped");
  float Circum_mm = WHEEL_CIRCUM_MM;
  long WheelRev_ms = data->TickTime_ms - data->OldTick_ms;
  
  if (InterruptState == IRQ_SECOND) //&& data->oldSpeed_mmPs == NO_DATA && WheelRev_ms > 0)
  { //  first computed speed
    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    *speedCyclo  =
      data->oldSpeed_mmPs = Circum_mm * SpeedCyclometer_revPs;
    data->oldTime_ms = data->TickTime_ms;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber;
    Serial.print("First compute  ");
    Serial.println(*speedCyclo);
    return;
  }
  if (InterruptState == IRQ_RUNNING && data->olderSpeed_mmPs == NO_DATA && WheelRev_ms > 0
      && data->nowClickNumber != data->oldClickNumber)
  { //  new data for second computed speed
    data->olderSpeed_mmPs = data->oldSpeed_mmPs;
    data->olderTime_ms = data->oldTime_ms;

    SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
    *speedCyclo  =
      data->oldSpeed_mmPs = Circum_mm * SpeedCyclometer_revPs;
    data->oldTime_ms = data->TickTime_ms;  // time stamp for oldSpeed_mmPs
    data->oldClickNumber = data->nowClickNumber;

    Serial.print("Nominal compute  ");
    Serial.println(*speedCyclo);
    return;
  }
  if (InterruptState == IRQ_RUNNING && data->olderSpeed_mmPs != NO_DATA && WheelRev_ms > 0)
  { // Normal situation after initialization
    if (data->nowClickNumber == data->oldClickNumber)
    { // No new information; extrapolate the speed if decelerating; else keep old speed
      if (data->olderSpeed_mmPs > data->oldSpeed_mmPs)
      { // decelerrating
        float deceleration = (float) (data->olderSpeed_mmPs - data->oldSpeed_mmPs) /
                             (data->oldTime_ms - data->olderTime_ms);
        *speedCyclo = data->oldSpeed_mmPs - deceleration *
                               (data->nowTime_ms - data->oldTime_ms);
        if (*speedCyclo < 0)
          *speedCyclo = 0;
        SpeedCyclometer_revPs = *speedCyclo / Circum_mm;
      }
      else
      { // accelerating; should get new data soon

      }
      if (data->nowTime_ms - data->oldTime_ms > MaxTickTime_ms)
      { // too long without getting a tick
        *speedCyclo = 0;
        SpeedCyclometer_revPs = 0;
        if (data->nowTime_ms - data->oldTime_ms > 2 * MaxTickTime_ms)
        {
          InterruptState = IRQ_FIRST;  //  Invalidate old data
          data->oldSpeed_mmPs = NO_DATA;
          data->olderSpeed_mmPs = NO_DATA;
        }
      }
    }
    else
    { // moving; use new data to compute speed
      data->olderSpeed_mmPs = data->oldSpeed_mmPs;
      data->olderTime_ms = data->oldTime_ms;

      SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
      *speedCyclo  =
        data->oldSpeed_mmPs = Circum_mm * SpeedCyclometer_revPs;
      data->oldTime_ms = data->TickTime_ms;  // time stamp for oldSpeed_mmPs
      data->oldClickNumber = data->nowClickNumber;
    }
    Serial.print("Nominal compute  ");
    Serial.println(*speedCyclo);
  }
  if (data->nowTime_ms - data->TickTime_ms > WheelRev_ms)
  { // at this speed, should have already gotten a tick?; If so, we are slowing.
    float SpeedSlowing_revPs = 1000.0 / (data->nowTime_ms - data->TickTime_ms);
    long SpeedSlowing_mmPs  = Circum_mm * SpeedCyclometer_revPs;
    SpeedCyclometer_revPs = min(SpeedCyclometer_revPs, SpeedSlowing_revPs);
    *speedCyclo = min(*speedCyclo, SpeedSlowing_mmPs);
  }
  return;
}
