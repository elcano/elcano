// include the pinchangeint library 
#include <PinChangeInt.h>

#define RC_NUM_CHANNELS 4

#define RC_CH1 0
#define RC_CH2 1
#define RC_CH3 2
#define RC_CH4 3

// Assign your channel in pins
#define RC_CH1_INPUT A8
#define RC_CH2_INPUT A9
#define RC_CH3_INPUT A10
#define RC_CH4_INPUT A11

volatile uint16_t rc_shared[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
uint16_t rc_values[RC_NUM_CHANNELS];
volatile bool rc_done[RC_NUM_CHANNELS];

void setup()
{
  Serial.begin(9600);
  Serial.println("multiChannels");

  // using the PinChangeInt library, attach the interrupts
  // used to read the channels
  PCintPort::attachInterrupt(RC_CH1_INPUT, calc_ch1,CHANGE); 
  PCintPort::attachInterrupt(RC_CH2_INPUT, calc_ch2,CHANGE); 
  PCintPort::attachInterrupt(RC_CH3_INPUT, calc_ch3,CHANGE); 
  PCintPort::attachInterrupt(RC_CH4_INPUT, calc_ch4,CHANGE);
}

void loop()
{
    Serial.println(rc_shared[RC_CH1]);

  //+ " " + String(unSteeringInShared) + " " + String(unAuxInShared) + " " + String(unLStickShared)); 
}

void calc_input(uint8_t channel, uint8_t input_pin){
  if(digitalRead(input_pin) == HIGH){
    rc_start[channel] = micros();
  }else{
    rc_shared[channel] = (uint16_t)(micros() - rc_start[channel]);
    rc_done[channel] = 1;
  }
}

void calc_ch1(){ calc_input(RC_CH1, RC_CH1_INPUT); }
void calc_ch2(){ calc_input(RC_CH2, RC_CH2_INPUT); }
void calc_ch3(){ calc_input(RC_CH3, RC_CH3_INPUT); }
void calc_ch4(){ calc_input(RC_CH4, RC_CH4_INPUT); }
