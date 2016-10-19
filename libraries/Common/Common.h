#pragma once

#define WHEEL_BASE_mm         800
#define TURNING_RADIUS_mm    4000
#define MAX_ACCEL_mmPs2      2000
#define STANDARD_ACCEL_mmPs2 1000
// but we send a number from 0 to 255
#define STANDARD_ACCEL       55
#define MAX_ACCEL            110
#define HALF_BRAKE           127
#define FULL_BRAKE           255
#define WALK_SPEED_mmPs      1000
// 10 mph = 4.44 m/s
#define MAX_SPEED_mmPs       4444