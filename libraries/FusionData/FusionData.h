// FusionData.h - header included for all functions to fuse the 
// IMU sensor data, speed data and GPS data


#define HEADING_PRECISION 1000
// Hard coded values
#define COMPASS_ERROR ((double) 2)
#define DISTANCE_ERROR ((double) 0.04)
#define GPS_ERROR ((long) 4236)

#define ROTATE_CLOCKWISE 0
#define ROTATE_COUNTER_CLOCKWISE 1

/* This struct will be used to store the sensor or GPS data for location position */
struct PositionData {
	long x_Pos;
	long y_Pos;
	long bearing_deg;
	long time_ms;
	long speed_cmPs;
	long distance_mm;
	long sigma_mm;
	void Clear();
};

void ComputePositionWithDR(PositionData &oldData, PositionData &newData);
void CopyData(PositionData &oldData, PositionData &newData);
void TranslateCoordinates(PositionData &in, PositionData &out, int shift);
void RotateCoordinates(PositionData &gps, long bearing_deg, int direction);
double LineSlope(double x1, double x2, double y1, double y2);
double LineIntercept(double x1, double y1, double m1);
double CrossPointX(double m1, double c1, double m2, double c2);
double CrossPointY(double m1, double c1, double m2, double c2);
void FindFuzzyCrossPointXY(PositionData &gps, long distance, long bearing, PositionData &out);


