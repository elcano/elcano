#pragma once

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
  const int RightBrakeOnPin = 2;
  const int LeftBrakeVoltPin = 8;
  const int RightBrakeVoltPin = 7;
  const unsigned long MaxHi_ms = 800;
 } ;

