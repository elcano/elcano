//   KF_IO_Arduino.ico: Arduino specific parts of Kalman filter package.
//
//   C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.  
//   C++ version by Tyler Folsom; March 6, 2012.
//
//  Code has been tested on Arduino 1.0, on Mega 2560 
//  Other modules required:
//		KalmanFilter.cpp; Matrix.h; Matrix.c

#include "Matrix.h"


void filter(matrix& x, matrix& P, matrix& measure);

float initial_uncertainty[] = {1000.F, 0, 0, 1000.F};
matrix gPosition = matrix(2,1); // 2x1 initial state (location and velocity)
matrix gUncertainty = matrix(2,2, initial_uncertainty); // 2x2 initial uncertainty


bool matrix_test()
{
	bool passed = true;
	float va1[] = {0,2,  1,2, 0,-1};
	float va2[] = {-1,2, 0,4, 3,6};
	float va3[] = {-1,4, 1,6, 3,5};
	float va4[] = {-1,1,3, 4,6,5};
	float va5[] = {1,0,1, -2,-3,-7};
	float vb1[] = {1,2,3, 4,5,6};
	float vb2[] = {10,31, 19,76};
	float vb3[] = {1,2,3, -7,-8,-9, -31,-41,-51};
	float vc1[] = {25,-5,15, -5,5,-3, 15,-3,10};
	float vc5[] = {0.41f,0.05f,-0.6f, 0.05f,0.25f,0, -0.6f,0,1};

	matrix MA1 = matrix(3,2,va1);
	matrix MA2 = matrix(3,2,va2);
	matrix MA3 = MA1 + MA2;
	matrix MA4 = MA3.transpose();
	matrix MA5 = MA1 - MA2;
	matrix MB1 = matrix(2,3,vb1);
	matrix MB2 = MB1 * MA3;
	matrix MB3 = MA5 * MB1;
	matrix MC1 = matrix(3,3,vc1);
	matrix MC3 = MC1.inverse();
	matrix MC2a = MC1.Cholesky();  // triangular; works
	MC2a.show();
	matrix MC4a = MC2a.CholeskyInverse();
	MC4a.show();
	matrix MC5 = matrix(3,3,vc5);
	matrix MC6 = MC1 * MC5;
	matrix I4 = matrix(4);

	MA1.show();
	MA2.show();
	MA3.show();
	MA4.show();
	MA5.show();
	MB1.show();
	MB2.show();
	MB3.show();
	MC1.show();
	Show("triangle Python");
	MC2a.show();
	Show("Inverse");
	MC3.show();
	MC4a.show();
	MC5.show();
	MC6.show();
	I4.show();

        matrix answerMA3 = matrix(3,2,va3);
	if (MA3 != answerMA3)
	{
		Show("Addition failed.\n");
		passed = false;
	}
        matrix answerMA4 = matrix(2,3,va4);
	if (MA4 != answerMA4)
	{
		Show("Transpose failed.\n");
		passed = false;
	}
        matrix answerMA5 = matrix(3,2,va5);
	if (MA5 != answerMA5)
	{
		Show("Subtraction failed.\n");
		passed = false;
	}
        matrix answerMB2 = matrix(2,2,vb2);
        matrix answerMB3 = matrix(3,3,vb3);
	if (MB2 != answerMB2 || MB3 != answerMB3)
	{
		Show("Multiplication failed.\n");
		passed = false;
	}
        matrix answerMC3 = matrix(3,3,vc5);
	if (MC3 != answerMC3)
	{
		Show("Inverse failed.\n");
		passed = false;
	}

	if (passed)
        {
		Show("All matrix tests passed.\n");
        }
	else
		Show("\n***** MATRIX TESTS FAILED *****\n");
	return passed;  
}
void setup()
{
  Serial.begin(9600);
  Serial.print("Testing Matrices...");
  matrix_test();
}
void loop()
{
  const unsigned long pause = 1000; // msec
  unsigned long time = millis();
  unsigned long endTime = time + pause;
  float measurement;
  float motion = 1.5f;

  int data = analogRead(1);
/*
    Normally we would read inputs from the analog inputs (0 to 1023).
    Assume that nothing is connected and the data is noise.
    Combine it with motion to get data to filter.
*/
  measurement = data + motion;
  matrix measure = matrix(1,1,&measurement);

  filter(gPosition, gUncertainty, measure);
  
  // delay, but don't count time in loop
  while (time < endTime)
  {
    time = millis();
  }

}
void Show(char* x)
{
  Serial.print(x);
}
void Show(float x)
{
  Serial.print(x);
  Serial.print(", ");
}
