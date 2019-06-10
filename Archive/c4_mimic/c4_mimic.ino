
#include <Common.h>
#include <IO.h>

#include <SPI.h>
#include <SD.h>
//#include <Elcano_Serial.h>
#include <ElcanoSerial.h>

using namespace elcano;
SerialData data;
ParseState ps;




void setup() {
 Serial3.begin(baudrate);
  ps.dt = &data;
  ps.input = &Serial3;
  ps.output = &Serial3;
}

void loop() {
  data.clear();
    
  data.kind = MsgType::seg;
  data.number = 8;
  data.posE_cm = 100;//course->east_mm / 10;
  data.posN_cm = 10;
  // float angle = atan2(10) * 180 / PI + 90.;
  data.bearing_deg = -90;
  data.speed_cmPs = 20;
  delay(1000);
  data.write(&Serial3);

}
