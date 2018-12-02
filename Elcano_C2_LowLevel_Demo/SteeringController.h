#pragma once
#include <Servo.h>
#include "PID_v1.h"


class SteeringController{
	Servo Steer_Servo;
	PID steerPID;
	double steerAngleUS;
	double PIDSteeringOutput_us;
	double desiredTurn_us;
	int currentSteeringUS =0;


	void SteeringPID(int input);
public:
	SteeringController();
	~SteeringController();
  void initialize();
	void updateAngle(double input) { steerAngleUS = input; };
	void setDesiredTurn(int input) { SteeringPID(input); };
  void engageSteering(double input){
    if(input > MAX_TURN)
      input = MAX_TURN;
    else if (input < MIN_TURN)
      input = MIN_TURN;
    if (currentSteeringUS != input){
      Serial.print("Steering: ");
      Serial.println(input);
      Steer_Servo.write(input);
      currentSteeringUS = input;
    }
  }
  ;
};
