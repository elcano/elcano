/*
Paul Curry, Elcano Throttle Simulator
5/22/2017


Recieves throttle from Analogue pin 0
Writes to Pin 3 to trigger interrupt that sends digital pulses (speed) through pin 5
An isr is an interrupt service routine, a function that can be used for interrupts.
*/

// Define pins
#define PULSE_PIN 5
#define READ_THROTTLE_PIN A0
#define READ_BRAKE_PIN 3

// Number of throttle values to average in set_pulse_delay
#define NUM_THROTTLE_AVERAGE 8

// Theshold to overcome static friction (double)
#define FRICTION_THRESHOLD 300.8

// Threshold by which to define "not moving" (double mm/s)
#define VEL_THRESHOLD 1.0

// In mm, this should be removed eventually to use the 
// value defined in Settings.h (double)
#define WHEEL_CIRCUM 492.0

#include "TimerOne.h"

// throttle_counter used to assign throttle values
int throttle_counter; 
int throttleVals[NUM_THROTTLE_AVERAGE];

// whether brake is on 
int isBraking;

// Previous speed value for use in get_pulse_delay
int previous_vel; 

// Previous acceleration value for use in get_pulse delay
int previous_acc;

// the setup routine runs once when you press reset or open serial monitor/window:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // Set up pins
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(READ_THROTTLE_PIN,INPUT);
  pinMode(READ_BRAKE_PIN,INPUT);

  // Initialize throttle values to 0
  for(int i = 0; i < NUM_THROTTLE_AVERAGE; i++){
    throttleVals[i] = 0;
  }

  // Initialize variables
  throttle_counter = 0;
  previous_vel = 0;
  previous_acc = 0;
  isBraking = 0;
  
}


// the loop routine runs continously 
// updating throttle values, whether breaks are on
// and what the pulse delay (speed) should be
void loop() {
  
  // read throttle
  int throttle = analogRead(READ_THROTTLE_PIN);

  // store analogue value in NUM_THROTTLE_AVERAGE sized buffer
  throttleVals[throttle_counter] = throttle;

  // Reinitialzie to 0 to prevent overflow
  if(throttle_counter == NUM_THROTTLE_AVERAGE - 1) {
    throttle_counter = 0;
  }

  // read break
  int brake = digitalRead(READ_BRAKE_PIN);
  if(brake) {
    isBraking = brake;
  }
  
  // update pulse delay
  set_pulse_delay();
  
  // print out the value you read:
  Serial.print("Throttle in Value: ");
  Serial.println(throttle);
  delay(1); // delay in between reads for stability
}

// Sets the pulse delay for sending pulses
// Is called every loop
int set_pulse_delay() {

  // MC = magic constant used in velocity calculation
  // TODO find out what these are
  double MC1 = 300.0;
  double MC2 = 200.0;
  double MC3 = 100.0; // Rolling friction
  double BRAKE_EFFECT = 100; // Effect of break on rolling friction

  // Get average of previous NUM_THROTTLE_AVERAGE throttle values
  int throttle_ave = 0;
  for(int i = 0; i < NUM_THROTTLE_AVERAGE; i++) {
    throttle_ave += throttleVals[i];
  }
  throttle_ave = throttle_ave/NUM_THROTTLE_AVERAGE;

  // if trike is not moving the average throttle needs to reach a certain threshold 
  // to overcome static friction
  boolean moving = throttle_ave > FRICTION_THRESHOLD || abs(previous_vel) > VEL_THRESHOLD;
  if(moving) {

    // Simplified velocity equation, based off 3 magic constants wheighting 
    // the throttle, instantanous velocity and acceleration
    double velocity = MC1 * throttle_ave + MC2 * previous_vel - (MC3 + BRAKE_EFFECT * isBraking) * previous_acc;

    // Set previous acceleration and velocity
    previous_acc = velocity - previous_vel;
    previous_vel = velocity; 
  
    // Set pulse delay based off of wheel circumfrence
    int pulse_delay = (int)(WHEEL_CIRCUM/velocity);
    
    Timer1.initialize(pulse_delay); 
    Timer1.attachInterrupt(send_pulse); // initialize internal interrupt function pulse_isr
    
  } else {

    // We arn't moving so don't send pulses
    Timer1.stop(); 

    previous_vel = 0;
    previous_acc = 0;
    
  }

}

// Sends a pulse to the low level which uses the pulse to calculate
// the simulated trike's velocty
void send_pulse() {

  // Ssend a pulse through pin 5
  digitalWrite(PULSE_PIN,HIGH);
  digitalWrite(PULSE_PIN,LOW);

  // DEBUG 
  Blink();
  
}

// Debug method that blinks a light on the arduino
void Blink() {

  digitalWrite(10,digitalRead(10)^1); // Toggle pin 10 for blink

}


