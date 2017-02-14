#include <ElcanoSerial.h>
using namespace elcano;
SerialData sd;
ParseState ps;

void setup() 
{
  pinMode(16, OUTPUT);
  Serial2.begin(baudrate);
  Serial1.begin(baudrate);
  ps.dt = &sd;
  ps.input = &Serial1;
  ps.output = &Serial2;
  ps.capture = elcano::MsgType::drive;
  sd.clear();
}

void loop() 
{
  sd.kind = MsgType::drive;
  sd.speed_cmPs = 100;
  sd.angle_deg = 10;
}
