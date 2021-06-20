#ifndef motion_processor_h
#define motion_processor_h

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include "types.h"

#define conver_rad_deg  57.295779
#define conver_dag_rad  0.0174533   // pi/180
#define R	6371000


class MotProc {
	public:
	/* Variables */
	struct location_t myLocation_old;
	struct location_t myLocation;
	int myDirection;
	uint8_t myVelocity;
	uint8_t serialNumber;
	int beaconTimer;
	/* Functions */
	MotProc();
	int getDirection(location_t locA, location_t locB);
	void setMyLocAndV(stmData_t stmData);
	location_t getMyLocation();
	int getMyDirection();
	void setMyDirection();
	bool isSameDirection(location_t senderLocation);
	int getDistanse(location_t senderLocation);
	bool brakePacketForwardChecker(int senderDirection, location_t senderLocation);
	bool stm_init();
	void stmInterfaceOn();
	bool acce_init();
	void acceInterfaceOn();
	void sendPacket(uint8_t type);
};

extern MotProc motionProceccor;

#endif
