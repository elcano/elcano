// KalmanFilter.cpp : Uses all available information to make optimal estimation.
//   C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.
//   C++ version by Tyler Folsom; March 6, 2012.
//


//#include "stdafx.h"  // required for MSVC. Remove for Arduino.

#include "Matrix.h"

/*
Kalman filter
Matrix x state: longs for position_mm_east, position_mm_north,
  velocity_mm_s_east, velocity_mm_s_north
*/

void Filter(REAL* State, // e.g  x, y position, x, y velocity
            REAL* uncertainty,   // P uncertainty covariance
            REAL* measurements, // Z: measured x and y position in mm
            REAL deltaT_s)  // time from measurement to prediction in seconds
{
      REAL initial_next_state[] = {1., 0,  deltaT_s, 0,
                                   0,  1., 0,        deltaT_s,
                                   0,  0,  1,        0,
                                   0,  0,  0,        1};
      // position' = 1*position + 1*velocity
      // velocity' = 1*velocity
      REAL initial_measurement[] = {1.F, 0, 0, 0,
                                     0, 1., 0, 0};
      // only position is measurable; not velocity
      matrix x = matrix(4,1,State); // initial state (location and velocity)
      matrix P = matrix(4,4, uncertainty); // 4x4 initial uncertainty
      matrix meas = matrix(2,1,measurements);

	matrix u = matrix( 4, 1); // 4x1 external motion; set to 0.
	matrix F = matrix(4,4,initial_next_state); // 2x2 next state function
	matrix H = matrix(2,4,initial_measurement); //1x2  measurement function
	matrix R = matrix(2); // 1x1 measurement uncertainty
	matrix I = matrix(4); // 4x4 identity matrix
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


