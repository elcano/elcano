#pragma once
/**
 * current CAN Protocol (4-23-19)
 * used for identifying source of signals, must be used
 * for high and low-level boards
 */ 
namespace elcano {
  

#define RCStatus_CANID 0x50
#define HiStatus_CANID 0x100
#define GoalReached_CANID 0x101
#define LowStatus_CANID 0x200
#define RCDrive_CANID 0x300
#define HiDrive_CANID 0x350
#define Actual_CANID 0x400
#define LiDAR_CANID	0x420
#define Sonar_CANID	0x440
#define CameraObstacl_CANID 0x460
#define CameraCone_CANID 0x480
#define CameraRiEdge_CANID 0x4A0
#define CameraLeEdge_CANID 0x4A1
#define UNUSED1 0x4C1 //for Goal Position 1
#define UNUSED2 0x4C2 //for Goal Position 2
#define UNUSED3 0x4C3 //for Goal Position 3
#define UNUSED4 0x4C4 //for Goal Position 4
#define UNUSED5 0x4C5 //for Goal Position 5
#define UNUSED6 0x4C6 //for Goal Position 6

} // namespace elcano
