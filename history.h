#pragma once


struct hist
{
	
	float currentSpeed_kmPh;
	long oldSpeed_mmPs;  // last data from the interrupt
	long olderSpeed_mmPs;  // older data
	
	
	long tickMillis;
	long oldTickMillis;
	
	unsigned long nowTime_ms;
	unsigned long oldTime_ms;  // time stamp of old speed
	unsigned long olderTime_ms;   // time stamp of older speed
	
	unsigned long TickTime_ms;  // Tick times are used to compute speeds
	unsigned long OldTick_ms;   // Tick times may not match time stamps if we don't process
	// results of every interrupt
};