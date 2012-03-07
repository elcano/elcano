// KalmanFilter.cpp : Uses all available information to make optimal estimation.
//   C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.
//   C++ version by Tyler Folsom; March 6, 2012.
//


//#include "stdafx.h"  // required for MSVC. Remove for Arduino.

#include "Matrix.h"


/*########################################
# Write a function 'filter' that implements a multi-
# dimensional Kalman Filter for the example given

Kalman filter
*/

void filter(matrix& x, matrix& P, matrix& meas)
{
	float initial_next_state[] = {1.F, 1.F, 0, 1.F};
	float initial_measurement[] = {1.F, 0};

	matrix u = matrix( 2, 1); // 2x1 external motion
	matrix F = matrix(2,2,initial_next_state); // 2x2 next state function
	matrix H = matrix(1,2,initial_measurement); //1x2  measurement function
	matrix R = matrix(1); // 1x1 measurement uncertainty
	matrix I = matrix(2); // 2x2 identity matrix
 //
	{      
        // measurement update
        // Arduino compiler gets confused on combining multiple matrix operations.

                matrix Hx =  H * x;
		matrix y =  meas - Hx; // y = Z- H * x
                matrix transp = H.transpose();
                matrix PHt = P * transp;
                matrix HPH = H * PHt;
		matrix S = HPH + R; // S = H * P * H.transpose() + R;
                matrix Sinv = S.inverse();
		matrix K = PHt * Sinv;// K = P * H.transpose()*S.inverse();
		matrix Ky = K * y;
		matrix x_Ky = x + Ky;
                x = x_Ky;
		matrix KH = K*H;
		matrix IKH = I-(KH);
		matrix Pnew = IKH * P; // (I-(K*H)) * P;  
                P = Pnew;
       // prediction
                matrix Fx = F * x;
		matrix xNew = Fx + u;
                x = xNew;  // x = F*x + u
                matrix Ftrans = F.transpose();
                matrix PFt = P * Ftrans;
                matrix P2 = F * PFt;
		P =  P2; // P = F * P * F.transpose();
       
        Show("x= ");
        x.show();
        Show("P= ");
        P.show(); 
	}
	return;
}


