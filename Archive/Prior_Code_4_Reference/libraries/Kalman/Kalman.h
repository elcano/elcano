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
            REAL deltaT_s,  // time from previous state to measurement in seconds
            REAL* variance);     // variance of the measurements
