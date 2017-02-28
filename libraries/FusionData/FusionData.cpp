
#include <math.h>
#include <Common.h>
#include <FusionData.h>
#include <ElcanoSerial.h>

/* Initializes the member variables for PositionData struct */
void PositionData::Clear()
{
	x_Pos = 0;
	y_Pos = 0;
	bearing_deg = 0;
	time_ms = 0;
	speed_cmPs = 0;
	distance_mm = 0;
	sigma_mm = 0;
}


/* The function display member variables of PositionData struct
*/
void PositionData::Display()
{
	Serial.print("PositionData::x_Pos->");
	Serial.println(x_Pos);
	Serial.print("PositionData::y_Pos->");
	Serial.println(y_Pos);
	Serial.print("PositionData::bearing_deg->");
	Serial.println(bearing_deg);
	Serial.print("PositionData::time_ms->");
	Serial.println(time_ms);
	Serial.print("PositionData::speed_cmPs->");
	Serial.println(speed_cmPs);
	Serial.print("PositionData::distance_mm->");
	Serial.println(distance_mm);
}


/* The function calculates position using Dead Reckoning 
*/
void ComputePositionWithDR(PositionData &oldData, PositionData &newData)
{
	Serial.print("ComputePositionWithDR::NewTime:");
	Serial.print(newData.time_ms);
	Serial.print(",OldTime:");
	Serial.println(oldData.time_ms);
	

	// To check if this is new reading or the same reading
	if ( newData.time_ms > oldData.time_ms) {
		// Calculate distance
		newData.distance_mm = (newData.time_ms - oldData.time_ms) * ((newData.speed_cmPs * 10.0) / 1000.0);

		Serial.print("ComputePositionWithDR::newSpeed_cmPs:");
		Serial.print(newData.speed_cmPs);
		Serial.print(",distance:");
		Serial.println(newData.distance_mm);

		newData.x_Pos = oldData.x_Pos + cos((newData.bearing_deg / HEADING_PRECISION) * TO_RADIANS) * newData.distance_mm;
		newData.y_Pos = oldData.y_Pos + sin((newData.bearing_deg / HEADING_PRECISION) * TO_RADIANS) * newData.distance_mm;

		Serial.print("ComputePositionWithDR::X_pos:");
		Serial.print(newData.x_Pos);
		Serial.print(",Y_pos:");
		Serial.println(newData.y_Pos);
	}
}

/* The function copies data from new to old
*/
void CopyData(PositionData &oldData, PositionData &newData)
{
	oldData.x_Pos = newData.x_Pos;
	oldData.y_Pos = newData.y_Pos;
	oldData.bearing_deg = newData.bearing_deg;
	oldData.time_ms = newData.time_ms;
	oldData.speed_cmPs = newData.speed_cmPs;
	oldData.distance_mm = newData.distance_mm;
}


/*  Translate co-ordinates

PositionData &in - input reference
PositionData &outs - output to be shifted
shift - add or substract

*/
void TranslateCoordinates(PositionData &in, PositionData &out, int shift)
{
	if (shift == 1)
	{
		// Translate the data to current co-ordinates (substract)
		out.x_Pos = out.x_Pos - in.x_Pos;
		out.y_Pos = out.y_Pos - in.y_Pos;
	}
	else
	{
		// Translate the data to current co-ordinates (add)
		out.x_Pos = out.x_Pos + in.x_Pos;
		out.y_Pos = out.y_Pos + in.y_Pos;
	}
}

