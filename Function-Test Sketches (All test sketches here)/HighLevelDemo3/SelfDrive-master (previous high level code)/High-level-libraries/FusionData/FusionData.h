// FusionData.h - header included for all functions to fuse the 
// IMU sensor data, speed data and GPS data

#define HEADING_PRECISION 1000000.0
// Hard coded values
#define COMPASS_ERROR ((double) 2.0)
#define DISTANCE_ERROR ((double) 0.04) //percentage error = 4%

#define ROTATE_CLOCKWISE 0
#define ROTATE_COUNTER_CLOCKWISE 1

#define SUBTRACT_FROM_ORIGIN	1
#define ADD_TO_ORIGIN			0

/* This struct will be used to store the sensor or GPS data for location position */
struct PositionData {
	long x_Pos_mm;
	long y_Pos_mm;
	long bearing_deg;
	long speed_mmPs;
	long distance_mm;
	long sigma_mm;
	void Clear();
	void Display();
    unsigned long time_ms;
};

void CopyData(PositionData &oldData, PositionData &newData);
void TranslateCoordinates(PositionData &in, PositionData &out, int shift);
void RotateCoordinates(PositionData &gps, long bearing_deg, int direction);
double LineSlope(double x1, double x2, double y1, double y2);
double LineIntercept(double x1, double y1, double m1);
//double CrossPointY(double m1, double c1, double m2, double c2);
//void FindFuzzyCrossPointXY(PositionData &gps, long distance, long bearing, PositionData &out);


