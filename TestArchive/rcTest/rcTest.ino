#include <Settings.h>
#include <Servo.h>
#include <SPI.h>

#define RC_NUM_SIGNALS 7
#define ProcessRiseOfINT(Index) RC_rise[Index]=micros()
#define ProcessFallOfINT(Index)  RC_elapsed[Index]=(micros()-RC_rise[Index])
volatile unsigned long RC_rise[RC_NUM_SIGNALS];
volatile unsigned long RC_elapsed[RC_NUM_SIGNALS];
volatile bool RC_Done[RC_NUM_SIGNALS+1];

const int SelectCD = 49; // Select IC 3 DAC (channels C and D)
const int SelectAB = 53; // Select IC 2 DAC (channels A and B)


void setup() {
    //Set up pins

    Serial.begin(9600);
//  for (int i = 0; i < RC_NUM_SIGNALS; i++)
//  {
//    RC_rise[i] = INVALID_DATA;
//    RC_elapsed[i] = INVALID_DATA;
//    RC_Done[i] = 0;
//  }
//  for (int i = 0; i < ERROR_HISTORY; i++)
//  {
//    speed_errors[i] = 0;
//  }

  //setupWheelRev(); // WheelRev4 addition
  
        attachInterrupt(digitalPinToInterrupt(IRPT_TURN),  ISR_TURN_rise,  RISING);   // right stick l/r
//        attachInterrupt(digitalPinToInterrupt(IRPT_RDR),   ISR_RDR_rise,   RISING);   // nothing
        attachInterrupt(digitalPinToInterrupt(IRPT_GO),    ISR_GO_rise,    RISING);   // left stick l/r
        attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);   // red switch
//        attachInterrupt(digitalPinToInterrupt(IRPT_RVS),   ISR_RVS_rise,   RISING);   // nothing
        attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);   // left stick u/d
//        attachInterrupt(digitalPinToInterrupt(IRPT_MOTOR_FEEDBACK), ISR_MOTOR_FEEDBACK_rise, RISING);
}

void loop() {
  //auto, 1, left up/down
  //go, 4, left r/l
  //turn, 5, right up/down
  //
  int i = RC_GO;
  Serial.println(String(RC_elapsed[i]) + " " + String(RC_elapsed[RC_ESTP]) + " " + String(RC_elapsed[RC_BRAKE]) + " " + String(RC_elapsed[RC_TURN]));
  delay(200);
}

void ISR_AUTO_rise()
{
  noInterrupts();
  ProcessRiseOfINT(RC_AUTO);
  attachInterrupt(digitalPinToInterrupt(17), ISR_AUTO_fall, FALLING);
  interrupts();
}

void ISR_AUTO_fall()
{
  noInterrupts();
  ProcessFallOfINT(RC_AUTO);
  RC_Done[RC_AUTO] = 1;
  attachInterrupt(digitalPinToInterrupt(17), ISR_AUTO_rise, RISING);
  interrupts();
}

void ISR_MOTOR_FEEDBACK_rise() {
  noInterrupts();
  // This differs from the other interrupt routines since we need the *cycle*
  // duration, not the width of the high pulse.  So here, we get the time from
  // the previous rising edge to the current rising edge.  Q: Are we properly
  // ignoring the first value from all of these interval computations?
  unsigned long old_phase_rise = RC_rise[RC_MOTOR_FEEDBACK];
  ProcessRiseOfINT(RC_MOTOR_FEEDBACK);
  RC_elapsed[RC_MOTOR_FEEDBACK] = RC_rise[RC_MOTOR_FEEDBACK] - old_phase_rise;
  //Serial.println("MOTOR");
  interrupts();
}

void ISR_TURN_rise(){
  noInterrupts();
  ProcessRiseOfINT(RC_TURN);
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
// RDR (rudder) is currently not used.
void ISR_RDR_rise() {
  // RDR (rudder) is not used. Instead, use this interrupt for the motor phase feedback, which gives speed.
  noInterrupts();
  unsigned long old_phase_rise = RC_rise[RC_RDR];
  ProcessRiseOfINT(RC_RDR);
  RC_elapsed[RC_RDR] = RC_rise[RC_RDR] - old_phase_rise;
  // The phase frequency is proportional to wheel rotation.
  // An e-bike hub is powered by giving it 3 phased 36 V lines
  // The e-bike controller needs feeback from the hub.
  // The hub supplies 3 Hall Phase sensors; each is a 5V square wave and tells how fast the wheel rotates.
  // The square wave feedback has sone noise, which is cleaned up by an RC low pass filter
  //  with R = 1K, C = 100 nF
  //Serial.println("RDR");
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
//Now used for Brakes 
void ISR_BRAKE_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_BRAKE);
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
//Should be bound to the red switch
void ISR_ESTOP_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_ESTP);
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_RVS_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_RVS);
  attachInterrupt(digitalPinToInterrupt(IRPT_RVS), ISR_RVS_fall, FALLING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_TURN_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_TURN);
  RC_Done[RC_TURN] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_TURN), ISR_TURN_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_BRAKE_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_BRAKE);
  RC_Done[RC_BRAKE] = 1;
  //Serial.println("GO");
  attachInterrupt(digitalPinToInterrupt(IRPT_BRAKE), ISR_BRAKE_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_ESTOP_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_ESTP);
  RC_Done[RC_ESTP] = 1;
  //  if (RC_elapsed[RC_ESTP] > MIDDLE)
  //     E_Stop();
  //Serial.println("ESTOP");
  attachInterrupt(digitalPinToInterrupt(IRPT_ESTOP), ISR_ESTOP_rise, RISING);
  interrupts();
}
/*---------------------------------------------------------------------------------------*/
void ISR_RVS_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_RVS);
  RC_Done[RC_RVS] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_RVS), ISR_RVS_rise, RISING);
  interrupts();
}

void ISR_GO_rise() {
  noInterrupts();
  ProcessRiseOfINT(RC_GO);
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_fall, FALLING);
  //RC_Done[RC_RDR] = 1;
  interrupts();
}

void ISR_GO_fall() {
  noInterrupts();
  ProcessFallOfINT(RC_GO);
  RC_Done[RC_GO] = 1;
  attachInterrupt(digitalPinToInterrupt(IRPT_GO), ISR_GO_rise, RISING);
  interrupts();
}



