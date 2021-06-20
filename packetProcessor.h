#ifndef packet_processor_h
#define packet_processor_h

#include <string.h>
#include "types.h"

#define BUFFPTR (uint8_t *)&currentPacket

class PackProc {
	private:
	/* Functions */
	void sendToSmartPhone(int len);
	
	
	public:
	/* Variables */
	CarPacket_t currentPacket;
	/* Functions(Public) */
	PackProc();
	void stackInit();
	void on();
	void off();
};

extern PackProc packetProceccor;

#endif
