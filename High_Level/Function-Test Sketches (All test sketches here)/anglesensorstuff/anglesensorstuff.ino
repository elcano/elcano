#include <Servo.h>

#define RELAYInversion true
#define DEBUG true

#define STEER_OUT_PIN 6
#define STEER_ON 8
#define AngleSensorRight  A3
#define AngleSensorLeft   A2
#define AngleSensorRight_B  A7
#define AngleSensorLeft_B   A6
#define VEHICLE_LENGTH_MM 5
#define DISTANCE_BETWEEN_WHEELS_MM 3


#define MIN_TURN_Mdegrees -24000
#define MAX_TURN_Mdegrees 25000
#define MIN_Right_Sensor 725
#define MAX_Right_Sensor 785
#define MIN_Left_Sensor 485
#define MAX_Left_Sensor 313
#define Left_Read_at_MIN_TURN  485
#define Right_Read_at_MIN_TURN 725
#define Left_Read_at_MAX_TURN 313
#define Right_Read_at_MAX_TURN 785
#define MIN_TURN_MS 1000
#define MAX_TURN_MS 1850
#define MIN_TURN 70
#define MAX_TURN 130
#define CENTER_TURN 94

const int32_t baud = 115200;
Servo steer_servo;

void setup() {

  Serial.begin(baud);
  pinMode(STEER_ON,OUTPUT);
  digitalWrite(STEER_ON, RELAYInversion ? HIGH : LOW);
  steer_servo.attach(STEER_OUT_PIN);
  steer_servo.writeMicroseconds(1500);
}

void loop() {
  
  if (Serial.available() > 0) {
    if (Serial.peek() == 's') {
      Serial.readString();
      Serial.println("reading analog sensors");
      int32_t Lval = analogRead(AngleSensorLeft);
      int32_t Rval = analogRead(AngleSensorRight);
      Serial.print("Left Sensor reading: ");
      Serial.println(Lval);
      
      Serial.print("Right Sensor reading: ");
      Serial.println(Rval);
      
      Lval = map(Lval, Left_Read_at_MIN_TURN,Left_Read_at_MAX_TURN, MIN_TURN_Mdegrees,MAX_TURN_Mdegrees);
      Rval = map(Rval, Right_Read_at_MIN_TURN,Right_Read_at_MAX_TURN, MIN_TURN_Mdegrees,MAX_TURN_Mdegrees);
      Serial.print("Left Sensor reading in Microdegrees: ");
      Serial.println(Lval);
      
      Serial.print("Right Sensor reading in Microdegrees: ");
      Serial.println(Rval);
      
      Serial.println("finished");
    }
      
    int32_t extension = Serial.parseInt();
    Serial.readString(); // clear serial
    Serial.print("Input in Microdegrees: ");
    Serial.println(extension);
    
    int32_t input_microseconds = map(extension,MIN_TURN_Mdegrees,MAX_TURN_Mdegrees, MIN_TURN_MS,MAX_TURN_MS);
    Serial.print("Input in Microseconds: ");
    Serial.println(input_microseconds);
    
    if (input_microseconds > MAX_TURN_MS)
      input_microseconds = MAX_TURN_MS;
    else if (input_microseconds < MIN_TURN_MS)
      input_microseconds = MIN_TURN_MS;
    Serial.print("Constrained Input in Microseconds: ");
    Serial.println(input_microseconds);
    steer_servo.writeMicroseconds(input_microseconds);
    Serial.println("reading analog sensors");
    delay(1000);
    int32_t Lval = analogRead(AngleSensorLeft);
    int32_t Rval = analogRead(AngleSensorRight);
    Serial.print("Left Sensor reading: ");
    Serial.println(Lval);
    
    Serial.print("Right Sensor reading: ");
    Serial.println(Rval);
    
    Lval = map(Lval, Left_Read_at_MIN_TURN,Left_Read_at_MAX_TURN, MIN_TURN_Mdegrees,MAX_TURN_Mdegrees);
    Rval = map(Rval, Right_Read_at_MIN_TURN,Right_Read_at_MAX_TURN, MIN_TURN_Mdegrees,MAX_TURN_Mdegrees);
    Serial.print("Left Sensor reading in Microdegrees: ");
    Serial.println(Lval);
    
    Serial.print("Right Sensor reading in Microdegrees: ");
    Serial.println(Rval);
    
    Serial.println("finished");
    }
    
  
}

/*
int32_t computeTrikeAngle() {
  //read angle
  int32_t lAngle = analogRead(AngleSensorLeft);
  //convert from voltage to milli degrees
  //need to measure two data points from sensor to actual angle
  lAngle = map(lAngle, MIN_Left_Sensor, MAX_Left_Sensor, MIN_TURN, MAX_TURN);
  int32_t r_mm = VEHICLE_LENGTH_MM / tan(lAngle * PI / 180);
  //How do we know which way the vehicle is turning? will one of these be reasonable and the other not?
  int32_t complAngle_goingLeft = atan(VEHICLE_LENGTH_MM / (r_mm + DISTANCE_BETWEEN_WHEELS_MM / 2));
  int32_t complAngle_goingRight = atan(VEHICLE_LENGTH_MM / (r_mm - DISTANCE_BETWEEN_WHEELS_MM / 2));
  //then do same thing with right sensor
  //how should we compare the two?
  //to do check units
    
}  
*/
