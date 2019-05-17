// KalmanFilter.cpp : Uses all available information to make optimal estimation.
//   C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.
//   C++ version by Tyler Folsom; March 6, 2012.
//


//#include "stdafx.h"  // required for MSVC. Remove for Arduino.

// #include "Matrix.h"
#include "Kalman.h"
/*
Kalman filter
Matrix x state: longs for position_mm_east, position_mm_north,
  velocity_mm_s_east, velocity_mm_s_north
*/
#define NSTATES 4
#define NSEEN 2
void Filter(REAL* State, // e.g  x, y position, x, y velocity
            REAL* uncertainty,   // P uncertainty covariance
            REAL* measurements, // Z: measured x and y position in mm
            REAL deltaT_s,  // time from previous state to measurement in seconds
            REAL* variance)     // variance of the measurements
{
      REAL state_transition[NSTATES*NSTATES]   = 
                            {1., 0,  deltaT_s, 0,
                             0,  1., 0,        deltaT_s,
                             0,  0,  1,        0,
                             0,  0,  0,        1};
      //REAL state_transition[]   = {1., 0,  deltaT_s, 0,
      //                             0,  1., 0,        deltaT_s,
      //                             0,  0,  1,        0,
      //                             0,  0,  0,        1};
      // position' = 1*position + 1*velocity
      // velocity' = 1*velocity
      REAL observable[NSEEN*NSTATES]; // = {1., 0, 0, 0,
                   //        0, 1., 0, 0};
      // only position is measurable; not velocity
      matrix x = matrix(NSTATES,1,State); // initial state (location and velocity)
      matrix P = matrix(NSTATES,NSTATES, uncertainty); // 4x4 initial uncertainty
      matrix meas = matrix(NSEEN,1,measurements);
 
	matrix u = matrix( NSTATES, 1); // 4x1 external motion; set to 0.
        // u comes from what the vehicle is trying to do, e.g. accelerate.
	matrix F = matrix(NSTATES,NSTATES, state_transition); //  next state function
        for (int i = 0; i < NSEEN*NSTATES; i++)
        {
          observable[i] = i%(NSTATES+1) ? 0 : 1;
        }
	matrix H = matrix(NSEEN,NSTATES, observable); //  measurement function
        H.show();
	matrix R = matrix(NSEEN,NSEEN, variance);  //  measurement variance
	matrix I = matrix(NSTATES); // 4x4 identity matrix
 
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
        Show("dt = ");
        Show(deltaT_s);
        Show("P= ");
        P.show();  

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
        Show("R= ");
        R.show();
        Show("K= ");
        K.show();
        Show("x= ");
        x.show();
        Show("P= ");
        P.show();  
        // Put x into State and P into Uncertainty.
        x.values(State);
        P.values(uncertainty);
	return;
}
