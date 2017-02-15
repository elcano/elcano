#include <ElcanoSerial.h>
using namespace elcano;
SerialData sd;
ParseState ps;

void setup() 
{
  pinMode(16, OUTPUT);
//  digitalWrite(16, LOW);
  Serial.begin(9600);
  Serial1.begin(9600);
  ps.dt = &sd;
  ps.input = &Serial1;
  ps.output = &Serial1;
  ps.capture = elcano::MsgType::drive;
  sd.clear();
}

void loop() 
{
  sd.kind = MsgType::drive;
  sd.speed_cmPs = 50;
  sd.angle_deg = 10;
  sd.write(&Serial1);
//  Serial.println("here");
  delay(10);
}
