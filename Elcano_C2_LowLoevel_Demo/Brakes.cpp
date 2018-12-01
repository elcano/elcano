#include "Settings.h"
#include <Arduino.h>
#include "Brakes.h"

Brakes::Brakes(){
  pinMode( BrakeOnPin, OUTPUT);
  pinMode( BrakeVoltPin, OUTPUT);
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
  digitalWrite(BrakeOnPin, LOW);
  digitalWrite(BrakeVoltPin, LOW);
  state = BR_OFF;
}
void Brakes::Stop()
{
  /*  Expected behavior:
   *  Both LEDs come on for Relays 2 and 3
   *  Relay 2 connects NO (solenoids) to COM (ground)
   *  Relay 3 connects COM (other end of solenoids) to NC (36V)
   */
  digitalWrite(BrakeVoltPin, HIGH);  // Need the higher voltage to activate the solenoid.
  if (state != BR_HI_VOLTS)
  {
    clock_hi_ms = millis();  // keep track of when the higher voltage was applied.
  }
  digitalWrite(BrakeOnPin, HIGH); // Activate solenoid to apply brakes.
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
    digitalWrite(BrakeVoltPin, LOW); // Set to lower voltage, which will keep brakes applied
    state = BR_LO_VOLTS;
  }
}