/*  Rotates data it based on bearing

	PositionData &compass - Data from Compass
	PositionData &gps - Data from GPS
	direction - clockwiSPE or counter-clockwiSPE

*/
void RotateCoordinates(PositionData &gps, long bearing_deg, int direction)
{
	Serial.print("RotateCoordinates::x_Pos(Before):");
	Serial.println(gps.x_Pos);
	Serial.print("RotateCoordinates::y_Pos(Before):");
	Serial.println(gps.y_Pos);

	// Rotate by the bearing
	if (direction == 1) 
	{
		// counter-clockwise
		// x_Pos = x.CosA + y.SinA
		long temp_x;
		temp_x = gps.x_Pos * cos(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS) + 
			gps.y_Pos * sin(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS );
		// y_Pos = y.CosA - x.SinA
		gps.y_Pos = gps.y_Pos * cos(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS) - 
			gps.x_Pos * sin(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS);
		gps.x_Pos = temp_x;
	}
	else 
	{
		// clockwise
		// x_Pos = x.CosA - y.SinA
		long temp_x;
		temp_x = gps.x_Pos * cos(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION)* TO_RADIANS) - 
			gps.y_Pos * sin(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS);
		// y_Pos = x.SinA + y.CosA
		gps.y_Pos = gps.x_Pos * sin(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS) + 
			gps.y_Pos * cos(((90 * HEADING_PRECISION - bearing_deg) / HEADING_PRECISION) * TO_RADIANS);
		gps.x_Pos = temp_x;
	}

	Serial.print("RotateCoordinates::x_Pos(After):");
	Serial.println(gps.x_Pos);
	Serial.print("RotateCoordinates::y_Pos(After):");
	Serial.println(gps.y_Pos);
}


/* Function Calculates Line Slope */
double LineSlope(double x1, double x2, double y1, double y2)
{
	if (x2 == x1)
		return 0;

	return ((y2 - y1) * 1. / (x2 - x1) * 1.);
	
}

/* Function Calculated Line Intercept */
double LineIntercept(double x1, double y1, double m1)
{
	return (y1 - m1 * x1) * 1.;
}

/* Function Determines crosspoint on X-axis */
double CrossPointX(double m1, double c1, double m2, double c2)
{
	if (m2 == m1)
		return 0;
	return (c2 - c1) * 1. / (m1 - m2) * 1.;
}

/* Function Determines crosspoint on Y-axis */
double CrossPointY(double m1, double c1, double m2, double c2)
{
	if (m2 == m1)
		return 0;
	return (m2*c1 - m1*c2) * 1. / (m2 - m1) * 1.;
}

