#include <ElcanoSerial.h>
#include <Serial_Communication.h>

using namespace elcano;
ParseState ps, ps2;
SerialData SendData, RecieveData;

//sending
long desired_speed = 850;
long turn_angle = 8;
long pre_desired_speed = 0;
//recieving
long actual_speed = 0;
long wheel_angle = 0;

int count = 0;

void setup() {
  // set up for Sending SendData
  Serial.begin(9600);
  Serial2.begin(9600);
  ps.dt  = &SendData;
  ps.input = &Serial2;
  ps.output = &Serial2;
  ps.capture = MsgType::drive;
  SendData.clear();

  // setting up for receiving SendData
  ps2.dt  = &RecieveData;
  ps2.input = &Serial2;
  ps2.output = &Serial2;
  ps2.capture = MsgType::drive;
  RecieveData.clear();

  //    Serial.print("Initializing SD card...");
  //
  //    if (!SD.begin(10)) {
  //      Serial.println("initialization failed!");
  //      return;
  //    }
  //    Serial.println("initialization done.");
}

void loop1() {


  Serial.println(actual_speed);
  Serial.println(wheel_angle);
}

void loop2() {
  count++;
  if (count == 1000) {
    desired_speed = 1670;
  }
  else if (count == 2000) {
    desired_speed = 2240;
  }
  else if (count == 3000) {
    desired_speed = 280;
  }
  // Sending SendData
  // Update code here that does not depend on having received a SendData set

  if (pre_desired_speed != desired_speed) {
    SendData.clear();
    SendData.kind = MsgType::drive;
    SendData.speed_mmPs = sendData(desired_speed);
    SendData.angle_mDeg = turn_angle;
    SendData.write(&Serial2);
    //pre_desired_speed = desired_speed;
  }
}

void loop() {
  // Receiving SendData
  ParseStateError r = ps2.update();
  if (r == ParseStateError::success) {
    actual_speed = receiveData(RecieveData.speed_mmPs);
    wheel_angle = RecieveData.angle_mDeg;

    loop1();
  }
  loop2();
}
