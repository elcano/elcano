//   KF_IO_PC.cpp: PC specific parts of Kalman filter package.
//
//   C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.
//   C++ version by Tyler Folsom; March 6, 2012.
//
//  Code has been tested on Microsoft Visual Studio C++ 2008, 
//		Professional Edition, version 3.5 SP1 under Windows 7.
//      Compiled as a Windows console application.
//  Other modules required:
//		KalmanFilter.cpp; Matrix.h; Matrix.c
/*
#include "stdafx.h"   // required for MSVC. Remove for Arduino.
#include "Matrix.h"
void filter(matrix& x, matrix& P, matrix& meas);

#ifndef abs
#define abs(x) ((x)>=0?(x):-(x))
#endif

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

	if (MA3 != matrix(3,2,va3))
	{
		Show("Addition failed.\n");
		passed = false;
	}
	if (MA4 != matrix(2,3,va4))
	{
		Show("Transpose failed.\n");
		passed = false;
	}
	if (MA5 != matrix(3,2,va5))
	{
		Show("Subtraction failed.\n");
		passed = false;
	}
	if (MB2 != matrix(2,2,vb2) || MB3 != matrix(3,3,vb3))
	{
		Show("Multiplication failed.\n");
		passed = false;
	}
	if (MC3 != matrix(3,3,vc5))
	{
		Show("Inverse failed.\n");
		passed = false;
	}

	if (passed)
		Show("All matrix tests passed.\n");
	else
		Show("\n***** MATRIX TESTS FAILED *****\n");
	return passed;  
}
int _tmain(int argc, _TCHAR* argv[])
{
	float initial_uncertainty[] = {1000.F, 0, 0, 1000.F};
	matrix x = matrix(2,1); // 2x1 initial state (location and velocity)
	matrix P = matrix(2,2, initial_uncertainty); // 2x2 initial uncertainty
	float measurements[] = {1.F, 2.F, 3.F};

	matrix_test();

   for (int n=0; n<sizeof(measurements)/sizeof(float); n++) 
   {
	   matrix meas = matrix(1,1,&measurements[n]);
	   filter(x, P, meas);
   }
	x.show();
	P.show();
	return 0;
}
void Show(char* x)
{
	printf(x);
}
void Show(float x)
{
	printf("%.6f, ", x);
}
*/