/* This function finds fuzzy crosspoint between GPS & Compass data
PositionData &gps -> GPS input
long distance_mm -> Distance in mm
long bearing_deg -> bearing in degree
PositionData &output -> Fuzzy output
*/
void FindFuzzyCrossPointXY(PositionData &gps, long distance_mm, long bearing_deg, PositionData &out)
{
	long sx1 = 0;
	long gx1 = 0;

	Serial.print("FindFuzzyCrossPointXY::distance_mm = ");
	Serial.println(distance_mm);
	double DE = distance_mm * 100*DISTANCE_ERROR * 1.;
	Serial.print("Distance Error = " );
	Serial.println(DE);

	// Calculating 3 x-positions using the distance error and observed x-position (DR)
	// sx0 - DE, sx0, sx0 + DE 
	// where sx0 is observed x-position and DE is distance error
	// sx0 as distance_mm
	long sx0 = distance_mm*100;
	//Serial.println(gps.sigma_mm);
	long GE = (gps.sigma_mm)*100;
	//Serial.println(GE);
	// Calculating 3 x-positions using the GPS error and observed x-position (GPS)
	// gx0 - GE, gx0, gx0 + GE 
	// where gx0 is observed x-position and GE is compass error
	long gx0 = gps.x_Pos/10000;

	// Slope Selection
	// Taking sx0 and gx0 as reference	
	if (sx0 <= gx0)
	{
		// line to be used would be sx1 -> sx0 + DE, gx1-> gx0 - GE
		Serial.println("sx1 -> sx0 + DE, gx1 -> gx0 - GE");
		sx1 = sx0 + DE;
		gx1 = gx0 - GE;
	}
	else
	{
		// line to be used would be sx1 -> sx0 - DE, gx1 -> gx0 + GE
		Serial.println("sx1 -> sx0 - DE, gx0 -> gx1 + GE");
		sx1 = sx0 - DE;
		gx1 = gx0 + GE;
	}

	Serial.print("FindFuzzyCrossPointXY::sx0=");
	Serial.print(sx0);
	Serial.print(",GE=");
	Serial.print(GE);
	Serial.print(",sx1=");
	Serial.print(sx1);
	Serial.print(",gx0=");
	Serial.print(gx0);
	Serial.print(",gx1=");
	Serial.println(gx1);

	// Get Slope of Lines 
	// Now, we have to X-points sx0, sx1 from Compass
	// and gx0, gy1 from GPS
	double m1 = LineSlope(sx0, sx1, 100, 0);
	double m2 = LineSlope(gx0, gx1, 100, 0);

	// Get Intercept
	double c1 = LineIntercept(sx1, 0, m1);
	double c2 = LineIntercept(gx1, 0, m2);

	Serial.print("FindFuzzyCrossPointXY::m1= ");
	Serial.print(m1);
	Serial.print(",m2=");
	Serial.println(m2);
	Serial.print("FindFuzzyCrossPointXY::c1=");
	Serial.print(c1);
	Serial.print(",c2=");
	Serial.println(c2);

	// Get Cross Point X
	out.x_Pos = CrossPointX(m1, c1, m2, c2);
	Serial.print("FindFuzzyCrossPointXY::out.x_Pos = ");
	Serial.println(out.x_Pos);

	// Calculating 3 y-positions using the compass error and observed y-position (DR)
	// sy0 - CE, sy0, sy0 + CE 
	// where sy0 is observed y-position and CE is distance error
	long sy1 = 0;
	long gy1 = 0;	
	double CE = distance_mm * sin((COMPASS_ERROR ) * TO_RADIANS)*100;
	Serial.println(CE*100);
	Serial.print("shailja ce");

	// Calculating 3 y-positions using the GPS error and observed y-position (GPS)
	// gy0 - GE, gy0, gy0 + GE 
	// where gy0 is observed y-position and GE is compass error

	// Slope Selection
	// Taking sy0 and gy0 as reference
	// sy0 = 0 
	long sy0 = 0; 
	long gy0 = gps.y_Pos/10000;

	if (sy0 <= gy0)
	{
		// line to be used would be sy1 -> sy0 + CE, gy1 -> gy0 - GE
		Serial.println("sy1 -> sy0 + CE, gy1 -> gy0 - GE");
		sy1 = sy0 + CE;
		gy1 = gy0 - GE;
	}
	else
	{
		// line to be used would be sy1 -> sy0 - CE, gy1 -> gy0 + GE
		Serial.println("sy1 -> sy0 - CE, gy1 -> gy0 + GE");
		sy1 = sy0 - CE;
		gy1 = gy0 + GE;
	}

	Serial.print("FindFuzzyCrossPointXY::sy0=");
	Serial.print(sy0);
	Serial.print(",CE=");
	Serial.print(CE);
	Serial.print(",sy1=");
	Serial.print(sy1);
	Serial.print(",gy0=");
	Serial.print(gy0);
	Serial.print(",gy1=");
	Serial.println(gy1);

	// Get Slope of Lines 
	m1 = LineSlope(100, 0, sy0, sy1);
	m2 = LineSlope(100, 0, gy0, gy1);

	// Get Intercept
	c1 = LineIntercept(100, sy1, m1);
	c2 = LineIntercept(100, gy1, m2);

	Serial.print("FindFuzzyCrossPointXY::m1= ");
	Serial.print(m1);
	Serial.print(",m2=");
	Serial.println(m2);
	Serial.print("FindFuzzyCrossPointXY::c1=");
	Serial.print(c1);
	Serial.print(",c2=");
	Serial.println(c2);

	// Get CrossPoint Y
	out.y_Pos = CrossPointY(m1, c1, m2, c2);
	Serial.print("FindFuzzyCrossPointXY::out.y_Pos = ");
	Serial.println(out.y_Pos);
}
