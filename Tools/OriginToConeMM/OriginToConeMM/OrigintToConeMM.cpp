// ElcanoTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

int main(int argc, const char* argv[])
{
	// compute relative to origin, since Arduino double is limited to 6 digits.
#define EARTH_RADIUS_MM 6371000000.
	double LATITUDE_ORIGIN = 47.760854;
	double LONGITUDE_ORIGIN = -122.190033;
	
#define PId 3.14159265358979
#define TO_RADIANS (PId/180.)				
	double latitude = 47.760681;
	double longitude = -122.190255;
	double diff;
	double CosLatitude = cos((LATITUDE_ORIGIN) * TO_RADIANS);
	long east_mm = 0;
	long north_mm = 0;
	double relative;
	diff = latitude - LATITUDE_ORIGIN;
	relative = diff * TO_RADIANS * EARTH_RADIUS_MM;
	north_mm = relative;
	cout << "North_mm is " << north_mm << endl;
	diff = longitude - LONGITUDE_ORIGIN;
	relative = diff * TO_RADIANS * EARTH_RADIUS_MM * CosLatitude;
	east_mm = relative;
	cout << "East_mm is " << east_mm;
	cin.get();
	
	return 1;
}
