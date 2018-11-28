#pragma once


class SteeringController
{
	
	Servo Steer_Servo;
	PID steerPID;
	double steerAngleUS;
	double PIDSteeringOutput_us;
	double desiredTurn_us;
	int currentSteeringUS;
	const double proportional_steering = .0175;
	const double integral_steering = .5;
	const double derivative_steering = .00001;

	void SteeringPID(int input);
public:
	SteeringController(double, double, int, int);
	~SteeringController();
	void setSteerAngleUS(double input) { steerAngleUS = input; };
	void setDesiredTurn(int input) { SteeringPID(input); };
};

