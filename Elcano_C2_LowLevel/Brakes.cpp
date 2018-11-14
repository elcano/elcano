#include <Arduino.h>
#include "Brakes.h"

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
