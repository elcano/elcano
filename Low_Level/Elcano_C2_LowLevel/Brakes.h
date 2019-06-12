#pragma once
/* Solenoid controlled Brakes.
 *  
 *  Solenoid (Johnson Electic model 150 174432-024) 
 *  can be kept at lower voltage (12V) indefinitely. 
 *  It has a holding force of 14.5 lb.
 *  At the higher voltage, data sheet expects it to be high (24V) for
 *  25% of the time and low for 75%. However, it can stay high for 100 sec.
 *  The solenoid typically reacts in less than a second.
 *  For 0.25 inch throw and 24V it can pull 7 lb.
 *  We are using the part with 0.3 inch throw, 12V in low state and 24V in high
 *  state, but keeping voltage high for only a second or two.
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
  void Update();
 private:
  volatile enum brake_state {BR_OFF, BR_HI_VOLTS, BR_LO_VOLTS} state;
  volatile uint32_t clock_hi_ms;
 } ;
